#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_ptr = { "Ptr", SZ_INT, &t_object, te_ptr};

const m_str get_type_name(const m_str s, const m_uint index) {
  m_str name = strstr(s, "<");
  m_uint i = 0;
  m_uint lvl = 0;
  m_uint n = 1;
  size_t len = name ? strlen(name) : 0;
  char c[strlen(s)];

  if(!name)
    return index ? NULL : s_name(insert_symbol(s));
  memset(c, 0, strlen(s));
  if(index == 0) {
    strncpy(c, s, strlen(s) - len);
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
  return strlen(c) ? s_name(insert_symbol(c)) : NULL;
}

static OP_CHECK(opck_ptr_assign) {
  Exp_Binary* bin = (Exp_Binary*)data;
  if(!strcmp(bin->lhs->type->name, get_type_name(bin->rhs->type->name, 1))) {
    if(bin->lhs->meta != ae_meta_var) {
      err_msg(INSTR_, 0, "left side operand is constant");
      return &t_null;
    }
    bin->lhs->emit_var = 1;
    return bin->lhs->type;
  }
  return &t_null;
}

static INSTR(instr_ptr_assign) {
  POP_REG(shred, SZ_INT * 2)
  M_Object o = *(M_Object*)REG(SZ_INT);
  *(m_uint**)o->data = *(m_uint**)REG(0);
  PUSH_REG(shred, SZ_INT)
}

static OP_CHECK(opck_ptr_deref) {
  Exp_Unary* unary = (Exp_Unary*)data;
  unary->self->type = nspc_lookup_type1(unary->exp->type->owner, insert_symbol(get_type_name(unary->exp->type->name, 1)));
  return unary->self->type;
}

static INSTR(instr_ptr_deref) {
  POP_REG(shred, SZ_INT)
  M_Object o = *(M_Object*)REG(0);
  if(instr->m_val2)
    memcpy(REG(0), o->data, SZ_INT);
  else
  memcpy(REG(0), *(char**)o->data, instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

static OP_EMIT(opem_ptr_deref) {
  Exp_Unary* unary = (Exp_Unary*)data;
  Instr instr = emitter_add_instr(emit, instr_ptr_deref);
  instr->m_val = unary->self->type->size;
  instr->m_val2 = unary->self->emit_var;
  return 1;
}

INSTR(Cast2Ptr) {
  POP_REG(shred, instr->m_val)
  M_Object o = new_M_Object(shred);
  o->data = malloc(SZ_INT);
  *(m_uint**)o->data = *(m_uint**)REG(0);
  *(M_Object*)REG(0) = o;
  PUSH_REG(shred, SZ_INT)
}

/*
// handle builtin pointer. we shoud provide a function for creation
static DTOR(ptr_dtor) {
  if(sizeof(o->data) > SZ_INT)
    free(o->data + SZ_INT);
}
*/

m_bool import_ptr(Gwi gwi) {
  const m_str list[] = { "A" };
  CHECK_BB(gwi_tmpl_ini(gwi, 1, list))
  CHECK_BB(gwi_class_ini(gwi, &t_ptr, NULL, NULL))
  CHECK_BB(gwi_tmpl_end(gwi))
  CHECK_BB(gwi_item_ini(gwi, "int", "@val"))
  CHECK_BB(gwi_item_end(gwi, 0, NULL))
  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "Ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_ptr_assign))
  CHECK_BB(gwi_oper_end(gwi, op_trig, instr_ptr_assign))
  CHECK_BB(gwi_oper_ini(gwi, NULL, "Ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_ptr_deref))
  CHECK_BB(gwi_oper_emi(gwi, opem_ptr_deref))
  CHECK_BB(gwi_oper_end(gwi, op_times, instr_ptr_deref))
  return 1;
}
