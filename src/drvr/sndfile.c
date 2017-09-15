#ifdef USE_DOUBLE
#define sf_write sf_write_double
#else
#define sf_write sf_write_float
#endif
#include <stdlib.h>
#include <sndfile.h>
#include "vm.h"
#include "driver.h"

static SNDFILE** sf;
static m_uint nchan;
static m_uint bufsize;
VM* vm;

static m_bool sndfile_ini(VM* v, DriverInfo* di) {
  nchan = di->chan;
  bufsize = di->bufsize;
  sf = calloc(nchan, sizeof(void*));
  char tmp[140];
  SF_INFO info;
  vm = v;
  info.samplerate = di->sr;
  info.channels = 1;
  info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
  m_uint chan;
  if(nchan == 1) {
    sprintf(tmp, "%s.wav", di->card);
    sf[0] = sf_open(tmp, SFM_WRITE, &info);
  } else for(chan = 0; chan < nchan; chan++) {
      sprintf(tmp, "%s_%02ld.wav", di->card, chan);
      sf[chan] = sf_open(tmp, SFM_WRITE, &info);
    }
  return 1;
}

static void sndfile_run(VM* vm, DriverInfo* di) {
  m_uint i, chan;
  sp_data* sp = vm->sp;
  m_float buf[nchan][bufsize];
  while(vm->is_running) {
    for(i = 0; i < bufsize; i++) {
      di->run(vm);
      for(chan = 0; chan < nchan; chan++)
        buf[chan][i] = sp->out[chan];
      sp->pos++;
    }
    for(chan = 0; chan < nchan; chan++)
      sf_write(sf[chan], (const m_float*)buf[chan], bufsize);
    GWION_CTL
  }
}

static void sndfile_del() {
  m_uint i;
  for(i = 0; i < nchan; i++)
    sf_close(sf[i]);
  free(sf);
}

void sndfile_driver(Driver* d, VM* vm) {
  d->ini = sndfile_ini;
  d->run = sndfile_run;
  d->del = sndfile_del;
  vm->wakeup = no_wakeup;
}

