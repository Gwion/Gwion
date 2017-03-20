#include "defs.h"
#include "map.h"
#include "oo.h"

struct Context_ {
  m_str     filename;
  Ast       tree;
  NameSpace nspc;
  Class_Def public_class_def;
  Vector    new_funcs;
  Vector    new_values;
  Vector    new_types;
  Vector    new_class;
  VM_Object obj;
};

void context_add_type(Context context, Type type, VM_Object obj);
void context_add_value(Context context, Value value, VM_Object obj);
void context_add_func(Context context, Func func, VM_Object obj);
void context_add_class(Context context, Value value, VM_Object obj);
int load_context(Context context, Env env);
int unload_context(Context context, Env env);
Context new_Context( Ast prog, char* filename );
void free_Context(Context context);
