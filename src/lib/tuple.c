#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "instr.h"
#include "nspc.h"
#include "type.h"
#include "object.h"
#include "emit.h"
#include "env.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "traverse.h"
#include "value.h"
#include "parse.h"
#include "cpy_ast.h"

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
  memcpy(shred->mem + info->mem_offset, o->data + t_tuple->nspc->info->offset + info->obj_offset, info->size);
}

INSTR(TupleMember) {
  const M_Object o = *(M_Object*)(shred->reg - SZ_INT);
  const Type base = o->type_ref;
  const m_bit* byte = shred->code->bytecode + shred->pc * BYTECODE_SZ;
  const Type t = (Type)vector_at(&base->e->tuple_form, instr->m_val);
  const m_uint offset = vector_at(&base->e->tuple_offset, instr->m_val);
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
  struct UnpackInfo_ *info = mp_malloc2(emit->gwion->mp, sizeof(struct UnpackInfo_));
  info->obj_offset = te->tmp_offset;
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
  do {
    if(te->e->exp_type == ae_exp_decl) {
      const Value value = te->e->d.exp_decl.list->self->value;
      te->sz += value->type->size;
      value->offset = emit_local(emit, value->type->size, 0);
    te->tmp_offset = te->obj_offset;
    te->obj_offset += ((Type)vector_at(te->v, te->idx))->size;
    } else {
      sz = ((Type)vector_at(te->v, te->idx))->size;
      break;
    }
  } while(tuple_continue(te));
      te->obj_offset += sz;
}

ANN void emit_unpack_instr(const Emitter emit, struct TupleEmit *te) {
  te->mem_offset = emit_code_offset(emit);
  unpack_instr_decl(emit, te);
  if(te->sz)
    emit_unpack_instr_inner(emit, te);
  if(te->e && (te->e = te->e->next))
    emit_unpack_instr(emit, te);
}

static m_bool tuple_match(const Env env, const Type type[2]) {
  const Vector lv = &type[0]->e->tuple_form;
  const Vector rv = &type[1]->e->tuple_form;
  for(m_uint i = 0; i < vector_size(rv); i++) {
    DECL_OB(const Type, l, = (Type)vector_at(lv, i))
    const Type r = (Type)vector_at(rv, i);
    if(r != t_undefined)
    CHECK_BB(isa(l, r))
  }
  return GW_OK;
}

static OP_CHECK(opck_at_tuple) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  if(bin->rhs->exp_type == ae_exp_primary &&
    bin->rhs->d.exp_primary.primary_type == ae_primary_unpack) {
    Exp e = bin->rhs->d.exp_primary.d.tuple.exp;
    int i = 0;
    do {
      if(e->exp_type == ae_exp_decl) {
        e->d.exp_decl.td->xid->xid = insert_symbol(//could be better
             ((Type)VPTR(&bin->lhs->type->e->tuple_form, i))->name);
        CHECK_BO(traverse_decl(env, &e->d.exp_decl))
      }
      ++i;
    } while((e = e->next));
  } else {
    if(opck_rassign(env, data, mut) == t_null)
      return t_null;
    const Type type[2] = { bin->lhs->type, bin->rhs->type };
    if(tuple_match(env, type) < 0)
      return t_null;
    bin->rhs->emit_var = 1;
  }
  return bin->lhs->type;
}

static OP_CHECK(opck_cast_tuple) {
  Exp_Cast *cast = (Exp_Cast*)data;
  if(exp_self(cast)->type->e->def && cast->exp->type->e->def) {
    const Type type[2] = { exp_self(cast)->type, cast->exp->type };
    CHECK_BO(tuple_match(env, type))
  }
  return exp_self(cast)->type;
}

static OP_CHECK(opck_impl_tuple) {
  struct Implicit *imp = (struct Implicit*)data;
  const Type type[2] = { imp->e->type, imp->t };
  CHECK_BO(tuple_match(env, type))
  return imp->t;
}

static OP_EMIT(opem_at_tuple) {
  const Exp_Binary *bin = (Exp_Binary*)data;
  if(!(bin->rhs->exp_type == ae_exp_primary &&
    bin->rhs->d.exp_primary.primary_type == ae_primary_unpack)) {
    emit_add_instr(emit, ObjectAssign);
//    emit_add_instr(emit, int_r_assign);
//    const Instr instr = emit_add_instr(emit, RegAddRef);
//instr->m_val = 1;
    return GW_OK;
  }
  const Exp e = bin->rhs->d.exp_primary.d.tuple.exp;
  const Vector v = &bin->lhs->type->e->tuple_form;
  struct TupleEmit te = { .e=e, .v=v };
  emit_unpack_instr(emit, &te);
  return GW_OK;
}

ANN void tuple_info(const Env env, Type_Decl *base, const Var_Decl var) {
  const Value v = var->value;
  const m_uint offset = vector_back(&env->class_def->e->tuple_offset);
  vector_add(&env->class_def->e->tuple_form, (vtype)v->type);
  vector_add(&env->class_def->e->tuple_offset, offset + v->type->size);
  Type_Decl *td = cpy_type_decl(env->gwion->mp, base);
  if(var->array) {
    if(td->array)
      td->array->depth += var->array->depth;
    else
      td->array = cpy_array_sub(env->gwion->mp, var->array);
  }
  if(env->class_def->e->tuple_tl) {
    Type_List tl = env->class_def->e->tuple_tl;
    while(tl->next)
      tl = tl->next;
    tl->next = new_type_list(env->gwion->mp, td, NULL);
  } else
  env->class_def->e->tuple_tl = new_type_list(env->gwion->mp, td, NULL);
}

INSTR(TupleCtor) {
  const Type t = (Type)instr->m_val;
  const M_Object o = new_object(shred->info->vm->gwion->mp, shred, t);
  const size_t sz = t_tuple->nspc->info->offset;
  memcpy(o->data + t_tuple->nspc->info->offset,
      shred->reg - (t->nspc->info->offset - sz), (t->nspc->info->offset - sz));
  shred->reg -= (t->nspc->info->offset - sz - SZ_INT);
  *(M_Object*)(shred->reg - SZ_INT) = o;
}

GWION_IMPORT(tuple) {
  GWI_OB((t_tuple = gwi_mk_type(gwi, "Tuple", SZ_INT, t_object)))
  GWI_BB(gwi_class_ini(gwi, t_tuple, NULL, NULL))
  GWI_BB(gwi_class_end(gwi))
  SET_FLAG(t_tuple, abstract | ae_flag_template);
  GWI_BB(gwi_oper_ini(gwi, "Object", "Tuple", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_at_tuple))
  GWI_BB(gwi_oper_emi(gwi, opem_at_tuple))
  GWI_BB(gwi_oper_end(gwi, "@=>", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_cast_tuple))
  GWI_BB(gwi_oper_end(gwi, "$", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_impl_tuple))
  GWI_BB(gwi_oper_end(gwi, "@implicit", NULL))
  register_freearg(gwi, TupleUnpack, freearg_tuple_at);
  return GW_OK;
}

ANN static Symbol tuple_sym(const Env env, const Vector v) {
  GwText text = { .mp=env->gwion->mp };
  text_add(&text, t_tuple->name);
  text_add(&text, "<~");
  for(m_uint i = 0; i < vector_size(v); ++i) {
    const Type t = (Type)vector_at(v, i);
    text_add(&text, t != 1 ? t->name : "_");
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
  const Type exists = nspc_lookup_type1(env->curr, sym);
  if(exists)
    return exists;
  Stmt_List base = NULL, curr = NULL;
  Type_List tlbase = NULL, tl = NULL;
  for(m_uint i = 0; i < vector_size(v); ++i) {
    char name[num_digit(i) + 2];
    sprintf(name, "@e%lu", i);
    const Symbol sym = insert_symbol(name);
    const Type t = (Type)vector_at(v, i);
    const Symbol tsym = insert_symbol(t != 1 ? t->name : "@Undefined");
//    const Symbol tsym = t != 1 ? insert_symbol(t->name) : insert_symbol("@undefined");
    Exp decl = decl_from_id(env->gwion->mp, tsym, sym, loc_cpy(env->gwion->mp, pos));
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
  Class_Body body = new_class_body(env->gwion->mp, section, NULL);
  const ID_List ilist = new_id_list(env->gwion->mp, insert_symbol(t_tuple->name),
      loc_cpy(env->gwion->mp, pos));
  Type_Decl *td = new_type_decl(env->gwion->mp, ilist);
  Class_Def cdef = new_class_def(env->gwion->mp, ae_flag_template,
        sym, td, body, loc_cpy(env->gwion->mp, pos));
  Tmpl* tmpl = new_tmpl_call(env->gwion->mp, tlbase);
  cdef->base.tmpl = tmpl;
  CHECK_BO(scan0_class_def(env, cdef))
  SET_FLAG(cdef->base.type, abstract);
  cdef->base.type->e->tuple_tl = tlbase;
//  CHECK_BO(scan1_cdef(env, cdef))
  CHECK_BO(traverse_cdef(env, cdef))
  nspc_add_type(env->curr, sym, cdef->base.type);
//  map_set((Map)vector_front(&env->curr->info->type), sym, cdef->base.type);
  return cdef->base.type;
}
