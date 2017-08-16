#include <stdlib.h>
#include <string.h>
#include "env.h"
#include "context.h"
#include "type.h"
#include "err_msg.h"

Env new_env() {
  Env env = malloc(sizeof(struct Env_));
  env->global_nspc = new_nspc("global_nspc", "global_nspc");
//  env->user_nspc = NULL;
  vector_init(&env->breaks);
  vector_init(&env->conts);
  vector_init(&env->contexts);
  vector_init(&env->class_stack);
  vector_init(&env->nspc_stack);
  map_init(&env->known_ctx);
  env->type_xid = te_last; // ????????
  env->do_type_xid = 0;
  env_reset(env);
  return env;
}

void env_reset(Env env) {
  vector_clear(&env->nspc_stack);
  vector_add(&env->nspc_stack, (vtype)env->global_nspc);

//  if(env->user_nspc)
//    vector_add(env->nspc_stack, (vtype)env->user_nspc);

  vector_clear(&env->class_stack);
  vector_add(&env->class_stack, (vtype)NULL);

//  if(env->user_nspc)
//    env->curr = env->user_nspc;
//  else
  env->curr = env->global_nspc;
  env->class_def = NULL;
  env->func = NULL;
  env->class_scope = 0;
}

void free_env(Env a) {
  m_uint i;
  for(i = 0; i < map_size(&a->known_ctx); i++) {
    Context ctx = (Context)map_at(&a->known_ctx, i);
    REM_REF(ctx);
  }
  vector_release(&a->contexts);
  map_release(&a->known_ctx);

  for(i = 0; i < vector_size(&a->nspc_stack); i++) {
    Nspc  nspc = (Nspc)vector_pop(&a->nspc_stack);
    REM_REF(nspc);
  }
  vector_release(&a->nspc_stack);
  vector_release(&a->class_stack);
  vector_release(&a->breaks);
  vector_release(&a->conts);
  free(a);
}

m_bool env_push_class(Env env, Type type) {
  vector_add(&env->nspc_stack, (vtype)env->curr);
  env->curr = type->info;
  vector_add(&env->class_stack, (vtype)env->class_def);
  env->class_def = type;
  env->class_scope = 0;
  return 1;
}

m_bool env_pop_class(Env env) {
  env->class_def = (Type)vector_pop(&env->class_stack);
  env->curr = (Nspc)vector_pop(&env->nspc_stack);
  return 1;
}

m_bool env_add_value(Env env, m_str name, Type type, m_bool is_const, void* data) {
  Value v = new_value(type, name);
  ae_flag flag = ae_flag_checked | ae_flag_global | ae_flag_builtin | (is_const ? ae_flag_const : 0);
  v->flag = flag;
  v->ptr = data;
  v->owner = env->global_nspc;
  nspc_add_value(env->global_nspc, insert_symbol(name), v);
  return 1;
}

m_bool env_add_type(Env env, Type type) {
  if(type->name[0] != '@')
    CHECK_BB(name_valid(type->name));
  Type v_type = type_copy(env, &t_class);
  v_type->d.actual_type = type;
  INIT_OO(type, e_type_obj);
  SET_FLAG(type, ae_flag_builtin);
  nspc_add_type(env->curr, insert_symbol(type->name), type);
  Value v = new_value(v_type, type->name);
  SET_FLAG(v, ae_flag_checked | ae_flag_const | ae_flag_global | ae_flag_builtin);
  nspc_add_value(env->curr, insert_symbol(type->name), v);
  type->owner = env->curr;
  if(env->do_type_xid) {
    env->type_xid++;
    type->xid = env->type_xid;
  }
  return 1;
}

m_bool isres(Env env, S_Symbol xid, int pos) {
  m_str s = s_name(xid);
  if(!strcmp(s, "this") || !strcmp(s, "now") || !name2op(s)) {
    err_msg(TYPE_, 0, "%s is reserved.", s_name(xid));
    return 1;
  }
  return -1;
}
