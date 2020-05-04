#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "object.h"
#include "driver.h"
#include "shreduler_private.h"

ANN void shreduler_set_loop(const Shreduler s, const m_bool loop) {
  s->loop = loop > 0;
}

ANN VM_Shred shreduler_get(const Shreduler s) {
  Driver *const bbq = s->bbq;
  struct ShredTick_ *const tk = s->list;
  if(!tk) {
    if(!vector_size(&s->shreds) && !s->loop)
      bbq->is_running = 0;
    return NULL;
  }
  const m_float time = (m_float)bbq->pos + (m_float)GWION_EPSILON;
  if(tk->wake_time <= time) {
    if((s->list = tk->next))
      s->list->prev = NULL;
    tk->next = tk->prev = NULL;
    s->curr = tk;
    return tk->self;
  }
  return NULL;
}

ANN static void shreduler_parent(const VM_Shred out, const Vector v) {
  vector_rem2(v, (vtype)out);
  if(!vector_size(v)) {
    vector_release(v);
    out->tick->parent->child.ptr = NULL;
  }
}

ANN static inline void shreduler_child(const Vector v) {
  for(m_uint i = vector_size(v) + 1; --i;) {
    const VM_Shred child = (VM_Shred)vector_at(v, i - 1);
    shreduler_remove(child->info->vm->shreduler, child, 1);
  }
}

ANN static void shreduler_erase(const Shreduler s, struct ShredTick_ *tk) {
  if(tk->parent)
    shreduler_parent(tk->self, &tk->parent->child);
  if(tk->child.ptr)
    shreduler_child(&tk->child);
  vector_rem2(&s->shreds, (vtype)tk->self);
}

ANN void shreduler_remove(const Shreduler s, const VM_Shred out, const m_bool erase) {
  MUTEX_LOCK(s->mutex);
  struct ShredTick_ *tk = out->tick;
  if(tk == s->curr)
    s->curr = NULL;
  else if(tk == s->list)
    s->list = tk->next;
  if(tk->prev)
    tk->prev->next = tk->next;
  if(tk->next)
    tk->next->prev = tk->prev;
  tk->prev = tk->next = NULL;
  if(erase) {
    shreduler_erase(s, tk);
    _release(out->info->me, out);
  }
  MUTEX_UNLOCK(s->mutex);
}

ANN void shredule(const Shreduler s, const VM_Shred shred, const m_float wake_time) {
  const m_float time = wake_time + (m_float)s->bbq->pos;
  struct ShredTick_ *tk = shred->tick;
  tk->wake_time = time;
  if(s->list) {
    struct ShredTick_ *curr = s->list, *prev = NULL;
    do {
      if(curr->wake_time > time)
        break;
      prev = curr;
    } while((curr = curr->next));
    if(!prev) {
      tk->next = s->list;
      s->list = (s->list->prev = tk);
    } else {
      if((tk->next = prev->next))
        prev->next->prev = tk;
      tk->prev = prev;
      prev->next = tk;
    }
  } else
    s->list = tk;
  if(tk == s->curr)
    s->curr = NULL;
}

ANN void shreduler_ini(const Shreduler s, const VM_Shred shred) {
  shred->tick = mp_calloc(shred->info->mp, ShredTick);
  shred->tick->self = shred;
  shred->tick->shreduler = s;
}

ANN void shreduler_add(const Shreduler s, const VM_Shred shred) {
  shreduler_ini(s, shred);
  shred->tick->xid = ++s->shred_ids;
  vector_add(&s->shreds, (vtype)shred);
  shredule(s, shred, GWION_EPSILON);
}
