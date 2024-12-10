#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "instr.h"
#include "object.h"
#include "array.h"
#include "emit.h"
#include "operator.h"
#include "import.h"
#include "traverse.h"
#include "parse.h"
#include "gwi.h"
#include "emit.h"

ANN static inline m_uint Vector_size(const Type t) {
  return *(m_uint*)t->nspc->class_data;
}

static DTOR(vector_dtor) {
  const m_uint size = Vector_size(o->type_ref);
  const Type base = o->type_ref->info->base_type;
  const m_uint base_size = base->size;
  for (m_uint i = 0; i < size; ++i)
    compound_release(shred, base, o->data + i * base_size);
}

// TODO: use one of the memcpy functions?
static MFUN(vector_new) {
  const m_uint size = Vector_size(o->type_ref);
  const Type base = o->type_ref->info->base_type;
  const m_uint base_size = base->size;
  memcpy(o->data, shred->mem + SZ_INT, size * base_size);
  *(M_Object*)RETURN = o;
}

ANN Type check_array_access(const Env env, const Array_Sub array);

static OP_CHECK(opck_vector_access) {
  const Array_Sub array = (Array_Sub)data;
  const Type index_t = (Type)map_at(&array->type->nspc->info->type->map, 0);
  CHECK_B(check_implicit(env, array->exp, index_t));
  if(is_prim_int(array->exp)) {
    const m_uint num = array->exp->d.prim.d.gwint.num;
    // haha. can't have signed numbers with the grammar
    // that said.
    // we could make gwint.num signed
    // and compile time resolve unary minus and stuff
    // but that would need to be in the validation pass
    if(num >= Vector_size(array->type)) {
      gwlog_error("index out of bounds", "requested index to big for vector",
                  env->name, array->exp->loc, 0);
      //return false;
      return env->gwion->type[et_error];
    }
  } // TODO: else add effect!!!
  const Type t = array->type->info->base_type;
  if (array->depth == 1)
    return array->type->info->base_type;
  struct Array_Sub_ next = {array->exp->next, array->type->info->base_type,
                            array->depth - t->array_depth};
  return check_array_access(env, &next) ?: env->gwion->type[et_error];
}

static INSTR(VectorAccess) {
  const M_Object o = *(M_Object*)(shred->reg);
  const m_uint idx = *(m_uint*)(shred->reg + SZ_INT);
  memcpy(shred->reg, o->data + (instr->m_val * idx), instr->m_val);
}

static INSTR(VectorAccessRef) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  const m_uint idx = *(m_uint*)(shred->reg);
  *(m_bit**)(shred->reg - SZ_INT) = (o->data + (instr->m_val * idx));
}

static ANN bool emit_dynamic_access(const Emitter emit, Exp *exp,
                                    const Type base, const bool is_var) {
  CHECK_B(emit_exp(emit, exp));
  if(!is_var) {
    // TODO: we could have logical->goto opcodes
    emit_regmove(emit, -2 * SZ_INT);
    const Instr instr = emit_add_instr(emit, VectorAccess);
    instr->m_val = base->size;
//    instr->m_val2 = Vector_size(info->array.type);
    emit_regmove(emit, base->size);
  } else {
    emit_regmove(emit, -SZ_INT);
    const Instr instr = emit_add_instr(emit, VectorAccessRef);
    instr->m_val = base->size;
  }
  return true;
}


static OP_EMIT(opem_vector_access) {
  struct ArrayAccessInfo *const info = (struct ArrayAccessInfo *)data;
  const Type base = info->array.type->info->base_type;
  const bool is_var = !info->array.exp->next && info->is_var;
  Exp *next = info->array.exp->next;
  if(is_prim_int(info->array.exp)) {
    // TODO: have a function to retrieve ints
    const m_uint num = info->array.exp->d.prim.d.gwint.num;
    emit_dotmember(emit, base->size * num, base->size, is_var);
  } else {
    info->array.exp->next = NULL;
    emit_dynamic_access(emit, info->array.exp, base, is_var);
    info->array.exp->next = next;
  }
  if(!next) return true;
  info->array.type = base;
  info->array.exp = next;
  return emit_array_access(emit, info);
}

/*! build arg_list for `new`          *
 * `N` arguments with Type_Decl `td`  *
 * where `td` points to the base type */
static ArgList *new_args(const MemPool mp, const Type_Decl *td, 
                          const m_uint N) {
  ArgList *args = new_arglist(mp, N);
  for(uint32_t i = 0; i < N; i++) {
    Arg arg = { .var = { .td = cpy_type_decl(mp, td) }};
    arglist_set(args, i, arg);
  }
  return args;
}

static Type check_vector(const Env env, const Class_Def cdef,
                         const Type base, const m_uint N) {
  if(type_global(env, base))
    SET_FLAG(cdef, global);
  CHECK_ON(scan0_class_def(env, cdef)); // error
  const Type t = cdef->base.type; 
  t->nspc->offset = N * base->size;
  t->info->base_type = base;
  CHECK_B(traverse_cdef(env, t));
//  CHECK_B(emit_class_def(env, cdef));
  *(m_uint*)t->nspc->class_data = N;
  //set_tflag(t, tflag_emit);
  set_tflag(t, tflag_cdef);
  SET_FLAG(t, final);
  SET_FLAG(t, abstract);
  return t;
}

static OP_CHECK(opck_vector_scan) {
  struct TemplateScan *ts      = (struct TemplateScan *)data;
  m_str name = tdpp(env->gwion->mp, env->gwion->st, ts->td, true, true);
  const Symbol sym = insert_symbol(name);
  free_mstr(env->gwion->mp, name);
  // TODO: this should be in some other upper function
  const Type exist = nspc_lookup_type1(env->curr, sym);
  if (exist) return exist;
  const Class_Def c= ts->t->info->cdef;
  Type_Decl *base_td = tmplarglist_at(ts->td->types, 0).d.td;
  DECL_ON(const Type, base,  = known_type(env, base_td));

  // TODO: this is basically the same as in Array
  // and possibly Dict and Ref
  // make it in a function
  if (base->size == 0) {
    gwlog_error("Can't use type of size 0 as vector base", NULL,
                env->name, ts->td->tag.loc, 0);
    env_set_error(env, true);
    return env->gwion->type[et_error];
  }
  if (tflag(base, tflag_infer) || tflag(base, tflag_noret)) {
    gwlog_error("invalid type for vector", base->name,
                env->name, ts->td->tag.loc, 0);
    env_set_error(env, true);
    return env->gwion->type[et_error];
  }
  const TmplArg ta = tmplarglist_at(ts->td->types, 1);
  const m_uint size = ta.d.exp->d.prim.d.gwint.num;
  const Class_Def cdef  = cpy_class_def(env->gwion->mp, c);
  cdef->base.ext        = type2td(env->gwion, ts->t, ts->td->tag.loc);
  cdef->base.tag.sym    = sym;
  cdef->base.tmpl->call = cpy_tmplarg_list(env->gwion->mp, ts->td->types);
  Func_Def fdef = sectionlist_at(cdef->body, 0).d.func_def;
  fdef->base->args = new_args(env->gwion->mp, base_td, size);

  struct EnvSet es = {
    .env = env,
    .data = env,
    .scope = env->scope->depth
  };
  envset_pushv(&es, base->info->value);
  const Type t = check_vector(env, cdef, base, size);

  struct Op_Func opfunc = {
    .ck = opck_vector_access,
    .em = opem_vector_access,
    //.effect
  };
  struct Op_Import opi = {
 //   .lhs = tdef->type,
    .lhs = env->gwion->type[et_int],
    .rhs = cdef->base.type,
    .ret = base,
    .op  = insert_symbol("[]"),
    .func = &opfunc,
    .loc = ts->td->tag.loc,
  };
  add_op(env->gwion, &opi);
  
  envset_popv(&es, base->info->value);

  if(!t) return NULL;
  if (tflag(base, tflag_release))
    mk_dtor(env->gwion->mp, t, vector_dtor);
  return t;
}

static OP_CHECK(opck_vector_new) {
  Exp *e = data;
  Exp_Call *call = &e->d.exp_call;
  Exp *arg = call->args;
  const uint32_t nargs = exp_count(arg);
  if(!nargs) {
    gwlog_error("invalid 0 size for Vector", NULL,
                env->name, e->loc, 0);
    return NULL;
  }
  CHECK_B(check_exp(env, arg));
  TmplArgList *tl = new_tmplarglist(env->gwion->mp, 2);
  tmplarglist_set(tl, 0, MK_TMPLARG_TD(
    type2td(env->gwion, arg->type, arg->loc)));
  tmplarglist_set(tl, 1, MK_TMPLARG_EXP(
     new_prim_int(env->gwion->mp, nargs, e->loc)));
  call->tmpl = new_tmpl_call(env->gwion->mp, tl);
  Type_Decl *td = new_type_decl(env->gwion->mp,
     insert_symbol("Vector"), call->func->d.exp_dot.base->loc);
  td->types = call->tmpl->call;
  const Type owner = known_type(env, td);
  td->types = NULL;
  call->tmpl->list = owner->info->cdef->base.tmpl->list;
  free_type_decl(env->gwion->mp, td);
  call->func->d.exp_dot.base->type = owner;
  call->func->type = NULL;
  return check_exp(env, e);
}

GWION_IMPORT(vector) {
  const Type t_vector = gwi_class_ini(gwi, "Vector:[T, const int N]", "Object");
  GWI_B(gwi_func_ini(gwi, "auto", "new"));
  GWI_B(gwi_func_end(gwi, vector_new, ae_flag_none));
  GWI_B(gwi_item_ini(gwi, "T", "N"));
  GWI_B(gwi_item_end(gwi, ae_flag_static, num, 0));

  GWI_B(gwi_typedef_ini(gwi, "int", "index_t"));
  //GWI_B(gwi_typedef_exp(gwi, "self >= 0 && self < N"));
  GWI_B(gwi_typedef_exp(gwi, "self >= 0 && self < N"));
  GWI_B(gwi_typedef_end(gwi, ae_flag_none));

  GWI_B(gwi_class_end(gwi));
 /* 
  puts("before traverse");
  unset_tflag(t_vector, tflag_check);
  traverse_class_def(gwi->gwion->env, t_vector->info->cdef);
  puts("after traverse");
 */ 
  const Env env = gwi->gwion->env;
  Func_Def fdef = sectionlist_at(t_vector->info->cdef->body, 0).d.func_def;
  env_push_type(env, t_vector);
  traverse_func_def(env, fdef);
  env_pop(env, 0);
  struct Op_Func opfunc = {
    .ck = opck_vector_new,
  };
  add_op_func_check(gwi->gwion->env, t_vector, &opfunc, 0);
/*
  Type_Def tdef = sectionlist_at(t_vector->info->cdef->body, 2).d.type_def;
  env_push_type(env, t_vector);
  traverse_type_def(env, tdef);
  env_pop(env, 0);

 GWI_B(gwi_oper_ini(gwi, "Vector.index_t", "Vector", NULL));
 GWI_B(gwi_oper_add(gwi, opck_vector_access));
 GWI_B(gwi_oper_emi(gwi, opem_vector_access));
 GWI_B(gwi_oper_end(gwi, "[]", NULL));
*/ 
  GWI_B(gwi_oper_ini(gwi, "Vector", NULL, NULL));
  GWI_B(gwi_oper_add(gwi, opck_vector_scan));
  GWI_B(gwi_oper_end(gwi, "class", NULL));
  return true;
}
