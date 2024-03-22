#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "template.h"
#include "tmp_resolve.h"
#include "vm.h"
#include "parse.h"
#include "gwion.h"
#include "operator.h"
#include "instr.h"
#include "object.h"
#include "import.h"

ANN static bool _push_types(const Env env, const Nspc nspc,
                              const Tmpl *tmpl) {
  Specialized_List sl = tmpl->list;
  TmplArg_List        tl = tmpl->call;
  Specialized *spec = mp_vector_at(sl, Specialized, sl->len - 1);

  const uint32_t len = strcmp(s_name(spec->tag.sym), "...") ? sl->len : sl->len-1;
  if(!tl) return true;
  for(uint32_t i = 0; i < len; i++) {
    if (i >= tl->len) return true;
    TmplArg arg = *mp_vector_at(tl, TmplArg, i);
    if(unlikely(arg.type == tmplarg_exp)) continue;
    const Type t = known_type(env, arg.d.td);
    Specialized *spec = mp_vector_at(sl, Specialized, i);
    nspc_add_type(nspc, spec->tag.sym, t);
  };
  if(len != sl->len) return true;
  return tl->len == sl->len;
}

ANN static bool push_types(const Env env, const Nspc nspc, const Tmpl *tmpl) {
  const Type t = env->class_def;
  if (t) {
    env->class_def = t->info->value->from->owner_class;
    env->curr = t->info->value->from->owner;
  }
  const bool ret = _push_types(env, nspc, tmpl);
  if (t) {
    env->class_def = t;
    env->curr = t->nspc;
  }
  return ret;
}

ANN static bool _template_push(const Env env, const Type t) {
  if (t->info->value->from->owner_class)
    CHECK_B(template_push(env, t->info->value->from->owner_class));
  return tflag(t, tflag_tmpl)
     ? push_types(env, t->nspc, get_tmpl(t))
     : true;
}

ANN bool template_push(const Env env, const Type t) {
   nspc_push_type(env->gwion->mp, env->curr);
   return _template_push(env, t);
}

#include <ctype.h>
ANN Exp* td2exp(const MemPool mp, const Type_Decl *td);
ANN void check_call(const Env env, const Tmpl *tmpl) {
  for(uint32_t i = 0; i < tmpl->call->len; i++) {
    Specialized *spec = i < tmpl->list->len
       ? mp_vector_at(tmpl->list, Specialized, i)
       : NULL;
    TmplArg *targ = mp_vector_at(tmpl->call, TmplArg, i);
//if(targ->type != tmplarg_td) exit(16);
if(targ->type == tmplarg_td) {
    if(spec && strcmp(s_name(spec->tag.sym), "...")) {
if(unlikely(spec->td)) {
if(targ->type == tmplarg_td) {
targ->type = tmplarg_exp;
Exp* exp = td2exp(env->gwion->mp, targ->d.td);
targ->d.exp = exp;

}
}
else      if(spec->tag.sym == targ->d.td->tag.sym) {
        if (!nspc_lookup_type1(env->curr, spec->tag.sym))
          targ->d.td->tag.sym = insert_symbol("auto");
        else {
           const Type t = nspc_lookup_type1(env->curr, spec->tag.sym);
           Type_Decl *td = type2td(env->gwion, t, targ->d.td->tag.loc);
           free_type_decl(env->gwion->mp, targ->d.td);
           targ->d.td = td;
        }
      } else { // normal case?r
//exit(11 + !!spec->xid);
   }
}
    } else {
      //if(targ->type == tmplarg_td)
      //  targ->d.td->xid = insert_symbol("auto");
      //else what
    }
  }
}

ANN bool const_generic_typecheck(const Env env, const Specialized *spec, const TmplArg *targ) {
  CHECK_B(check_exp(env, targ->d.exp));
  // check implicits?
  const Type target = known_type(env, spec->td);
  if(!isa(targ->d.exp->type, target)) {
    char msg[256];
    tcol_snprintf(msg, 255, "expected {G+}%s{0}", target->name);
    gwlog_error("invalid type for const generic argument", msg, env->name, spec->tag.loc, 0);
    tcol_snprintf(msg, 255, "got {G+}%s{0}", targ->d.exp->type->name);
    gwlog_related(msg, env->name, targ->d.exp->loc);
    return false;
  }
  return true;
}

ANN bool template_push_types(const Env env, const Tmpl *tmpl) {
  nspc_push_type(env->gwion->mp, env->curr);
  if (tmpl->call) check_call(env, tmpl);
  if (push_types(env, env->curr, tmpl) > 0) return true;
  POP_RET(false);
}

ANN Tmpl *mk_tmpl(const Env env, const Tmpl *tm, const TmplArg_List types) {
  Tmpl *tmpl = new_tmpl(env->gwion->mp, tm->list);
  tmpl->call = cpy_tmplarg_list(env->gwion->mp, types);
  return tmpl;
}

static ANN Type scan_func(const Env env, const Type t, const Type_Decl *td) {
  DECL_O(const m_str, tl_name, = tl2str(env->gwion, td->types, td->tag.loc));
  const Symbol sym = func_symbol(env, t->info->value->from->owner->name,
                                 t->info->func->name, tl_name, 0);
  free_mstr(env->gwion->mp, tl_name);
  const Type base_type = nspc_lookup_type1(t->info->value->from->owner, sym);
  if (base_type) return base_type;
  const Type ret    = type_copy(env->gwion->mp, t);
  ret->info->parent = t;
  ret->info->value  = t->info->func->value_ref;
  ret->name         = s_name(sym);
  set_tflag(ret, tflag_ftmpl);
  nspc_add_type_front(t->info->value->from->owner, sym, ret);
  void *func_ptr = t->info->func->def->d.dl_func_ptr;
  if (vflag(t->info->func->value_ref, vflag_builtin))
    t->info->func->def->d.dl_func_ptr = NULL;
  const Func_Def def  = cpy_func_def(env->gwion->mp, t->info->func->def);
  const Func     func = ret->info->func =
      new_func(env->gwion->mp, s_name(sym), def);
  const Value value = new_value(env, ret, MK_TAG(sym, def->base->tag.loc));
  func->flag        = def->base->flag;
  if (vflag(t->info->func->value_ref, vflag_member))
    set_vflag(value, vflag_member);
  value->d.func_ref  = func;
  value->from->owner = t->info->value->from->owner;
  value->from->owner_class = t->info->value->from->owner_class;
  func->value_ref = value;
  func->def->base->tmpl =
      mk_tmpl(env, t->info->func->def->base->tmpl, td->types);
  def->base->func = func;
  nspc_add_value_front(t->info->value->from->owner, sym, value);
  if (vflag(t->info->func->value_ref, vflag_builtin)) {
    builtin_func(env->gwion, func, func_ptr);
    t->info->func->def->d.dl_func_ptr = func_ptr;
  }
  return ret;
}

static ANN Type maybe_func(const Env env, const Type t, const Type_Decl *td) {
  if (is_func(env->gwion, t) && t->info->func->def->base->tmpl) // is_callable needs type
    return scan_func(env, t, td);
  ERR_O(td->tag.loc,
        _("type '%s' is not template. You should not provide template types"),
        t->name);
}

static ANN bool is_single_variadic(const MP_Vector *v) {
  if(v->len != 1) return false;
  const Specialized *spec = mp_vector_at(v, Specialized, 0);
  return !strcmp(s_name(spec->tag.sym), "...");
}

ANN2(1,2) bool check_tmpl(const Env env, const TmplArg_List tl, const Specialized_List sl, const loc_t loc, const bool is_spread) {
  if (!sl || sl->len > tl->len || (tl->len != sl->len && !is_spread))
     ERR_B(loc, "invalid template type number");
  for (uint32_t i = 0; i < sl->len; i++) {
    TmplArg *targ = mp_vector_at(tl, TmplArg, i);
    Specialized *spec = mp_vector_at(sl, Specialized, i);
    if(targ->type == tmplarg_td) {
      if(spec->td) {
       Type_Decl *base = targ->d.td;
       Type_Decl *next = base;
       Type_Decl *last = next->next;
       while(next && last) {
       if(!last->next) break;
         last = last->next;
         next = next->next;
       }
       if(last) {
         next->next = NULL;
         const Type t = known_type(env, base);
         if(t) {
           targ->type = tmplarg_exp;
           Exp* e = new_exp_td(env->gwion->mp, base, base->tag.loc);
           targ->d.exp = new_exp_dot(env->gwion->mp, e, last->tag.sym, base->tag.loc);
           free_type_decl(env->gwion->mp, last);
           i--;
           continue;
         }
         next->next = last;
       }
      ERR_B(loc, "template type argument mismatch. expected %s",
          spec->td ? "constant" : "type");
      }

      DECL_B(const Type, t, = known_type(env, targ->d.td));
      if(spec->traits) {
        Symbol missing = miss_traits(t, spec);
        if (missing) {
          ERR_B(loc, "does not implement requested trait '{/}%s{0}'",
              s_name(missing));
        }
      }
    } else {
      if(!spec->td) {
        ERR_B(loc, "template const argument mismatch. expected %s",
            spec->td ? "constant" : "type");
      }
      CHECK_B(const_generic_typecheck(env, spec, targ));
    }
  }
  return true;
}

ANN static Type _scan_type(const Env env, const Type t, Type_Decl *td) {
  if (tflag(t, tflag_tmpl) && !is_func(env->gwion, t)) { // is_callable
    if (tflag(t, tflag_ntmpl) && !td->types) return t;
    Tmpl *tmpl = get_tmpl(t);
    const bool single_variadic = is_single_variadic(tmpl->list);
    if(!td->types) {
      const Type new_type = nspc_lookup_type1(env->curr, td->tag.sym);
      Type_Decl *new_td = type2td(env->gwion, new_type, td->tag.loc);
      Type_Decl *d = new_td;
      while(d->next) d = d->next;
      if(!d->types) {
        if(!single_variadic)
          ERR_O(td->tag.loc, _("you must provide template types for type '%s'"), t->name);
        d->types = new_mp_vector(env->gwion->mp, TmplArg, 0);
      }
      const Type ret = _scan_type(env, t, d);
      free_type_decl(env->gwion->mp, new_td);
      return ret;
    }
    struct TemplateScan ts = {.t = t, .td = td};
    TmplArg_List           tl = td->types;
    Specialized_List    sl = tmpl
        ? tmpl->list : NULL;
    const bool is_spread = is_spread_tmpl(tmpl);
    if(!single_variadic)
      CHECK_O(check_tmpl(env, tl, sl, td->tag.loc, is_spread));
    struct Op_Import opi = {.op   = insert_symbol("class"),
                            .lhs  = t,
                            .data = (uintptr_t)&ts,
                            .loc  = td->tag.loc};
    return op_check(env, &opi);
  } else if (td->types)
    return maybe_func(env, t, td);
  return find_type(env, td);
}

ANN Type scan_type(const Env env, const Type t, Type_Decl *td) {
  if (td->next) {
    Type_Decl *next        = td->next;
    td->next               = NULL;
    DECL_O(const Type, maybe_array, = known_type(env, td));
    const Type owner       = array_base_simple(maybe_array);
    td->next               = next;
    CHECK_O(owner);
    if (!owner->nspc) ERR_O(td->tag.loc, "type '%s' has no namespace", owner->name);
    struct EnvSet es = {.env   = env,
                        .data  = env,
                        .scope = env->scope->depth,
                        .flag  = tflag_none};
    envset_pushv(&es, owner->info->value);
    (void)env_push_type(env, owner);
    const Type ret = known_type(env, td->next);
    env_pop(env, es.scope);
    envset_popv(&es, owner->info->value);
    if (!td->array) return ret;
    return array_type(env, ret, td->array->depth, td->tag.loc);
  }
  return !t->array_depth ? _scan_type(env, t, td) : t;
}
