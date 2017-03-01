#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <soundio/soundio.h>

#include "defs.h"
#include "vm.h"
#include "driver.h"
#include "bbq.h"

static struct SoundIo          *soundio    = NULL;
static struct SoundIoOutStream *outstream  = NULL;
static struct SoundIoInStream  *instream   = NULL;
static struct SoundIoDevice    *out_device = NULL;
static struct SoundIoDevice    *in_device  = NULL;

//static struct SoundIoChannelArea* areas;


static enum SoundIoBackend backend = SoundIoBackendNone;
static m_str  device_id = NULL;
static m_bool raw = false;

void sio_wakeup()
{
  soundio_wakeup(soundio);
}
static void write_sample_s16ne(char *ptr, double sample)
{
  int16_t *buf = (int16_t *)ptr;
  double range = (double)INT16_MAX - (double)INT16_MIN;
  double val = sample * range / 2.0;
  *buf = val;
}

static void write_sample_s32ne(char *ptr, double sample)
{
  int32_t *buf = (int32_t *)ptr;
  double range = (double)INT32_MAX - (double)INT32_MIN;
  double val = sample * range / 2.0;
  *buf = val;
}

static void write_sample_float32ne(char *ptr, double sample)
{
  float *buf = (float *)ptr;
  *buf = sample;
}

static void write_sample_float64ne(char *ptr, double sample)
{
  double *buf = (double *)ptr;
  *buf = sample;
}

static void read_sample_s16ne(char *ptr, double *sample)
{
  int16_t *buf = (int16_t *)ptr;
  double range = (double)INT16_MAX - (double)INT16_MIN;
  double val = *buf * range / 2.0;
  *sample = val;
}

static void read_sample_s32ne(char *ptr, double *sample)
{
  int32_t *buf = (int32_t *)ptr;
  double range = (double)INT32_MAX - (double)INT32_MIN;
  double val = *buf * range / 2.0;
  *sample = val;
}

static void read_sample_float32ne(char *ptr, double *sample)
{
  *sample = *(float *)ptr;
}

static void read_sample_float64ne(char *ptr, double *sample)
{
  *sample = *(double *)ptr;
}

static void (*write_sample)(char *ptr, double sample);
static void (*read_sample)(char *ptr, double *sample);

static void underflow_callback(struct SoundIoOutStream *outstream)
{
  static int count = 0;
  fprintf(stderr, "underflow %d\n", count++);
}

static void overflow_callback(struct SoundIoInStream *stream)
{
  static int count = 0;
  fprintf(stderr, "overflow %d\n", count++);
}

static void write_callback(struct SoundIoOutStream *outstream, int
                           frame_count_min, int frame_count_max)
{
//	double sr = outstream->sample_rate;
//	double seconds_per_frame = 1.0 / sr;
  struct SoundIoChannelArea *areas;
  int    err;
  int    left = frame_count_max;
  const struct SoundIoChannelLayout *layout = &outstream->layout;
  VM* vm = (VM*)outstream->userdata;
  double* data = vm->bbq->sp->out;
  for (;;) {
    int count = left;
    if ((err = soundio_outstream_begin_write(outstream, &areas, &count))) {
      fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
      vm->is_running = 0;
      return;
    }

    if (!count)
      break;

    for (int frame = 0; frame < count; frame++) {
//			for (int channel = 0; channel < instream->layout.channel_count; channel++)
//				vm->bbq->in[channel] = *(double*)instream->areas[channel].ptr;
      vm_run(vm);
      for (int channel = 0; channel < layout->channel_count; channel++) {
        write_sample(areas[channel].ptr, data[channel]);
        areas[channel].ptr += areas[channel].step;
      }
      vm->bbq->sp->pos++;
    }
    if ((err = soundio_outstream_end_write(outstream))) {
      if (err == SoundIoErrorUnderflow)
        return;
      fprintf(stderr, "unrecoverable stream error: %s\n", soundio_strerror(err));
      vm->is_running = 0;
    }

    left -= count;
    if (left <= 0)
      break;
  }
  soundio_outstream_pause(outstream, 0);
}

static void read_callback(struct SoundIoInStream *instream, int frame_count_min, int frame_count_max)
{
  struct SoundIoChannelArea *areas;
  int err;
  int frames_left = frame_count_max;
  VM* vm = (VM*)instream->userdata;
  for (;;) {
    int frame_count = frames_left;

    if ((err = soundio_instream_begin_read(instream, &areas, &frame_count))) {
      vm->is_running = 0;
      fprintf(stderr, "begin read error: %s", soundio_strerror(err));
      return;
    }

    if (!frame_count)
      break;

    if (!areas) {
//            memset(data, 0, frame_count * instream->bytes_per_frame);
      fprintf(stderr, "Dropped %d frames due to internal overflow\n", frame_count);
    } else {
//frame_count= 256;
      char* data[SZ_FLOAT];
      for (int frame = 0; frame < frame_count; frame += 1) {
        for (int ch = 0; ch < instream->layout.channel_count; ch += 1) {
//                    memcpy(data, areas[ch].ptr, instream->bytes_per_sample);
// FIXME
//vm->bbq->in[ch] = *(m_float*)data;
          read_sample(areas[ch].ptr, &vm->bbq->in[ch]);
          areas[channel].ptr += areas[channel].step;
        }
      }
    }

    if ((err = soundio_instream_end_read(instream))) {
      fprintf(stderr, "end read error: %s", soundio_strerror(err));
      vm->is_running = 0;
      return;
    }
    frames_left -= frame_count;
    if (frames_left <= 0)
      break;
  }
}


static m_bool sio_ini(VM* vm, DriverInfo* di)
{
  soundio = soundio_create();
  device_id = di->card;
  if (!soundio) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }

  int err = (backend == SoundIoBackendNone) ?
            soundio_connect(soundio) : soundio_connect_backend(soundio, backend);

  if (err) {
    fprintf(stderr, "Unable to connect to backend: %s\n", soundio_strerror(err));
    return -1;
  }
  soundio_flush_events(soundio);

  int selected_device_index = -1;
  if (device_id) {
    int device_count = soundio_output_device_count(soundio);
    for (int i = 0; i < device_count; i += 1) {
      struct SoundIoDevice *device = soundio_get_output_device(soundio, i);
      bool select_this_one = strcmp(device->id, device_id) == 0 && device->is_raw == raw;
      soundio_device_unref(device);
      if (select_this_one) {
        selected_device_index = i;
        break;
      }
    }
  } else {
    selected_device_index = soundio_default_output_device_index(soundio);
  }


  if (selected_device_index < 0) {
    fprintf(stderr, "Output device not found\n");
    return -1;
  }

  out_device = soundio_get_output_device(soundio, selected_device_index);
  if (!out_device) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }
  in_device = soundio_get_input_device(soundio, selected_device_index);
  if (!in_device) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }

  if(out_device->probe_error) {
    fprintf(stderr, "Cannot probe device: %s\n", soundio_strerror(out_device->probe_error));
    return -1;
  }
  if(in_device->probe_error) {
    fprintf(stderr, "Cannot probe device: %s\n", soundio_strerror(out_device->probe_error));
    return -1;
  }

  outstream = soundio_outstream_create(out_device);
  if (!outstream) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }

  outstream->write_callback = write_callback;
  outstream->underflow_callback = underflow_callback;
  outstream->name = "Gwion output";
  outstream->software_latency = 0;
  outstream->sample_rate = di->sr;


  instream = soundio_instream_create(in_device);
  if (!instream) {
    fprintf(stderr, "out of memory\n");
    return -1;
  }
  instream->read_callback = read_callback;
  instream->overflow_callback = overflow_callback;
  instream->name = "Gwion input";
  instream->software_latency = 0;
  instream->sample_rate = di->sr;

  if (soundio_device_supports_format(out_device, SoundIoFormatFloat64NE)) {
    outstream->format = SoundIoFormatFloat64NE;
    write_sample = write_sample_float64ne;
  } else if (soundio_device_supports_format(out_device, SoundIoFormatFloat32NE)) {
    outstream->format = SoundIoFormatFloat32NE;
    write_sample = write_sample_float32ne;
  } else if (soundio_device_supports_format(out_device, SoundIoFormatS32NE)) {
    outstream->format = SoundIoFormatS32NE;
    write_sample = write_sample_s32ne;
  } else if (soundio_device_supports_format(out_device, SoundIoFormatS16NE)) {
    outstream->format = SoundIoFormatS16NE;
    write_sample = write_sample_s16ne;
  } else {
    fprintf(stderr, "No suitable device format available.\n");
    return -1;
  }

  if (soundio_device_supports_format(in_device, SoundIoFormatFloat64NE)) {
    instream->format = SoundIoFormatFloat64NE;
    read_sample = read_sample_float64ne;
  } else if (soundio_device_supports_format(in_device, SoundIoFormatFloat32NE)) {
    instream->format = SoundIoFormatFloat32NE;
    read_sample = read_sample_float32ne;
  } else if (soundio_device_supports_format(in_device, SoundIoFormatS32NE)) {
    instream->format = SoundIoFormatS32NE;
    read_sample = read_sample_s32ne;
  } else if (soundio_device_supports_format(in_device, SoundIoFormatS16NE)) {
    instream->format = SoundIoFormatS16NE;
    read_sample = read_sample_s16ne;
  } else {
    fprintf(stderr, "No suitable device format available.\n");
    return -1;
  }

  if ((err = soundio_outstream_open(outstream))) {
    fprintf(stderr, "unable to open output device: %s", soundio_strerror(err));
    return -1;
  }
  if ((err = soundio_instream_open(instream))) {
    fprintf(stderr, "unable to open input device: %s", soundio_strerror(err));
    return -1;
  }
  if (outstream->layout_error) {
    fprintf(stderr, "unable to set output channel layout: %s\n", soundio_strerror(outstream->layout_error));
    return -1;
  }
  if (instream->layout_error) {
    fprintf(stderr, "unable to set input channel layout: %s\n", soundio_strerror(instream->layout_error));
    return -1;
  }
  outstream->userdata = vm;
  instream->userdata = vm;
  return 1;
}

void sio_run()
{
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
    soundio_wait_events(soundio);
}


/* static */ void sio_del(VM* vm)
{
  soundio_outstream_destroy(outstream);
  soundio_instream_destroy(instream);
  soundio_device_unref(in_device);
  soundio_device_unref(out_device);
  soundio_destroy(soundio);
}

Driver* sio_driver(VM* vm)
{
  Driver* d = malloc(sizeof(Driver));
  d->ini = sio_ini;
  d->run = sio_run;
  d->del = sio_del;
  vm->wakeup = sio_wakeup;
  return d;
}
