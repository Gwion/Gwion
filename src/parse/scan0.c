#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "template.h"
#include "parse.h"
#include "gwion.h"
#include "object.h"
#include "instr.h"
#include "operator.h"
#include "import.h"
#include "spread.h"

static inline void add_type(const Env env, const Nspc nspc, const Type t) {
  nspc_add_type_front(nspc, insert_symbol(t->name), t);
}

static inline void context_global(const Env env) {
  if (env->context) env->context->global = true;
}

static inline Type scan0_type(const Env env, const m_str name, const Type t) {
  return new_type(env->gwion->mp, name, t);
}

ANN static inline m_bool scan0_defined(const Env env, const Symbol s,
                                       const loc_t pos) {
  if (nspc_lookup_type1(env->curr, s))
    ERR_B(pos, _("type '%s' already defined"), s_name(s));
  return already_defined(env, s, pos);
}

ANN static Arg_List fptr_arg_list(const Env env, const Fptr_Def fptr) {
  if(env->class_def && !GET_FLAG(fptr->base, static)) {
    Arg arg = { .td = type2td(env->gwion, env->class_def, fptr->base->td->pos) };
    if(fptr->base->args) {
      Arg_List args = new_mp_vector(env->gwion->mp, Arg, fptr->base->args->len + 1);
      mp_vector_set(args, Arg, 0, arg);
      for(uint32_t i = 0; i < fptr->base->args->len; i++) {
        Arg *base  = mp_vector_at(fptr->base->args, Arg, i);
        Arg arg = { .td = cpy_type_decl(env->gwion->mp, base->td) };
        mp_vector_set(args, Arg, i+1, arg);
      }
      return args;
    } else {
      Arg_List args = new_mp_vector(env->gwion->mp, Arg, 1);
      mp_vector_set(args, Arg, 0, arg);
      return args;
    }
  } else if(fptr->base->args)
    return cpy_arg_list(env->gwion->mp, fptr->base->args);
  return NULL;
}

ANN m_bool scan0_fptr_def(const Env env, const Fptr_Def fptr) {
  const loc_t loc = fptr->base->td->pos;
  CHECK_BB(env_access(env, fptr->base->flag, loc));
  CHECK_BB(scan0_defined(env, fptr->base->xid, loc));
  const Arg_List args = fptr_arg_list(env, fptr);
  Func_Base *const fbase = new_func_base(env->gwion->mp, cpy_type_decl(env->gwion->mp, fptr->base->td),
    insert_symbol("func"), args, ae_flag_static | ae_flag_private, loc);
  const Func_Def fdef = new_func_def(env->gwion->mp, fbase, NULL);
  Ast body = new_mp_vector(env->gwion->mp, Section, 1);
  mp_vector_set(body, Section, 0, MK_SECTION(func, func_def, fdef));
  Type_Decl* td = new_type_decl(env->gwion->mp, insert_symbol(env->gwion->type[et_closure]->name), loc);
  const Class_Def cdef = new_class_def(env->gwion->mp, ae_flag_final, fptr->base->xid, td, body, loc);
  if(GET_FLAG(fptr->base, global)) SET_FLAG(cdef, global);
  if(fptr->base->tmpl) {
    fbase->tmpl = cpy_tmpl(env->gwion->mp, fptr->base->tmpl);
    cdef->base.tmpl = cpy_tmpl(env->gwion->mp, fptr->base->tmpl);
  }
  fptr->cdef = cdef;
  return scan0_class_def(env, cdef);
}

static OP_CHECK(opck_implicit_similar) {
  const struct Implicit *imp = (struct Implicit *)data;
  return imp->e->type;
}

static OP_CHECK(opck_contract_similar) {
  const Exp_Cast *cast = (Exp_Cast *)data;
  if(tflag(exp_self(cast)->type, tflag_contract)) {
    struct Implicit imp = { .t=exp_self(cast)->type, .e=cast->exp};
    struct Op_Import opi    = {
      .op = insert_symbol("@implicit"), .lhs = cast->exp->type, .rhs = exp_self(cast)->type, .data=(m_uint)&imp };
    CHECK_NN(op_check(env, &opi));
  }
  return opck_similar_cast(env, data);
}

ANN /*static */ void scan0_implicit_similar(const Env env, const Type lhs,
                                            const Type rhs) {
  struct Op_Func   opfunc = {.ck = opck_similar_cast};
  struct Op_Import opi    = {
      .op = insert_symbol("$"), .lhs = lhs, .rhs = rhs, .func = &opfunc};
  add_op(env->gwion, &opi);
  opi.lhs = rhs;
  opi.rhs = lhs;
  add_op(env->gwion, &opi);
  opfunc.ck = opck_implicit_similar;
  opi.op    = insert_symbol("@implicit");
  add_op(env->gwion, &opi);
}

ANN static void scan0_explicit_distinct(const Env env, const Type lhs,
                                        const Type rhs) {
  struct Op_Func   opfunc = {.ck = opck_similar_cast};
  if(tflag(rhs, tflag_contract)) {
    opfunc.ck = opck_contract_similar;
    opfunc.em = opem_contract_similar;
  }
  struct Op_Import opi    = {
      .op = insert_symbol("$"), .lhs = lhs, .rhs = rhs, .func = &opfunc};
  add_op(env->gwion, &opi);
  opi.lhs = rhs;
  opi.rhs = lhs;
  add_op(env->gwion, &opi);
}

ANN static void typedef_simple(const Env env, const Type_Def tdef,
                               const Type base) {
  const Type t    = scan0_type(env, s_name(tdef->xid), base);
  t->size         = base->size;
  const Nspc nspc = (!env->class_def && GET_FLAG(tdef->ext, global))
                        ? env->global_nspc
                        : env->curr;
  if (GET_FLAG(tdef->ext, global)) context_global(env);
  add_type(env, nspc, t);
  tdef->type = t;
  if (base->nspc) // create a new nspc if `distinct`?
    nspc_addref((t->nspc = base->nspc));
  t->flag = tdef->ext->flag;
  if (tdef->ext->array && !tdef->ext->array->exp) set_tflag(t, tflag_empty);
  mk_class(env, tdef->type, tdef->pos);
}

ANN static m_bool typedef_complex(const Env env, const Type_Def tdef,
                                  const Type base) {
  const ae_flag   flag = base->info->cdef ? base->info->cdef->flag : 0;
  const Class_Def cdef = new_class_def(env->gwion->mp, flag, tdef->xid,
                                       cpy_type_decl(env->gwion->mp, tdef->ext),
                                       NULL, tdef->ext->pos);
  const bool final = GET_FLAG(base, final);
  if(final) UNSET_FLAG(base, final);
  CHECK_BB(scan0_class_def(env, cdef));
  tdef->type      = cdef->base.type;
  cdef->base.tmpl = tdef->tmpl; // check cpy
  set_tflag(tdef->type, tflag_cdef);
  if(final) SET_FLAG(base, final);
  mk_class(env, tdef->type, tdef->pos);
  return GW_OK;
}

ANN static void typedef_fptr(const Env env, const Type_Def tdef,
                             const Type base) {
  tdef->type               = type_copy(env->gwion->mp, base);
  tdef->type->info->func   = base->info->func;
  tdef->type->name         = s_name(tdef->xid);
  tdef->type->info->parent = base;
  add_type(env, env->curr, tdef->type);
  mk_class(env, tdef->type, tdef->pos);
  if (base->info->func->def->base->tmpl) set_tflag(tdef->type, tflag_ftmpl);
}

ANN m_bool scan0_type_def(const Env env, const Type_Def tdef) {
  CHECK_BB(env_access(env, tdef->ext->flag, tdef->ext->pos));
  DECL_OB(const Type, base, = tdef->tmpl ? find_type(env, tdef->ext)
                                         : known_type(env, tdef->ext));
  CHECK_BB(scan0_defined(env, tdef->xid, tdef->ext->pos));
  const bool global =
      GET_FLAG(tdef->ext, global); // TODO: handle global in class
  if (global) {
    context_global(env);
    env_push_global(env);
  }
  if (!is_func(env->gwion, base)) { // is_callable
    if (!tdef->ext->types && (!tdef->ext->array || !tdef->ext->array->exp)) {
      typedef_simple(env, tdef, base);
    } else
      CHECK_BB(typedef_complex(env, tdef, base));
  } else
    typedef_fptr(env, tdef, base);
  if (tdef->when) set_tflag(tdef->type, tflag_contract);
  if (tdef->type != base && !tdef->distinct && !tdef->when)
    scan0_implicit_similar(env, base, tdef->type);
  if (tdef->distinct || tdef->when) {
//    tdef->type->info->parent = base->info->parent;
    if (base->info->gack)
      vmcode_addref(tdef->type->info->gack = base->info->gack);
    set_tflag(tdef->type, tflag_distinct);
    struct Op_Import opi = {.lhs = base, .rhs = tdef->type};
    op_cpy(env, &opi);
    scan0_explicit_distinct(env, base, tdef->type);
    type_addref(tdef->type); // maybe because of scope_iter in nspc_free_values
  } else //if(tdef->ext->array)
    set_tflag(tdef->type, tflag_typedef);
  if(tflag(base, tflag_ref)) {
    set_tflag(tdef->type, tflag_ref);
    set_tflag(tdef->type, tflag_infer);
  }
  if (global) env_pop(env, 0);
  if (tdef->type != base)
    tdef->type->info->base_type = base;
  return GW_OK;
}

#define scan0_nspc(env, a)                                                     \
  GET_FLAG(a, global) ? !env->class_def ? env->global_nspc : NULL : env->curr

#include "gack.h"
static GACK(gack_enum) {
  const Value v = (Value)map_at(&t->nspc->info->value->map, *(m_uint*)VALUE);
  INTERP_PRINTF("%s", v->name);
}

ANN static Type enum_type(const Env env, const Enum_Def edef) {
  const Type   t    = type_copy(env->gwion->mp, env->gwion->type[et_int]);
  t->name           = s_name(edef->xid);
  t->info->parent   = env->gwion->type[et_int];
  const bool global = GET_FLAG(edef, global); // TODO: handle global in class
  if (global) {
    context_global(env);
    env_push_global(env);
  }
  add_type(env, env->curr, t);
  mk_class(env, t, edef->pos);
  set_tflag(t, tflag_enum);
  CHECK_BO(mk_gack(env->gwion->mp, t, gack_enum));
  if (global) env_pop(env, 0);
//  scan0_implicit_similar(env, t, env->gwion->type[et_int]);
//  scan0_implicit_similar(env, env->gwion->type[et_int], t);
  return t;
}

ANN static inline m_bool scan0_global(const Env env, const ae_flag flag,
                                      const loc_t pos) {
  if (!env->class_def || !((flag & ae_flag_global) == ae_flag_global))
    return GW_OK;
  ERR_B(pos, _("can't declare as global in class def"))
}

ANN m_bool scan0_enum_def(const Env env, const Enum_Def edef) {
  CHECK_BB(env_storage(env, edef->flag, edef->pos));
  CHECK_BB(scan0_defined(env, edef->xid, edef->pos));
  CHECK_BB(scan0_global(env, edef->flag, edef->pos));
  edef->t = enum_type(env, edef);
  vector_init(&edef->values);
  if (GET_FLAG(edef, global)) context_global(env);
  return GW_OK;
}

ANN static Type union_type(const Env env, const Symbol s, const loc_t loc) {
  const m_str name = s_name(s);
  const Type  t    = new_type(env->gwion->mp, name, env->gwion->type[et_union]);
  t->nspc          = new_nspc(env->gwion->mp, name);
  t->nspc->parent  = env->curr;
  t->info->tuple   = new_tupleform(env->gwion->mp, NULL); // ???
  set_tflag(t, tflag_union);
  add_type(env, env->curr, t);
  mk_class(env, t, loc);
  SET_FLAG(t, final);
  if (strncmp(t->name, "Option", 6)) SET_FLAG(t, abstract);
  return t;
}

ANN static void union_tmpl(const Env env, const Union_Def udef) {
  if (tmpl_base(udef->tmpl)) {
    const Union_Def u      = cpy_union_def(env->gwion->mp, udef);
    u->type                = udef->type;
    udef->type->info->udef = u;
    set_tflag(u->type, tflag_tmpl);
    set_tflag(u->type, tflag_udef);
  }
  //  if(GET_FLAG(udef, global))
  //    SET_FLAG(udef->type, global);
}

ANN m_bool scan0_union_def(const Env env, const Union_Def udef) {
  CHECK_BB(env_storage(env, udef->flag, udef->pos));
  CHECK_BB(scan0_global(env, udef->flag, udef->pos));
  const bool global = GET_FLAG(udef, global); // TODO: handle global in class
  if (global) {
    context_global(env);
    env_push_global(env);
  }
  if (GET_FLAG(udef, global)) context_global(env);
  CHECK_BB(scan0_defined(env, udef->xid, udef->pos));
  udef->type   = union_type(env, udef->xid, udef->pos);
  SET_ACCESS(udef, udef->type);
  if (udef->tmpl) union_tmpl(env, udef);
  if (global) env_pop(env, 0);
  set_tflag(udef->type, tflag_scan0);
  return GW_OK;
}

ANN static m_bool scan0_class_def_pre(const Env env, const Class_Def cdef) {
  CHECK_BB(env_storage(env, cdef->flag, cdef->pos));
  CHECK_BB(isres(env, cdef->base.xid, cdef->pos));
  return GW_OK;
}

ANN static inline void cdef_flag(const Class_Def cdef, const Type t) {
  if (cdef->base.tmpl) set_tflag(t, tflag_tmpl);
  if (cdef->base.ext && cdef->base.ext->array) set_tflag(t, tflag_typedef);
}

ANN static Type get_parent_base(const Env env, Type_Decl *td) {
  DECL_OO(const Type, t, = find_type(env, td));
  Type owner = env->class_def;
  while (owner) {
    if (t == owner)
      ERR_O(td->pos, _("'%s' as parent inside itself\n."), owner->name);
    owner = owner->info->value->from->owner_class;
  }
  return t;
}

ANN static inline Type scan0_final(const Env env, Type_Decl *td) {
  DECL_OO(const Type, t, = known_type(env, td));
  if (!GET_FLAG(t, final)) return t;
  ERR_O(td->pos, _("can't inherit from final parent class '%s'\n."), t->name);
}

ANN static Type cdef_parent(const Env env, const Class_Def cdef) {
  if (cflag(cdef, cflag_struct)) return env->gwion->type[et_compound];
  if (!cdef->base.ext) return env->gwion->type[et_object];
  if (tmpl_base(cdef->base.tmpl)) return get_parent_base(env, cdef->base.ext);
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) template_push_types(env, cdef->base.tmpl);
  const Type t = scan0_final(env, cdef->base.ext);
  if (tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  return t ?: (Type)GW_ERROR;
}

ANN static m_bool find_traits(const Env env, ID_List traits, const loc_t pos) {
  for(uint32_t i = 0; i < traits->len; i++) {
    Symbol xid = *mp_vector_at(traits, Symbol, i);
    if (!nspc_lookup_trait1(env->curr, xid)) {
      gwerr_basic(_("can't find trait"), NULL, NULL, env->name, pos, 0);
      did_you_mean_trait(env->curr, s_name(xid));
      env_set_error(env, true);
      return GW_ERROR;
    }
  }
  return GW_OK;
}

ANN static Type scan0_class_def_init(const Env env, const Class_Def cdef) {
  CHECK_BO(scan0_defined(env, cdef->base.xid, cdef->pos));
  const Type parent = cdef_parent(env, cdef);
  if (parent == (Type)GW_ERROR) return NULL;
  if(GET_FLAG(cdef, global) && isa(parent, env->gwion->type[et_closure]) < 0 && !type_global(env, parent)) {
    gwerr_basic(_("parent type is not global"), NULL, NULL, env->name, cdef->base.ext ? cdef->base.ext->pos : cdef->base.pos, 0);
    const Value v = parent->info->value;
    gwerr_warn("declared here", NULL, NULL, v->from->filename, v->from->loc);
    env_set_error(env,  true);
    return NULL;
  }
  if (cdef->traits) CHECK_BO(find_traits(env, cdef->traits, cdef->pos));
  const Type t = scan0_type(env, s_name(cdef->base.xid), parent);
  if (cflag(cdef, cflag_struct)) {
    t->size = 0;
    set_tflag(t, tflag_struct);
  }
  t->info->tuple  = new_tupleform(env->gwion->mp, parent);
  t->nspc         = new_nspc(env->gwion->mp, t->name);
  t->nspc->parent = env->curr;
  t->info->cdef   = cdef;
  t->flag |= cdef->flag;
  cdef_flag(cdef, t);
  return t;
}


ANN static m_bool _spread_tmpl(const Env env, const Type t, const Spread_Def spread) {
  if(t->info->value->from->owner_class)
    CHECK_BB(_spread_tmpl(env, t->info->value->from->owner_class, spread));
  const Tmpl *tmpl = t->info->cdef->base.tmpl;
  if(!tmpl || !tmpl->call) return GW_OK;
  if(is_spread_tmpl(tmpl))
    CHECK_BB(spread_ast(env, spread, tmpl));
  return GW_OK;
}

ANN static m_bool spread_tmpl(const Env env, const Spread_Def spread) {
  if(env->class_def) CHECK_BB(_spread_tmpl(env, env->class_def, spread));
  if(!env->func) return GW_OK;
  const Tmpl *tmpl = env->func->def->base->tmpl;
  if(!tmpl || !tmpl->call) return GW_OK;
  if(is_spread_tmpl(tmpl))
      CHECK_BB(spread_ast(env, spread, tmpl));
  return GW_OK;
}

ANN static bool spreadable(const Env env) {
  const Func f = env->func;
  if(f && f->def->base->tmpl && is_spread_tmpl(f->def->base->tmpl))
    return true;
  Type t = env->class_def;
  while(t) {
    const Tmpl *tmpl = t->info->cdef->base.tmpl;
    if(tmpl && is_spread_tmpl(tmpl))
      return true;
    t = t->info->value->from->owner_class;
  }
  return false;
}

ANN static m_bool scan0_stmt_list(const Env env, Stmt_List l) {
  for(m_uint i = 0; i < l->len; i++) {
    const Stmt stmt = mp_vector_at(l, struct Stmt_, i);
    if (stmt->stmt_type == ae_stmt_pp) {
      if (stmt->d.stmt_pp.pp_type == ae_pp_include)
        env->name = stmt->d.stmt_pp.data;
      else if (stmt->d.stmt_pp.pp_type == ae_pp_import)
        CHECK_BB(plugin_ini(env->gwion, stmt->d.stmt_pp.data, stmt->pos));
    } else if (stmt->stmt_type == ae_stmt_spread) {
      if(!spreadable(env))
        ERR_B(stmt->pos, "spread statement outside of variadic environment");
      if(!env->context->extend)
         env->context->extend = new_mp_vector(env->gwion->mp, Section, 0);
      CHECK_BB(spread_tmpl(env, &stmt->d.stmt_spread));
    }
  }
  return GW_OK;
}

ANN m_bool scan0_func_def(const Env env, const Func_Def fdef) {
  const Ast old_extend = env->context ? env->context->extend : NULL;
  if(!fdef->base->tmpl || !fdef->base->tmpl->call) return GW_OK;
  if(env->context) {
    if(fdef->base->tmpl && fdef->base->tmpl->call && is_spread_tmpl(fdef->base->tmpl)) {
    struct Func_ fake = {.name = s_name(fdef->base->xid), .def = fdef }, *const former =
                                                            env->func;
    env->func = &fake;
    if(!fdef->builtin && fdef->d.code && fdef->d.code->d.stmt_code.stmt_list)
      scan0_stmt_list(env, fdef->d.code->d.stmt_code.stmt_list);
    if(env->context->extend)
      fdef->d.code->d.stmt_code.stmt_list = spread_func(env, fdef->d.code->d.stmt_code.stmt_list);
    env->func = former;
    env->context->extend = old_extend;
  }}
  return GW_OK;
}

ANN static m_bool scan0_extend_def(const Env env, const Extend_Def xdef) {
  DECL_OB(const Type, t, = known_type(env, xdef->td));
  if(type_global(env, t)) {
    for(uint32_t i = 0; i < xdef->traits->len; i++) {
      const Symbol xid = *mp_vector_at(xdef->traits, Symbol, i);
      const Trait global = nspc_lookup_trait1(env->global_nspc, xid);
      if(!global) {
        const Trait trait = nspc_lookup_trait1(env->curr, xid);
        gwerr_basic("trait should be declared global", NULL, NULL, trait->filename, trait->loc, 0);
        gwerr_secondary("from the request ", env->name, xdef->td->pos);
        env_set_error(env, true);
      }
    }
  }
  xdef->t = t;
  return GW_OK;
}

ANN static m_bool _scan0_trait_def(const Env env, const Trait_Def pdef) {
  const Trait trait = new_trait(env->gwion->mp, pdef->pos);
  trait->loc        = pdef->pos;
  trait->name       = s_name(pdef->xid);
  trait->filename   = env->name;
  if(GET_FLAG(pdef, global))
    nspc_add_trait(env->global_nspc, pdef->xid, trait);
  else nspc_add_trait(env->curr, pdef->xid, trait);
  Ast ast = pdef->body;
  if(!ast) return GW_OK; // ???
  for(m_uint i = 0; i < ast->len; i++) {
    Section *section = mp_vector_at(ast, Section, i);
    if (section->section_type == ae_section_func) {
      const Func_Def fdef = section->d.func_def;
      if (fdef->base->flag != ae_flag_none &&
          fdef->base->flag != (ae_flag_none | ae_flag_abstract))
        ERR_B(fdef->base->pos, "Trait function must be declared without qualifiers");
      if (!trait->fun) trait->fun = new_mp_vector(env->gwion->mp, Func_Def, 0);
      mp_vector_add(env->gwion->mp, &trait->fun, Func_Def, fdef);
    }
  }
  return GW_OK;
}

ANN static m_bool scan0_trait_def(const Env env, const Trait_Def pdef) {
  const Symbol s      = pdef->xid;
  const Trait  exists = nspc_lookup_trait1(env->curr, s);
  if (exists) {
    gwerr_basic("trait already defined", NULL, NULL, env->name, pdef->pos, 0);
    gwerr_secondary("defined here", env->name, exists->loc);
    env_set_error(env, true);
    return already_defined(env, s, pdef->pos);
  }
  if (pdef->traits) CHECK_BB(find_traits(env, pdef->traits, pdef->pos));
  _scan0_trait_def(env, pdef);
  return GW_OK;
}

HANDLE_SECTION_FUNC(scan0, m_bool, Env)

ANN static m_bool scan0_class_def_inner(const Env env, const Class_Def cdef) {
  CHECK_OB((cdef->base.type = scan0_class_def_init(env, cdef)));
  cdef->base.type->info->traits = cdef->traits;
  set_tflag(cdef->base.type, tflag_scan0);
  (void)mk_class(env, cdef->base.type, cdef->pos);
  add_type(env, cdef->base.type->info->value->from->owner, cdef->base.type);
  const m_bool ret = cdef->body ? env_body(env, cdef, scan0_section) : GW_OK;
  return ret;
}



ANN Ast spread_class(const Env env, const Ast body);
ANN m_bool scan0_class_def(const Env env, const Class_Def c) {
  CHECK_BB(scan0_global(env, c->flag, c->pos));
  const Ast old_extend = env->context ? env->context->extend : NULL;
  const int       cpy  = tmpl_base(c->base.tmpl) || GET_FLAG(c, global);
  const Class_Def cdef = !cpy ? c : cpy_class_def(env->gwion->mp, c);
  if (GET_FLAG(cdef, global)) { // could be updated
    vector_add(&env->scope->nspc_stack, (vtype)env->curr);
    env->curr = env->global_nspc;
    context_global(env);
  }
  const m_bool ret = scan0_class_def_pre(env, cdef) > 0
                         ? scan0_class_def_inner(env, cdef)
                         : GW_ERROR;

  if (GET_FLAG(cdef, global))
    env->curr = (Nspc)vector_pop(&env->scope->nspc_stack);
  if (cpy && cdef->base.type) {
    c->base.type = cdef->base.type;
    c->base.type->info->cdef = cdef;
    set_tflag(c->base.type, tflag_cdef);
  }
  if(env->context) {
    if(!tmpl_base(c->base.tmpl) && env->context->extend) cdef->body = spread_class(env, cdef->body);
    env->context->extend = old_extend;
  }
  return ret;
}

ANN m_bool scan0_ast(const Env env, Ast *ast) {
  Ast a = *ast;
  for(m_uint i = 0; i < a->len; i++) {
    Section * section = mp_vector_at(a, Section, i);
    CHECK_BB(scan0_section(env, section));
  }
  return GW_OK;
}
