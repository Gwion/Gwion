#include "vm.h"
#include "driver.h"
#include <stdlib.h>
void free_Driver(Driver* d, VM* vm)
{
  d->del(vm, 1);
  free(d);
}
