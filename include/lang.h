#ifndef LANG_H
#define LANG_H

#include "object.h"
#include "array.h"
#include "emit.h"
//#include "ugen.h"

extern m_bool import_int(Env env);
extern m_bool import_array(Env env);
extern m_bool import_float(Env env);
extern m_bool import_complex(Env env);
extern m_bool import_vec3(Env env);
extern m_bool import_vec4(Env env);
extern m_bool import_shred(Env env);
extern m_bool import_machine(Env env);
extern m_bool import_object(Env env);
extern m_bool import_string(Env env);
extern m_bool import_event(Env env);
extern m_bool import_fileio(Env env);
extern m_bool import_lib(Env env);
extern m_bool import_analys(Env env);


//extern m_bool free_oscin();
M_Object new_Shred(VM* vm, VM_Shred shred);
extern m_int o_fileio_file;
extern m_uint o_object_doc;

extern void array_ctor(M_Object o,  VM_Shred shred);
extern INSTR(Array_Append);
extern m_int o_string_data;
extern m_int o_event_shred;
extern m_int o_shred_me;

void broadcast(M_Object o);
void release(M_Object obj, VM_Shred shred);
m_bool initialize_object(M_Object object, Type type);
#endif
