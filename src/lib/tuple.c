#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "traverse.h"
#include "object.h"
#include "parse.h"
#include "tuple.h"

struct TupleEmit {
  Exp e;
  Vector v;
  m_uint obj_offset;
  m_uint tmp_offset;
  m_uint mem_offset;
  m_uint sz;
  m_uint idx;
};

struct UnpackInfo_ {
  m_uint obj_offset;
  m_uint mem_offset;
  m_uint size;
};

static INSTR(TupleUnpack) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  struct UnpackInfo_ *info = (struct UnpackInfo_*)instr->m_val;
//  memcpy(shred->mem + info->mem_offset, o->data + shred->info->vm->gwion->type[et_tuple]->nspc->info->offset + info->obj_offset, info->size);
  memcpy(shred->mem + info->mem_offset, o->data + info->obj_offset, info->size);
}

INSTR(TupleMember) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  const Type base = o->type_ref;
  const m_bit* byte = shred->code->bytecode + shred->pc * BYTECODE_SZ;
  const Type t = (Type)vector_at(&base->e->tuple->types, instr->m_val);
  const m_uint offset = vector_at(&base->e->tuple->offset, instr->m_val);
  *(m_uint*)(byte + SZ_INT) = offset;
  if(!instr->m_val2) {
    if(t->size == SZ_INT)
      *(m_uint*)(byte) = eDotMember;
    else if(t->size == SZ_FLOAT)
      *(m_uint*)(byte) = eDotMember2;
    else {
      *(m_uint*)(byte) = eDotMember3;
      *(m_uint*)(byte + SZ_INT*2) = t->size;
    }
  } else
    *(m_uint*)(byte) = eDotMember4;
}

static FREEARG(freearg_tuple_at) {
  mp_free(((Gwion)gwion)->mp, UnpackInfo, (struct UnpackInfo*)instr->m_val);
}

ANN static void emit_unpack_instr_inner(const Emitter emit, struct TupleEmit *te) {
  const Instr instr = emit_add_instr(emit, TupleUnpack);
  struct UnpackInfo_ *info = mp_malloc(emit->gwion->mp, UnpackInfo);
  info->obj_offset = te->obj_offset;
  info->mem_offset = te->mem_offset;
  info->size = te->sz;
  instr->m_val = (m_uint)info;
}

ANN static int tuple_continue(struct TupleEmit *te) {
  const m_bool ret = (te->e = te->e->next) &&
       ++te->idx < VLEN(te->v);
  if(!ret)
    te->e = NULL;
  return ret;
}

ANN static void unpack_instr_decl(const Emitter emit, struct TupleEmit *te) {
  m_uint sz = 0;
  te->sz = 0;
  te->obj_offset = te->tmp_offset;
  do {
    if(te->e->exp_type == ae_exp_decl) {
      const Value value = te->e->d.exp_decl.list->self->value;
      te->sz += value->type->size;
      sz += value->type->size;
      value->from->offset = emit_local(emit, value->type->size, 0);
    } else {
      sz += ((Type)vector_at(te->v, te->idx))->size;
      break;
    }
  } while(tuple_continue(te));
  te->tmp_offset += sz;
}

ANN void emit_unpack_instr(const Emitter emit, struct TupleEmit *te) {
  te->mem_offset = emit_code_offset(emit);
  unpack_instr_decl(emit, te);
  if(te->sz)
    emit_unpack_instr_inner(emit, te);
  if(te->e && (te->e = te->e->next)) // antepenultimate ?
    emit_unpack_instr(emit, te);
}

static m_bool tuple_match(const Env env, const Type lhs, const Type rhs) {
  DECL_OB(const Vector, lv, = &lhs->e->tuple->types)
  DECL_OB(const Vector, rv, = &rhs->e->tuple->types)
  for(m_uint i = 0; i < vector_size(rv); i++) {
    DECL_OB(const Type, l, = (Type)vector_at(lv, i))
    const Type r = (Type)vector_at(rv, i);
    if(r != env->gwion->type[et_undefined])
      CHECK_BB(isa(l, r))
  }
  return GW_OK;
}

static OP_CHECK(opck_at_object_tuple) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  if(opck_rassign(env, data, mut) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  if(tuple_match(env, bin->lhs->type, bin->rhs->type) < 0)
    return env->gwion->type[et_null];
  bin->rhs->emit_var = 1;
  return bin->rhs->type;
}

static OP_CHECK(opck_at_tuple) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  if(bin->rhs->exp_type == ae_exp_primary &&
    bin->rhs->d.prim.prim_type == ae_prim_unpack) {
    Exp e = bin->rhs->d.prim.d.tuple.exp;
    int i = 0;
    do {
      if(e->exp_type == ae_exp_decl) {
        DECL_OO(const Type, t, = (Type)VPTR(&bin->lhs->type->e->tuple->types, i))
        e->d.exp_decl.td->xid->xid = insert_symbol(t->name);
        const Exp next = e->next;
        e->next = NULL;
        const m_bool ret = traverse_exp(env, e);
        e->next = next;
        CHECK_BO(ret)
        bin->rhs->meta = ae_meta_var;
      }
      ++i;
    } while((e = e->next));
    return bin->lhs->type;
  }
  return opck_at_object_tuple(env, data, mut);
}

static OP_CHECK(opck_at_tuple_object) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  if(opck_rassign(env, data, mut) == env->gwion->type[et_null])
    return env->gwion->type[et_null];
  if(!bin->rhs->type->e->tuple)
    return bin->rhs->type;
  if(tuple_match(env, bin->rhs->type, bin->lhs->type) < 0)
    return env->gwion->type[et_null];
  bin->rhs->emit_var = 1;
  set_decl_ref(bin->rhs);
  return bin->rhs->type;
}

static OP_CHECK(opck_cast_tuple_object) {
  const Exp_Cast *cast = (Exp_Cast*)data;
  if(tuple_match(env, exp_self(cast)->type, cast->exp->type) < 0)
    return env->gwion->type[et_null];
  return exp_self(cast)->type;
}

static INSTR(Tuple2Object) {
  const M_Object o = *(M_Object*)(shred->reg - instr->m_val2);
  const Type t = (Type)instr->m_val;
  if(o && isa(o->type_ref, t) < 0)
  // TODO: pass position by m_val2
//    Except(shred, "can't cast %s to %s\n", o->type_ref->name, t->name);
    Except(shred, _("can't cast\n"));
}

  // TODO: do not emit Tuple2Object if full match
#define mk_opem_tuple2object(name, type, rhs)             \
static OP_EMIT(opem_##name##_tuple_object) {              \
  const type exp = (type)data;                            \
  const Instr instr = emit_add_instr(emit, Tuple2Object); \
  instr->m_val = (m_uint)rhs;                             \
  instr->m_val2 = SZ_INT;                                 \
  return instr;                                           \
}
static OP_EMIT(opem_at_tuple_object) {                     \
  const Exp_Binary *bin = (Exp_Binary*)data;               \
  const Instr instr = emit_add_instr(emit, Tuple2Object);  \
  instr->m_val = (m_uint)bin->rhs->type;                   \
  instr->m_val2 = SZ_INT*2;                                \

  const Instr assign = emit_add_instr(emit, ObjectAssign); \

return assign;
}
mk_opem_tuple2object(cast, Exp_Cast *, exp_self(exp)->type)

static OP_CHECK(opck_cast_tuple) {
  const Exp_Cast *cast = (Exp_Cast*)data;
  CHECK_BN(tuple_match(env, exp_self(cast)->type, cast->exp->type))
  return exp_self(cast)->type;
}

static OP_CHECK(opck_impl_tuple) {
  struct Implicit *imp = (struct Implicit*)data;
  CHECK_BN(tuple_match(env, imp->e->type, imp->t))
  return imp->t;
}

static OP_EMIT(opem_at_tuple) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  if(!(bin->rhs->exp_type == ae_exp_primary &&
      bin->rhs->d.prim.prim_type == ae_prim_unpack)) {
    return emit_add_instr(emit, ObjectAssign);
  }
  const Exp e = bin->rhs->d.prim.d.tuple.exp;
  const Vector v = &bin->lhs->type->e->tuple->types;
  struct TupleEmit te = { .e=e, .v=v };
  emit_unpack_instr(emit, &te);
  return (Instr)GW_OK;
}

ANN void tuple_info(const Env env, Type_Decl *base, const Var_Decl var) {
  const Value v = var->value;
  const m_uint offset = vector_back(&env->class_def->e->tuple->offset);
  vector_add(&env->class_def->e->tuple->types, (vtype)v->type);
  vector_add(&env->class_def->e->tuple->offset, offset + v->type->size);
  Type_Decl *td = cpy_type_decl(env->gwion->mp, base);
  if(var->array)
    td->array = cpy_array_sub(env->gwion->mp, var->array);
  if(env->class_def->e->tuple->list) {
    Type_List tl = env->class_def->e->tuple->list;
    while(tl->next)
      tl = tl->next;
    tl->next = new_type_list(env->gwion->mp, td, NULL);
  } else
    env->class_def->e->tuple->list = new_type_list(env->gwion->mp, td, NULL);
}

INSTR(TupleCtor) {
  const Type t = (Type)instr->m_val;
  const M_Object o = new_object(shred->info->vm->gwion->mp, shred, t);
  const m_uint sz = t->nspc->info->offset;
  memcpy(o->data, shred->reg - sz, sz);
  shred->reg -= (sz - SZ_INT);
  *(M_Object*)(shred->reg - SZ_INT) = o;
}

ANN static Symbol tuple_sym(const Env env, const Vector v) {
  GwText text = { .mp=env->gwion->mp };
  text_add(&text, env->gwion->type[et_tuple]->name);
  text_add(&text, "<~");
  for(m_uint i = 0; i < vector_size(v); ++i) {
    const Type t = (Type)vector_at(v, i);
    text_add(&text, t != (Type)1 ? t->name : "_");
    if(i+1 < vector_size(v))
      text_add(&text, ",");
  }
  text_add(&text, "~>");
  const Symbol sym = insert_symbol(text.str);
  text_release(&text);
  return sym;
}

ANN Type tuple_type(const Env env, const Vector v, const loc_t pos) {
  const Symbol sym = tuple_sym(env, v);
  const Type exists = nspc_lookup_type0(env->curr, sym);
  if(exists)
    return exists;
  Stmt_List base = NULL, curr = NULL;
  Type_List tlbase = NULL, tl = NULL;
  for(m_uint i = 0; i < vector_size(v); ++i) {
    char name[num_digit(i) + 2];
    sprintf(name, "@tuple member %lu", i);
    const Symbol sym = insert_symbol(name);
    const Type t = (Type)vector_at(v, i);
    const Symbol tsym = insert_symbol(t != (Type)1 ? t->name : "@Undefined");
    Exp decl = decl_from_id(env->gwion->mp, tsym, sym, pos);
    const Stmt stmt = new_stmt_exp(env->gwion->mp, ae_stmt_exp, decl);
    const Stmt_List slist = new_stmt_list(env->gwion->mp, stmt, NULL);
    if(curr)
      curr->next = slist;
    if(!base)
      base = slist;
{// build Type_List
  const ID_List ilist = new_id_list(env->gwion->mp, tsym,
      loc_cpy(env->gwion->mp, pos));
  Type_Decl *td = new_type_decl(env->gwion->mp, ilist);
  Type_List tmp_tl = new_type_list(env->gwion->mp, td, NULL);
    if(tl)
      tl->next = tmp_tl;
    if(!tlbase)
      tlbase = tl = tmp_tl;

}
    curr = slist;
  }
  Section * section = new_section_stmt_list(env->gwion->mp, base);
  Ast body = new_ast(env->gwion->mp, section, NULL);
  const ID_List ilist = new_id_list(env->gwion->mp, insert_symbol(env->gwion->type[et_tuple]->name),
      loc_cpy(env->gwion->mp, pos));
  Type_Decl *td = new_type_decl(env->gwion->mp, ilist);
  Class_Def cdef = new_class_def(env->gwion->mp, ae_flag_template,
        sym, td, body, loc_cpy(env->gwion->mp, pos));
  Tmpl* tmpl = new_tmpl_call(env->gwion->mp, tlbase);
  cdef->base.tmpl = tmpl;
  CHECK_BO(scan0_class_def(env, cdef))
  SET_FLAG(cdef->base.type, abstract);
  cdef->base.type->e->tuple->list = tlbase;
  CHECK_BO(traverse_cdef(env, cdef))
  nspc_add_type_front(env->curr, sym, cdef->base.type);
  return cdef->base.type;
}

ANN TupleForm new_tupleform(MemPool p) {
  TupleForm tuple = mp_malloc(p, TupleForm);
  vector_init(&tuple->types);
  vector_init(&tuple->offset);
  vector_add(&tuple->offset, 0);
  tuple->list = NULL;
  return tuple;
}

ANN void free_tupleform(MemPool p, const TupleForm tuple) {
  vector_release(&tuple->types);
  vector_release(&tuple->offset);
  if(tuple->list)
    free_type_list(p, tuple->list);
}

ANN /*static*/ Type at_depth(const Env env, const Array_Sub array);
static OP_CHECK(opck_tuple) {
  const Array_Sub array = (Array_Sub)data;
  const Vector v = &array->type->e->tuple->types;
  const Exp exp = array->exp;
  if(exp->exp_type != ae_exp_primary ||
     exp->d.prim.prim_type != ae_prim_num)
    ERR_O(exp->pos, _("tuple subscripts must be litteral"))
  const m_uint idx = exp->d.prim.d.num;
  if(idx >= vector_size(v))
    ERR_O(exp->pos, _("tuple subscripts too big"))
  const Type type = (Type)vector_at(v, idx);
  if(type == env->gwion->type[et_undefined])
    ERR_O(exp->pos, _("tuple subscripts is undefined at index %lu"), idx)
  if(!exp->next)
    return type;
  struct Array_Sub_ next = { exp->next, type, array->depth - 1 };
  return at_depth(env, &next);
}

GWION_IMPORT(tuple) {
  const Type t_tuple = gwi_mk_type(gwi, "Tuple", SZ_INT, "Object");
  gwi_add_type(gwi, t_tuple);
  SET_FLAG(t_tuple, checked | ae_flag_scan2 | ae_flag_check | ae_flag_emit);
  gwi->gwion->type[et_tuple] = t_tuple;
  SET_FLAG(t_tuple, abstract | ae_flag_template);
  GWI_BB(gwi_oper_ini(gwi, "Object", "Tuple", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_at_tuple))
  GWI_BB(gwi_oper_emi(gwi, opem_at_tuple))
  GWI_BB(gwi_oper_end(gwi, "@=>", ObjectAssign))
  GWI_BB(gwi_oper_add(gwi, opck_cast_tuple))
  GWI_BB(gwi_oper_end(gwi, "$", NoOp))
  GWI_BB(gwi_oper_add(gwi, opck_impl_tuple))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Tuple", "Object", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_at_tuple_object))
  GWI_BB(gwi_oper_emi(gwi, opem_at_tuple_object))
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_cast_tuple_object))
  GWI_BB(gwi_oper_emi(gwi, opem_cast_tuple_object))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_impl_tuple))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  GWI_BB(gwi_oper_ini(gwi, "Tuple", "Tuple", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_at_tuple))
  GWI_BB(gwi_oper_emi(gwi, opem_at_tuple))
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
  GWI_BB(gwi_oper_ini(gwi, "int", "Tuple", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_tuple))
//  GWI_BB(gwi_oper_emi(gwi, opem_at_tuple))
  GWI_BB(gwi_oper_end(gwi, "@array", NULL))
  gwi_register_freearg(gwi, TupleUnpack, freearg_tuple_at);
  return GW_OK;
}
