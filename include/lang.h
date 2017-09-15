#ifndef LANG_H
#define LANG_H

#include "object.h"
#include "array.h"
#include "emit.h"
/*
extern m_bool import_int(Importer importer);
extern m_bool import_array(Importer importer);
extern m_bool import_float(Importer importer);
extern m_bool import_complex(Importer importer);
extern m_bool import_vec3(Importer importer);
extern m_bool import_vec4(Importer importer);
extern m_bool import_shred(Importer importer);
extern m_bool import_machine(Importer importer);
extern m_bool import_object(Importer importer);
extern m_bool import_vararg(Importer importer);
extern m_bool import_string(Importer importer);
extern m_bool import_event(Importer importer);
extern m_bool import_fileio(Importer importer);
extern m_bool import_std(Importer importer);
extern m_bool import_math(Importer importer);
extern m_bool import_soundpipe(Importer importer);
*/
M_Object new_shred(VM* vm, VM_Shred shred);

extern void array_ctor(M_Object o,  VM_Shred shred);
extern m_int o_string_data;
extern m_int o_event_shred;
extern m_int o_shred_me;

void broadcast(M_Object o);
void release(M_Object obj, VM_Shred shred);
m_bool initialize_object(M_Object object, Type type);
#endif
