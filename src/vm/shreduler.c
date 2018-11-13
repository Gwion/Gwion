#include <stdlib.h>
#include "defs.h"
#include "map.h"
#include "oo.h"
#include "vm.h"
#include "object.h"
#include "shreduler_private.h"

ANN void shreduler_set_loop(const Shreduler s, const m_bool loop) {
  s->loop = loop < 0 ? 0 : 1;
}

ANN VM_Shred shreduler_get(const Shreduler s) {
  VM* vm = s->vm;
  const VM_Shred shred = s->list;
  if(!shred) {
    if(!vector_size(&vm->shred) && !s->loop)
      vm->is_running = 0;
    return NULL;
  }
  const m_float time = (m_float)vm->bbq->pos + (m_float).5;
  if(shred->wake_time <= time) {
    if((s->list = shred->next))
      s->list->prev = NULL;
    shred->next = shred->prev = NULL;
    return s->curr = shred;
  }
  return NULL;
}

ANN static void shreduler_parent(const VM_Shred out, const Vector v) {
  vector_rem2(v, (vtype)out);
  if(!vector_size(v)) {
    vector_release(v);
    out->parent->child.ptr = NULL;
  }
  out->parent = NULL;
}

ANN static void shreduler_child(const Shreduler s, const Vector v) {
  for(m_uint i = vector_size(v) + 1; --i;) {
    const VM_Shred child = (VM_Shred)vector_at(v, i - 1);
    shreduler_remove(s, child, 1);
  }
}

ANN static void shreduler_erase(const Shreduler s, const VM_Shred out) {
  if(out->parent)
    shreduler_parent(out, &out->parent->child);
  if(out->child.ptr)
    shreduler_child(s, &out->child);
  vector_rem2(&s->vm->shred, (vtype)out);
}

ANN void shreduler_remove(const Shreduler s, const VM_Shred out, const m_bool erase) {
  if(s->curr == out)
    s->curr = NULL;
  else if(out == s->list)
    s->list = out->next;
  if(out->prev)
    out->prev->next = out->next;
  if(out->next)
    out->next->prev = out->prev;
  out->prev = out->next = NULL;
  if(erase) {
    shreduler_erase(s, out);
    free_vm_shred(out);
  }
}

ANN void shredule(const Shreduler s, const VM_Shred shred, const m_float wake_time) {
  const m_float time = wake_time + (m_float)s->vm->bbq->pos;
  shred->wake_time = time;
  if(s->list) {
    VM_Shred curr = s->list, prev = NULL;
    do {
      if(curr->wake_time > time)
        break;
      prev = curr;
    } while((curr = curr->next));
    if(!prev) {
      shred->next = s->list;
      s->list = (s->list->prev = shred);
    } else {
      if((shred->next = prev->next))
        prev->next->prev = shred;
      shred->prev = prev;
      prev->next = shred;
    }
  } else
    s->list = shred;
  if(s->curr == shred)
    s->curr = NULL;
}
