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

static inline void context_add_type(Context context, Type type, VM_Object obj)
{  vector_append(context->new_types, (vtype)type); }
static inline void context_add_value(Context context, Value value, VM_Object obj)
{  vector_append(context->new_values, (vtype)value); }
static inline void context_add_func(Context context, Func func, VM_Object obj)
{  vector_append(context->new_funcs, (vtype)func); }
static inline void context_add_class(Context context, Value value, VM_Object obj)
{  vector_append(context->new_class, (vtype)value); }
m_bool load_context(Context context, Env env);
m_bool unload_context(Context context, Env env);
Context new_Context( Ast prog, char* filename );
void free_Context(Context context);
