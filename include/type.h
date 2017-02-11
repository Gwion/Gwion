#ifndef __TYPE
#define __TYPE

#include <complex.h>
#include <stdlib.h>
#include <stdio.h>
#include "symbol.h"
#ifndef __cplusplus
#include "namespace.h"
#include "env.h"
#endif
#include "value.h"
#include "dl.h"

/*
typedef struct
{
	f_tick tick;
	m_uint in, out, trig;
} UGenInfo;
*/
struct Type_ {
  m_str     name;
  m_uint    size;
  Type      parent;
  te_type   xid;
  NameSpace info;
  NameSpace owner;
  Func      func;
  Type      actual_type;
  m_uint    array_depth;
  Type      array_type;
  m_bool    is_complete;
  Class_Def def;
  VM_Object obj;
  m_str      doc;
  m_uint    obj_size;
  m_bool has_constructor;
  m_bool has_destructor;
  m_bool initialize; // hack for gack
//	UGenInfo ugen_info;
};

m_bool type_engine_check_prog(Env env, Ast ast, m_str str);
Type new_Type(Context context);
Type type_copy(Env env, Type type);
void free_Type(Type a);
Env type_engine_init(VM* vm);
/***
  UTILS
    ***/
void start_type_xid();
Value find_value(Type type, S_Symbol xid );
Type find_type(Env env, ID_List list);
int isprim(Type type);
int isa(Type var, Type parent);
int isres(Env env, S_Symbol xid, int pos);
int verify_array(Array_Sub array);
Type new_array_type(Env env, Type array_parent, m_uint depth, Type base_type, NameSpace owner_nspc);
Type find_common_anc(Type lhs, Type rhs );
m_str type_path(ID_List path );
m_bool add_global_value(Env env, m_str name, Type type, m_bool is_const,  void* value);
m_bool add_global_type(Env env, Type type);
m_bool add_global_value_double(Env env, m_str name, Type type, m_float data);
Kindof kindof(Type type);
m_int str2char( const m_str c, m_int linepos );
extern struct Type_ t_void;
extern struct Type_ t_int;
extern struct Type_ t_float;
extern struct Type_ t_complex, t_polar, t_vec3, t_vec4;
extern struct Type_ t_function;
extern struct Type_ t_func_ptr;
extern struct Type_ t_dur;
extern struct Type_ t_time;
extern struct Type_ t_now;
extern struct Type_ t_class;
extern struct Type_ t_std;
extern struct Type_ t_machine;
//extern struct Type_ t_template;
extern struct Type_ t_null;

// base classes
extern struct Type_ t_object, t_string, t_shred, t_event, t_ugen;
// concept
extern struct Type_ t_array;
// event child
//extern struct Type_ t_io, t_fileio, t_oscsend, t_oscin, t_evdev, t_midiout;
extern struct Type_ t_io, t_fileio;
extern struct Type_ t_vararg, t_varobj;
#endif
