#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "traverse.h"
#include "template.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "parse.h"
#include "match.h"
#include "specialid.h"
#include "tmp_resolve.h"
#include "partial.h"
#include "array.h"

ANN bool check_stmt(const Env env, Stmt*);
ANN bool check_class_def(const Env env, const Class_Def class_def);

ANN static Type check_internal(const Env env, const Symbol sym, Exp* e,
                               const Type t) {
  struct Implicit  imp = {.e = e, .t = t, .loc = e->loc};
  struct Op_Import opi = {.op   = sym,
                          .lhs  = e->type,
                          .rhs  = t,
                          .data = (uintptr_t)&imp,
                          .loc  = e->loc};
  return op_check(env, &opi);
}

ANN bool check_implicit(const Env env, Exp* e, const Type t) {
  if (e->type == t) return true;
  if (isa(e->type, t)) return true;
  const Symbol sym = insert_symbol("@implicit");
  return !!(e->cast_to = check_internal(env, sym, e, t));
}

ANN bool check_subscripts(Env env, const Array_Sub array,
                            const bool is_decl) {
  Exp*    e     = array->exp;
  CHECK_B(check_exp(env, e));
  m_uint depth = 0;
  bool ok = true;
  do {
    if (is_decl) {
      if(!check_implicit(env, e, env->gwion->type[et_int]))
        ERR_OK_NODE(ok, e, array->exp->loc, _("invalid array declaration index type."));
    }
  } while (++depth && (e = e->next));
  if (depth != array->depth)
    ERR_B(array->exp->loc, _("invalid array access expression."));
  return ok;
}

ANN static inline bool check_exp_decl_parent(const Env      env,
                                               const Var_Decl *var) {
  const Value value = find_value(env->class_def->info->parent, var->tag.sym);
  if (value) {
    env_err(env, var->tag.loc, _("Value defined in parent class"));
    defined_here(value);
    return false;
  }
  return true;
}

ANN static m_uint get_decl_size(const Env env, const Value v) {
  if(safe_tflag(env->class_def, tflag_packed) && v->type->actual_size)
    return v->type->actual_size;
  return v->type->size;

}

#define describe_check_decl(a, b, flag)                            \
  ANN static inline void decl_##a(const Env env, const Value v) {  \
    const Nspc nspc = env->curr;                                   \
    flag;                                                          \
    v->from->offset = nspc->b;                                     \
    nspc->b += get_decl_size(env, v);                              \
  }

describe_check_decl(member, offset, v->vflag |= vflag_member);
describe_check_decl(static, class_data_size, SET_FLAG(v, static));

ANN static void var_effects(const Env env, const Type t, const Symbol sym, const loc_t loc) {
  if (t->info->parent) var_effects(env, t->info->parent, sym, loc);
  if(!tflag(t, tflag_ctor)) return;
  const Value ctor = nspc_lookup_value0(t->nspc, sym);
  if(!ctor) return; // bit sus
  const Func f = ctor->d.func_ref;
  const Vector v = &f->def->base->effects;
  if(!v->ptr) return;
  for (m_uint i = 0; i < vector_size(v); i++)
    env_add_effect(env, (Symbol)vector_at(v, i), loc);
}

ANN static bool check_var(const Env env, const Var_Decl *var) {
  if (env->class_def && !env->scope->depth && env->class_def->info->parent)
    CHECK_B(check_exp_decl_parent(env, var));
  var_effects(env, var->value->type, insert_symbol("@ctor"), var->tag.loc);
  return true;
}

ANN static bool check_var_td(const Env env, const Var_Decl *var,
                               Type_Decl *const td) {
  const Value v = var->value;
  if (env->class_def) {
    if (vflag(v, vflag_member)) {
      decl_member(env, v);
      if (env->class_def->info->tuple) tuple_info(env, v);
    } else if (GET_FLAG(td, static))
      decl_static(env, v);
  }
  return true;
}

ANN static bool check_decl(const Env env, const Exp_Decl *decl) {
  const Var_Decl *vd = &decl->var.vd;
  CHECK_B(check_var(env, vd));
  CHECK_B(check_var_td(env, vd, decl->var.td));
  if (decl->var.td->array && decl->var.td->array->exp) {
    CHECK_B(check_subscripts(env, decl->var.td->array, true));
    if (GET_FLAG(array_base(decl->type), abstract))
      CHECK_B(check_array_instance(env, decl->var.td, decl->args));
  }
  valid_value(env, vd->tag.sym, vd->value);
  // set_vflag(var->value, vflag_used));
  return true;
}

ANN /*static inline*/ bool ensure_check(const Env env, const Type t) {
  if (tflag(t, tflag_check) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return true;
  if(!tflag(t, tflag_tmpl)) return true;
  struct EnvSet es = {.env   = env,
                      .data  = env,
                      .func  = (_envset_func)check_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_check};
  return envset_run(&es, t);
}

ANN bool ensure_traverse(const Env env, const Type t) {
  if (tflag(t, tflag_check) || !(tflag(t, tflag_cdef) || tflag(t, tflag_udef)))
    return true;
  if(!tflag(t, tflag_tmpl)) return true;
  struct EnvSet es = {.env   = env,
                      .data  = env,
                      .func  = (_envset_func)traverse_cdef,
                      .scope = env->scope->depth,
                      .flag  = tflag_check};
  return envset_run(&es, t);
}

ANN static inline bool inferable(const Env env, const Type t,
                                   const loc_t loc) {
  if (!tflag(t, tflag_infer)) return true;
  ERR_B(loc, _("can't infer type."));
}

ANN Type check_exp_decl(const Env env, Exp_Decl *const decl) {
  if (decl->var.td->array && decl->var.td->array->exp)
    CHECK_O(check_exp(env, decl->var.td->array->exp));
  if (decl->args) {
    Exp* e = new_exp_unary2(env->gwion->mp, insert_symbol("new"), cpy_type_decl(env->gwion->mp, decl->var.td), decl->args, decl->var.td->tag.loc);
    CHECK_O(check_exp(env, e));
    decl->type = e->type;
    decl->var.vd.value->type = e->type; // TODO: beware rc shenanigans
    decl->args = e;
    e->ref = exp_self(decl);
  }
  if (decl->var.td->tag.sym == insert_symbol("auto")) { // should be better
    CHECK_O(scan1_exp(env, exp_self(decl)));
    CHECK_O(scan2_exp(env, exp_self(decl)));
  }
  if (!decl->type) ERR_O(decl->var.td->tag.loc, _("can't find type"));
  {
    CHECK_O(ensure_check(env, decl->type));
    if(!inferable(env, decl->type, decl->var.td->tag.loc)) {
      if(!tflag(decl->type, tflag_check) && decl->type->ref > 1 && env->class_def && !env->scope->depth)
        type_remref(decl->type, env->gwion);
      return NULL;
   }
  }
  const bool global = GET_FLAG(decl->var.td, global);
  const m_uint scope  = !global ? env->scope->depth : env_push_global(env);
  const bool ret    = check_decl(env, decl);
  if (global) {
    env_pop(env, scope);
    set_vflag(decl->var.vd.value, vflag_direct);
  }
  env_weight(env, 1 + is_object(env->gwion, decl->type));
  return ret ? decl->var.vd.value->type : NULL;
}

ANN static bool check_collection(const Env env, Type type, Exp* e,
                                   const loc_t loc) {
  const Type common = find_common_anc(e->type, type);
  if (common) return true;
  if (check_implicit(env, e, type)) return true;

  char fst[20 + strlen(type->name)];
  sprintf(fst, "expected `{+/}%s{0}`", type->name);
  gwlog_error(_("literal contains incompatible types"), fst,
              env->name, loc, 0);
  gwlog_hint(_("the first element determines the type"), env->name, loc);
  char sec[16 + strlen(e->type->name)];
  sprintf(sec, "got `{+/}%s{0}`", e->type->name);
  gwlog_related(sec, env->name, e->loc);
  return false;
}

ANN static inline Type prim_array_match(const Env env, Exp* e) {
  const Type  type = e->type;
  bool        err  = false;
  const loc_t loc  = e->loc;
  do
    if (!check_collection(env, type, e, loc)) err = true;
  while ((e = e->next));
  if (!err) return array_type(env, array_base_simple(type), type->array_depth + 1, loc);
  env_set_error(env, true);
  return NULL;
}

ANN static Type check_prim_array(const Env env, const Array_Sub *data) {
  const Array_Sub array = *data;
  Exp*       e     = array->exp;
//  if (!e)
//    ERR_O(prim_pos(data), _("must provide values/expressions for array [...]"));
  CHECK_O(check_exp(env, e));
  env_weight(env, 1);
  return array->type = prim_array_match(env, e);
}

ANN static bool check_range(const Env env, Range *range) {
  if (range->start) CHECK_B(check_exp(env, range->start));
  if (range->end) CHECK_B(check_exp(env, range->end));
  if (range->start && range->end) {
    if (!isa(range->end->type, range->start->type))
      ERR_B(range->start->loc, _("range types do not match"));
  }
  return true;
}

ANN static Type check_prim_range(const Env env, Range **data) {
  Range *range = *data;
  CHECK_O(check_range(env, range));
  env_weight(env, 1);
  Exp* e = range->start ?: range->end;
  assert(e);
  const Symbol     sym = insert_symbol("[:]");
  struct Op_Import opi = {.op   = sym,
                          .lhs  = e->type,
                          .loc  = e->loc,
                          .data = (uintptr_t)prim_exp(data)};
  return op_check(env, &opi);
}

ANN static Type check_prim_dict(const Env env, Exp* *data) {
  Exp* base = *data;
  CHECK_O(check_exp(env, base));
  const Type  key = base->type;
  const Type  val = base->next->type;
  bool        err = false;
  const loc_t loc = base->loc;
  Exp* e = base;
  env_weight(env, 1);
  do {
    if (!check_collection(env, key, e, loc)) err = true;
    e = e->next;
    if (!check_collection(env, val, e, loc)) err = true;
  } while ((e = e->next));
  if (!err) return dict_type(env->gwion, key, val, loc);
  env_set_error(env, true); return NULL;
}

ANN bool not_from_owner_class(const Env env, const Type t, const Value v,
                                const loc_t loc) {
  if (!v->from->owner_class || !isa(t, v->from->owner_class)) {
    if(!is_class(env->gwion, v->type))
      ERR_B(loc, _("'%s' from owner namespace '%s' used in '%s'."), v->name,
            v->from->owner ? v->from->owner->name : "?", t->name);
  }
  return true;
}

ANN static inline Value get_value(const Env env, const Symbol sym) {
  const Value value = nspc_lookup_value1(env->curr, sym);
  if(value) {
    if(value->from->owner_class && is_func(env->gwion, value->from->owner_class))
      return value;
    if (!value->from->owner_class || isa(env->class_def, value->from->owner_class))
      return value;
    if(env->class_def) {
      if (isa(env->class_def, value->from->owner_class) || value == env->class_def->info->value)
        return value;

    }
  }
  if (env->func && env->func->def->base->values) {
    DECL_O(const Value, v, = upvalues_lookup(env->func->def->base->values, sym));
    if(fbflag(env->func->def->base, fbflag_lambda))
      CHECK_O(not_upvalue(env, v));
    return v;
  }
  return NULL;
}

ANN static inline bool is_value_global(const Env env, const Value v) {
  if(GET_FLAG(v, global)) return true;
  return from_global_nspc(env, v->from->owner);
}

ANN static Value check_non_res_value(const Env env, const Symbol *data) {
  const Symbol var   = *data;
  const Value  value = get_value(env, var);
  if (env->class_def) {
    if (value) {
      if(value->from->owner_class)
      CHECK_O(not_from_owner_class(env,
        env->class_def, value, prim_pos(data)));
    }
    const Value v = value ?: find_value(env->class_def, var);
    if (v) {
      if (env->func && GET_FLAG(env->func->def->base, static) &&
          vflag(v, vflag_member))
        ERR_O(prim_pos(data),
              _("non-static member '%s' used from static function."),
              s_name(var));
    }
    return v;
  } else if (SAFE_FLAG(env->class_def, global) ||
             (env->func && GET_FLAG(env->func->def->base, global))) {
    if (!value || !(is_value_global(env, value) || vflag(value, vflag_arg)))
      ERR_O(prim_pos(data),
            _("non-global variable '%s' used from global function/class."),
            s_name(var));
  } else if(env->func && fbflag(env->func->def->base, fbflag_locale)) {
    if(!is_func(env->gwion, value->type) && value->from->owner && !from_global_nspc(env, value->from->owner)) // is_callable
      ERR_O(prim_pos(data), _("invalid variable access from locale definition"));
  }
  return value;
}

ANN static Type check_dot(const Env env, const Exp_Dot *member) {
  struct Op_Import opi = {.op   = insert_symbol("."),
                          .lhs  = member->base->type,
                          .data = (uintptr_t)member,
                          .loc  = exp_self(member)->loc};
  env_weight(env, 1);
  return op_check(env, &opi);
}

ANN static bool check_upvalue(const Env env, const Exp_Primary *prim, const Value v) {
  if(not_upvalue(env, v))
    return true;
  gwlog_error(_("value not in lambda scope"), NULL, env->name, exp_self(prim)->loc, 4242);
  declared_here(v);
  gwlog_hint(_("{0} try adding it to capture list"), env->name, exp_self(prim)->loc);
  env_set_error(env,  true);
  return false;
}

ANN static Type prim_owned(const Env env, const Symbol *data) {
  Exp*   exp  = exp_self(prim_exp(data));
  const Value v    = exp->d.prim.value;
  if(is_class(env->gwion, v->type) && env->class_def == v->type->info->base_type) // write it better
    return v->type->info->base_type;
//  if(v->from->owner_class && is_func(env->gwion, v->from->owner_class))
//    return v->type;
  const m_str name = !GET_FLAG(v, static) ? "this" : v->from->owner_class->name;
  Exp* base =
      new_prim_id(env->gwion->mp, insert_symbol(name), prim_pos(data));
  exp_setuse(base, 1);
  exp->exp_type = ae_exp_dot;
  exp->d.exp_dot.base = base;
  base->d.prim.value = v->from->owner_class->info->value;
//  base->type = v->from->owner_class;
  exp->d.exp_dot.var.tag.sym  = *data;
  return check_exp(env, exp);
}

ANN static Type prim_id_non_res(const Env env, const Symbol *data) {
  const Symbol sym = *data;
  const Value  v   = check_non_res_value(env, data);
  if (!safe_vflag(v, vflag_valid)) {
    const m_str name = s_name(*data);
    if (!isalpha(*name) && *name != '_') {
      prim_self(data)->value = env->gwion->type[et_op]->info->value;
      return env->gwion->type[et_op];
    }
    if (env->func && fbflag(env->func->def->base, fbflag_lambda) && env->func->def->base->values) {
      const Value v = upvalues_lookup(env->func->def->base->values, sym);
      if(v) {
        CHECK_O(check_upvalue(env, prim_self(data), v));
        return v->type;
      }
    }
    if(env->curr->info->gwusing) {
      for(uint32_t i = 0; i < env->curr->info->gwusing->len; i++) {
          const Stmt_Using using = usinglist_at(env->curr->info->gwusing, i);
          if(!using->tag.sym) {
            // NOTE: we know type is valid and has nspc
            const Type type = known_type(env, using->d.td);
            Value value = nspc_lookup_value1(type->nspc, sym);
            if(value) {
            Exp *exp = prim_exp(data);
            exp->exp_type = ae_exp_dot;
            Type_Decl *td = cpy_type_decl(env->gwion->mp, using->d.td);
            exp->d.exp_dot.base = new_exp_td(env->gwion->mp, td, exp->loc);
            exp->d.exp_dot.var.tag.sym = insert_symbol(value->name);
            return check_exp(env, exp);
          }
        } else if(sym == using->tag.sym) {
          Exp *exp = prim_exp(data);
          const loc_t loc = exp->loc;
          Exp *base = cpy_exp(env->gwion->mp, using->d.exp);
          *exp = *base;
          exp->loc = loc;
          mp_free2(env->gwion->mp, sizeof(Exp), base);
          return check_exp(env, exp);
        }
      }
    }
    m_str str = NULL;
    gw_asprintf(env->gwion->mp, &str, "Invalid variable {R}%s{0}\n", name);
    gwlog_error(str, _("not legit at this point."),
                env->name, prim_pos(data), 0);
    free_mstr(env->gwion->mp, str);
    did_you_mean_nspc(v ? v->from->owner : env->curr, s_name(sym));
    env_set_error(env, true);
    return NULL;
  }
  prim_self(data)->value = v;
  if (v->from->owner_class) return prim_owned(env, data);
  if (GET_FLAG(v, const)) exp_setmeta(prim_exp(data), 1);

  if (env->func && strcmp(env->func->name, "in spork")) {
//    if(vflag(v, vflag_fglobal) /*&& !vflag(v, vflag_builtin) */&& !is_func(env->gwion, v->type)) {
    if(GET_FLAG(v, global) && !vflag(v, vflag_builtin) && !is_func(env->gwion, v->type)) {
      if(!env->func->_wait)
        env->func->_wait = new_valuelist(env->gwion->mp, 0);
      if (!v->used_by) {
        v->used_by = new_funclist(env->gwion->mp, 1);
        funclist_set(v->used_by, 0, env->func);
        valuelist_add(env->gwion->mp, &env->func->_wait, v);
      } else {
        bool found = false;
        for(uint32_t i = 0; i < v->used_by->len; i++) {
          const Func f = funclist_at(v->used_by, i);
          if(f == env->func) {
            found = true;
            break;
          }
        }
        if(!found) {
          funclist_add(env->gwion->mp, &v->used_by, env->func);
          valuelist_add(env->gwion->mp, &env->func->_wait, v);
        }
      }
    }

    if (!GET_FLAG(v, const) && v->from->owner)
      unset_fflag(env->func, fflag_pure);
    if (fbflag(env->func->def->base, fbflag_lambda))
      CHECK_O(check_upvalue(env, prim_self(data), v));
  }
  // set_vflag(v->vflag, vflag_used);
  return v->type;
}

ANN Type check_prim_str(const Env env, const struct AstString *data) {
  if (!prim_self(data)->value)
    prim_self(data)->value = global_string(env, data->data, prim_pos(data));
  exp_setmeta(prim_exp(data), true);
  return env->gwion->type[et_string]; // prim->value
}

ANN static Type check_prim_id(const Env env, const Symbol *data) {
  struct SpecialId_ *spid = specialid_get(env->gwion, *data);
  if (spid) return specialid_type(env, spid, prim_self(data));
  return prim_id_non_res(env, data);
}

ANN static Type check_prim_perform(const Env env, const Symbol *data) {
  if(*data) env_add_effect(env, *data, prim_pos(data));
  else if(!env->scope->in_try)
    ERR_O(prim_pos(data), "`rethrow` outside try/handle statement");
  env_weight(env, 1);
  return env->gwion->type[et_void];
}

ANN static Type check_prim_interp(const Env env, Exp* *exp) {
  CHECK_O(check_exp(env, *exp));
  env_weight(env, 1);
  return env->gwion->type[et_string];
}

ANN static Type check_prim_hack(const Env env, Exp* *data) {
  if (env->func) unset_fflag(env->func, fflag_pure);
  CHECK_O(check_prim_interp(env, data));
  env_weight(env, 1);
  return env->gwion->type[et_gack];
}

ANN static Type check_prim_locale(const Env env, const Symbol *data NUSED) {
  exp_setmeta(prim_exp(data), true);
  return env->gwion->type[et_float];
}

#define describe_prim_xxx(name, type)                                          \
  ANN static Type check_prim_##name(const Env env               NUSED,         \
                                    const union prim_data *data NUSED) {       \
    return type;                                                               \
  }

describe_prim_xxx(num, env->gwion->type[et_int]);
describe_prim_xxx(char, env->gwion->type[et_char]);
describe_prim_xxx(float, env->gwion->type[et_float]);
describe_prim_xxx(nil, env->gwion->type[et_void]);

DECL_PRIM_FUNC(check, Type, Env,);

ANN static Type check_prim(const Env env, Exp_Primary *prim) {
  return exp_self(prim)->type = check_prim_func[prim->prim_type](env, &prim->d);
}

ANN Type check_array_access(const Env env, const Array_Sub array) {
  const Symbol     sym = insert_symbol("[]");
  struct Op_Import opi = {.op   = sym,
                          .lhs  = array->exp->type,
                          .rhs  = array->type,
                          .loc  = array->exp->loc,
                          .data = (uintptr_t)array};
  return op_check(env, &opi);
}

static ANN Type check_exp_array(const Env env, const Exp_Array *array) {
  CHECK_O((array->array->type = check_exp(env, array->base)));
  CHECK_O(check_subscripts(env, array->array, false));
  if(exp_getmeta(array->base)) exp_setmeta(exp_self(array), true);
  return check_array_access(env, array->array);
}

static ANN Type check_exp_slice(const Env env, const Exp_Slice *range) {
  CHECK_O(check_exp(env, range->base));
  CHECK_O(check_range(env, range->range));
  env_weight(env, 1);
  const Symbol sym = insert_symbol("[:]");
  Exp*    e   = range->range->start ?: range->range->end;
  assert(e);
  struct Op_Import opi = {.op   = sym,
                          .lhs  = e->type,
                          .rhs  = range->base->type,
                          .loc  = e->loc,
                          .data = (uintptr_t)exp_self(range)};
  return op_check(env, &opi);
}

// get the type of the function
// without the mangling
ANN static inline Type tmplarg_list_base_func(const Type type) {
  const Nspc owner = type->info->value->from->owner;
  const Symbol xid = type->info->func->def->base->tag.sym;
  return nspc_lookup_type0(owner, xid);
}

ANN static inline Type tmplarg_list_base(const Gwion gwion, const Type type) {
  return !(is_func(gwion, type)) ? type : tmplarg_list_base_func(type);
}

ANN static Type_Decl*  mk_td(const Env env, Type_Decl *td,
                                  const Type type, const loc_t loc) {
  const Type base = tmplarg_list_base(env->gwion, type);
  const Type t    = !td->array ?
          base : array_type(env, base, td->array->depth, loc);
  return type2td(env->gwion, t, loc);
}

ANN static Type tmplarg_match(const Env env, const Symbol xid, const Symbol tgt, const Type t) {
  if (xid == tgt) return t;
//  Tmpl *tmpl = get_tmpl(t);//tflag(t, tflag_cdef) ? t->info->cdef->base.tmpl
  Tmpl *tmpl = tflag(t, tflag_cdef) ? t->info->cdef->base.tmpl
       : tflag(t, tflag_udef) ? t->info->udef->tmpl
       : NULL;
  if (!tmpl) return NULL;
  const uint32_t len = specializedlist_len(tmpl->list);
  for(uint32_t i = 0; i < len; i++) {
    const Specialized spec = specializedlist_at(tmpl->list, i);
    const Type base = nspc_lookup_type1(t->nspc, spec.tag.sym);
    const Type t = tmplarg_match(env, xid, spec.tag.sym, base);
    if(t) return t;
  }
  return NULL;
}

ANN2(1, 2)
static Func find_func_match_actual(const Env env, const Func f, Exp* exp,
                                   const bool implicit, const bool specific) {
  Func func = f;
  do {
    Exp* e = exp;
    ArgList *args = func->def->base->args;
    m_uint i = 0;
    const m_uint args_len = arglist_len(args);
    while (e) {
      e->cast_to = NULL;
      if (!e->type) // investigate
        return NULL;
      Arg *arg = i < args_len ? arglist_ptr_at(args, i++) : NULL;
      if (!arg) {
        CHECK_O(func->next);
        return find_func_match_actual(env, func->next, exp, implicit,
                                      specific);
      }
      if (tflag(e->type, tflag_ref) && isa(e->type, arg->type)) {
        if(!e->cast_to)e->cast_to = arg->type;
      }

      if (arg->type == env->gwion->type[et_auto]) {
        const Type owner = func->value_ref->from->owner_class;
        if (owner) CHECK_O(template_push(env, owner));
        arg->type = known_type(env, arg->var.td);
        if (owner) nspc_pop_type(env->gwion->mp, env->curr);
        CHECK_O(arg->type);
      }

      if (!func_match_inner(env, e, arg->type, implicit, specific)) break;
      e  = e->next;
    }
    if (!e && args_len == i) return func;
  } while ((func = func->next));
  return NULL;
}

ANN static Type check_exp_call(const Env env, Exp_Call *exp);

ANN static Func call2ufcs(const Env env, Exp_Call *call, const Value v) {
  Exp* this               = call->func->d.exp_dot.base;
  this->next                   = call->args;
  call->args                   = this;
  call->func->type             = v->type;
  call->func->d.prim.value     = v;
  call->func->d.prim.d.var     = call->func->d.exp_dot.var.tag.sym;
  call->func->exp_type         = ae_exp_primary;
  call->func->d.prim.prim_type = ae_prim_id;
  CHECK_O(check_exp_call(env, call));
  return call->func->type->info->func;
}

ANN static Func ufcs(const Env env, const Func up, Exp_Call *const call) {
  const Value v = nspc_lookup_value1(env->curr, up->def->base->tag.sym);
  if (v && is_func(env->gwion, v->type) && !v->from->owner_class) // is_callable
    return call2ufcs(env, call, v);
  return NULL;
}

static ANN Func find_func_match_normal(const Env env, const Func up, Exp_Call *const call) {
  Func      func;
  Exp* exp = call->args;
  Exp* args =
      (exp && !isa(exp->type, env->gwion->type[et_void])) ? exp : NULL;
  if ((func = find_func_match_actual(env, up, args, false, true)) ||
      (func = find_func_match_actual(env, up, args, true, true)) ||
      (func = find_func_match_actual(env, up, args, false, true)) ||
      (func = find_func_match_actual(env, up, args, true, false)))
    return func;
  return call->func->exp_type == ae_exp_dot && up->value_ref->from->owner_class
             ? ufcs(env, up, call)
             : NULL;
}

ANN bool call_has_named_args(Exp_Call *call, uint32_t *nargs) {
  Exp *exp = call->args;
  bool ret = false;
  do {
    if(exp->exp_type == ae_exp_named)
      ret = true;
    (*nargs)++;
  } while ((exp = exp->next));
  return ret;
}

struct NamedChecker {
  EList *call_list;
  EList *named;
  EList *unnamed;
  uint32_t  nargs;
};

static ANN Func __find_func_match_named(const Env env, const Func up, Exp_Call *const call, const struct NamedChecker *nc) {
  const ArgList *args = up->def->base->args;
  if(!args || nc->nargs != args->len) {
    return NULL;
  }

  uint32_t nnamed = 0;
  uint32_t nunnamed = 0;
  for(uint32_t i = 0; i < args->len; i++) {
    bool found = false;
    const Arg arg = arglist_at(args, i);
    for(uint32_t j = 0; j < nc->named->len; j++) {
      Exp *exp = elist_at(nc->named, j);
      if(arg.var.vd.tag.sym == exp->d.exp_named.tag.sym) {
        elist_set(nc->call_list, i, exp);
        nnamed++;
        found = true;
        break;
      }
    }
    if(!found && nunnamed < nc->unnamed->len)
      elist_set(nc->call_list, i, elist_at(nc->unnamed, nunnamed++));
  }
  if((nunnamed + nnamed) != nc->nargs) return NULL;
  for(uint32_t i = 1; i < nc->call_list->len; i++) {
    (elist_at(nc->call_list, i-1))->next = elist_at(nc->call_list, i);
  }
  (elist_at(nc->call_list, nc->call_list->len-1))->next = NULL;
  call->args = elist_at(nc->call_list, 0);
  return find_func_match_normal(env, up, call);
}

static ANN Func _find_func_match_named(const Env env, const Func up, Exp_Call *const call, const struct NamedChecker *nc) {
  Func ret = __find_func_match_named(env, up, call, nc);
  if(ret) return ret;
  if(!up->next) return NULL;
  return _find_func_match_named(env, up->next, call, nc);
}

static ANN Func find_func_match_named(const Env env, const Func up, Exp_Call *const call, const uint32_t nargs) {
  struct NamedChecker nc = {
    .call_list = new_elist(env->gwion->mp, nargs),
    .named     = new_elist(env->gwion->mp, 0),
    .unnamed   = new_elist(env->gwion->mp, 0),
    .nargs     = nargs,
  };
  EList *arg_list = new_elist(env->gwion->mp, nargs);
  Exp *exp = call->args;
  uint32_t i = 0;
  do {
    elist_set(arg_list, i, exp);
    if(exp->exp_type == ae_exp_named) {
      exp->d.exp_named.is_arg = true;
      elist_add(env->gwion->mp, &nc.named, exp);
    } else
      elist_add(env->gwion->mp, &nc.unnamed, exp);
    i++;
  } while((exp = exp->next));

  const Func ret = _find_func_match_named(env, up, call, &nc);

  if(!ret) {
    for(uint32_t i = 1; i < arg_list->len; i++) {
      (elist_at(arg_list, i-1))->next = 
        elist_at(arg_list, i);
    }
    (elist_at(arg_list, arg_list->len-1))->next = NULL;
    call->args = (elist_at(arg_list, 0));
  }

  free_elist(env->gwion->mp, arg_list);
  free_elist(env->gwion->mp, nc.call_list);
  free_elist(env->gwion->mp, nc.named);
  free_elist(env->gwion->mp, nc.unnamed);

  return ret;
}

ANN Func find_func_match(const Env env, const Func up, Exp_Call *const call) {
  uint32_t nargs = 0;
  if(!call->args || !call_has_named_args(call, &nargs))
    return find_func_match_normal(env, up, call);
  return find_func_match_named(env, up, call, nargs);
}

ANN bool check_traverse_fdef(const Env env, const Func_Def fdef) {
  struct Vector_ v     = {};
  const m_uint scope   = env->scope->depth;
  env->scope->depth    = 0;
  vector_init(&v);
  const Vector w = (Vector)&env->curr->info->value->ptr;
  m_uint i = vector_size(w);
  while (i-- > 1) vector_add(&v, vector_at(w, i));
  VLEN(w) = 1;
  const bool ret = traverse_func_def(env, fdef);
  for (m_uint i = vector_size(&v) + 1; --i;)
    vector_add(w, vector_at(&v, i - 1));
  vector_release(&v);
  env->scope->depth     = scope;
  return ret;
}

ANN static bool check_func_args(const Env env, ArgList *args) {
  bool ok = true;
  for(uint32_t i = 0; i < args->len; i++) {
    const Arg arg = arglist_at(args, i);
    const Var_Decl *decl = &arg.var.vd;
    const Value    v    = decl->value;
    if(decl->tag.sym) {
      if(!can_define(env, decl->tag.sym, decl->tag.loc)) {
        POISON(ok, env);
        continue;
      }
      valid_value(env, decl->tag.sym, v);
    }
  }
  return ok;
}

ANN static inline Exp* next_arg_exp(const Exp *e) {
  Exp* next = e->next;
  if (next) gw_err(", ");
  return next;
}

ANN static void print_current_args(Exp* e) {
  do {
    if (e->exp_type == ae_exp_named)
      gw_err(" {B}%s{0} =", s_name(e->d.exp_named.tag.sym));
    gw_err(" {G}%s{0}", e->type ? e->type->name : "<Unknown>");
  } while ((e = next_arg_exp(e)));
  gw_err("\n");
}

ANN2(1, 2)
static void function_alternative(const Env env, const Type t, Exp* args,
                                 const loc_t loc) {
  const bool is_closure = !isa(t, env->gwion->type[et_closure]);
  Func f = is_closure
         ? t->info->func
        : closure_def(t)->base->func;
  if(!f) return;
  gwlog_error("Argument type mismatch", "call site",
              env->name, loc, 0);
  // TODO: hint valid alternatives
  do print_signature(env->gwion, f);
  while ((f = f->next));
  gw_err(_("and not:\n  "));
  if (args)
    print_current_args(args);
  else
    gw_err(_("  {G}void{0}\n"));
  env_set_error(env, true);
}

ANN static Func get_template_func(const Env env, Exp_Call *const func,
                                  const Value v) {
  const Func f = find_template_match(env, v, func);
  if (f) {
    Tmpl *tmpl               = new_tmpl_call(env->gwion->mp, func->tmpl->call);
    tmpl->list               = v->d.func_ref
                                   ? v->d.func_ref->def->base->tmpl->list
                                   : func->func->type->info->func->def->base->tmpl->list;
    ((Exp_Call *)func)->tmpl = tmpl;
    func->func->type         = f->value_ref->type;
    return f;
  }
  ((Exp_Call *)func)->tmpl = NULL;
  assert(exp_self(func));
  ERR_O(exp_self(func)->loc,
        _("function is a template for which automatic type inference is not fully implemented"));
}

ANN static Func predefined_func(const Env env, const Value v, Exp_Call *exp,
                                const Tmpl *tm) {
  Tmpl tmpl = {.call = tm->call};
  exp->tmpl = &tmpl;
  DECL_O(const Func, func, = get_template_func(env, exp, v));
  return v->d.func_ref = func;
}

ANN static Type check_predefined(const Env env, Exp_Call *exp, const Value v,
                                 const Tmpl *tm, const Func_Def fdef) {
  DECL_O(const Func, func,
          = v->d.func_ref ?: predefined_func(env, v, exp, tm));
  if (!fdef->base->ret_type) { // template fptr
    struct EnvSet es = {.env   = env,
                        .data  = env,
                        .func  = (_envset_func)check_cdef,
                        .scope = env->scope->depth,
                        .flag  = tflag_check};
    CHECK_O(envset_pushv(&es, v));
    func->def->base->fbflag |= fbflag_internal;
    const bool ret = check_traverse_fdef(env, func->def);
    envset_popv(&es, v);
    CHECK_O(ret);
  }
  exp->func->type = func->value_ref->type;
  return func->def->base->ret_type;
}

ANN static TmplArgList *check_template_args(const Env env, Exp_Call *exp,
                                         const Tmpl *tm, const Func_Def fdef) {
  SpecializedList *sl = tm->list;
  const bool spread = is_spread_tmpl(fdef->base->tmpl);
  const uint32_t len = sl->len - spread;
  TmplArgList   *tl = new_tmplarglist(env->gwion->mp, len);
  m_uint       args_number = 0;
// infer template types from args
// should not work with const generic
  bool ok = true;
  if(exp->other) {
    for(uint32_t i = 0; i < len; i++) {
      const Specialized spec = specializedlist_at(sl, i);
      if (tmplarg_match(env, spec.tag.sym, fdef->base->td->tag.sym, fdef->base->ret_type)) {
         if(!check_exp(env, exp->other)) {
           POISON_NODE(ok, env, exp->other);
           continue;
        }
         if(!is_func(env->gwion, exp->other->type)) {
           TmplArg targ = {
             .type = tmplarg_td,
             .d = { .td = type2td(env->gwion, exp->other->type, fdef->base->td->tag.loc) }
           };
           tmplarglist_set(tl, 0, targ);
         } else {
           Func func = exp->other->type->info->func;
           do {
             if(arglist_len(func->def->base->args) == 1) {
               const Arg arg = arglist_at(func->def->base->args, 0);
               TmplArg targ = {
                 .type = tmplarg_td,
                 .d = { .td = cpy_type_decl(env->gwion->mp, arg.var.td) }
               };
               tmplarglist_set(tl, 0, targ);
               break;
             }
           } while((func = func->next));
        }
        ++args_number;
        break;
      }
    }
  }
  CHECK_B(ok);
  for(uint32_t i = 0; i < len; i++) {
    const Specialized spec = specializedlist_at(sl, i);
    ArgList     *args = fdef->base->args;
    const uint32_t args_len = arglist_len(args);
    Exp*      template_arg = exp->args;
    uint32_t count = 0;
    while (count < args_len && template_arg) {
      const Arg arg = arglist_at(args, count);
      if (tmplarg_match(env, spec.tag.sym, arg.var.td->tag.sym, template_arg->type)) {
        TmplArg targ = {
          .type = tmplarg_td,
          .d = { .td = mk_td(env, arg.var.td, template_arg->type, fdef->base->tag.loc) }
        };
        tmplarglist_set(tl, args_number, targ);
        ++args_number;
        break;
      }
      count++;
      template_arg = template_arg->next;
    }
  }
  if (args_number < len) //TODO: free tmplarg_list
    ERR_O(exp->func->loc, _("not able to infer types for template call."));

  if(spread) {
    Exp* e = exp->args;
    const uint32_t len = fdef->base->args ? fdef->base->args->len : 0;
//    if(fdef->base->args) {
      for(uint32_t i = 0; i < len && e; i++) e = e->next;
      while(e) {
        TmplArg targ = {
          .type = tmplarg_td,
          .d = { .td = type2td(env->gwion, e->type, e->loc) }
        };
        tmplarglist_add(env->gwion->mp, &tl, targ);
        e = e->next;
      }
 //   }
  }
  return tl;
}

ANN static Type check_exp_call_template(const Env env, Exp_Call *exp) {
  /*const */Type t = exp->func->type;
  if(isa(t, env->gwion->type[et_closure])) {
    const Func_Def fdef = closure_def(t);
    t = fdef->base->func->value_ref->type;
  }
  DECL_O(const Value, value, = type_value(env->gwion, t));
  const Func_Def fdef =
      value->d.func_ref ? value->d.func_ref->def : t->info->func->def;
  Tmpl *tm = fdef->base->tmpl;
  if (tm->call) return check_predefined(env, exp, value, tm, fdef);
  DECL_O(TmplArgList*, tl, = check_template_args(env, exp, tm, fdef));
  Tmpl tmpl               = {.call = tl};
  ((Exp_Call *)exp)->tmpl = &tmpl;
  DECL_O(const Func, func, = get_template_func(env, exp, value));
  return func->def->base->ret_type != env->gwion->type[et_auto] ?
    func->def->base->ret_type : exp->func->d.exp_dot.base->type;
}

ANN Type upvalue_type(const Env env, Capture *cap);

ANN static bool lambda_args_ref(const Env env, Exp_Call *const call) {
  Exp* e = call->args;
  CHECK_B(check_exp(env, e));
  do if(tflag(e->type, tflag_ref) && !safe_tflag(exp_self(e)->cast_to, tflag_ref))
     exp_setvar(e, true);
  while((e = e->next));
  return true;
}

ANN2(1) static bool lambda_append_args(const Env env, Exp_Call *const call, Exp* add) {
  if(!add) return false;
  if (call->args) {
    Exp* e = call->args;
    while(e->next) e = e->next;
    e->next = add;
  } else call->args = add;
  return traverse_exp(env, add);
}

ANN static Exp* check_lambda_captures(const Env env, const Func_Def fdef) {
  if(!fdef->base->args)
    fdef->base->args = new_arglist(env->gwion->mp, 0);
  Exp* args = NULL;
  Exp* tmp;
  for(uint32_t i = 0; i < fdef->captures->len; i++) {
    Capture *cap = capturelist_ptr_at(fdef->captures, i);
    const Type t = upvalue_type(env, cap);
    if(!t) {
      if(args) free_exp(env->gwion->mp, args);
      return NULL;
    }
    Arg arg = { .var = MK_VAR(
        type2td(env->gwion, t, cap->var.tag.loc),
        (Var_Decl){ .tag = MK_TAG(cap->var.tag.sym, cap->var.tag.loc) }
        ),
        .loc = cap->var.tag.loc
    };
    arglist_add(env->gwion->mp, &fdef->base->args, arg);
    Exp* exp = new_prim_id(env->gwion->mp, cap->var.tag.sym, cap->var.tag.loc);
    if(args) tmp = tmp->next = exp;
    else args = tmp = exp;
  }
  free_capturelist(env->gwion->mp, fdef->captures);
  fdef->captures = NULL;
  return args;
}

ANN static Type check_lambda_call(const Env env, Exp_Call *const exp) {
  const Func_Def fdef = exp->func->d.exp_lambda.def;
  const bool captures = !!fdef->captures;
  if (exp->args) CHECK_O(lambda_args_ref(env, exp));
  Exp* _args = !captures ? NULL : check_lambda_captures(env, fdef);
  if(captures) CHECK_O(lambda_append_args(env, exp, _args));
  Exp_Lambda *l   = &exp->func->d.exp_lambda;
  ArgList    *args = l->def->base->args;
  Exp*         e   = exp->args;
  for(uint32_t i = 0; i < (args ? args->len : 0); i++) {
    if(!e)
      ERR_O(exp_self(exp)->loc, _("argument number does not match for lambda"));
    Arg *arg = arglist_ptr_at(args, i);
    arg->type = e->type;
    if(is_class(env->gwion, arg->type))
      type_addref(arg->type);
    e = e->next;
  }
  if(e)
     ERR_O(exp_self(exp)->loc, _("argument number does not match for lambda"));
  Upvalues upvalues = { .values = env->curr->info->value};
  if(env->func && env->func->def->base->values)
    upvalues.parent = env->func->def->base->values;
  l->def->base->values = &upvalues;
  const bool ret     = traverse_func_def(env, l->def);
  if (l->def->base->func) {
    free_scope(env->gwion->mp, env->curr->info->value);
    env->curr->info->value = l->def->base->values->values;
    if (env->class_def) set_vflag(l->def->base->func->value_ref, vflag_member);
    exp->func->type = l->def->base->func->value_ref->type;
    if (!l->def->base->ret_type)
      l->def->base->ret_type = env->gwion->type[et_void];
  }
  return ret ? l->def->base->ret_type : NULL;
}

ANN bool func_check(const Env env, Exp_Call *const exp, bool *mod) {
  exp->func->is_call = true;
  CHECK_B(check_exp(env, exp->func));
  if (exp->func->exp_type == ae_exp_decl)
    ERR_B(exp->func->loc, _("Can't call late function pointer at declaration "
                            "site. did you meant to use `:=>`?"));
  const Type t = actual_type(env->gwion, exp->func->type);
  if (is_func(env->gwion, t) && exp->func->exp_type == ae_exp_dot && // is_callable
      !t->info->value->from->owner_class) {
    if (exp->args) CHECK_B(check_exp(env, exp->args));
    return !!call2ufcs(env, exp, t->info->func->value_ref);
  }
  Exp*        e   = exp_self(exp);
  struct Op_Import opi = {.op   = insert_symbol("@func_check"),
                          .rhs  = t,
                          .loc  = e->loc,
                          .data = (uintptr_t)e};
  if(op_get(env, &opi))
    CHECK_B(op_check(env, &opi));
  if (e->exp_type != ae_exp_call) {
    *mod = true;
    return true;
  }
  return e->type != env->gwion->type[et_error];
}

ANN void call_add_effect(const Env env, const Func func, const loc_t loc) {
  if (func != env->func && func->def->base->effects.ptr) {
    const Vector v = &func->def->base->effects;
    for (m_uint i = 0; i < vector_size(v); i++)
      env_add_effect(env, (Symbol)vector_at(v, i), loc);
  }
}

ANN Type call_type(const Env env, Exp_Call *const exp) {
  const Type t = exp->func->type;
  if (is_func(env->gwion, t)) return t;
  if(isa(exp->func->type, env->gwion->type[et_closure]))
    return closure_def(t)->base->func->value_ref->type;
  if(is_class(env->gwion, t) && tflag(t->info->base_type, tflag_struct)) {
    const Value v = nspc_lookup_value0(t->info->base_type->nspc, insert_symbol("new"));
    if(v) return exp->func->type = v->type;
  }
  struct Op_Import opi = {.op   = insert_symbol("call_type"),
                          .rhs  = actual_type(env->gwion, exp->func->type),
                          .data = (uintptr_t)exp,
                          .loc  = exp_self(exp)->loc};
  return op_check(env, &opi);
}

ANN static inline bool is_super(Exp* func) {
  return func->exp_type == ae_exp_primary &&
         func->d.prim.prim_type == ae_prim_id &&
         !strcmp(s_name(func->d.prim.d.var), "super");
}

ANN static Type call_return(const Env env, Exp_Call *const exp,
       const Func func) {
  const Value v = func->value_ref;
  exp->func->type = v->type;
  call_add_effect(env, func, exp->func->loc);
  if(func->def->base->ret_type != env->gwion->type[et_auto])
    return func->def->base->ret_type;
  if(tflag(v->from->owner_class, tflag_struct))
    return v->from->owner_class;
  if(exp->func->exp_type == ae_exp_dot)
    return exp->func->d.exp_dot.base->type;
  if(is_super(exp->func))
    return exp->func->type;
  return NULL;
}

static ANN Type _check_exp_call1(const Env env, Exp_Call *const exp) {
  DECL_O(const Type, t, = call_type(env, exp));
  if (t == env->gwion->type[et_op]) return check_op_call(env, exp);
  if (!t->info->func) // TODO: effects?
    return check_lambda_call(env, exp);
  if (exp->args) {
    CHECK_O(check_exp(env, exp->args));
    Exp* e = exp->args;
    do exp_setuse(e, true);
    while((e = e->next));
  }
  if (tflag(t, tflag_ftmpl))
    return check_exp_call_template(env, (Exp_Call *)exp); // TODO: effects?
  const Func func = find_func_match(env, t->info->func, exp);
  if (func) return call_return(env, exp, func);
  return exp->func->exp_type != ae_exp_lambda
    ? NULL : partial_type(env, exp);
}

ANN static Type check_static(const Env env, Exp* e) {
  const Type t = e->type;
  if(unlikely(!is_func(env->gwion, t))             ||
     !t->info->func                                ||
     !GET_FLAG(t->info->func->def->base, abstract) ||
     !is_static_call(env->gwion, e)) return t;
  env_err(env, e->loc, "making a static call to an abstract function");
  declared_here(t->info->value);
  return NULL;
}

ANN Type check_exp_call1(const Env env, Exp_Call *const exp) {
  bool mod = false;
  CHECK_O(func_check(env, exp, &mod));
  if (mod) return exp_self(exp)->type;
  const Type t = exp->func->type;
  CHECK_O(check_static(env, exp->func));
  const Type _ret = _check_exp_call1(env, exp);
  if(_ret) {
    if(exp->func->exp_type == ae_exp_dot)
      exp->func->d.exp_dot.var.value = exp->func->type->info->value;
    return _ret;
  }
  if(isa(exp->func->type, env->gwion->type[et_closure])) {
    if(exp->func->exp_type == ae_exp_dot && t->info->value->from->owner_class) {
      Exp* args = exp->args;
      Exp* this_arg = cpy_exp(env->gwion->mp, exp->func->d.exp_dot.base);
      this_arg->next = args;
      exp->args = this_arg;
      const Type t = _check_exp_call1(env, exp);
      if(t) return t;
    }
  }
  function_alternative(env, t, exp->args, exp->func->loc);
  return NULL;
}

ANN static Type check_exp_binary(const Env env, const Exp_Binary *bin) {
  if(bin->lhs->exp_type == ae_exp_call && !bin->lhs->d.exp_call.tmpl) {
    CHECK_O(check_exp(env, bin->lhs->d.exp_call.func));
    // check is template?
    bin->lhs->d.exp_call.other = bin->rhs;
  }
  CHECK_O(check_exp(env, bin->lhs));
  const bool is_auto = //bin->op == insert_symbol(":=>")  &&
                         bin->rhs->exp_type == ae_exp_decl &&
                         bin->rhs->d.exp_decl.type == env->gwion->type[et_auto];
  if (is_auto) bin->rhs->d.exp_decl.type = bin->lhs->type;
  // allow foo => new C to mean new C(foo)
  // do we actually still need that?
  if(bin->op == insert_symbol("=>") &&
     bin->rhs->exp_type == ae_exp_unary && bin->rhs->d.exp_unary.unary_type == unary_td &&
     !bin->rhs->d.exp_unary.ctor.td->array &&
     !bin->rhs->d.exp_unary.ctor.exp) {
   Exp* lhs = bin->lhs;
   Exp* rhs = bin->rhs;
   Exp* e = exp_self(bin);
   Exp_Unary *const unary = &e->d.exp_unary;
   e->exp_type = ae_exp_unary;
   unary->unary_type = unary_td;
   unary->op = insert_symbol("new");
   unary->ctor.td = cpy_type_decl(env->gwion->mp, bin->rhs->d.exp_unary.ctor.td);
   unary->ctor.exp = lhs;
   free_exp(env->gwion->mp, rhs);
   return check_exp(env, e);
  }
  if(bin->rhs->exp_type == ae_exp_call && !bin->rhs->d.exp_call.tmpl)
    bin->rhs->d.exp_call.other = bin->lhs;
  CHECK_O(check_exp(env, bin->rhs));
  if (is_auto) {
    assert(bin->rhs->type == bin->lhs->type);
    set_vflag(bin->rhs->d.exp_decl.var.vd.value, vflag_assigned);
  }
  struct Op_Import opi = {.op   = bin->op,
                          .lhs  = bin->lhs->type,
                          .rhs  = bin->rhs->type,
                          .data = (uintptr_t)bin,
                          .loc  = exp_self(bin)->loc};
  exp_setuse(bin->lhs, 1);
  exp_setuse(bin->rhs, 1);
  const Type ret = op_check(env, &opi);
  if (!ret && is_auto && exp_self(bin)->exp_type == ae_exp_binary)
    bin->rhs->d.exp_decl.var.vd.value->type = env->gwion->type[et_auto];
  return ret;
}

ANN static Type check_exp_cast(const Env env, const Exp_Cast *cast) {
  DECL_O(const Type, t, = check_exp(env, cast->exp));
  CHECK_O((exp_self(cast)->type = known_type(env, cast->td)));
  struct Op_Import opi = {.op   = insert_symbol("$"),
                          .lhs  = t,
                          .rhs  = exp_self(cast)->type,
                          .data = (uintptr_t)cast,
                          .loc  = exp_self(cast)->loc};
  return op_check(env, &opi);
}

ANN static Type check_exp_post(const Env env, const Exp_Postfix *post) {
  struct Op_Import opi = {.op   = post->op,
                          .lhs  = check_exp(env, post->exp),
                          .data = (uintptr_t)post,
                          .loc  = exp_self(post)->loc};
  CHECK_O(opi.lhs);
  exp_setuse(post->exp, 1);
  const Type t = op_check(env, &opi);
  if (t && !is_object(env->gwion, t))
    exp_setmeta(exp_self(post), 1);
  return t;
}

ANN static bool predefined_call(const Env env, const Type t,
                                  const loc_t loc) {
  const m_str str =
      tl2str(env->gwion, t->info->func->def->base->tmpl->call, loc);
  env_err(env, loc, _("Type '%s' has '%s' as pre-defined types."), t->name,
          str);
  free_mstr(env->gwion->mp, str);
  if (tflag(t, tflag_typedef)) {
    gwlog_related("from definition:", env->name,
                    t->info->func->def->base->tag.loc);
  }
  return false;
}

ANN2(1) static inline bool is_partial(const Env env, Exp* exp) {
  while (exp) {
    if (is_hole(env, exp) || is_typed_hole(env, exp))
      return true;
    exp = exp->next;
  }
  return false;
}

ANN static bool tl_match(const Env env, const TmplArgList *tl0, const TmplArgList *tl1) {
  if (tl0->len != tl1->len) return false;
  bool ok = true;
  for(uint32_t i = 0; i < tl0->len; i++) {
    const TmplArg targ0 = tmplarglist_at(tl0, i);
    const TmplArg targ1 = tmplarglist_at(tl1, i);
    if (targ0.type != targ1.type) return false;
    if(targ0.type == tmplarg_td && known_type(env, targ0.d.td) != known_type(env, targ1.d.td))
      POISON(ok, env);
    // how do we check exps???
  }
  return ok;
}

ANN static Type check_exp_call_tmpl(const Env env, Exp_Call *exp, const Type t) {
  if (exp->args) CHECK_O(check_exp(env, exp->args));
  if (!t->info->func->def->base->tmpl)
    ERR_O(exp_self(exp)->loc, _("template call of non-template function."));
  if (t->info->func->def->base->tmpl->call) {
    if (env->func == t->info->func) {
      exp->func->type = env->func->value_ref->type;
      return env->func->def->base->ret_type;
    } else {
      if(tl_match(env, t->info->func->def->base->tmpl->call, exp->tmpl->call))
        return check_exp_call1(env, exp);
      CHECK_O(predefined_call(env, t, exp_self(exp)->loc));
    }
  }
  const Value v = type_value(env->gwion, t);
  DECL_O(const Func, f, = find_template_match(env, v, exp));
  exp->func->type = f->value_ref->type;
  return f->def->base->ret_type;
}

ANN static Type check_exp_call(const Env env, Exp_Call *exp) {
  if (is_partial(env, exp->args)) {
    CHECK_O(check_exp(env, exp->func));
    struct Op_Import opi = {.op   = insert_symbol("@partial"),
                            .lhs  = exp->func->type,
                            .loc  = exp->func->loc,
                            .data = (uintptr_t)exp};
    return op_check(env, &opi);
  }
  if (exp->tmpl) {
    bool mod = false;
    CHECK_O(func_check(env, exp, &mod));
    if (mod) return exp_self(exp)->type;
    Type t = actual_type(env->gwion, exp->func->type);
    if(isa(exp->func->type, env->gwion->type[et_closure])) {
      t = typedef_base(t);
      t = sectionlist_at(t->info->cdef->body, 0).d.func_def->base->func->value_ref->type;
    }
// check for closure and b ring it back
    if (!is_func(env->gwion, t)) return check_exp_call1(env, exp);
    if(!is_new(t->info->func->def))
      return check_exp_call_tmpl(env, exp, t);
  }
  return check_exp_call1(env, exp);
}

ANN static Type check_exp_unary(const Env env, const Exp_Unary *unary) {
  const Type rhs =
      unary->unary_type == unary_exp ? check_exp(env, unary->exp) : NULL;
  if (unary->unary_type == unary_exp) {
    CHECK_O(rhs);
    exp_setuse(unary->exp, 1);
  }
  struct Op_Import opi = {.op   = unary->op,
                          .rhs  = rhs,
                          .data = (uintptr_t)unary,
                          .loc  = exp_self(unary)->loc};
  DECL_O(const Type, ret, = op_check(env, &opi));
  return ret;
}

ANN static Type _flow(const Env env, Exp* e, const bool b) {
  DECL_O(const Type, type, = check_exp(env, e));
  struct Op_Import opi = {
      .op   = insert_symbol(b ? "@conditional" : "@unconditional"),
      .rhs  = type,
      .loc  = e->loc,
      .data = (uintptr_t)e};
  return op_check(env, &opi);
}
#define check_flow(emit, b) _flow(emit, b, 1)

ANN static Type check_exp_if(const Env env, Exp_If *const exp_if) {
  const Type cond = check_flow(env, exp_if->cond);
  const Type if_exp = check_exp(env, exp_if->if_exp);
  const Type else_exp = check_exp(env, exp_if->else_exp);
  if(!cond || !if_exp || !else_exp)
    return NULL;

  const uint meta =
      exp_getmeta(exp_if->if_exp) || exp_getmeta(exp_if->else_exp);
  exp_setmeta(exp_self(exp_if), meta);
  const Type ret = find_common_anc(if_exp, else_exp);
  if (!ret)
    ERR_O(exp_self(exp_if)->loc,
          _("incompatible types '%s' and '%s' in if expression..."),
          if_exp->name, else_exp->name);
  if (!isa(if_exp, else_exp))
    ERR_O(exp_self(exp_if)->loc, _("condition type '%s' does not match '%s'"),
          cond->name, ret->name);
  exp_setuse(exp_if->cond, true);
  exp_setuse(exp_if->if_exp, true);
  exp_setuse(exp_if->else_exp, true);
  return ret;
}

ANN static Type check_exp_dot(const Env env, Exp_Dot *member) {
  CHECK_O(check_exp(env, member->base));
  return check_dot(env, member);
}

ANN bool check_type_def(const Env env, const Type_Def tdef) {
  if (tdef->when) {
    set_tflag(tdef->type, tflag_contract);
    struct Var_Decl_ decl = { .tag = MK_TAG(insert_symbol("self"), tdef->when->loc) };
    Type_Decl *td = cpy_type_decl(env->gwion->mp, tdef->ext);
    ArgList *args = new_arglist(env->gwion->mp, 1);
    arglist_set(args, 0, ((Arg) { .var = MK_VAR(td, decl), .loc = tdef->when->loc}));
    Func_Base *fb = new_func_base(
        env->gwion->mp, type2td(env->gwion, tdef->type, tdef->tag.loc),
        insert_symbol("@implicit"), args, ae_flag_none, tdef->tag.loc);
    set_fbflag(fb, fbflag_op);
    Exp* helper = new_prim_id(env->gwion->mp, insert_symbol("@predicate"),
                                   tdef->when->loc);
    Exp* when   = tdef->when;
    tdef->when = NULL;
    when->next       = helper;
    StmtList *code = new_stmtlist(env->gwion->mp, 2);
    stmtlist_set(code, 0, MK_STMT_EXP(when->loc, when));
    stmtlist_set(code, 1, MK_STMT_EXP(when->loc, NULL));
    const Func_Def fdef = new_func_def(env->gwion->mp, fb, code);
    tdef->when_def           = fdef;
    CHECK_B(traverse_func_def(env, fdef));
    if (!isa(when->type, env->gwion->type[et_bool])) {
      char explain[strlen(when->type->name) + 20];
      sprintf(explain, "found `{/+}%s{0}`", when->type->name);
      gwlog_error("Invalid `{/+}when{0}` predicate expression type", explain,
                  env->name, when->loc, 0);
      gwlog_hint(_("use `bool`"), env->name, when->loc);
      char from[strlen(tdef->type->name) + 39];
      sprintf(from, "in `{/+}%s{0}` definition", tdef->type->name);
      gwlog_related(from, env->name, tdef->tag.loc);
      env_set_error(env, true);
      return false;
    }
    // we handle the return after, so that we don't get *cant' use implicit
    // casting while defining it*
    const Symbol sym = insert_symbol("self");
    Exp* ret_id = new_prim_id(env->gwion->mp, sym, when->loc);
    ret_id->d.prim.value = new_value(env, tdef->type, MK_TAG(sym, tdef->tag.loc));
    Stmt ret = MK_STMT_RETURN(when->loc, ret_id);
    stmtlist_set(fdef->d.code, 1, ret);
    ret_id->type = tdef->type;
  }
  if (tflag(tdef->type, tflag_cdef))
    return check_class_def(env, tdef->type->info->cdef);
  else inherit(tdef->type);
  return true;
}

ANN static Type check_exp_lambda(const Env env, const Exp_If *exp_if NUSED) {
  return env->gwion->type[et_function];
}

ANN static Type check_exp_td(const Env env, Type_Decl **td) {
  DECL_O(const Type, t, = known_type(env, *td));
  if(t == env->gwion->type[et_class])
    ERR_O(exp_self(td)->loc, "can't use {G+}Class{0} in type decl expression");
  if (!is_func(env->gwion, t)) return type_class(env->gwion, t);
  return t;
}

ANN static Type check_exp_named(const Env env, Exp_Named *exp_named) {
  return check_exp(env, exp_named->exp);
}
DECL_EXP_FUNC(check, Type, Env,)

ANN Type check_exp(const Env env, Exp* exp) {
  Exp* curr = exp;
  bool ok = true;
  env_weight(env, 1);
  do {
    if(curr->type) continue;
    if(curr->poison) { ok = false; continue;}
    if(!(curr->type = check_exp_func[curr->exp_type](env, &curr->d)) || curr->type->error) {
      POISON_NODE(ok, env, curr);
      continue;
    }
    if (env->func && !is_class(env->gwion, curr->type) &&
        is_func(env->gwion, curr->type) &&
        !safe_fflag(curr->type->info->func, fflag_pure))
      unset_fflag(env->func, fflag_pure);
  } while ((curr = curr->next));
  return ok ? exp->type : NULL;
}

ANN bool check_enum_def(const Env env, const Enum_Def edef) {
  const m_uint scope = env_push_type(env, edef->type);
  EnumValueList *list = edef->list;
  for(uint32_t i = 0; i < list->len; i++) {
    const EnumValue ev = enumvaluelist_at(list, i);
    decl_static(env, nspc_lookup_value0(env->curr, ev.tag.sym));
  }
  env_pop(env, scope);
  return true;
}

ANN static bool check_stmt_code(const Env env, const Stmt_Code stmt) {
  if (stmt->stmt_list) { RET_NSPC(check_stmt_list(env, stmt->stmt_list)) }
  return true;
}

ANN static inline bool _check_breaks(const Env env, Stmt* b) {
    RET_NSPC(check_stmt(env, b))}

ANN static bool check_breaks(const Env env, Stmt* a, Stmt* b) {
  vector_add(&env->scope->breaks, (vtype)a);
  const bool ret = _check_breaks(env, b);
  vector_pop(&env->scope->breaks);
  return ret;
}

ANN static bool check_conts(const Env env, Stmt* a, Stmt* b) {
  vector_add(&env->scope->conts, (vtype)a);
  CHECK_B(check_breaks(env, a, b));
  vector_pop(&env->scope->conts);
  return true;
}

ANN static void check_idx(const Env env, const Type base, Var_Decl *const idx) {
  idx->value = new_value(env, base, idx->tag);
  valid_value(env, idx->tag.sym, idx->value);
  SET_FLAG(idx->value, const);
}

/** sets for the key expression value
    with eg  type *int* for an array or the *Key* type of a Dict **/
ANN static bool check_each_idx(const Env env, Exp* exp, Var_Decl *const idx) {
  struct Op_Import opi = {
    .lhs = exp->type,
    .op  = insert_symbol("@each_idx"),
    .data = (m_uint)exp,
    .loc = idx->tag.loc
  };
  DECL_B(const Type, t, = op_check(env, &opi));
  check_idx(env, t, idx);
  return true;
}

/** return the base type for the foreach expression
    eg the base type of an array or the *Val* type of a Dict **/
ANN static Type check_each_val(const Env env, Exp* exp) {
  struct Op_Import opi = {
    .lhs  = exp->type,
    .op   = insert_symbol("@each_val"),
    .data = (m_uint)exp,
    .loc = exp->loc
  };
  return op_check(env, &opi);
}

ANN static bool do_stmt_each(const Env env, const Stmt_Each stmt) {
  CHECK_B(check_exp(env, stmt->exp));
  if (stmt->idx.tag.sym)
    CHECK_B(check_each_idx(env, stmt->exp, &stmt->idx));
  DECL_B(const Type, ret, = check_each_val(env, stmt->exp));
  stmt->var.value = new_value(env, ret, stmt->var.tag);
  valid_value(env, stmt->var.tag.sym, stmt->var.value);
  return check_conts(env, stmt_self(stmt), stmt->body);
}

ANN static bool do_stmt_repeat(const Env env, const Stmt_Loop stmt) {
  if (stmt->idx.tag.sym) check_idx(env, env->gwion->type[et_int], &stmt->idx);
  return check_conts(env, stmt_self(stmt), stmt->body);
}

ANN static inline bool repeat_type(const Env env, Exp* e) {
  const Type t_int = env->gwion->type[et_int];
  if (!check_implicit(env, e, t_int)) {
    char explain[40 + strlen(e->type->name)];
    sprintf(explain, "expected `{/+}int{0}`, got `{/+}%s{0}`", e->type->name);
    gwlog_error(_("invalid repeat condition type"), explain,
                env->name, e->loc, 0);
    gwlog_hint(_("use an integer or cast to int if possible"), env->name, e->loc);
    env_set_error(env, true);
    return false;
  }
  return true;
}

#define stmt_func_xxx(name, type, prolog, exp)                                 \
  describe_stmt_func(check, name, type, prolog, exp)
stmt_func_xxx(if, Stmt_If,, !(!check_flow(env, stmt->cond)   ||
  !check_stmt(env, stmt->if_body) ||
  (stmt->else_body && !check_stmt(env, stmt->else_body))) ? true : false)
stmt_func_xxx(flow, Stmt_Flow, env_inline_mult(env, 1.5),
  !(!check_exp(env, stmt->cond) ||
    !_flow(env, stmt->cond, !stmt->is_do ?
       stmt_self(stmt)->stmt_type == ae_stmt_while :
       stmt_self(stmt)->stmt_type != ae_stmt_while) ||
    !check_conts(env, stmt_self(stmt), stmt->body)) ? true : false)
stmt_func_xxx(for, Stmt_For, env_inline_mult(env, 1.5), !(
  !check_stmt(env, stmt->c1) ||
  !check_flow(env, stmt->c2->d.stmt_exp.val) ||
  (stmt->c3 && !check_exp(env, stmt->c3)) ||
  !check_conts(env, stmt_self(stmt), stmt->body)) ? true : false)
stmt_func_xxx(loop, Stmt_Loop, env_inline_mult(env, 1.5); check_idx(env, stmt->idx), !(!check_exp(env, stmt->cond) ||
  !repeat_type(env, stmt->cond) ||
  !do_stmt_repeat(env, stmt)) ? true : false)
stmt_func_xxx(each, Stmt_Each, env_inline_mult(env, 1.5), do_stmt_each(env, stmt))

ANN static bool check_stmt_return(const Env env, const Stmt_Exp stmt) {
  if (is_new(env->func->def)) {
    if(stmt->val)
      ERR_B(stmt_self(stmt)->loc,
            _("'return' statement inside constructor function should have no expression"));
    return true;
  }
  DECL_B(const Type, ret_type,
          = stmt->val ? check_exp(env, stmt->val) : env->gwion->type[et_void]);
  if (!env->func->def->base->ret_type) {
    env->func->def->base->ret_type = ret_type;
    return true;
  }
  if (isa(ret_type, env->func->def->base->ret_type)) return true;
  if (tflag(ret_type, tflag_noret))
    ERR_B(stmt->val->loc, _("Can't use type `{+G}%s{+G}` for return"),
          ret_type->name);
  if (stmt->val) {
    if (env->func->def->base->tag.sym == insert_symbol("@implicit")) {
      const Arg arg = arglist_at(env->func->def->base->args, 0);
      if(ret_type == arg.type)
      ERR_B(stmt_self(stmt)->loc,
            _("can't use implicit casting while defining it"));
    }
    if (check_implicit(env, stmt->val, env->func->def->base->ret_type))
      return true;
    ERR_B(stmt_self(stmt)->loc,
          _("invalid return type: got '%s', expected '%s'"), ret_type->name,
          env->func->def->base->ret_type->name);
  }
  if (isa(env->func->def->base->ret_type, env->gwion->type[et_void]))
    return true;
  ERR_B(stmt_self(stmt)->loc, _("missing value for return statement"));
}

#define describe_check_stmt_stack(stack, name)                                \
  ANN static bool check_stmt_##name(const Env env, const Stmt* stmt) {        \
    if (!vector_size(&env->scope->stack))                                     \
      ERR_B(stmt->loc, _("'" #name "' found outside of for/while/until...")); \
    return true;                                                              \
  }
describe_check_stmt_stack(conts, continue);
describe_check_stmt_stack(breaks, break);

ANN bool check_union_def(const Env env NUSED, const Union_Def udef) {
  if (tmpl_base(udef->tmpl)) // there's a func for this
    return true;
  set_tflag(udef->type, tflag_check);
  return true;
}

ANN static bool check_stmt_exp(const Env env, const Stmt_Exp stmt) {
  if(stmt->val) {
    CHECK_B(check_exp(env, stmt->val));
    if(stmt->val->exp_type == ae_exp_lambda) {
     const loc_t loc = stmt->val->d.exp_lambda.def->base->tag.loc;
     env_warn(env, loc, _("Partial application not used"));
    }
  }
  return stmt->val ? !!check_exp(env, stmt->val) : true;
}

ANN static Value match_value(const Env env, const Type base,
                             const Exp_Primary *prim) {
  const Symbol sym = prim->d.var;
  const Value  v   = new_value(env, base, MK_TAG(sym, prim_pos(prim)));
  // valuefrom?
  valid_value(env, sym, v);
  return v;
}

ANN Symbol case_basic_op(const Env env, const Type base, Exp* e) {
  const Symbol _op = insert_symbol("==");
  struct Op_Import opi = {.op   = _op,
                          .lhs  = base,
                          .rhs  = e->type,
                          .data = (uintptr_t)NULL,
                          .loc  = e->loc};
  return op_get(env, &opi)
         ? insert_symbol("==")
         : insert_symbol("?=");
}

ANN static Symbol case_op(const Env env, const Type base, Exp* e) {
  if (e->exp_type == ae_exp_primary) {
    if (e->d.prim.prim_type == ae_prim_id) {
      if (e->d.prim.d.var == insert_symbol("_")) return NULL;
      if (!nspc_lookup_value1(env->curr, e->d.prim.d.var)) {
        e->d.prim.value = match_value(env, base, &e->d.prim);
        return NULL;
      }
    }
  } else if (tflag(base, tflag_union) &&
             e->exp_type == ae_exp_call) {
    Exp* func = e->d.exp_call.func;
    if (func->d.prim.prim_type == ae_prim_id) {
      const Value v = find_value(base, func->d.prim.d.var);
      if (v) {
        e->type = v->type;
        case_op(env, v->type, e->d.exp_call.args);
        e->d.exp_call.args->type = v->type;
        return NULL;
      }
    }
  }
  return case_basic_op(env, base, e);
}

ANN static bool match_case_exp(const Env env, Exp* e) {
  Exp* last = e;
  for (m_uint i = 0; i < vector_size(&env->scope->match->cond); e = e->next, ++i) {
    if (!e) ERR_B(last->loc, _("no enough to match"));
    last              = e;
    Exp*    base = (Exp*)vector_at(&env->scope->match->cond, i);
    const Symbol op   = case_op(env, base->type, e);
    if (op) {
      Exp* next = e->next;
      e->next        = NULL;
      const Type t   = check_exp(env, e);
      e->next        = next;
      CHECK_B(t);
      Exp_Binary       bin  = {.lhs = cpy_exp(env->gwion->mp, base), .rhs = cpy_exp(env->gwion->mp, e), .op = op};
      Exp      ebin = {.d = {.exp_binary = bin}, .exp_type = ae_exp_binary, .loc = e->loc };
      CHECK_B(traverse_exp(env, &ebin));
      const Type ret = ebin.type;
      if(ebin.exp_type == ae_exp_binary) {
        free_exp(env->gwion->mp, bin.lhs);
        free_exp(env->gwion->mp, bin.rhs);
      }
      CHECK_B(ret);
    }
  }
  if (e) ERR_B(e->loc, _("too many expression to match"));
  return true;
}

ANN static bool _check_stmt_case(const Env env, const Stmt_Match stmt) {
  CHECK_B(match_case_exp(env, stmt->cond));
  if (stmt->when) CHECK_B(check_flow(env, stmt->when));
  return check_stmt_list(env, stmt->list);
}

ANN static bool check_stmt_case(const Env env, const Stmt_Match stmt) {
    RET_NSPC(_check_stmt_case(env, stmt))}

ANN static bool case_loop(const Env env, const Stmt_Match match) {
  bool ok = true;
  for(m_uint i = 0; i < match->list->len; i++) {
    Stmt* stmt = stmtlist_ptr_at(match->list, i);
    if(!check_stmt_case(env, &stmt->d.stmt_match))
      POISON_NODE(ok, env, stmt);
  }
  return ok;
}

ANN static inline bool check_handler(const restrict Env env,
                                            const Handler *handler) {
  RET_NSPC(check_stmt(env, handler->stmt));
}

ANN static inline bool check_handler_list(const restrict Env env,
                                            const HandlerList *handlers) {
  const bool   in_try = env->scope->in_try;
  env->scope->in_try  = true;
  bool ok = true;
  for(uint32_t i = 0; i < handlers->len; i++) {
    const Handler handler = handlerlist_at(handlers, i);
    if(!check_handler(env, &handler))
      POISON(ok ,env);
  }
  env->scope->in_try  = in_try;
  return ok;
}

ANN static inline bool find_handler(const HandlerList *handlers, const Symbol xid) {
  for(uint32_t i = 0; i < handlers->len; i++) {
    const Handler handler = handlerlist_at(handlers, i);
    if(xid == handler.tag.sym) return true;
    if(!handler.tag.sym) return true;
  }
  return false;
}

ANN static inline bool check_stmt_try_start(const restrict Env env,
                                              const Stmt_Try     stmt) {
    RET_NSPC(check_stmt(env, stmt->stmt))
}

ANN static inline bool check_stmt_try(const restrict Env env, const Stmt_Try stmt) {
  CHECK_B(check_handler_list(env, stmt->handler));
  vector_add(&env->scope->effects, 0);
  const bool ret = check_stmt_try_start(env, stmt);
  ScopeEffectList *const v = (ScopeEffectList*)vector_pop(&env->scope->effects);
  if (v) {
    for (m_uint i = 0; i < v->len; i++) {
      const ScopeEffect eff = scopeeffectlist_at(v, i);
      bool found   = find_handler(stmt->handler, eff.sym);
      if (!found) env_add_effect(env, eff.sym, eff.loc);
    }
    free_scopeeffectlist(env->gwion->mp, v);
  }
  return ret;
}

ANN static bool _check_stmt_match(const Env env, const Stmt_Match stmt) {
  CHECK_B(check_exp(env, stmt->cond));
  MATCH_INI(env->scope)
  const bool ret = case_loop(env, stmt);
  MATCH_END(env->scope)
  return ret;
}

ANN static inline bool handle_where(const Env env, const Stmt_Match stmt) {
  if (stmt->where) CHECK_B(check_stmt(env, stmt->where));
  RET_NSPC(_check_stmt_match(env, stmt))
}

ANN static bool check_stmt_match(const Env env, const Stmt_Match stmt) {
  RET_NSPC(handle_where(env, stmt))
}

#define check_stmt_while check_stmt_flow
#define check_stmt_until check_stmt_flow

ANN static bool check_stmt_pp(const Env env, const Stmt_PP stmt) {
  if (stmt->pp_type == ae_pp_include) env->name = stmt->data;
  // check for memoization
  else if (env->func && stmt->pp_type == ae_pp_pragma &&
      !strncmp(stmt->data, "memoize", strlen("memoize")))
    env->func->memoize = strtol(stmt->data + 7, NULL, 10);
  else if(stmt->pp_type == ae_pp_locale) {
    const loc_t loc = stmt_self(stmt)->loc;
    Exp* id   = new_prim_id(env->gwion->mp, stmt->xid, loc);
    Exp* arg   = new_prim_id(env->gwion->mp, insert_symbol("_"), loc);
    arg->next = stmt->exp;
    Exp* call = new_exp_call(env->gwion->mp, id, arg, loc);
    stmt->exp = call;
    CHECK_B(traverse_exp(env, id));
    CHECK_B(partial_type(env, &call->d.exp_call));
  }
  return true;
}

ANN static bool check_stmt_defer(const Env env, const Stmt_Defer stmt) {
  return check_stmt(env, stmt->stmt);
}

ANN static bool check_stmt_using(const Env env, const Stmt_Using stmt) {
  if(!stmt->tag.sym) {
    DECL_B(const Type, type, = known_type(env, stmt->d.td));
    for(m_uint i = 0; i < map_size(&type->nspc->info->value->map); ++i) {
      const Symbol sym = (Symbol)VKEY(&type->nspc->info->value->map, i);
      const Value value = nspc_lookup_value1(env->curr, sym);
      if(value) {
        char msg[256];
        sprintf(msg, "{Y}%s{0} is already defined", value->name);
        gwlog_error(_(msg), "from this `using` statement", env->name, stmt->d.td->tag.loc, 0);
        declared_here(value);
        const Value other = nspc_lookup_value1(type->nspc, sym);
        declared_here(other);
        return false;
      }
    }
  } else {
    const Value value = nspc_lookup_value1(env->curr, stmt->tag.sym);
    if(value) {
      char msg[256];
      sprintf(msg, "{Y}%s{0} is already defined", value->name);
      gwlog_error(_(msg), NULL, env->name, stmt->tag.loc, 0);
      declared_here(value);
      return false;
    }
    if(!stmt->d.exp->type)
      CHECK_B(check_exp(env, stmt->d.exp));
  }
  usinglist_add(env->gwion->mp, &env->curr->info->gwusing, stmt);
  return true;
}

#define check_stmt_import dummy_func

#define check_stmt_retry dummy_func
#define check_stmt_spread dummy_func
DECL_STMT_FUNC(check, bool, Env,)

ANN bool check_stmt(const Env env, Stmt* stmt) {
  return check_stmt_func[stmt->stmt_type](env, &stmt->d);
}

ANN bool check_stmt_list(const Env env, StmtList *l) {
  bool ok = true;
  const uint32_t nusing = usinglist_len(env->curr->info->gwusing);
  for(m_uint i = 0; i < l->len; i++) {
    Stmt* stmt = stmtlist_ptr_at(l, i);
    if(stmt->poison) { ok = false; continue;}
    if(!check_stmt(env, stmt))
      POISON_NODE(ok, env, stmt);
  }
  if(env->curr->info->gwusing)
    env->curr->info->gwusing->len = nusing;
  return ok;
}

ANN static bool check_signature_match(const Env env, const Func_Def fdef,
                                        const Func parent) {
  if (GET_FLAG(parent->def->base, final))
    ERR_B(fdef->base->td->tag.loc, _("can't override final function '%s'\n"),
          parent->name);
  if (GET_FLAG(parent->def->base, static) != GET_FLAG(fdef->base, static)) {
    const m_str c_name = fdef->base->func->value_ref->from->owner_class->name;
    const m_str p_name = parent->value_ref->from->owner_class->name;
    const m_str f_name = s_name(fdef->base->tag.sym);
    ERR_B(fdef->base->td->tag.loc,
          _("function '%s.%s' ressembles '%s.%s' but cannot override...\n"
            "  ...(reason: '%s.%s' is declared as 'static')"),
          c_name, f_name, p_name, c_name,
          GET_FLAG(fdef->base, static) ? c_name : p_name, f_name);
  }
  if(fdef->base->tmpl || isa(fdef->base->ret_type, parent->def->base->ret_type))
    return true;
  gwlog_error_from("invalid overriding", NULL, fdef->base->func->value_ref->from, 0);
  gwlog_related_from("does not match", parent->value_ref->from);
  env_set_error(env,  true);
  return false;
}

ANN static bool parent_match_actual(const Env               env,
                                      const restrict Func_Def fdef,
                                      const restrict Func     func) {
  Func parent_func = func;
  do {
    if (parent_func->def->base && compat_func(fdef, parent_func->def)) {
      CHECK_B(check_signature_match(env, fdef, parent_func));
      if (!fdef->base->tmpl) {
        fdef->vt_index = parent_func->def->vt_index;
        vector_set(&env->curr->vtable, fdef->vt_index,
                   (vtype)fdef->base->func);
      }
      return true;
    }
  } while ((parent_func = parent_func->next));
  return 0;
}

ANN static bool check_parent_match(const Env env, const Func_Def fdef) {
  const Func func   = fdef->base->func;
  const Type parent = env->class_def->info->parent;
  if (!env->curr->vtable.ptr) vector_init(&env->curr->vtable);
  if (parent) {
    const Value v = find_value(parent, fdef->base->tag.sym);
    if (v && is_func(env->gwion, v->type)) {
      const bool match = parent_match_actual(env, fdef, v->d.func_ref);
      if (match) return match;
    }
  }
  func->def->vt_index = vector_size(&env->curr->vtable);
  vector_add(&env->curr->vtable, (vtype)func);
  return true;
}

ANN static inline Func get_overload(const Env env, const Func_Def fdef,
                                    const m_uint i) {
  const Symbol sym =
      func_symbol(env, env->curr->name, s_name(fdef->base->tag.sym), NULL, i);
  return nspc_lookup_func1(env->curr, sym);
}

ANN static bool check_func_overload(const Env env, const Func_Def fdef) {
  const Value v = fdef->base->func->value_ref;
  for (m_uint i = 0; i <= v->from->offset; ++i) {
    const Func f1 = get_overload(env, fdef, i);
    for (m_uint j = i + 1; f1 && j <= v->from->offset; ++j) {
      const Func f2 = get_overload(env, fdef, j);
      if (f2 && compat_func(f1->def, f2->def) &&
          fbflag(f1->def->base, fbflag_op) ==
              fbflag(f2->def->base, fbflag_op) &&
          fbflag(f1->def->base, fbflag_unary) ==
              fbflag(f2->def->base, fbflag_unary) &&
          fbflag(f1->def->base, fbflag_postfix) ==
              fbflag(f2->def->base, fbflag_postfix))
        ERR_B(f2->def->base->tag.loc,
              _("global function '%s' already defined"
                " for those arguments"),
              s_name(fdef->base->tag.sym));
    }
  }
  return true;
}

ANN static bool check_effect_overload(const Vector base, const Func override) {
  if (!base->ptr) return true;
  if (!override->def->base->effects.ptr) return false;
  const Vector v = &override->def->base->effects;
  for (m_uint i = 0; i < vector_size(v); i++) {
    if (vector_find(base, vector_at(v, i)) == -1) return false;
  }
  return true;
}

ANN static bool check_func_def_override(const Env env, const Func_Def fdef,
                                          Value *ov) {
  const Func func = fdef->base->func;
  if (env->class_def && env->class_def->info->parent) {
    const Value override =
        find_value(env->class_def->info->parent, fdef->base->tag.sym);
    if (override && override->from->owner_class &&
        !is_func(env->gwion, override->type))
      ERR_B(fdef->base->tag.loc,
            _("function name '%s' conflicts with previously defined value...\n"
              "  from super class '%s'..."),
            s_name(fdef->base->tag.sym), override->from->owner_class->name);
    *ov = override;
  }
  if (func->value_ref->from->offset &&
      (!fdef->base->tmpl /*|| !fdef->base->tmpl->base*/))
    CHECK_B(check_func_overload(env, fdef));
  return true;
}

ANN static bool check_fdef_effects(const Env env, const Func_Def fdef) {
  ScopeEffectList *v = (ScopeEffectList*)vector_back(&env->scope->effects);
  if (v) {
    if (fdef->base->tag.sym == insert_symbol("@dtor"))
      ERR_B(fdef->base->tag.loc, _("can't use effects in destructors"));
    const Vector base = &fdef->base->effects;
    if (!base->ptr) vector_init(base);
    for (uint32_t i = 0; i < v->len; i++) {
      const ScopeEffect eff = scopeeffectlist_at(v, i);
      if(vector_find(base, (m_uint)eff.sym) == -1)
        vector_add(base, (m_uint)eff.sym);
    }
    free_scopeeffectlist(env->gwion->mp, v);
  }
  return true;
}

ANN static void fdef_const_generic_value(const Env env, const Type owner, const Type t, const Tag tag) {
  const Value v = new_value(env, t, tag);
  SET_FLAG(v, static|ae_flag_const);
  valid_value(env, tag.sym, v);
  nspc_add_value_front(owner->nspc, tag.sym, v);
  v->from->owner = owner->nspc;
  v->from->owner_class = owner;
}

ANN static bool check_fdef_const_generic(const Env env, const Func_Def fdef) {
  const Tmpl *tmpl = fdef->base->tmpl;
  if(tmplarg_ntypes(tmpl->call) == tmpl->call->len) return true;
  const Type t = fdef->base->func->value_ref->type;
  assert(!t->nspc);
  t->nspc = new_nspc(env->gwion->mp, t->name);

  bool ok = true;
  for(uint32_t i = 0; i < fdef->base->tmpl->call->len; i++) {
    const TmplArg targ = tmplarglist_at(fdef->base->tmpl->call, i);
    if(targ.type == tmplarg_td)continue;
    // spec could be null cause of spread ops
    // there is some other instance we need to check.
    // maybe unify?
    const Specialized *spec = specializedlist_ptr_at(fdef->base->tmpl->list, i);
    if(!spec) break;
    if(!const_generic_typecheck(env, spec, &targ))
      POISON(ok, env);
    fdef_const_generic_value(env, t, targ.d.exp->type, spec->tag);
  }
  return ok;
}

ANN static bool check_fdef_code(const Env env, const Func_Def fdef) {
  const bool ctor = is_ctor(fdef);
  if(!ctor) {
    nspc_push_value(env->gwion->mp, env->curr);
    env->scope->depth++;
  }
  (void)check_stmt_list(env, fdef->d.code);
  if(!ctor) {
    env->scope->depth--;
    nspc_pop_value(env->gwion->mp, env->curr);
  }
  return check_fdef_effects(env, fdef);
}

ANN bool check_fdef(const Env env, const Func_Def fdef) {
  if (fdef->base->args) CHECK_B(check_func_args(env, fdef->base->args));
  if (fdef->base->tmpl) CHECK_B(check_fdef_const_generic(env, fdef));
  if (fdef->builtin) return true;
  if (fdef->d.code) CHECK_B(check_fdef_code(env, fdef));
  return true;
}

ANN static bool check_ctor(const Env env, const Func func) {
  if(!func->def->builtin && !GET_FLAG(func, const)) {
    const Type_Decl *td = env->class_def->info->cdef->base.ext;
    const m_uint depth = !td || !td->array
      ? 1 : td->array->exp->d.prim.d.gwint.num;
    if(depth) { // check if size is 0
      const Type parent = env->class_def->info->parent;
      const Value v = nspc_lookup_value0(parent->nspc, insert_symbol("new"));
      if(v && !GET_FLAG(v->d.func_ref->def->base, abstract))
        ERR_B(func->def->base->tag.loc, "missing call to parent constructor");
    }
  }
  return true;
}

ANN bool _check_func_def(const Env env, const Func_Def f) {
  const Func     func = f->base->func;
  const Func_Def fdef = func->def;
  if(fflag(func, fflag_valid))return true;
  set_fflag(func, fflag_valid);
  assert(func == fdef->base->func);
  if (env->class_def && !fbflag(func->def->base, fbflag_lambda))
    CHECK_B(check_parent_match(env, fdef));
  if (tmpl_base(fdef->base->tmpl)) return true;
  Value override = NULL;
  CHECK_B(check_func_def_override(env, fdef, &override));
  const m_int scope = GET_FLAG(fdef->base, global)
                                    ? env_push_global(env)
                                    : env->scope->depth;
  const Func former = env->func;
  env->func         = func;
  nspc_push_value(env->gwion->mp, env->curr);
//  env->scope->depth++;
  struct Op_Import opi = {};
  if (fbflag(fdef->base, fbflag_op)) {
    func_operator(f, &opi);
    operator_suspend(env->gwion, &opi);
  }
  if(fdef->captures) {
    uint32_t offset = fdef->stack_depth;
    for(uint32_t i = 0; i < fdef->captures->len; i++) {
      const Capture cap = capturelist_at(fdef->captures, i);
      valid_value(env, cap.var.tag.sym, cap.temp);
      cap.temp->from->offset = offset;
      offset += cap.temp->type->size;
    }
  }
  vector_add(&env->scope->effects, 0);
  const bool ret = scanx_fdef(env, env, fdef, (_envset_func)check_fdef);
  vector_pop(&env->scope->effects);
  if (fbflag(fdef->base, fbflag_op)) operator_resume(&opi);
  nspc_pop_value(env->gwion->mp, env->curr);
//  env->scope->depth--;
  env->func = former;
  if (ret) {
    if (env->class_def && fdef->base->effects.ptr &&
        (override &&
         !check_effect_overload(&fdef->base->effects, override->d.func_ref))) {
      ERR_B(fdef->base->tag.loc, _("too much effects in override."),
            s_name(fdef->base->tag.sym));
      }
      if(is_new(f) && !tflag(env->class_def, tflag_struct))
        CHECK_B(check_ctor(env, func));
  }
  if (GET_FLAG(fdef->base, global)) env_pop(env, scope);
  if (func->value_ref->from->owner_class)
    func->inline_mult += 3;
  else
    func->inline_mult += 4;
  return ret;
}

ANN bool check_func_def(const Env env, const Func_Def fdef) {
  const uint16_t depth = env->scope->depth;
  env->scope->depth = 0;
  const bool ret = _check_func_def(env, fdef);
  env->scope->depth = depth;
  return ret;
}

ANN bool check_trait_requests(const Env env, const Type t, const TagList *list, const ValueFrom *from);
ANN static bool check_extend_def(const Env env, const Extend_Def xdef) {
  const Type t = xdef->type;
  ValueFrom from = { .filename = env->name, .loc=xdef->td->tag.loc, .ctx=env->context,
      .owner = env->curr, .owner_class = env->class_def
  };
  const bool ret = check_trait_requests(env, t, xdef->traits, &from);
  if(ret) {
    if(!t->info->traits) {
      t->info->traits = new_taglist(env->gwion->mp, xdef->traits->len);
      for(uint32_t i = 0; i < xdef->traits->len; i++) {
        const Tag tag = taglist_at(t->info->traits, i);
        taglist_set(t->info->traits, i, tag);
      }
    } else {
      for(uint32_t i = 0; i < xdef->traits->len; i++) {
        const Tag tag = taglist_at(t->info->traits, i);
        taglist_add(env->gwion->mp, &t->info->traits, tag);
      }
    }
    return true;
  }
  return false;
}

ANN static bool _check_trait_def(const Env env, const Trait_Def pdef) {
  const Trait trait = nspc_lookup_trait1(env->curr, pdef->tag.sym);
  Ast         ast   = pdef->body;
  if(!ast) return true;
  bool ok = true;
  for(m_uint i = 0; i < ast->len; i++) {
    const Section section = sectionlist_at(ast, i);
    // NOTE: we should traverse the funcs here also
    if (section.section_type == ae_section_stmt) {
      StmtList *l = section.d.stmt_list;
      for(m_uint i = 0; i < l->len; i++) {
        const Stmt* stmt = stmtlist_ptr_at(l, i);
        if (stmt->stmt_type == ae_stmt_exp) {
          if(!traverse_exp(env, stmt->d.stmt_exp.val)) {
            POISON_NODE(ok, env, stmt->d.stmt_exp.val);
            continue;
          }
          Var_Decl vd = stmt->d.stmt_exp.val->d.exp_decl.var.vd;
          const Value value = vd.value;
          valuefrom(env, value->from);
          if (!trait->var)
            trait->var = new_valuelist(env->gwion->mp, 0);
          valuelist_add(env->gwion->mp, &trait->var, value);
        }
      }
    }
  }
  return ok;
}

ANN static bool check_trait_def(const Env env, const Trait_Def pdef) {
  RET_NSPC(_check_trait_def(env, pdef));
}

ANN bool check_fptr_def(const Env env, const Fptr_Def fptr) {
if(fptr->cdef->base.type->error) return false;
  if(GET_FLAG(fptr->cdef, global)) env_push_global(env);
  const bool ret = check_class_def(env, fptr->cdef);
  if(GET_FLAG(fptr->cdef, global)) env_pop(env, 0);
  return ret;
}

#define check_prim_def dummy_func
HANDLE_SECTION_FUNC(check, bool, Env,)

ANN static bool check_parent(const Env env, const Class_Def cdef) {
  const Type       parent = cdef->base.type->info->parent;
  const Type_Decl *td     = cdef->base.ext;
  if (td->array && td->array->exp)
    CHECK_B(check_subscripts(env, td->array, 1));
  CHECK_B(ensure_traverse(env, parent));
  if(GET_FLAG(parent, abstract))
    SET_FLAG(cdef->base.type, abstract);
  return true;
}

ANN static bool cdef_parent(const Env env, const Class_Def cdef) {
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) CHECK_B(template_push_types(env, cdef->base.tmpl));
  const bool ret = check_parent(env, cdef);
  if (tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  return ret;
}

ANN bool check_abstract(const Env env, const Class_Def cdef) {
  if (!cdef->base.type->nspc->vtable.ptr) return true;
  bool err = false;
  for (m_uint i = 0; i < vector_size(&cdef->base.type->nspc->vtable); ++i) {
    const Func f = (Func)vector_at(&cdef->base.type->nspc->vtable, i);
   if (f && f->def->base && GET_FLAG(f->def->base, abstract)) {
      if (!err) {
        err = true;
        gwlog_error(_("missing function definition"),
                    _("must be declared 'abstract'"),
                    env->name, cdef->base.tag.loc, 0);
      }
      ValueFrom *from = f->value_ref->from;
      gwlog_related_from("implementation missing", from);
      env_set_error(env, true);
    }
  }
  return !err;
}

ANN static bool class_def_has_body(Ast ast) {
  const Section section = sectionlist_at(ast, 0);
  if(section.section_type != ae_section_func) return false;
  Func_Def f = section.d.func_def;
  if(strcmp(s_name(f->base->tag.sym), "@ctor"))return false;
  StmtList *l = f->d.code;
  for(m_uint i = 0; i < l->len; i++) {
    const Stmt stmt = stmtlist_at(l, i);
    if(stmt.poison)continue;
    if (stmt.stmt_type == ae_stmt_pp) continue;
    if (stmt.stmt_type == ae_stmt_exp) {
      const Exp* exp = stmt.d.stmt_exp.val;
      if (!exp) continue;
      if (exp->exp_type != ae_exp_decl) return true;
      if (GET_FLAG(exp->d.exp_decl.var.td, late)) continue;
      const Var_Decl vd = exp->d.exp_decl.var.vd;
      if (GET_FLAG(vd.value, late)) continue;
      if (tflag(vd.value->type, tflag_compound))
        return true;
    } else return true;
  }
  return false;
}

ANN static inline bool type_is_recurs(const Type t, const Type tgt) {
  return isa(tgt, t) || isa(t, tgt) || (tgt->info->tuple && vector_find(&tgt->info->tuple->contains, (m_uint)t) > -1);
}

ANN static bool recursive_type_base(const Env env, const Type t);
ANN static bool recursive_type(const Env env, const Type t, const Type tgt);
ANN static bool recursive_value(const Env env, const Type t, const Value v) {
  const Type tgt = array_base(v->type);
  if(type_is_recurs(t, tgt)) {
    env_err(env, v->from->loc, _("recursive type"));
    env_set_error(env,  false);
    gwlog_related("in class", t->name, t->info->cdef->base.tag.loc);

    const Type first = tgt->info->value->from->loc.first.line < t->info->value->from->loc.first.line ?
      v->type : t;
    const Type second = tgt->info->value->from->loc.first.line > t->info->value->from->loc.first.line ?
      v->type : t;

    if(first != second) {
      const Map m1 = &first->info->value->from->owner->info->type->map;
      map_remove(m1, (m_uint)insert_symbol(first->name));
      const Map m2 = &second->info->value->from->owner->info->type->map;
      map_remove(m2, (m_uint)insert_symbol(second->name));
      if(first->ref > 2)
        type_remref(first, env->gwion);
      if(second->ref > 2)
        type_remref(second, env->gwion);
      if(v->from->owner_class->ref > 2)
        type_remref(v->from->owner_class, env->gwion);
      if(t->ref > 2)
        type_remref(t, env->gwion);
    }
    set_tflag(t, tflag_infer);
    set_tflag(tgt, tflag_infer);
    unset_tflag(t, tflag_check);
    unset_tflag(tgt, tflag_check);
    return true;
  }

  if(t != tgt && v->type->nspc && (!GET_FLAG(v, late) ||  vflag(v, vflag_assigned)) && strncmp(tgt->name, "Option:[", 8) &&
      tflag(tgt, tflag_compound))
    return recursive_type(env, t, tgt);

  return false;
}

ANN static bool recursive_type(const Env env, const Type t, const Type tgt) {
  Value             v;
  struct scope_iter inner = {tgt->nspc->info->value, 0, 0};
  bool error = false;
  while (scope_iter(&inner, &v)) {
    if((!GET_FLAG(v, late) || vflag(v, vflag_assigned)) && v->type != tgt && recursive_value(env, t, v)) {
      error = true;
    }
  }
  return error;
}

ANN static bool recursive_type_base(const Env env, const Type t) {
  Value             value;
  bool error = false;
  struct scope_iter iter = {t->nspc->info->value, 0, 0};
  while (scope_iter(&iter, &value)) {
    if (!tflag(value->type, tflag_compound)) continue;
    if (value->type->nspc && (!GET_FLAG(value, late) || vflag(value, vflag_assigned))) {
      if(value->type == t || recursive_type(env, t, value->type)) {
        env_err(env, value->from->loc, _("recursive type"));
        gw_err("use {+G}late{0} on one (or more) of the variables?\n");
        error = true;
      }
    }
  }
  return !error;
}

ANN static bool check_class_tmpl(const Env env, const Class_Def cdef) {
  bool ok = true;
  const Type t = cdef->base.type;
  const Tmpl *tmpl = cdef->base.tmpl;
  if(tmplarg_ntypes(tmpl->call) != tmpl->list->len) { // TODO: there multiple occurences, mk a func?
    env_push_type(env, t);
    for(uint32_t i = 0; i < tmpl->list->len; i++) {
      const TmplArg targ = tmplarglist_at(tmpl->call, i);
      if(likely(targ.type == tmplarg_td)) continue;
      if(!check_exp(env, targ.d.exp)) {
        POISON_NODE(ok, env, targ.d.exp);
        continue;
      }
      const Specialized spec = specializedlist_at(tmpl->list, i);
      const Value v = new_value(env, targ.d.exp->type, MK_TAG(spec.tag.sym, targ.d.exp->loc));
      valuefrom(env, v->from);
      set_vflag(v, vflag_valid);
      nspc_add_value(t->nspc, spec.tag.sym, v);
      SET_FLAG(v, const| ae_flag_static);
      set_vflag(v, vflag_builtin);
    }
    env_pop(env, 0);
  }
  return ok;
}

ANN static bool _check_class_def(const Env env, const Class_Def cdef) {
  const Type t = cdef->base.type;
  if (cdef->base.ext) CHECK_B(cdef_parent(env, cdef));
  if (!tflag(t, tflag_struct)) inherit(t);
  if(cdef->base.tmpl) CHECK_B(check_class_tmpl(env, cdef));
  if (cdef->body) {
    if(env_body(env, cdef, check_section)) {
      if (cflag(cdef, cflag_struct) || class_def_has_body(cdef->body))
        set_tflag(t, tflag_ctor);
    }
  }
  if (!GET_FLAG(cdef, abstract)) CHECK_B(check_abstract(env, cdef));
  if (cdef->traits) {
    TagList *list        = cdef->traits;
    if (!check_trait_requests(env, t, list, t->info->value->from)) {
      env->class_def = t;
      env_set_error(env, true);
      return false;
    }
  }
  CHECK_B(recursive_type_base(env, t));
  nspc_allocdata(env->gwion->mp, t->nspc);
  return true;
}

ANN bool check_class_def(const Env env, const Class_Def cdef) {
  if (tmpl_base(cdef->base.tmpl))
    return true;
  const Type       t   = cdef->base.type;
  if (tflag(t, tflag_check)) return true;
  set_tflag(t, tflag_check);
  return _check_class_def(env, t->info->cdef);
}

ANN static inline void check_unhandled(const Env env) {
  const Vector v  = &env->scope->effects;
  ScopeEffectList *const w = (ScopeEffectList*)vector_back(v);
  if(!w) return;
  for (m_uint j = 0; j < w->len; j++) {
    const ScopeEffect eff = scopeeffectlist_at(w, j);
    if(s_name(eff.sym)[0] == '!')
      continue;
    gwlog_warning("Unhandled effect", env->name, eff.loc);
    env_set_error(env,  false);
  }
  free_scopeeffectlist(env->gwion->mp, w);
  vector_pop(v);
}

ANN static bool check_extend(const Env env, Ast ast) {
  bool ok = true;
  for(m_uint i = 0; i < ast->len; i++) {
    Section * section = sectionlist_ptr_at(ast, i);
    if(section->poison) continue;
    if(!check_section(env, section)) {
      POISON_SECTION(ok, env, section);
      continue;
    }
    sectionlist_add(env->gwion->mp, &env->context->tree, *section);
  }
  free_sectionlist(env->gwion->mp, env->context->extend);
  env->context->extend = NULL;
  return ok;
}

ANN bool check_ast(const Env env, Ast *ast) {
  Ast a = *ast;
  bool ok = true;
  for(m_uint i = 0; i < a->len; i++) {
    Section *section = sectionlist_ptr_at(a, i);
    if(section->poison) { ok = false; continue;} 
    if(!check_section(env, section))
      POISON_SECTION(ok, env, section);
  }
  if(env->context->extend) check_extend(env, env->context->extend);
  if(ok && vector_size(&env->scope->effects)) check_unhandled(env);
  return ok;
}
