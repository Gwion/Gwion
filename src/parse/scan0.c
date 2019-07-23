#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "func.h"
#include "nspc.h"
#include "vm.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"

static inline void add_type(const Env env, const Nspc nspc, const Type t) {
  map_set(&nspc->info->type->map, (m_uint)insert_symbol(t->name), (m_uint)t);
//  map_set((Map)vector_front((Vector)&nspc->info->type->ptr), (m_uint)insert_symbol(t->name), (m_uint)t);
}

ANN static Value mk_class(const Env env, const Type base) {
  const Type t = type_copy(env->gwion->mp, t_class);
  const Value v = new_value(env->gwion->mp, t, base->name);
  t->e->d.base_type = base;
  v->owner = base->e->owner;
  SET_FLAG(v, const | ae_flag_checked);
  nspc_add_value(base->e->owner, insert_symbol(base->name), v);
  return v;
}

ANN static inline m_bool scan0_defined(const Env env, const Symbol s, const loc_t pos) {
  if(nspc_lookup_type0(env->curr, s))
    ERR_B(pos, _("type '%s' already defined"), s_name(s));
  return already_defined(env, s, pos);
}

static void fptr_def(const Env env, const Stmt_Fptr stmt) {
  const Func_Def def = new_func_def(env->gwion->mp, new_func_base(env->gwion->mp, stmt->base->td, stmt->base->xid, stmt->base->args),
    NULL,stmt->base->td->flag, loc_cpy(env->gwion->mp, stmt_self(stmt)->pos));
  stmt->base->func = new_func(env->gwion->mp, s_name(stmt->base->xid), def);
  stmt->value->d.func_ref = stmt->base->func;
  stmt->base->func->value_ref = stmt->value;
  stmt->type->e->d.func = stmt->base->func;
  def->base->tmpl = stmt->base->tmpl;
  def->base->func = stmt->base->func;
}

ANN m_bool scan0_stmt_fptr(const Env env, const Stmt_Fptr stmt) {
  CHECK_BB(env_access(env, stmt->base->td->flag, stmt_self(stmt)->pos))
  CHECK_BB(scan0_defined(env, stmt->base->xid, td_pos(stmt->base->td)));
  const m_str name = s_name(stmt->base->xid);
  const Type t = new_type(env->gwion->mp, t_fptr->xid, name, t_fptr);
  t->e->owner = !(!env->class_def && GET_FLAG(stmt->base->td, global)) ?
    env->curr : env->global_nspc;
  t->nspc = new_nspc(env->gwion->mp, name);
  t->flag = stmt->base->td->flag;
  stmt->type = t;
  stmt->value = mk_class(env, t);
  stmt->value->owner = env->curr;
  stmt->value->owner_class = env->class_def;
  fptr_def(env, stmt);
  if(env->class_def)
    fptr_assign(env, stmt);
  SET_FLAG(stmt->value, func);
  add_type(env, t->e->owner, t);
  return GW_OK;
}

ANN static void typedef_simple(const Env env, const Stmt_Type stmt, const Type base) {
  const Type t = new_type(env->gwion->mp, ++env->scope->type_xid, s_name(stmt->xid), base);
  t->size = base->size;
  const Nspc nspc = (!env->class_def && GET_FLAG(stmt->ext, global)) ?
  env->global_nspc : env->curr;
  add_type(env, nspc, t);
  t->e->owner = nspc;
  stmt->type = t;
  if(base->nspc)
    ADD_REF((t->nspc = base->nspc));
  t->flag = stmt->ext->flag | ae_flag_checked;
  if(stmt->ext->array && !stmt->ext->array->exp)
    SET_FLAG(t, empty);
}

ANN static m_bool typedef_complex(const Env env, const Stmt_Type stmt, const Type base) {
  const ae_flag flag = base->e->def ? base->e->def->flag : 0;
  const Class_Def cdef = new_class_def(env->gwion->mp, flag, stmt->xid, stmt->ext, NULL,
    loc_cpy(env->gwion->mp, td_pos(stmt->ext)));
  CHECK_BB(scan0_class_def(env, cdef))
  stmt->type = cdef->base.type;
  cdef->base.tmpl = stmt->tmpl;
  return GW_OK;
}

ANN static void typedef_fptr(const Env env, const Stmt_Type stmt, const Type base) {
  stmt->type = type_copy(env->gwion->mp, base);
  stmt->type->name = s_name(stmt->xid);
  stmt->type->e->parent = base;
  add_type(env, env->curr, stmt->type);
  mk_class(env, stmt->type);
  if(base->e->d.func->def->base->tmpl)
    SET_FLAG(stmt->type, func);
}

ANN m_bool scan0_stmt_type(const Env env, const Stmt_Type stmt) {
  CHECK_BB(env_access(env, stmt->ext->flag, stmt_self(stmt)->pos))
  DECL_OB(const Type, base, = stmt->tmpl ? find_type(env, stmt->ext->xid) : known_type(env, stmt->ext))
  CHECK_BB(scan0_defined(env, stmt->xid, td_pos(stmt->ext)))
  if(isa(base, t_function) < 0) {
    if(!stmt->ext->types && (!stmt->ext->array || !stmt->ext->array->exp))
      typedef_simple(env, stmt, base);
    else
      CHECK_BB(typedef_complex(env, stmt, base))
  } else
    typedef_fptr(env, stmt, base);
  SET_FLAG(stmt->type, typedef);
  return GW_OK;
}

ANN m_bool scan0_enum_def(const Env env, const Enum_Def edef) {
  CHECK_BB(env_storage(env, edef->flag, edef->pos))
  if(edef->xid) {
    const Value v = nspc_lookup_value1(env->curr, edef->xid);
    if(v)
      ERR_B(edef->pos, _("'%s' already declared as variable of type '%s'."),
        s_name(edef->xid),  v->type->name)
    CHECK_BB(scan0_defined(env, edef->xid, edef->pos))
  }
  const Type t = type_copy(env->gwion->mp, t_int);
  t->xid = ++env->scope->type_xid;
  t->name = edef->xid ? s_name(edef->xid) : "int";
  t->e->parent = t_int;
  const Nspc nspc = GET_FLAG(edef, global) ? env->global_nspc : env->curr;
  t->e->owner = nspc;
  edef->t = t;
  if(edef->xid) {
    add_type(env, nspc, t);
    mk_class(env, t);
  }
  return GW_OK;
}

ANN static Type union_type(const Env env, const Nspc nspc, const Symbol s, const m_bool add) {
  const m_str name = s_name(s);
  const Type t = type_copy(env->gwion->mp, t_union);
  t->xid = ++env->scope->type_xid;
  t->name = name;
  t->nspc = new_nspc(env->gwion->mp, name);
  t->nspc->parent = nspc;
  t->nspc->is_union = 1;
  t->e->owner = nspc;
  t->e->parent = t_union;
  if(add) {
    add_type(env, nspc, t);
    mk_class(env, t);
  }
  return t;
}

ANN m_bool scan0_stmt_union(const Env env, const Stmt_Union stmt) {
  CHECK_BB(env_storage(env, stmt->flag, stmt_self(stmt)->pos))
  const m_uint scope = !GET_FLAG(stmt, global) ? env->scope->depth :
      env_push_global(env);
  if(stmt->xid) {
    CHECK_BB(scan0_defined(env, stmt->xid, stmt_self(stmt)->pos))
    const Nspc nspc = !GET_FLAG(stmt, global) ?
      env->curr : env->global_nspc;
    const Type t = union_type(env, nspc, stmt->type_xid ?: stmt->xid,
       !!stmt->type_xid);
    stmt->value = new_value(env->gwion->mp, t, s_name(stmt->xid));
    stmt->value->owner_class = env->class_def;
    stmt->value->owner = nspc;
    nspc_add_value(nspc, stmt->xid, stmt->value);
    add_type(env, nspc, t);
    SET_FLAG(stmt->value, checked | stmt->flag);
    if(env->class_def && !GET_FLAG(stmt, static)) {
      SET_FLAG(stmt->value, member);
      SET_FLAG(stmt, member);
    }
  } else if(stmt->type_xid) {
    CHECK_BB(scan0_defined(env, stmt->type_xid, stmt_self(stmt)->pos))
    const Nspc nspc = !GET_FLAG(stmt, global) ?
      env->curr : env->global_nspc;
    stmt->type = union_type(env, nspc, stmt->type_xid, 1);
    SET_FLAG(stmt->type, checked);
  } else {
    const Nspc nspc = !GET_FLAG(stmt, global) ?
      env->curr : env->global_nspc;
    const Type t = union_type(env, nspc, insert_symbol("union"), 1);
    stmt->value = new_value(env->gwion->mp, t, "union");
    stmt->value->owner_class = env->class_def;
    stmt->value->owner = nspc;
    nspc_add_value(nspc, stmt->xid, stmt->value);
    char c[16];
    sprintf(c, "%p", stmt);
    nspc_add_type(nspc, insert_symbol(c), t);
    SET_FLAG(stmt->value, checked | stmt->flag);
  }
  if(stmt->tmpl) {
    if(tmpl_base(stmt->tmpl)) {
      const Class_Def cdef = new_class_def(env->gwion->mp, stmt->flag, stmt->type_xid,
          NULL, (Class_Body)stmt->l, loc_cpy(env->gwion->mp, stmt_self(stmt)->pos));
      stmt->type->e->def = cdef;
      cdef->base.tmpl = stmt->tmpl;
      cdef->base.type = stmt->type;
      cdef->list = stmt->l;
      SET_FLAG(cdef, union);
      SET_FLAG(stmt->type, pure);
      SET_FLAG(stmt, template);
      SET_FLAG(stmt->type, template);
    }
    SET_FLAG(stmt->type, union);
  }
  if(GET_FLAG(stmt, global))
    env_pop(env, scope);
  return GW_OK;
}

ANN static m_bool scan0_stmt_switch(const Env env, const Stmt_Switch stmt);
ANN static m_bool scan0_stmt_code(const Env env, const Stmt_Code stmt);
ANN static m_bool scan0_stmt(const Env env, const Stmt stmt) {
  if(stmt->stmt_type == ae_stmt_fptr)
    return scan0_stmt_fptr(env, &stmt->d.stmt_fptr);
  if(stmt->stmt_type == ae_stmt_type)
    return scan0_stmt_type(env, &stmt->d.stmt_type);
  if(stmt->stmt_type == ae_stmt_union)
    return scan0_stmt_union(env, &stmt->d.stmt_union);
  if(stmt->stmt_type == ae_stmt_code)
    return scan0_stmt_code(env, &stmt->d.stmt_code);
  if(stmt->stmt_type == ae_stmt_switch)
    return scan0_stmt_switch(env, &stmt->d.stmt_switch);
  return GW_OK;
}

ANN static m_bool scan0_stmt_list(const Env env, Stmt_List l);
ANN static m_bool scan0_stmt_switch(const Env env, const Stmt_Switch stmt) {
  return scan0_stmt(env, stmt->stmt);
}
ANN static m_bool scan0_stmt_code(const Env env, const Stmt_Code stmt) {
  return stmt->stmt_list ? scan0_stmt_list(env, stmt->stmt_list) : GW_OK;
}

ANN static m_bool scan0_stmt_list(const Env env, Stmt_List l) {
  do CHECK_BB(scan0_stmt(env, l->stmt))
  while((l = l->next));
  return GW_OK;
}

ANN static m_bool scan0_class_def_pre(const Env env, const Class_Def cdef) {
  CHECK_BB(env_storage(env, cdef->flag, cdef->pos))
  if(GET_FLAG(cdef, global)) {
    vector_add(&env->scope->nspc_stack, (vtype)env->curr);
    env->curr = env->global_nspc;
  }
  CHECK_BB(scan0_defined(env, cdef->base.xid, cdef->pos))
  CHECK_BB(isres(env, cdef->base.xid, cdef->pos))
  return GW_OK;
}

ANN static Type scan0_class_def_init(const Env env, const Class_Def cdef) {
  const Type t = new_type(env->gwion->mp, ++env->scope->type_xid, s_name(cdef->base.xid), t_object);
  t->e->owner = env->curr;
  t->nspc = new_nspc(env->gwion->mp, t->name);
  t->nspc->parent = (GET_FLAG(cdef, global) ? env_nspc(env) : env->curr);
  t->e->def = cdef;
  t->flag = cdef->flag;
  if(!strstr(t->name, "<"))
    add_type(env, t->e->owner, t);
  if(cdef->base.tmpl) {
    SET_FLAG(t, template);
    SET_FLAG(cdef, template);
  }
  if(cdef->base.ext && cdef->base.ext->array)
    SET_FLAG(t, typedef);
  return t;
}

ANN static m_bool scan0_func_def(const Env env, const Func_Def fdef) {
  return fdef->d.code ? scan0_stmt(env, fdef->d.code) : GW_OK;
}

ANN static m_bool scan0_section(const Env env, const Section* section) {
  if(section->section_type == ae_section_stmt)
    return scan0_stmt_list(env, section->d.stmt_list);
  if(section->section_type == ae_section_class)
    return scan0_class_def(env, section->d.class_def);
  if(section->section_type == ae_section_func)
    return scan0_func_def(env, section->d.func_def);
  if(section->section_type == ae_section_enum)
    return scan0_enum_def(env, section->d.enum_def);
  return GW_OK;
}

ANN m_bool scan0_class_def(const Env env, const Class_Def cdef) {
  CHECK_BB(scan0_class_def_pre(env, cdef))
  CHECK_OB((cdef->base.type = scan0_class_def_init(env, cdef)))
  SET_FLAG(cdef->base.type, scan0);
  if(cdef->body) {
    int call = cdef->base.tmpl && !cdef->base.tmpl->call;
    if(call)cdef->base.tmpl->call = (Type_List)1;
      CHECK_BB(env_body(env, cdef, scan0_section))
    if(call)cdef->base.tmpl->call = NULL;
  }
  (void)mk_class(env, cdef->base.type);
  if(GET_FLAG(cdef, global))
    env->curr = (Nspc)vector_pop(&env->scope->nspc_stack);
  return GW_OK;
}

ANN m_bool scan0_ast(const Env env, Ast ast) {
  do CHECK_BB(scan0_section(env, ast->section))
  while((ast = ast->next));
  return GW_OK;
}
