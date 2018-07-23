#ifndef LANG_H
#define LANG_H

#include "object.h"
#include "array.h"
#include "emit.h"

ANEW M_Object new_shred(const VM_Shred shred);
ANN void broadcast(const M_Object o);
#endif
