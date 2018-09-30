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
};

ANN void ctrl_set_pc(const struct ctrl* ctrl, const m_uint i) {
  ctrl->pcs[i] |= JIT_PC;
}

ANN void ctrl_set_ex(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx-1] |= JIT_EX;
}

ANN void ctrl_curr(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx - 1] |= JIT_PC;
}

ANN void ctrl_next(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx] |= JIT_PC;
}

ANN m_bool ctrl_pc(const struct ctrl* ctrl) {
  return (ctrl->pcs[ctrl->idx-1] & JIT_PC) == JIT_PC;
}

ANN m_bool ctrl_ex(const struct ctrl* ctrl) {
  return (ctrl->pcs[ctrl->idx-1] & JIT_EX) == JIT_EX;
}

ANEW struct ctrl* new_ctrl(m_uint size) {
  struct ctrl* ctrl = (struct ctrl*)xcalloc(1, sizeof(struct ctrl));
  ctrl->idx = 0;
  ctrl->len = size;
  ctrl->pcs = (m_bit*)xcalloc(size, sizeof(m_bit));
  ctrl->state = 0;
//  ctrl->pcs[0] = JIT_PC;
  return ctrl;
}

ANN void free_ctrl(struct ctrl* ctrl) {
  xfree(ctrl->pcs);
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
