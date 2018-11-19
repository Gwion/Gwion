#ifdef USE_DOUBLE
#define sf_write sf_write_double
#else
#define sf_write sf_write_float
#endif
#include <stdlib.h>
#include <sndfile.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "driver.h"

static m_bool sndfile_ini(VM* vm __attribute__((unused)), DriverInfo* di) {
  SNDFILE** sf = (SNDFILE**)xcalloc(di->chan, sizeof(void*));
  char tmp[140];
  SF_INFO info;
  info.samplerate = di->sr;
  info.channels = 1;
  info.format = SF_FORMAT_WAV | SF_FORMAT_PCM_24;
  m_uint chan;
  if(di->chan == 1) {
    sprintf(tmp, "%s.wav", di->card);
    sf[0] = sf_open(tmp, SFM_WRITE, &info);
  } else for(chan = 0; chan < di->chan; chan++) {
      sprintf(tmp, "%s_%02" UINT_F ".wav", di->card, chan);
      sf[chan] = sf_open(tmp, SFM_WRITE, &info);
  }
  di->data = sf;
  return 1;
}

static void sndfile_run(VM* vm, DriverInfo* di) {
  m_uint i, chan;
  SNDFILE** sf = (SNDFILE**)di->data;
  m_float buf[di->chan][di->bufsize];
  while(vm->is_running) {
    for(i = 0; i < di->bufsize; i++) {
      di->run(vm);
      for(chan = 0; chan < di->chan; chan++)
        buf[chan][i] = vm->bbq->out[chan];
      ++vm->bbq->pos;
    }
    for(chan = 0; chan < di->chan; chan++)
      sf_write(sf[chan], (const m_float*)buf[chan], di->bufsize);
  }
}

static void sndfile_del(VM* vm __attribute__((unused)), DriverInfo* di) {
  SNDFILE** sf = (SNDFILE**)di->data;
  for(m_uint i = 0; i < di->chan; i++)
    sf_close(sf[i]);
  free(sf);
}

void sndfile_driver(Driver* d) {
  d->ini = sndfile_ini;
  d->run = sndfile_run;
  d->del = sndfile_del;
}

