#include <stdlib.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "object.h"
#include "driver.h"
#include "shreduler_private.h"

ANN void shreduler_set_loop(const Shreduler s, const bool loop) {
  s->loop = loop;
}

ANN VM_Shred shreduler_get(const Shreduler s) {
  gwt_lock(&s->mutex);
  Driver *const            bbq = s->bbq;
  struct ShredTick_ *const tk  = s->list;
  if (tk) {
    const m_float time = (m_float)bbq->pos + GWION_EPSILON;
    if (tk->wake_time <= time) {
      if ((s->list = tk->next)) s->list->prev = NULL;
      tk->next = tk->prev = NULL;
      s->curr             = tk;
      gwt_unlock(&s->mutex);
      return tk->self;
    }
  }
  if (!s->loop && !vector_size(&s->active_shreds)) bbq->is_running = 0;
  gwt_unlock(&s->mutex);
  return NULL;
}

ANN static void shreduler_erase(const Shreduler, struct ShredTick_ *const);

ANN static void tk_remove(const Shreduler s, struct ShredTick_ *const tk) {
  if (tk == s->curr) s->curr = NULL;
  else if (tk == s->list) s->list = tk->next;
  if (tk->prev) tk->prev->next = tk->next;
  if (tk->next) tk->next->prev = tk->prev;
}

ANN static inline void child(const Shreduler s, const Vector v) {
  for (m_uint i = vector_size(v) + 1; --i;) {
    const VM_Shred shred = (VM_Shred)vector_at(v, i - 1);
    struct ShredTick_ *const tk = shred->tick;
    tk_remove(s, tk);
    shreduler_erase(s, tk);
  }
}

ANN static void shreduler_erase(const Shreduler          s,
                                struct ShredTick_ *const tk) {
  const VM_Shred shred = tk->self;
  if (tk->child.ptr) child(s, &tk->child);
  gwt_lock(&shred->mutex);
  tk->prev = (struct ShredTick_*)-1;
  gwt_unlock(&shred->mutex);
  const m_uint size =
      shred->info->frame.ptr ? vector_size(&shred->info->frame) : 0;
  if(size) unwind(shred, (Symbol)-1, size);
  vector_rem2(&s->active_shreds, (vtype)shred);
  release(shred->info->me, shred);
}

ANN void shreduler_remove(const Shreduler s, const VM_Shred out,
                          const bool erase) {
  gwt_lock(&s->mutex);
  struct ShredTick_ *const tk = out->tick;
  tk_remove(s, tk);
  if (likely(!erase)) tk->prev = tk->next = NULL;
  else {
    if (tk->parent) vector_rem2(&tk->parent->child, (vtype)out);
    shreduler_erase(s, tk);
  }
  gwt_unlock(&s->mutex);
}

ANN static void _shredule(const Shreduler s,   struct ShredTick_ *tk,
                  const m_float wake_time) {
  if(tk->prev == (struct ShredTick_*)-1) return;
  tk->wake_time           = wake_time;
  if (s->list) {
    struct ShredTick_ *curr = s->list, *prev = NULL;
    do {
      if (curr->wake_time > wake_time) break;
      prev = curr;
    } while ((curr = curr->next));
    if (!prev) {
      tk->next = s->list;
      s->list  = s->list->prev = tk;
    } else {
      if ((tk->next = prev->next)) prev->next->prev = tk;
      tk->prev   = prev;
      prev->next = tk;
    }
  } else s->list = tk;
  if (tk == s->curr) s->curr = NULL;
}

ANN void shredule_time(const Shreduler s, const VM_Shred shred,
                  const m_float wake_time) {
  if(wake_time < s->bbq->pos) {
    handle(shred, "NegativeWakeUpTime");
    return;
  }
  struct ShredTick_ *tk   = shred->tick;
  gwt_lock(&s->mutex);
  _shredule(s, tk, wake_time);
  gwt_unlock(&s->mutex);
}

ANN void shredule(const Shreduler s, const VM_Shred shred,
                  const m_float wake_time) {
  return shredule_time(s, shred, wake_time + s->bbq->pos);
}

ANN void shreduler_ini(const Shreduler s, const VM_Shred shred) {
  shred->tick            = mp_calloc(shred->info->mp, ShredTick);
  shred->tick->self      = shred;
  shred->tick->shreduler = s;
}

ANN void shreduler_add(const Shreduler s, const VM_Shred shred) {
  shreduler_ini(s, shred);
  shred->tick->xid = ++s->shred_ids;
  gwt_lock(&s->mutex);
  vector_add(&s->active_shreds, (vtype)shred);
  _shredule(s, shred->tick, GWION_EPSILON);
  gwt_unlock(&s->mutex);
}

ANN Shreduler new_shreduler(const MemPool mp) {
  Shreduler s = (Shreduler)mp_calloc(mp, Shreduler);
  vector_init(&s->active_shreds);
  gwt_lock_ini(&s->mutex);
  return s;
}

ANN void free_shreduler(const MemPool mp, const Shreduler s) {
  vector_release(&s->active_shreds);
  gwt_lock_end(&s->mutex);
  mp_free(mp, Shreduler, s);
}
