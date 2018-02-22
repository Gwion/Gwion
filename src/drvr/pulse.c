#include <pulse/simple.h>
#include "vm.h"
#include "driver.h"

#define BUFSIZE 1024

static pa_simple* in;
static pa_simple* out;

static const pa_sample_spec ss = { PA_SAMPLE_FLOAT32NE, 48000, 2};

static pa_simple* pulse_open(m_uint direction) {
  return pa_simple_new(NULL, "Gwion", direction,
    NULL, "Gwion", &ss, NULL, NULL, NULL);
}

static m_bool pulse_ini(VM* vm, DriverInfo* di) {
  CHECK_OB((out = pulse_open(PA_STREAM_PLAYBACK)))
  CHECK_OB((in  = pulse_open(PA_STREAM_RECORD)))
  return 1;
}

static void pulse_run(VM* vm, DriverInfo* di) {
  int error;
  sp_data* sp = vm->sp;
  while(vm->is_running) {
    m_uint frame, chan;
    float  in_data[BUFSIZE * sp->nchan];
    float out_data[BUFSIZE * sp->nchan];
    if(pa_simple_read(in, in_data, sizeof(in_data), &error) < 0)
      return;
    for(frame = 0; frame < BUFSIZE; frame++) {
      for(chan = 0; chan < sp->nchan; chan++)
        vm->in[chan] = in_data[frame * sp->nchan + chan];
      di->run(vm);
      for(chan = 0; chan < sp->nchan; chan++)
        out_data[frame * sp->nchan + chan] = (float)sp->out[chan];
      sp->pos++;
    }
    if(pa_simple_write(out, out_data, sizeof(out_data), &error) < 0)
      return;
  }
  if(pa_simple_drain(out, &error) < 0)
    return;
}

static void pulse_del(VM* vm, DiverInfo* di) {
  if(in)
    pa_simple_free(in);
  if(out)
    pa_simple_free(out);
}

void pulse_driver(Driver* d) {
  d->ini = pulse_ini;
  d->run = pulse_run;
  d->del = pulse_del;
}
