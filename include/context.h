#include "defs.h"
#include "absyn.h"
#include "map.h"
#include "oo.h"

struct Context_ {
  m_str     filename;
  Ast       tree;
  Nspc      nspc;
  Class_Def public_class_def;
  Vector    new_funcs;
//  Vector    new_types;
  Vector    new_values;
  struct VM_Object_ obj;
};

Context new_context(Ast prog, char* filename);
void free_context(Context context);
m_bool load_context(Context context, Env env);
m_bool unload_context(Context context, Env env);
/*
static inline void context_add_type(Context context, Type type, VM_Object obj) {
  vector_add(context->new_types, (vtype)type);
} */
static inline void context_add_value(Context context, Value value, VM_Object obj) {
  vector_add(context->new_values, (vtype)value);
}
static inline void context_add_func(Context context, Func func, VM_Object obj) {
  vector_add(context->new_funcs, (vtype)func);
}
