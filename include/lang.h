#ifndef LANG_H
#define LANG_H

#include "object.h"
#include "array.h"
#include "emit.h"

ANEW M_Object new_shred(VM_Shred shred);

extern m_int o_string_data;
extern m_int o_event_shred;
extern m_int o_shred_me;

ANN void broadcast(M_Object o);
__attribute__((nonnull(2)))
void release(M_Object obj, VM_Shred shred);
m_bool initialize_object(M_Object object, Type type)__attribute__((hot, nonnull));
#endif
