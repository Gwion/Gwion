#include "defs.h"
#include "absyn.h"
#include "map.h"
#include "oo.h"
#include "map_private.h"

struct Context_ {
  m_str     filename;
  Ast       tree;
  Nspc      nspc;
  Class_Def public_class_def;
  struct Map_ label;
  struct VM_Object_ obj;
};

Context new_context(Ast prog, char* filename);
void free_context(Context context);
m_bool load_context(Context context, Env env);
m_bool unload_context(Context context, Env env);
