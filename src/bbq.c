#include <stdlib.h>
#include "defs.h"
#include "vm.h"
#include "bbq.h"
#include "driver.h"

BBQ new_BBQ(VM* vm, DriverInfo* di, Driver** driver)
{
    BBQ a = malloc(sizeof(struct BBQ_));
    Driver* d = di->func(vm);
    CHECK_BO(d->ini(vm, di))
    sp_createn(&a->sp, di->out);
    memset(a->sp->out, 0, di->out * sizeof(SPFLOAT));
    a->in   = calloc(di->in, sizeof(SPFLOAT));
    a->n_in = di->in;
    a->sp->sr = di->sr;
    *driver = d;
    return a;
}
