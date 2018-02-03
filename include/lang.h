#ifndef LANG_H
#define LANG_H

#include "object.h"
#include "array.h"
#include "emit.h"

M_Object new_shred(VM* vm, VM_Shred shred);

extern void array_ctor(M_Object o,  VM_Shred shred);
extern m_int o_string_data;
extern m_int o_event_shred;
extern m_int o_shred_me;

void broadcast(M_Object o);
void release(M_Object obj, VM_Shred shred);
m_bool initialize_object(M_Object object, Type type);
#endif
