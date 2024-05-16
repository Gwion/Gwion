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
#include "emit.h"

static inline void add_type(const Env env, const Nspc nspc, const Type t) {
  nspc_add_type_front(nspc, insert_symbol(t->name), t);
}

ANN static inline bool scan0_defined(const Env env, const Tag tag) {
  if (nspc_lookup_type1(env->curr, tag.sym))
    ERR_B(tag.loc, _("type '%s' already defined"), s_name(tag.sym));
  return can_define(env, tag.sym, tag.loc);
}

ANN static Arg_List fptr_arg_list(const Env env, const Fptr_Def fptr) {
  if(env->class_def && !GET_FLAG(fptr->base, static)) {
    Arg arg = { .var = {.td = type2td(env->gwion, env->class_def, fptr->base->td->tag.loc)}};
    const uint32_t len = mp_vector_len(fptr->base->args);
    Arg_List args = new_mp_vector(env->gwion->mp, Arg, len + 1);
    mp_vector_set(args, Arg, 0, arg);
    for(uint32_t i = 0; i < len; i++) {
      Arg *base  = mp_vector_at(fptr->base->args, Arg, i);
      Arg arg = { .var = {.td = cpy_type_decl(env->gwion->mp, base->var.td)}};
      mp_vector_set(args, Arg, i+1, arg);
    }
    return args;
  } else if(fptr->base->args)
    return cpy_arg_list(env->gwion->mp, fptr->base->args);
  return NULL;
}

ANN static inline bool scan0_global(const Env env, const ae_flag flag,
                                      const loc_t loc, bool *global) {
  CHECK_B(env_storage(env, flag, loc));
  if (!((flag & ae_flag_global) == ae_flag_global)) return true;
  if (!env->class_def) {
    env_push_global(env);
    if (env->context) env->context->global = true;
    *global = true;
    return true;
  }
  ERR_B(loc, _("can't declare as global in class def"));
}

ANN bool scan0_fptr_def(const Env env, const Fptr_Def fptr) {
  const loc_t loc = fptr->base->td->tag.loc;
  CHECK_B(env_access(env, fptr->base->flag, loc));
  CHECK_B(scan0_defined(env, fptr->base->tag));
  const bool global = GET_FLAG(fptr->base, global);
  UNSET_FLAG(fptr->base, global);
  const Arg_List args = fptr_arg_list(env, fptr);
  Func_Base *const fbase = new_func_base(env->gwion->mp, cpy_type_decl(env->gwion->mp, fptr->base->td),
    insert_symbol("func"), args, ae_flag_static | ae_flag_private, loc);
  const Func_Def fdef = new_func_def(env->gwion->mp, fbase, NULL);
  Ast body = new_mp_vector(env->gwion->mp, Section, 1);
  mp_vector_set(body, Section, 0, MK_SECTION(func, func_def, fdef, loc));
  Type_Decl* td = new_type_decl(env->gwion->mp, insert_symbol(env->gwion->type[et_closure]->name), loc);
  const Class_Def cdef = new_class_def(env->gwion->mp, ae_flag_final, fptr->base->tag, td, body);
  if(global) SET_FLAG(cdef, global);
  if(fptr->base->tmpl) {
    fbase->tmpl = cpy_tmpl(env->gwion->mp, fptr->base->tmpl);
    if(!fptr->base->tmpl->call)
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

ANN static void scan0_implicit_similar(const Env env, const Type lhs,
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
  const Type t    = new_type(env->gwion->mp, s_name(tdef->tag.sym), base);
  t->size         = base->size;
  const Nspc nspc = (!env->class_def && GET_FLAG(tdef->ext, global))
                        ? env->global_nspc
                        : env->curr;
  add_type(env, nspc, t);
  tdef->type = t;
  if (base->nspc) {
    t->nspc = new_nspc(env->gwion->mp, t->name);
    t->nspc->parent = base->nspc;
  }
  t->flag = tdef->ext->flag;
  if (tdef->ext->array && !tdef->ext->array->exp) {
    set_tflag(t, tflag_empty);
    SET_FLAG(t, abstract);
  } else if(tflag(base, tflag_union))
    set_tflag(t, tflag_union);
}

ANN static bool typedef_complex(const Env env, const Type_Def tdef,
                                  const Type base) {
  const ae_flag   flag = base->info->cdef ? base->info->cdef->flag : 0;
  const Class_Def cdef = new_class_def(env->gwion->mp, flag, tdef->tag,
                                       cpy_type_decl(env->gwion->mp, tdef->ext),
                                       NULL);
  const bool final = GET_FLAG(base, final);
  if(final) UNSET_FLAG(base, final);
  CHECK_B(scan0_class_def(env, cdef));
  tdef->type      = cdef->base.type;
  cdef->base.tmpl = tdef->tmpl; // check cpy
  set_tflag(tdef->type, tflag_cdef);
  if(final) SET_FLAG(base, final);
  return true;
}

ANN bool scan0_type_def(const Env env, const Type_Def tdef) {
  CHECK_B(scan0_defined(env, tdef->tag));
  CHECK_B(env_access(env, tdef->ext->flag, tdef->ext->tag.loc));
  DECL_B(const Type, base, = known_type(env, tdef->ext));
  bool global = false;
  CHECK_B(scan0_global(env, tdef->ext->flag, tdef->ext->tag.loc, &global));
  if (!tdef->ext->types && (!tdef->ext->array || !tdef->ext->array->exp))
    typedef_simple(env, tdef, base);
  else CHECK_B(typedef_complex(env, tdef, base));
  mk_class(env, tdef->type, tdef->tag.loc);
  if (tdef->when) set_tflag(tdef->type, tflag_contract);
  if (tdef->type != base && !tdef->distinct && !tdef->when)
    scan0_implicit_similar(env, base, tdef->type);
  if (tdef->distinct || tdef->when) {
    if (base->info->gack)
      vmcode_addref(tdef->type->info->gack = base->info->gack);
    set_tflag(tdef->type, tflag_distinct);
    struct Op_Import opi = {.lhs = base, .rhs = tdef->type};
    op_cpy(env, &opi);
    scan0_explicit_distinct(env, base, tdef->type);
  } else //if(tdef->ext->array)
    set_tflag(tdef->type, tflag_typedef);
  if(tflag(base, tflag_ref)) {
    set_tflag(tdef->type, tflag_ref);
    set_tflag(tdef->type, tflag_infer);
  }
  if (global) env_pop(env, 0);
  return true;
}

ANN static Type enum_type(const Env env, const Enum_Def edef) {
  const Type   t    = type_copy(env->gwion->mp, env->gwion->type[et_enum]);
  t->name           = s_name(edef->tag.sym);
  t->info->parent   = env->gwion->type[et_enum];
  add_type(env, env->curr, t);
  mk_class(env, t, edef->tag.loc);
  return t;
}

ANN bool scan0_enum_def(const Env env, const Enum_Def edef) {
  CHECK_B(scan0_defined(env, edef->tag));
  bool global = false;
  CHECK_B(scan0_global(env, edef->flag, edef->tag.loc, &global));
  edef->type = enum_type(env, edef);
  if (global) env_pop(env, 0);
  return true;
}

static INSTR(StructAssign) {
  memcpy(*(m_bit**)REG(-SZ_INT), REG((m_int)instr->m_val), instr->m_val2);
}

static OP_EMIT(opem_struct_assign) {
  const Exp_Binary *bin = data;
  const Type t = bin->lhs->type;
  Exp* e = exp_self(bin);

  if(tflag(t, tflag_release)) {
    const f_instr exec = !tflag(t, tflag_union)
       ? StructReleaseRegAddr
       : UnionReleaseRegAddr;
    const Instr release = emit_add_instr(emit, exec);
    release->m_val = -SZ_INT;
    release->m_val2 = (m_uint)t;
  }

  const Instr instr = emit_add_instr(emit, StructAssign);
  instr->m_val  = -t->size - SZ_INT;
  instr->m_val2 = t->size;
  emit_struct_addref(emit, t, -SZ_INT, true);
  if(exp_getvar(e)) {
    emit_regmove(emit, -t->size);
    const Instr instr = emit_add_instr(emit, Reg2Reg);
    instr->m_val = -SZ_INT;
    instr->m_val2 = t->size - SZ_INT;
  } else emit_regmove(emit, -SZ_INT);
  return true;
}

ANN static OP_CHECK(opck_struct_assign) {
  CHECK_NN(opck_rassign(env, data));
  Exp_Binary *bin = data;
  bin->rhs->ref = bin->lhs;
  return bin->lhs->type;
}

ANN static void scan0_struct_assign(const Env env, const Type t) {
  struct Op_Func   opfunc = {.ck = opck_struct_assign, .em = opem_struct_assign };
  struct Op_Import opi    = {
      .op = insert_symbol(":=>"), .lhs = t, .rhs = t, .ret = t, .func = &opfunc};
  add_op(env->gwion, &opi);
}

ANN static Type union_type(const Env env, const Symbol s, const loc_t loc) {
  const m_str name = s_name(s);
  const Type  t    = new_type(env->gwion->mp, name, env->gwion->type[et_union]);
  t->nspc          = new_nspc(env->gwion->mp, name);
  t->nspc->parent  = env->curr;
  t->info->tuple   = new_tupleform(env->gwion->mp, NULL); // ???
  set_tflag(t, tflag_union);
  set_tflag(t, tflag_struct);
  add_type(env, env->curr, t);
  mk_class(env, t, loc);
  SET_FLAG(t, final);
  set_tflag(t, tflag_compound);
  SET_FLAG(t, abstract);
  scan0_struct_assign(env,  t);
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
}

ANN bool scan0_union_def(const Env env, const Union_Def udef) {
  bool global = false;
  CHECK_B(scan0_global(env, udef->flag, udef->tag.loc, &global));
  CHECK_B(scan0_defined(env, udef->tag));
  udef->type   = union_type(env, udef->tag.sym, udef->tag.loc);
  SET_ACCESS(udef, udef->type);
  if (udef->tmpl) union_tmpl(env, udef);
  if (global) env_pop(env, 0);
  set_tflag(udef->type, tflag_scan0);
  return true;
}

ANN static inline void cdef_flag(const Class_Def cdef, const Type t) {
  if (cdef->base.tmpl) set_tflag(t, tflag_tmpl);
  if (cdef->base.ext && cdef->base.ext->array) set_tflag(t, tflag_typedef);
}

ANN static Type get_parent_base(const Env env, Type_Decl *td) {
  DECL_O(const Type, t, = known_type(env, td));
  Type owner = env->class_def;
  while (owner) {
    if (t == owner)
      ERR_O(td->tag.loc, _("'%s' as parent inside itself\n."), owner->name);
    owner = owner->info->value->from->owner_class;
  }
  return t;
}

ANN static inline Type scan0_final(const Env env, Type_Decl *td) {
  const Type t = known_type(env, td);
  if(!t) ERR_O(td->tag.loc, _("can't find parent class %s\n."), s_name(td->tag.sym));
  return t;
}

ANN static Type cdef_parent(const Env env, const Class_Def cdef) {
  if (cflag(cdef, cflag_struct)) return env->gwion->type[et_compound];
  if (!cdef->base.ext) return env->gwion->type[et_object];
  bool ok = true;
  Exp* e = cdef->base.ext->array ? cdef->base.ext->array->exp : NULL;
  while(e) {
    if(!is_prim_int(e))
      ERR_OK_NODE(ok, e, e->loc, "non null array type extension must be literal");
    e = e->next;
  }
  if (tmpl_base(cdef->base.tmpl)) return get_parent_base(env, cdef->base.ext);
  const bool tmpl = !!cdef->base.tmpl;
  if (tmpl) CHECK_O(template_push_types(env, cdef->base.tmpl));
  const Type t = scan0_final(env, cdef->base.ext);
  if (tmpl) nspc_pop_type(env->gwion->mp, env->curr);
  return ok ? t : NULL;
}

ANN static bool find_traits(const Env env, ID_List traits, const loc_t loc) {
  bool ok = true;
  for(uint32_t i = 0; i < traits->len; i++) {
    Symbol xid = *mp_vector_at(traits, Symbol, i);
    if (!nspc_lookup_trait1(env->curr, xid)) {
      gwlog_error(_("can't find trait"), NULL, env->name, loc, 0);
      did_you_mean_trait(env->curr, s_name(xid));
      env_set_error(env, true);
      POISON(ok, env);
    }
  }
  return ok;
}

ANN static Type scan0_class_def_init(const Env env, const Class_Def cdef) {
  CHECK_O(scan0_defined(env, cdef->base.tag));
  DECL_O(const Type, parent, = cdef_parent(env, cdef));
  if(GET_FLAG(cdef, global) && !isa(parent, env->gwion->type[et_closure]) && !type_global(env, parent)) {
    gwlog_error(_("parent type is not global"), NULL,  env->name, cdef->base.ext ? cdef->base.ext->tag.loc : cdef->base.tag.loc, 0);
    declared_here(parent->info->value);
    env_set_error(env,  true);
    return NULL;
  }
  if (cdef->traits) CHECK_O(find_traits(env, cdef->traits, cdef->base.tag.loc));
  const Type t = new_type(env->gwion->mp, s_name(cdef->base.tag.sym), parent);
  if (cflag(cdef, cflag_struct)) {
    t->size = 0;
    set_tflag(t, tflag_struct);
  } else set_tflag(t, tflag_release);
  set_tflag(t, tflag_compound);
  t->info->tuple  = new_tupleform(env->gwion->mp, parent);
  t->nspc         = new_nspc(env->gwion->mp, t->name);
  t->nspc->parent = env->curr;
  t->info->cdef   = cdef;
  t->flag |= cdef->flag;
  cdef_flag(cdef, t);
  return t;
}

ANN static bool scan0_stmt_list(const Env env, Stmt_List l) {
  bool ok = true;
  const uint32_t nusing = env->curr->info->using
    ? env->curr->info->using->len
    : 0;
  for(m_uint i = 0; i < l->len; i++) {
    Stmt* stmt = mp_vector_at(l, Stmt, i);
    if(stmt->poison) { ok = false; continue; }
    if (stmt->stmt_type == ae_stmt_pp) {
      if (stmt->d.stmt_pp.pp_type == ae_pp_include)
        env->name = stmt->d.stmt_pp.data;
      else if (stmt->d.stmt_pp.pp_type == ae_pp_import) {
        if(!plugin_ini(env->gwion, stmt->d.stmt_pp.data, stmt->loc))
          POISON_NODE(ok, env, stmt);
      }
    } else if(stmt->stmt_type == ae_stmt_using) {
      if(!env->curr->info->using)
        env->curr->info->using = new_mp_vector(env->gwion->mp, Stmt_Using, 0);
      mp_vector_add(env->gwion->mp, &env->curr->info->using, Stmt_Using, &stmt->d.stmt_using);
    } /*else if (stmt->stmt_type == ae_stmt_spread) {
      if(!spreadable(env)) // TODO: we can prolly get rid of this
        ERR_OK_NODE(ok, stmt, stmt->loc, "spread statement outside of variadic environment");
    }*/
  }
  if(env->curr->info->using && env->scope->depth)
    env->curr->info->using->len = nusing;
  return ok;
}

ANN bool scan0_func_def(const Env env, const Func_Def fdef) {
  if(!fdef->base->tmpl || !fdef->base->tmpl->call) return true;
  bool ok = true;
  if(fdef->base->tmpl && fdef->base->tmpl->call && is_spread_tmpl(fdef->base->tmpl)) {
    struct Func_ fake = {.name = s_name(fdef->base->tag.sym), .def = fdef };
    const Func former = env->func;
    env->func = &fake;
    if(!fdef->builtin && fdef->d.code)
      scan0_stmt_list(env, fdef->d.code);
    env->func = former;
  }
  return ok;
}

ANN static bool scan0_extend_def(const Env env, const Extend_Def xdef) {
  DECL_B(const Type, t, = known_type(env, xdef->td));
  bool ok = true;
  if(type_global(env, t)) {
    for(uint32_t i = 0; i < xdef->traits->len; i++) {
      const Symbol xid = *mp_vector_at(xdef->traits, Symbol, i);
      const Trait global = nspc_lookup_trait1(env->global_nspc, xid);
      if(!global) {
        const Trait trait = nspc_lookup_trait1(env->curr, xid);
        gwlog_error("trait should be declared global", NULL, trait->filename, trait->loc, 0);
        gwlog_related("from the request ", env->name, xdef->td->tag.loc);
        env_set_error(env, true);
        POISON(ok, env);
      }
    }
  }
  xdef->type = t;
  return ok;
}

ANN static bool _scan0_trait_def(const Env env, const Trait_Def pdef) {
  CHECK_B(scan0_defined(env, pdef->tag));
  bool global = false;
  CHECK_B(scan0_global(env, pdef->flag, pdef->tag.loc, &global));
  const Trait trait = new_trait(env->gwion->mp, pdef->tag.loc);
  trait->loc    = pdef->tag.loc;
  trait->name       = s_name(pdef->tag.sym);
  trait->filename   = env->name;
  nspc_add_trait(env->curr, pdef->tag.sym, trait);
  if(global) env_pop(env, 0);
  Ast ast = pdef->body;
  if(!ast) return true; // ???
  bool ok = true;
  for(m_uint i = 0; i < ast->len; i++) {
    Section *section = mp_vector_at(ast, Section, i);
    if(section->poison) { ok = false; continue; }
    if (section->section_type == ae_section_func) {
      const Func_Def fdef = section->d.func_def;
      if (fdef->base->flag != ae_flag_none &&
          fdef->base->flag != (ae_flag_none | ae_flag_abstract))
        ERR_OK(ok, fdef->base->tag.loc, "Trait function must be declared without qualifiers");
      if (!trait->fun) trait->fun = new_mp_vector(env->gwion->mp, Func_Def, 0);
      mp_vector_add(env->gwion->mp, &trait->fun, Func_Def, fdef);
    } else if (section->section_type == ae_section_stmt) {
      Stmt_List list = section->d.stmt_list;
      for(uint32_t i = 0; i < list->len; i++) {
        Stmt* stmt = mp_vector_at(list, Stmt, i);
        if(stmt->poison) { ok = false; continue;}
        if(stmt->d.stmt_exp.val->exp_type != ae_exp_decl)
          ERR_OK(ok, stmt->loc, "trait can only contains variable requests and functions");
      }
    } else
        ERR_OK(ok, pdef->tag.loc, "invalid section for trait definition");
  }
  return ok;
}

ANN static bool scan0_trait_def(const Env env, const Trait_Def pdef) {
  const Symbol s      = pdef->tag.sym;
  const Trait  exists = nspc_lookup_trait1(env->curr, s);
  if (exists) {
    gwlog_error("trait already defined", NULL, env->name, pdef->tag.loc, 0);
    gwlog_related("defined here", env->name, exists->loc);
    env_set_error(env, true);
    return can_define(env, s, pdef->tag.loc);
  }
  if (pdef->traits) CHECK_B(find_traits(env, pdef->traits, pdef->tag.loc));
  _scan0_trait_def(env, pdef);
  return true;
}

ANN bool scan0_prim_def(const Env env, const Prim_Def pdef) {
  const loc_t loc = pdef->tag.loc;
  CHECK_B(env_access(env, pdef->flag, loc));
  CHECK_B(scan0_defined(env, pdef->tag));
  bool global = false;
  CHECK_B(scan0_global(env, pdef->flag, loc, &global));
  const Type t = mk_primitive(env, s_name(pdef->tag.sym), pdef->size);
  add_type(env, env->curr, t);
  mk_class(env, t, pdef->tag.loc);
  t->flag = pdef->flag;
  if(global) env_pop(env, 0);
  return true;
}

HANDLE_SECTION_FUNC(scan0, bool, Env)

ANN static bool scan0_class_def_inner(const Env env, const Class_Def cdef) {
  CHECK_B(not_reserved(env, cdef->base.tag));
  CHECK_B((cdef->base.type = scan0_class_def_init(env, cdef)));
  cdef->base.type->info->traits = cdef->traits; // should we copy the traits?
  set_tflag(cdef->base.type, tflag_scan0);
  (void)mk_class(env, cdef->base.type, cdef->base.tag.loc);
  add_type(env, cdef->base.type->info->value->from->owner, cdef->base.type);
  return cdef->body ? env_body(env, cdef, scan0_section) : true;
}

ANN bool scan0_class_def(const Env env, const Class_Def c) {
  bool global = false;
  CHECK_B(scan0_global(env, c->flag, c->base.tag.loc, &global));
  const int       cpy  = tmpl_base(c->base.tmpl) || GET_FLAG(c, global);
  const Class_Def cdef = !cpy ? c : cpy_class_def(env->gwion->mp, c);
  const bool ret = scan0_class_def_inner(env, cdef);
  if (cpy && cdef->base.type) c->base.type = cdef->base.type;
  if(cflag(cdef, cflag_struct)) scan0_struct_assign(env, cdef->base.type);
  if (GET_FLAG(cdef, global)) env_pop(env, 0);
  return ret;
}

ANN bool scan0_ast(const Env env, Ast *ast) {
  Ast a = *ast;
  bool ok = true;
  for(m_uint i = 0; i < a->len; i++) {
    Section * section = mp_vector_at(a, Section, i);
    if(section->poison) { ok = false; continue;}
    if(!scan0_section(env, section))
      POISON_SECTION(ok, env, section);
  }
  return ok;
}
