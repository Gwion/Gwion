#include <stdlib.h>
#include <string.h>
#include "defs.h"
#include "map.h"
#include "ctrl.h"

struct ctrl {
  m_bit* pcs;
  m_uint idx;
  m_uint len;
  m_uint state;
};

enum ctrl_flag {
  JIT_PC = 1 << 1,
  JIT_EX = 1 << 2
};

ANN void ctrl_set_pc(const struct ctrl* ctrl, const m_uint i) {
  ctrl->pcs[i] |= JIT_PC;
}

ANN void ctrl_set_ex(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx - 1] |= JIT_EX;
}

ANN void ctrl_curr(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx - 1] |= JIT_PC;
}

ANN void ctrl_next(const struct ctrl* ctrl) {
  ctrl->pcs[ctrl->idx] |= JIT_PC;
}
/*
#define ctrl_get(a, b)                                  \
ANN m_bool ctrl_##a(const struct ctrl* ctrl) {          \
  return (ctrl->pcs[ctrl->idx-1] & JIT_##b) == JIT_##b; \
}
ctrl_get(pc, PC)
ctrl_get(ex, EX)
*/
ANN m_bool ctrl_pc(const struct ctrl* ctrl) {
  return (ctrl->pcs[ctrl->idx-1] & JIT_PC) == JIT_PC;
}
ANN m_bool ctrl_ex(const struct ctrl* ctrl) {
  return (ctrl->pcs[ctrl->idx-1] & JIT_EX) == JIT_EX &&
    !((ctrl->pcs[ctrl->idx] & JIT_PC) == JIT_PC);
}

ANEW struct ctrl* new_ctrl(m_uint size) {
  struct ctrl* ctrl = (struct ctrl*)xmalloc(sizeof(struct ctrl));
  ctrl->idx = 0;
  ctrl->len = size;
  ctrl->pcs = (m_bit*)xcalloc(size, sizeof(m_bit));
  ctrl->state = 0;
  ctrl_set_pc(ctrl, 0);
  return ctrl;
}

ANN void free_ctrl(struct ctrl* ctrl) {
  xfree(ctrl->pcs);
  xfree(ctrl);
}

ANN m_uint ctrl_idx(const struct ctrl* ctrl) {
  return ctrl->idx;
}

ANN m_uint ctrl_state(const struct ctrl* ctrl) {
  return ctrl->state;
}

ANN void ctrl_set(const struct ctrl* ctrl, const m_uint idx) {
  ((struct ctrl*)ctrl)->idx = idx;
}

ANN void ctrl_done(struct ctrl* ctrl) {
  ++ctrl->state;
  ctrl->idx = 0;
}

ANN Instr ctrl_run(struct ctrl* ctrl, Vector v) {
  return ctrl->idx < ctrl->len ?
    (Instr)vector_at(v, ctrl->idx++) : NULL;
}
