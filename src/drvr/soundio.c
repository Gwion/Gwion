#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <soundio/soundio.h>

#include "defs.h"
#include "vm.h"
#include "driver.h"

static struct SoundIo          *soundio    = NULL;
static struct SoundIoOutStream *outstream  = NULL;
static struct SoundIoInStream  *instream   = NULL;
static struct SoundIoDevice    *out_device = NULL;
static struct SoundIoDevice    *in_device  = NULL;

static enum SoundIoBackend backend = SoundIoBackendNone;
static m_str  device_id = NULL;
static m_bool raw = false;
static void (*write_sample)(char *ptr, m_float sample);
static void (*read_sample)(char *ptr, m_float *sample);

typedef struct SoundIoChannelArea* Areas;
typedef struct SoundIoOutStream * Out;
typedef struct SoundIoInStream * In;

struct SioInfo {
  VM* vm;
  DriverInfo* di;
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

static void underflow_callback(Out outstream) {
  static int count = 0;
  fprintf(stderr, "underflow %d\n", count++);
}

static void overflow_callback(In stream) {
  static int count = 0;
  fprintf(stderr, "overflow %d\n", count++);
}

static m_bool check_cb_error1(VM* vm, void* data, Areas* areas, int* count, m_bool in) {
  int err = in ?
    soundio_instream_begin_read((In)data, areas, count) :
    soundio_outstream_begin_write((Out)data, areas, count);
  if(err) {
    fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
    vm->is_running = 0;
    return -1;
  }
  return 1;
}

static m_bool check_cb_error2(VM* vm, void* data, int (*f)(void*)) {
  int err = f(data);
  if(err) {
    fprintf(stderr, "%s\n", soundio_strerror(err));
    vm->is_running = 0;
    return -1;
  }
  return 1;
}

static void write_callback(Out stream, int min, int left) {
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
      fprintf(stderr, "Dropped %d frames due to internal underflow\n", count);
    else for(int  frame = 0; frame < count; frame++) {
      di->run(vm);
      for(int  channel = 0; channel < stream->layout.channel_count; channel++) {
        write_sample(areas[channel].ptr, sp->out[channel]);
        areas[channel].ptr += areas[channel].step;
      }
      sp->pos++;
    }
    check_cb_error2(info->vm, stream, (int (*)(void*))soundio_outstream_end_write);
    left -= count;
  }
  soundio_outstream_pause(stream, 0);
}

static void read_callback(In stream, int min, int left) {
  Areas areas;
  VM* vm = (VM*)stream->userdata;
  while(left > 0) {
    int count = left;
    if(check_cb_error1(vm, stream, &areas, &count, 1) < 0)
      return;
    if(!count)
      break;
    if(!areas)
      fprintf(stderr, "Dropped %d frames due to internal overflow\n", count);
    else for(int frame = 0; frame < count; frame += 1) {
      for(int ch = 0; ch < stream->layout.channel_count; ch += 1) {
        read_sample(areas[ch].ptr, &vm->in[ch]);
        areas[ch].ptr += areas[ch].step;
      }
    }
    check_cb_error2(vm, stream, (int(*)(void*))soundio_instream_end_read);
    left -= count;
  }
}

static m_bool init_soundio() {
  int err;
  if(!(soundio = soundio_create())) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }
  err = (backend == SoundIoBackendNone) ?
            soundio_connect(soundio) : soundio_connect_backend(soundio, backend);
  if(err) {
    fprintf(stderr, "Unable to connect to backend: %s\n", soundio_strerror(err));
    return -1;
  }
  soundio_flush_events(soundio);
  return 1;
}

static int get_index() {
  int selected_device_index = -1;
  if(device_id) {
    int device_count = soundio_output_device_count(soundio);
    for(int i = 0; i < device_count; i += 1) {
      struct SoundIoDevice *device = soundio_get_output_device(soundio, i);
      bool select_this_one = strcmp(device->id, device_id) == 0 && device->is_raw == raw;
      soundio_device_unref(device);
      if(select_this_one) {
        selected_device_index = i;
        break;
      }
    }
  } else
    selected_device_index = soundio_default_output_device_index(soundio);
  if(selected_device_index < 0) {
    fprintf(stderr, "Output device not found\n");
    return -1;
  }
  return selected_device_index;
}

static m_bool get_device(int selected_device_index) {
  out_device = soundio_get_output_device(soundio, selected_device_index);
  if(!out_device) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }
  in_device = soundio_get_input_device(soundio, selected_device_index);
  if(!in_device) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }
  return 1;
}

static m_bool probe() {
  if(out_device->probe_error) {
    fprintf(stderr, "Cannot probe device: %s\n", soundio_strerror(out_device->probe_error));
    return -1;
  }
  if(in_device->probe_error) {
    fprintf(stderr, "Cannot probe device: %s\n", soundio_strerror(out_device->probe_error));
    return -1;
  }
  return 1;
}

static m_bool out_create(VM* vm, DriverInfo* di) {
  struct SioInfo info = { vm, di };
  outstream = soundio_outstream_create(out_device);
  if(!outstream) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }
  outstream->write_callback = write_callback;
  outstream->underflow_callback = underflow_callback;
  outstream->name = "Gwion output";
  outstream->software_latency = 0;
  outstream->sample_rate = di->sr;
  outstream->userdata = &info;
  return 1;
}

static m_bool in_create(VM* vm, DriverInfo* di) {
  instream = soundio_instream_create(in_device);
  if(!instream) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }
  instream->read_callback = read_callback;
  instream->overflow_callback = overflow_callback;
  instream->name = "Gwion input";
  instream->software_latency = 0;
  instream->sample_rate = di->sr;
  instream->userdata = vm;
  return 1;
}

static m_bool out_format() {
  if(soundio_device_supports_format(out_device, SoundIoFormatFloat64NE)) {
    outstream->format = SoundIoFormatFloat64NE;
    write_sample = write_sample_float64ne;
  } else if(soundio_device_supports_format(out_device, SoundIoFormatFloat32NE)) {
    outstream->format = SoundIoFormatFloat32NE;
    write_sample = write_sample_float32ne;
  } else if(soundio_device_supports_format(out_device, SoundIoFormatS32NE)) {
    outstream->format = SoundIoFormatS32NE;
    write_sample = write_sample_s32ne;
  } else if(soundio_device_supports_format(out_device, SoundIoFormatS16NE)) {
    outstream->format = SoundIoFormatS16NE;
    write_sample = write_sample_s16ne;
  } else {
    fprintf(stderr, "No suitable device format available.\n");
    return -1;
  }
  return 1;
}

static m_bool in_format() {
  if(soundio_device_supports_format(in_device, SoundIoFormatFloat64NE)) {
    instream->format = SoundIoFormatFloat64NE;
    read_sample = read_sample_float64ne;
  } else if(soundio_device_supports_format(in_device, SoundIoFormatFloat32NE)) {
    instream->format = SoundIoFormatFloat32NE;
    read_sample = read_sample_float32ne;
  } else if(soundio_device_supports_format(in_device, SoundIoFormatS32NE)) {
    instream->format = SoundIoFormatS32NE;
    read_sample = read_sample_s32ne;
  } else if(soundio_device_supports_format(in_device, SoundIoFormatS16NE)) {
    instream->format = SoundIoFormatS16NE;
    read_sample = read_sample_s16ne;
  } else {
    fprintf(stderr, "No suitable device format available.\n");
    return -1;
  }
  return 1; 
}

static m_bool open_stream() {
  int err;
  if((err = soundio_outstream_open(outstream))) {
    fprintf(stderr, "unable to open output device: %s", soundio_strerror(err));
    return -1;
  }
  if((err = soundio_instream_open(instream))) {
    fprintf(stderr, "unable to open input device: %s", soundio_strerror(err));
    return -1;
  }
  return 1;
}

static m_bool check_layout() {
  if(outstream->layout_error) {
    fprintf(stderr, "unable to set output channel layout: %s\n",
        soundio_strerror(outstream->layout_error));
    return -1;
  }
  if(instream->layout_error) {
    fprintf(stderr, "unable to set input channel layout: %s\n",
        soundio_strerror(instream->layout_error));
    return -1;
  }
  return 1;
}

static m_bool sio_ini(VM* vm, DriverInfo* di) {
  device_id = di->card;
  CHECK_BB(init_soundio())
  int selected_device_index = get_index();
  CHECK_BB(selected_device_index)
  CHECK_BB(get_device(selected_device_index))
  CHECK_BB(probe())
  CHECK_BB(out_create(vm, di))
  CHECK_BB(in_create(vm, di))
  CHECK_BB(out_format())
  CHECK_BB(in_format())
  CHECK_BB(open_stream())
  CHECK_BB(check_layout())
  return 1;
}

static void sio_run(VM* vm, DriverInfo* info) {
  int err;
  if((err = soundio_instream_start(instream))) {
    fprintf(stderr, "unable to start input device: %s\n", soundio_strerror(err));
    return;
  }
  if((err = soundio_outstream_start(outstream))) {
    fprintf(stderr, "unable to start ouput device: %s\n", soundio_strerror(err));
    return;
  }
  while(vm->is_running)
    soundio_flush_events(soundio);
}

static void sio_del(VM* vm, DriverInfo* di) {
  soundio_outstream_destroy(outstream);
  soundio_instream_destroy(instream);
  soundio_device_unref(in_device);
  soundio_device_unref(out_device);
  soundio_destroy(soundio);
}

void sio_driver(Driver* d) {
  d->ini = sio_ini;
  d->run = sio_run;
  d->del = sio_del;
}
