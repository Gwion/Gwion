#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

ANN m_str get_type_name(const m_str s, const m_uint index) {
  m_str name = strstr(s, "<");
  m_uint i = 0;
  m_uint lvl = 0;
  m_uint n = 1;
  const size_t len = name ? strlen(name) : 0;
  const size_t slen = strlen(s);
  const size_t tlen = slen -len + 1;
  char c[slen];

  if(!name)
    return index ? NULL : s_name(insert_symbol(s));
  if(index == 0) {
    snprintf(c, tlen, "%s", s);
    return s_name(insert_symbol(c));
  }
  while(*name++) {
    if(*name == '<')
      lvl++;
    else if(*name == '>' && !lvl--)
      break;
    if(*name == ',' && !lvl) {
      ++name;
      ++n;
    }
    if(n == index)
      c[i++] = *name;
  }
  c[i] = '\0';
  return strlen(c) ? s_name(insert_symbol(c)) : NULL;
}

static OP_CHECK(opck_ptr_assign) {
  const Exp_Binary* bin = (Exp_Binary*)data;
  if(!strcmp(bin->lhs->type->name, get_type_name(bin->rhs->type->name, 1))) {
    if(bin->lhs->meta != ae_meta_var) {
      err_msg(INSTR_, 0, "left side operand is constant");
      return t_null;
    }
    bin->lhs->emit_var = 1;
    return bin->lhs->type;
  }
  return t_null;
}

static INSTR(instr_ptr_assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT)
  const M_Object o = *(M_Object*)REG(0);
  *(m_uint**)o->data = *(m_uint**)REG(-SZ_INT);
}

static OP_CHECK(opck_ptr_deref) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  unary->self->type = nspc_lookup_type1(unary->exp->type->owner, insert_symbol(get_type_name(unary->exp->type->name, 1)));
  return unary->self->type;
}

static OP_CHECK(opck_implicit_ptr) {
  const struct Implicit* imp = (struct Implicit*)data;
  const Exp e = (Exp)imp->e;
  if(!strcmp(get_type_name(imp->t->name, 1), e->type->name)) {
    if(e->meta == ae_meta_value) {
      err_msg(INSTR_, 0, "can't cast constant to Ptr");
      return t_null;
    }
    e->cast_to = imp->t;
    e->emit_var = 1;
    return imp->t;
  }
  return NULL;
}

static INSTR(instr_ptr_deref) { GWDEBUG_EXE
  const M_Object o = *(M_Object*)REG(-SZ_INT);
  if(instr->m_val2)
    memcpy(REG(-SZ_INT), o->data, SZ_INT);
  else
    memcpy(REG(-SZ_INT), *(m_bit**)o->data, instr->m_val);
}

static OP_EMIT(opem_ptr_deref) {
  const Exp_Unary* unary = (Exp_Unary*)data;
  const Instr instr = emitter_add_instr(emit, instr_ptr_deref);
  instr->m_val = unary->self->type->size;
  instr->m_val2 = unary->self->emit_var;
  return 1;
}

static INSTR(Cast2Ptr) { GWDEBUG_EXE
  const M_Object o = new_object(shred, t_ptr);
  *(m_uint**)o->data = *(m_uint**)REG(-SZ_INT);
  *(M_Object*)REG(-SZ_INT) = o;
}

GWION_IMPORT(ptr) {
  const m_str list[] = { "A" };
  CHECK_OB((t_ptr = gwi_mk_type(gwi, "Ptr", SZ_INT, t_object)))
  CHECK_BB(gwi_tmpl_ini(gwi, 1, list))
  CHECK_BB(gwi_class_ini(gwi, t_ptr, NULL, NULL))
  CHECK_BB(gwi_tmpl_end(gwi))
  CHECK_BB(gwi_item_ini(gwi, "int", "@val"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_ptr_assign))
  CHECK_BB(gwi_oper_end(gwi, op_trig, instr_ptr_assign))
  CHECK_BB(gwi_oper_add(gwi, opck_implicit_ptr))
  CHECK_BB(gwi_oper_end(gwi, op_impl, Cast2Ptr))
  CHECK_BB(gwi_oper_ini(gwi, NULL, "Ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_ptr_deref))
  CHECK_BB(gwi_oper_emi(gwi, opem_ptr_deref))
  CHECK_BB(gwi_oper_end(gwi, op_mul, instr_ptr_deref))
  return 1;
}

#ifdef JIT
#include "code/ptr.h"
#endif

