#include <string.h>
#include "defs.h"
#include "absyn.h"
#include "err_msg.h"
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
  return (e->exp_type == ae_exp_primary &&
e->d.exp_primary.primary_type == ae_primary_id &&
isa(e->type, t_int) > 0 &&
   !e->d.exp_primary.value->owner_class) ||
   e->exp_type == ae_exp_constprop2;
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
  SET_FLAG(v, ae_flag_constprop);
}

ANN static m_bool constant_propagation(const Exp_Binary* bin) {
  const Exp l = bin->lhs, r = bin->rhs;
  switch(bin->op) {
    case op_assign:
      if(is_constprop_value(l)) {
        if(is_const(r)) {
          constprop_value(l->d.exp_primary.value,
            r->d.exp_primary.d.num);
          constprop_exp(bin, r->d.exp_primary.d.num);
          return 1;
        } else
          UNSET_FLAG(l->d.exp_primary.value, ae_flag_constprop);
      }
      break;
    case op_chuck:
      if(isa(r->type, t_function) < 0) {
        if(is_constprop_value(r)) {
          if(is_const(l)) {
            constprop_value(r->d.exp_primary.value,
              l->d.exp_primary.d.num);
            constprop_exp(bin, l->d.exp_primary.d.num);
            return 1;
          }
        }
      } /* fallthrough */
    case op_plus_chuck:
    case op_minus_chuck:
    case op_times_chuck:
    case op_divide_chuck:
    case op_modulo_chuck:
    case op_rsl:
    case op_rsr:
    case op_rsand:
    case op_rsor:
    case op_rsxor:
    case op_at_chuck:
    case op_at_unchuck:
    case op_trig:
    case op_untrig:
      if(r->exp_type == ae_exp_constprop2) {
        r->d.exp_primary.value->d.ptr = 0;
        UNSET_FLAG(r->d.exp_primary.value, ae_flag_constprop);
      }
    default: break;
  }
  return 1;
}

ANN static m_bool constant_folding(const Exp_Binary* bin) {
  const Exp l = bin->lhs, r = bin->rhs;
  m_int ret;
  switch(bin->op) {
    case op_plus:
      ret = l->d.exp_primary.d.num + r->d.exp_primary.d.num;
      break;
    case op_minus:
      ret = l->d.exp_primary.d.num - r->d.exp_primary.d.num;
      break;
    case op_times:
       ret = l->d.exp_primary.d.num * r->d.exp_primary.d.num;
       break;
    case op_divide:
      if(r->d.exp_primary.d.num)
       ret = l->d.exp_primary.d.num / r->d.exp_primary.d.num;
      else
       CHECK_BB(err_msg(TYPE_, r->pos, "divide by zero"))
       break;
    case op_percent:
      if(r->d.exp_primary.d.num)
        ret = l->d.exp_primary.d.num % r->d.exp_primary.d.num;
      else
        CHECK_BB(err_msg(TYPE_, r->pos, "divide by zero"))
      break;
    case op_shift_left:
      ret = l->d.exp_primary.d.num >> r->d.exp_primary.d.num;
      break;
    case op_shift_right:
      ret = l->d.exp_primary.d.num << r->d.exp_primary.d.num;
      break;
    default:
      return 1;
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
  return 1;
}

m_bool optimize_const(const Exp_Binary* bin) {
  constant_propagation(bin);
  if(is_const(bin->lhs) && is_const(bin->rhs))
    CHECK_BB(constant_folding(bin))
  return 1;
}
