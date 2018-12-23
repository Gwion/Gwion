#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "vm.h"
#include "env.h"
#include "operator.h"
#include "value.h"
#include "type.h"
#include "context.h"
#include "nspc.h"

#include "mpool.h"

static Switch new_switch() {
  Switch sw = mp_alloc(Switch);
  sw->cases = new_map(); // could be struct ?
  vector_init(&sw->exp);
  sw->vec = new_vector();
  return sw;
}

ANN static void free_switch(const Switch sw) {
//  if(!sw->ok)
//    free_map(sw->cases);
  free_vector(sw->vec); // only for dynamic
  vector_release(&sw->exp);
  mp_free(Switch, sw);
}

struct SwInfo_ {
  Stmt_Switch s;
  Type t;
  Func f;
};

ANN static Switch new_swinfo(const Env env, const Stmt_Switch stmt) {
  struct SwInfo_ *info = mp_alloc(SwInfo);
  info->s = stmt;
  info->t = env->class_def;
  info->f = env->func;
  const Switch sw = new_switch();
  map_set(&env->swi.map, (vtype)info, (vtype)sw);
  return sw;
}

ANN static inline m_bool swinfo_cmp(const struct SwInfo_ *i1, const struct SwInfo_ *i2) {
  return i1->s == i2->s && i1->t == i2->t && i1->f == i2->f;
}

ANN Switch swinfo_get(const Env env, const struct SwInfo_ *info) {
  for(m_uint i = 0; i < VLEN(&env->swi.map); ++i) {
    const struct SwInfo_ *key = (const struct SwInfo_*)VKEY(&env->swi.map, i);
    if(swinfo_cmp(key, info))
      return (Switch)VVAL(&env->swi.map, i);
  }
  return NULL;
}

ANN m_bool switch_add(const Env env, const Stmt_Switch stmt) {
  const struct SwInfo_ info = { stmt, env->class_def, env->func };
  Switch sw = (Switch)swinfo_get(env, &info) ?: new_swinfo(env, stmt);
  vector_add((Vector)&env->swi, (vtype)sw);
  return GW_OK;
}

ANN void switch_get(const Env env, const Stmt_Switch stmt) {
  const struct SwInfo_ info = { stmt, env->class_def, env->func };
  const Switch sw = swinfo_get(env, &info);
  vector_add((Vector)&env->swi, (vtype)sw);
}

void switch_reset(const Env env) {
  for(m_uint i = VLEN(&env->swi.map) + 1; --i;) {
    struct SwInfo_ *info = (struct SwInfo_ *)VKEY(&env->swi.map, i - 1);
    mp_free(SwInfo, info);
    Switch sw = (Switch)VVAL(&env->swi.map, i - 1);
    free_map(sw->cases);
    free_switch(sw);
  }
  vector_clear((Vector)&env->swi);
  map_clear(&env->swi.map);
}

ANN void switch_release(const Scope sw) {
  vector_release((Vector)sw);
  map_release(&sw->map);
//  scope_release(sw);
}

ANN void switch_expset(const Env env, const Exp e) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  vector_add(&sw->exp, (vtype)e);
}

ANN Exp switch_expget(const Env env) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  return (Exp)vector_at(&sw->exp, sw->iter++);
}

ANN m_bool switch_inside(const Env env, const uint pos) {
  if(!VLEN(&env->swi))
    ERR_B(pos, "case found outside switch statement.")
  return GW_OK;
}
ANN m_bool switch_dup(const Env env, const m_int value, const uint pos) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  if(map_get(sw->cases, (vtype)value))
    ERR_B(pos, "duplicated cases value %i", value)
  return GW_OK;
}

ANN void switch_pc(const Env env, const m_uint pc) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  vector_add(sw->vec, pc);
}

ANN void switch_dynpc(const Env env, const m_int val, const m_uint pc) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  map_set(sw->cases, val, pc);
}

ANN m_bool switch_dyn(const Env env) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  return vector_size(&sw->exp);
}

ANN m_bool switch_default(const Env env, const m_uint pc, const uint pos) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  if(sw->default_case_index)
    ERR_B(pos, "default case already defined")
  sw->default_case_index = pc;
  return GW_OK;
}

ANN Map switch_map(const Env env) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  return sw->cases;
}

ANN Vector switch_vec(const Env env) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  return sw->vec; // new_vector(); // dyn only
}

ANN m_uint switch_idx(const Env env) {
  const Switch sw = (Switch)vector_back((Vector)&env->swi);
  return sw->default_case_index;
}

ANN void switch_pop(const Env env) {
  vector_pop((Vector)&env->swi);
}

ANN void switch_end(const Env env) {
  const Switch sw = (Switch)vector_pop((Vector)&env->swi);
  const vtype index = VKEY(&env->swi.map, VLEN(&env->swi.map) - 1);
  map_remove(&env->swi.map, index);
  free_switch(sw);
//  return sw->ok = 1;
}
