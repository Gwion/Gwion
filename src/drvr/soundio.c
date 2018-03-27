#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <soundio/soundio.h>
#include "defs.h"
#include "vm.h"
#include "driver.h"
#include "err_msg.h"

typedef struct SoundIoChannelArea* Areas;
typedef struct SoundIoOutStream * Out;
typedef struct SoundIoInStream * In;

struct SioInfo {
  VM* vm;
  DriverInfo* di;
  struct SoundIo          *soundio;
  struct SoundIoOutStream *outstream;
  struct SoundIoInStream  *instream;
  struct SoundIoDevice    *out_device;
  struct SoundIoDevice    *in_device;
  enum SoundIoBackend backend;// = SoundIoBackendNone;
  m_str  device_id;
  m_bool raw;
  void (*write_sample)(char *ptr, m_float sample);
  void (*read_sample)(char *ptr, m_float *sample);

};

static void write_sample_s16ne(char *ptr, m_float sample) {
  int16_t *buf = (int16_t *)ptr;
  m_float range = (m_float)INT16_MAX - (m_float)INT16_MIN;
  m_float val = sample * range / 2.0;
  *buf = val;
}

static void write_sample_s32ne(char *ptr, m_float sample) {
  int32_t *buf = (int32_t *)ptr;
  m_float range = (m_float)INT32_MAX - (m_float)INT32_MIN;
  m_float val = sample * range / 2.0;
  *buf = val;
}

static void write_sample_float32ne(char *ptr, m_float sample) {
  float *buf = (float*)ptr;
  *buf = sample;
}

static void write_sample_float64ne(char *ptr, m_float sample) {
  m_float *buf = (m_float *)ptr;
  *buf = sample;
}

static void read_sample_s16ne(char *ptr, m_float *sample) {
  int16_t *buf = (int16_t *)ptr;
  m_float range = (m_float)INT16_MAX - (m_float)INT16_MIN;
  m_float val = *buf * range / 2.0;
  *sample = val;
}

static void read_sample_s32ne(char *ptr, m_float *sample) {
  int32_t *buf = (int32_t *)ptr;
  m_float range = (m_float)INT32_MAX - (m_float)INT32_MIN;
  m_float val = *buf * range / 2.0;
  *sample = val;
}

static void read_sample_float32ne(char *ptr, m_float *sample) {
  *sample = *(float *)ptr;
}

static void read_sample_float64ne(char *ptr, m_float *sample) {
  *sample = *(m_float *)ptr;
}

static void underflow_callback(Out outstream __attribute__((unused))) {
  static int count = 0;
  gw_err("underflow %d\n", count++);
}

static void overflow_callback(In stream __attribute__((unused))) {
  static int count = 0;
  gw_err("overflow %d\n", count++);
}

static m_bool check_cb_error1(VM* vm, void* data, Areas* areas, int* count, m_bool in) {
  int err = in ?
    soundio_instream_begin_read((In)data, areas, count) :
    soundio_outstream_begin_write((Out)data, areas, count);
  if(err) {
    gw_err("unrecoverable stream error: %s\n", soundio_strerror(err));
    vm->is_running = 0;
    return -1;
  }
  return 1;
}

static m_bool check_cb_error2(VM* vm, void* data, int (*f)(void*)) {
  int err = f(data);
  if(err) {
    gw_err("%s\n", soundio_strerror(err));
    vm->is_running = 0;
    return -1;
  }
  return 1;
}

static void write_callback(Out stream, int min __attribute__((unused)), int left) {
  Areas areas;
  struct SioInfo* info = (struct SioInfo*)stream->userdata;
  VM* vm = info->vm;
  sp_data* sp = vm->sp;
  DriverInfo* di = info->di;
  while(left > 0) {
    int count = left;
    if(check_cb_error1(vm, stream, &areas, &count, 0) < 0)
      return;
    if(!count)
      break;
    if(!areas)
      gw_err("Dropped %d frames due to internal underflow\n", count);
    else for(int  frame = 0; frame < count; frame++) {
      di->run(vm);
      for(int  channel = 0; channel < stream->layout.channel_count; channel++) {
        info->write_sample(areas[channel].ptr, sp->out[channel]);
        areas[channel].ptr += areas[channel].step;
      }
      sp->pos++;
    }
    check_cb_error2(info->vm, stream, (int (*)(void*))soundio_outstream_end_write);
    left -= count;
  }
  soundio_outstream_pause(stream, 0);
}

static void read_callback(In stream, int min __attribute__((unused)), int left) {
  Areas areas;
  struct SioInfo* info = (struct SioInfo*)stream->userdata;
  VM* vm = info->vm;
  while(left > 0) {
    int count = left;
    if(check_cb_error1(vm, stream, &areas, &count, 1) < 0)
      return;
    if(!count)
      break;
    if(!areas)
      gw_err("Dropped %d frames due to internal overflow\n", count);
    else for(int frame = 0; frame < count; frame += 1) {
      for(int ch = 0; ch < stream->layout.channel_count; ch += 1) {
        info->read_sample(areas[ch].ptr, &vm->in[ch]);
        areas[ch].ptr += areas[ch].step;
      }
    }
    check_cb_error2(vm, stream, (int(*)(void*))soundio_instream_end_read);
    left -= count;
  }
}

static m_bool init_soundio(struct SioInfo* info) {
  int err;
  if(!(info->soundio = soundio_create())) {
    gw_err("out of memory\n");
    return -1;
  }
  err = (info->backend == SoundIoBackendNone) ?
            soundio_connect(info->soundio) : soundio_connect_backend(info->soundio, info->backend);
  if(err) {
    gw_err("Unable to connect to backend: %s\n", soundio_strerror(err));
    return -1;
  }
  soundio_flush_events(info->soundio);
  return 1;
}

static int get_index(struct SioInfo* info) {
  int selected_device_index = -1;
  if(info->device_id) {
    int device_count = soundio_output_device_count(info->soundio);
    for(int i = 0; i < device_count; i += 1) {
      struct SoundIoDevice *device = soundio_get_output_device(info->soundio, i);
      bool select_this_one = strcmp(device->id, info->device_id) == 0 && device->is_raw == info->raw;
      soundio_device_unref(device);
      if(select_this_one) {
        selected_device_index = i;
        break;
      }
    }
  } else
    selected_device_index = soundio_default_output_device_index(info->soundio);
  if(selected_device_index < 0) {
    gw_err("Output device not found\n");
    return -1;
  }
  return selected_device_index;
}

static m_bool get_device(struct SioInfo* info, int selected_device_index) {
  info->out_device = soundio_get_output_device(info->soundio, selected_device_index);
  if(!info->out_device) {
    gw_err("out of memory\n");
    return -1;
  }
  info->in_device = soundio_get_input_device(info->soundio, selected_device_index);
  if(!info->in_device) {
    gw_err("out of memory\n");
    return -1;
  }
  return 1;
}

static m_bool probe(struct SioInfo* info) {
  if(info->out_device->probe_error) {
    gw_err("Cannot probe device: %s\n", soundio_strerror(info->out_device->probe_error));
    return -1;
  }
  if(info->in_device->probe_error) {
    gw_err("Cannot probe device: %s\n", soundio_strerror(info->in_device->probe_error));
    return -1;
  }
  return 1;
}

static m_bool out_create(DriverInfo* di) {
  struct SioInfo* info = (struct SioInfo*)di->data;
  info->outstream = soundio_outstream_create(info->out_device);
  if(!info->outstream) {
    gw_err("out of memory\n");
    return -1;
  }
  info->outstream->write_callback = write_callback;
  info->outstream->underflow_callback = underflow_callback;
  info->outstream->name = "Gwion output";
  info->outstream->software_latency = 0;
  info->outstream->sample_rate = di->sr;
  info->outstream->userdata = info;
  return 1;
}

static m_bool in_create(DriverInfo* di) {
  struct SioInfo* info = (struct SioInfo*)di->data;
  info->instream = soundio_instream_create(info->in_device);
  if(!info->instream) {
    gw_err("out of memory\n");
    return -1;
  }
  info->instream->read_callback = read_callback;
  info->instream->overflow_callback = overflow_callback;
  info->instream->name = "Gwion input";
  info->instream->software_latency = 0;
  info->instream->sample_rate = di->sr;
  info->instream->userdata = info;
  return 1;
}

static m_bool out_format(struct SioInfo* info) {
  if(soundio_device_supports_format(info->out_device, SoundIoFormatFloat64NE)) {
    info->outstream->format = SoundIoFormatFloat64NE;
    info->write_sample = write_sample_float64ne;
  } else if(soundio_device_supports_format(info->out_device, SoundIoFormatFloat32NE)) {
    info->outstream->format = SoundIoFormatFloat32NE;
    info->write_sample = write_sample_float32ne;
  } else if(soundio_device_supports_format(info->out_device, SoundIoFormatS32NE)) {
    info->outstream->format = SoundIoFormatS32NE;
    info->write_sample = write_sample_s32ne;
  } else if(soundio_device_supports_format(info->out_device, SoundIoFormatS16NE)) {
    info->outstream->format = SoundIoFormatS16NE;
    info->write_sample = write_sample_s16ne;
  } else {
    gw_err("No suitable device format available.\n");
    return -1;
  }
  return 1;
}

static m_bool in_format(struct SioInfo* info) {
  if(soundio_device_supports_format(info->in_device, SoundIoFormatFloat64NE)) {
    info->instream->format = SoundIoFormatFloat64NE;
    info->read_sample = read_sample_float64ne;
  } else if(soundio_device_supports_format(info->in_device, SoundIoFormatFloat32NE)) {
    info->instream->format = SoundIoFormatFloat32NE;
    info->read_sample = read_sample_float32ne;
  } else if(soundio_device_supports_format(info->in_device, SoundIoFormatS32NE)) {
    info->instream->format = SoundIoFormatS32NE;
    info->read_sample = read_sample_s32ne;
  } else if(soundio_device_supports_format(info->in_device, SoundIoFormatS16NE)) {
    info->instream->format = SoundIoFormatS16NE;
    info->read_sample = read_sample_s16ne;
  } else {
    gw_err("No suitable device format available.\n");
    return -1;
  }
  return 1;
}

static m_bool open_stream(struct SioInfo* info) {
  int err;
  if((err = soundio_outstream_open(info->outstream))) {
    gw_err("unable to open output device: %s", soundio_strerror(err));
    return -1;
  }
  if((err = soundio_instream_open(info->instream))) {
    gw_err("unable to open input device: %s", soundio_strerror(err));
    return -1;
  }
  return 1;
}

static m_bool check_layout(struct SioInfo* info) {
  if(info->outstream->layout_error) {
    gw_err("unable to set output channel layout: %s\n",
        soundio_strerror(info->outstream->layout_error));
    return -1;
  }
  if(info->instream->layout_error) {
    gw_err("unable to set input channel layout: %s\n",
        soundio_strerror(info->instream->layout_error));
    return -1;
  }
  return 1;
}

static m_bool sio_ini(VM* vm, DriverInfo* di) {
  struct SioInfo* info = (struct SioInfo*)xcalloc(1, sizeof(struct SioInfo));
  info->vm = vm;
  info->di = di;
  info->device_id = di->card;
  info->backend = SoundIoBackendNone;
  di->data = info;
  CHECK_BB(init_soundio(info))
  int selected_device_index = get_index(info);
  CHECK_BB(selected_device_index)
  CHECK_BB(get_device(info, selected_device_index))
  CHECK_BB(probe(info))
  CHECK_BB(out_create(di))
  CHECK_BB(in_create(di))
  CHECK_BB(out_format(info))
  CHECK_BB(in_format(info))
  CHECK_BB(open_stream(info))
  CHECK_BB(check_layout(info))
  return 1;
}

static void sio_run(VM* vm, DriverInfo* di) {
  struct SioInfo* info = (struct SioInfo*)di->data;
  int err;
  if((err = soundio_instream_start(info->instream))) {
    gw_err("unable to start input device: %s\n", soundio_strerror(err));
    return;
  }
  if((err = soundio_outstream_start(info->outstream))) {
    gw_err("unable to start ouput device: %s\n", soundio_strerror(err));
    return;
  }
  while(vm->is_running)
    soundio_flush_events(info->soundio);
}

static void sio_del(VM* vm __attribute__((unused)), DriverInfo* di) {
  struct SioInfo* info = (struct SioInfo*)di->data;
  soundio_outstream_destroy(info->outstream);
  soundio_instream_destroy(info->instream);
  soundio_device_unref(info->in_device);
  soundio_device_unref(info->out_device);
  soundio_destroy(info->soundio);
  free(info);
}

void sio_driver(Driver* d) {
  d->ini = sio_ini;
  d->run = sio_run;
  d->del = sio_del;
}
