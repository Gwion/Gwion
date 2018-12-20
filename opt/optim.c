#include <string.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "optim.h"

void constprop_prim(const Exp_Primary* p, m_uint* ptr) {
  Exp_Primary* e = (Exp_Primary*)p;
  e->primary_type = ae_primary_constprop;
  e->d.num = (m_uint)ptr; // improve me
}

ANN static m_bool is_const(Exp e) {
  return (e->exp_type == ae_exp_primary &&
    e->d.exp_primary.primary_type == ae_primary_num) ||
    e->exp_type == ae_exp_constprop;
}

ANN static m_bool is_constprop_value(const Exp e) {
  if (e->exp_type == ae_exp_decl
    && isa(e->d.exp_decl.list->self->value->type, t_int) > 0 &&
    !e->d.exp_decl.list->self->value->type->array_depth &&
    !e->d.exp_decl.list->self->value->owner_class &&
    !GET_FLAG(e->d.exp_decl.list->self->value, arg))
  return (e->exp_type == ae_exp_primary &&
    e->d.exp_primary.primary_type == ae_primary_id &&
    isa(e->type, t_int) > 0 &&
    !e->d.exp_primary.value->owner_class) ||
    e->exp_type == ae_exp_constprop2;
  return 0;
}

ANN static void constprop_exp(const Exp_Binary* bin, long num) {
  const Exp e = bin->self, l = bin->lhs, r = bin->rhs;
  e->exp_type = ae_exp_constprop;
  e->d.exp_primary.d.num = num;
  free_exp(l);
  free_exp(r);
}

ANN static void constprop_value(const Value v, const long num) {
  v->d.ptr = (m_uint*)num; //fixme
  SET_FLAG(v, constprop);
}

ANN static m_bool constant_propagation(const Exp_Binary* bin) {
  const Exp l = bin->lhs, r = bin->rhs;
  switch(bin->op) {
    case op_chuck:
      if(isa(r->type, t_function) < 0) {
        if(is_constprop_value(r)) {
          if(is_const(l)) {
if(r->d.exp_primary.primary_type == ae_primary_num) {
            constprop_value(r->d.exp_primary.value,
              l->d.exp_primary.d.num);
            constprop_exp(bin, l->d.exp_primary.d.num);
}
  else if(r->exp_type == ae_exp_decl) {
    SET_FLAG(r->d.exp_decl.list->self->value, constprop);
  *(m_uint*)r->d.exp_decl.list->self->value->d.ptr = l->d.exp_primary.d.num;
  }
            return GW_OK;
          }
        }
      } /* fallthrough */
    case op_radd:
    case op_rsub:
    case op_rmul:
    case op_rdiv:
    case op_rmod:
    case op_rsl:
    case op_rsr:
    case op_rsand:
    case op_rsor:
    case op_rsxor:
    case op_ref:
    case op_unref:
    case op_trig:
    case op_untrig:
      if(r->exp_type == ae_exp_constprop2) {
        r->d.exp_primary.value->d.ptr = 0;
        UNSET_FLAG(r->d.exp_primary.value, constprop);
      }
    default: break;
  }
  return GW_OK;
}

ANN static m_bool constant_folding(const Exp_Binary* bin) {
  const Exp l = bin->lhs, r = bin->rhs;
  m_int ret = 0;
  switch(bin->op) {
    case op_add:
      ret = l->d.exp_primary.d.num + r->d.exp_primary.d.num;
      break;
    case op_sub:
      ret = l->d.exp_primary.d.num - r->d.exp_primary.d.num;
      break;
    case op_mul:
       ret = l->d.exp_primary.d.num * r->d.exp_primary.d.num;
       break;
    case op_div:
      if(r->d.exp_primary.d.num)
       ret = l->d.exp_primary.d.num / r->d.exp_primary.d.num;
      else
       ERR_B(r->pos, "div by zero")
       break;
    case op_mod:
      if(r->d.exp_primary.d.num)
        ret = l->d.exp_primary.d.num % r->d.exp_primary.d.num;
      else
        ERR_B(r->pos, "div by zero")
      break;
    case op_shl:
      ret = l->d.exp_primary.d.num >> r->d.exp_primary.d.num;
      break;
    case op_shr:
      ret = l->d.exp_primary.d.num << r->d.exp_primary.d.num;
      break;
    default:
      return GW_OK;
  }
  const Exp n = bin->self->next;
  const Exp e = bin->self;
  free_exp(l);
  free_exp(r);
  memset(e, 0, sizeof(struct Exp_));
  e->exp_type = ae_exp_primary;
  e->d.exp_primary.primary_type = ae_primary_num;
  e->d.exp_primary.d.num = ret;
  e->d.exp_primary.self = e;
  e->next = n;
  return GW_OK;
}

m_bool optimize_const(const Exp_Binary* bin) {
  constant_propagation(bin);
  if(is_const(bin->lhs) && is_const(bin->rhs))
    CHECK_BB(constant_folding(bin))
  return GW_OK;
}
