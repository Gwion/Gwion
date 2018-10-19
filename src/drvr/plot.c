#include <stdlib.h>
#include "vm.h"
#include "driver.h"

static m_bool plot_ini(VM* vm __attribute__((unused)), DriverInfo* di) {
  FILE** sf = (FILE**)xcalloc(di->chan, sizeof(void*));
  char tmp[140];
  if(di->chan == 1) {
    sprintf(tmp, "%s.wav", di->card);
    sf[0] = fopen(tmp, "w");
  } else LOOP_OPTIM for(m_uint chan = 0; chan < di->chan; chan++) {
      sprintf(tmp, "%s_%02" UINT_F ".plot", di->card, chan);
      sf[chan] = fopen(tmp, "w");
  }
  di->data = sf;
  return 1;
}

static void plot_run(VM* vm __attribute__((unused)), DriverInfo* di) {
  m_uint i, chan;
  FILE** sf = (FILE**)di->data;
  while(vm->is_running) {
    for(i = 0; i < di->bufsize; i++) {
      di->run(vm);
      LOOP_OPTIM
      for(chan = 0; chan < di->chan; chan++)
        fprintf(sf[chan], "%"UINT_F" %g\n", vm->bbq->pos, (const m_float)vm->bbq->out[chan]);
      ++vm->bbq->pos;
    }
  }
}

static void plot_del(VM* vm __attribute__((unused)), DriverInfo* di) {
  FILE** sf = (FILE**)di->data;
  LOOP_OPTIM
  for(m_uint i = 0; i < di->chan; i++)
    fclose(sf[i]);
  free(sf);
}

void plot_driver(Driver* d) {
  d->ini = plot_ini;
  d->run = plot_run;
  d->del = plot_del;
}

