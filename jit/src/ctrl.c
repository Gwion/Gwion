#include <stdlib.h>
#include <string.h>
#include <jit/jit.h> // labels
#include "defs.h"
#include "absyn.h"
#include "map.h"
#include "operator.h"
#include "ctrl.h"

struct ctrl {
  m_bit* pcs;
  jit_label_t** lbl;
  m_uint idx;
  m_uint len;
  m_uint state;
  Instr instr;
};

enum ctrl_flag {
  JIT_PC = 1 << 1,
  JIT_EX = 1 << 2,
  JIT_SK = 1 << 3
};
/*
ANN static void add_label(const struct ctrl* ctrl, const m_uint i) {
  jit_label_t* lbl = xmalloc(sizeof(jit_label_t));
  *lbl = jit_label_undefined;
  ctrl->lbl[i] = lbl;
}

ANN jit_label_t* ctrl_lbl(const struct ctrl* ctrl, const m_uint i) {
  return i < ctrl->len ? ctrl->lbl[i] : NULL;
}
*/
ANN void ctrl_set_pc(const struct ctrl* ctrl, const m_uint i) {
  ctrl->pcs[i] |= JIT_PC;
//  if(!ctrl->lbl[i])
//    add_label(ctrl, i);
}

ANN void ctrl_set_ex(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx-1] |= JIT_EX;
}
/*
ANN void ctrl_set_nf(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx - 1] |= JIT_NF;
//  if(!ctrl->lbl[ctrl->idx - 1])
//    add_label(ctrl, ctrl->idx -1);
}
*/
ANN void ctrl_curr(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx - 1] |= JIT_PC;
//  if(!ctrl->lbl[ctrl->idx - 1])
//    add_label(ctrl, ctrl->idx - 1);
}

ANN void ctrl_next(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx] |= JIT_PC;
//  if(!ctrl->lbl[ctrl->idx])
//    add_label(ctrl, ctrl->idx);
}

ANN void ctrl_set_skip(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx-1] |= JIT_SK;
  ctrl->pcs[ctrl->idx] |= JIT_PC;
//  if(!ctrl->lbl[ctrl->idx])
//    add_label(ctrl, ctrl->idx);
}

ANN m_bool ctrl_pc(const struct ctrl* ctrl) {
  return (ctrl->pcs[ctrl->idx-1] & JIT_PC) == JIT_PC;
}

ANN m_bool ctrl_ex(const struct ctrl* ctrl) {
  return (ctrl->pcs[ctrl->idx-1] & JIT_EX) == JIT_EX;
//  return ((ctrl->pcs[ctrl->idx-1] & JIT_EX) == JIT_EX) &&
//            ((ctrl->pcs[ctrl->idx] & JIT_PC) != JIT_PC);
}
ANN m_bool ctrl_skip(const struct ctrl* ctrl) {
  return (ctrl->pcs[ctrl->idx-1] & JIT_SK) == JIT_SK;
}
/*
ANN m_bool ctrl_nf(const struct ctrl* ctrl) {
  return (ctrl->pcs[ctrl->idx-1] & JIT_NF) == JIT_NF;
}

ANN m_bool ctrl_get_nf(const struct ctrl* ctrl, const m_uint idx) {
  return (ctrl->pcs[idx] & JIT_NF) == JIT_NF;
}
*/
ANEW struct ctrl* new_ctrl(m_uint size) {
  struct ctrl* ctrl = (struct ctrl*)xcalloc(1, sizeof(struct ctrl));
  ctrl->idx = 0;
  ctrl->len = size;
  ctrl->pcs = (m_bit*)xcalloc(size, sizeof(m_bit));
//  ctrl->lbl = (jit_label_t**)xcalloc(size, sizeof(jit_label_t));
  ctrl->state = 0;
  ctrl_set_pc(ctrl, 0);
//  ctrl->pcs[0] |= JIT_NF;
//  ctrl->pcs[ctrl->len] |= JIT_NF;
  return ctrl;
}

ANN void free_ctrl(struct ctrl* ctrl) {
  xfree(ctrl->pcs);
/*
  for(m_uint i = 0; i < ctrl->len; ++i) {
    jit_label_t* lbl = ctrl->lbl[i];
    if(lbl)
      xfree(ctrl->lbl[i]);
  }
  xfree(ctrl->lbl);
*/
  xfree(ctrl);
}

ANN m_uint ctrl_idx(const struct ctrl* ctrl) {
  return ctrl->idx;
}

ANN void ctrl_inc_idx(const struct ctrl* ctrl) {
  ++((struct ctrl*)ctrl)->idx;
}

ANN m_uint ctrl_state(const struct ctrl* ctrl) {
  return ctrl->state;
}

ANN void ctrl_set(const struct ctrl* ctrl, const m_uint idx) {
  ((struct ctrl*)ctrl)->idx = idx;
}

ANN void ctrl_done(const struct ctrl* ctrl) {
  struct ctrl* c = (struct ctrl*)ctrl;
  ++c->state;
  c->idx = 0;
}

ANN Instr ctrl_run(const struct ctrl* ctrl, const Vector v) {
  struct ctrl* c = (struct ctrl*)ctrl;
  return c->instr = c->idx < c->len ?
    (Instr)vector_at(v, c->idx++) : NULL;
}

ANN Instr ctrl_byte(const struct ctrl* ctrl) {
//if(ctrl_skip(ctrl))exit(2);
  return ctrl_skip(ctrl) ? (Instr)1 : ctrl->instr;
}
