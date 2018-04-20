#include <stdlib.h>
#include "vm.h"
#include "object.h"
#include "shreduler_private.h"

ANN void shreduler_set_loop(const Shreduler s, const m_bool loop) {
  s->loop = loop < 0 ? 0 : 1;
}

ANN VM_Shred shreduler_get(const Shreduler s) {
  const VM_Shred shred = s->list;
  if(!shred) {
    if(!vector_size(&s->vm->shred) && !s->loop)
      s->vm->is_running = 0;
    return NULL;
  }
  if(shred->wake_time <= (s->vm->sp->pos + .5)) {
    s->list = shred->next;
    shred->next = shred->prev = NULL;
    if(s->list)
      s->list->prev = NULL;
    s->curr = shred;
    return shred;
  }
  return NULL;
}

ANN static void shreduler_parent(const VM_Shred out, const Vector v) {
  const m_uint index = vector_find(v, (vtype)out);
  vector_rem(v, index);
  if(!vector_size(v)) {
    vector_release(v);
    out->parent->child.ptr = NULL;
  }
}

ANN static void shreduler_child(const Shreduler s, const Vector v) {
  m_uint size = vector_size(v) + 1;
  while(--size) {
    const VM_Shred child = (VM_Shred)vector_at(v, size - 1);
    if(child == s->list)
      s->list = child->next;
    if(child->prev)
      child->prev->next = child->next;
    if(child->next)
      child->next->prev = child->prev;
    child->prev = child->next = NULL;
    shreduler_remove(s, child, 1);
  }
}

ANN static void shreduler_gc(VM_Shred out) {
  m_uint size = vector_size(&out->gc) + 1;
  while(--size) {
    const M_Object o = (M_Object)vector_at(&out->gc, size - 1);
    if(o)
      release(o, out);
  }
  vector_release(&out->gc);
}

ANN static void shreduler_erase(const Shreduler s, const VM_Shred out) {
  if(out->parent)
    shreduler_parent(out, &out->parent->child);
  if(out->child.ptr)
    shreduler_child(s, &out->child);
  const vtype index = vector_find(&s->vm->shred, (vtype)out);
  vector_rem(&s->vm->shred, index);
  if(out->gc.ptr)
    shreduler_gc(out);
}

ANN void shreduler_remove(const Shreduler s, const VM_Shred out, const m_bool erase) {
  if(s->curr == out)
    s->curr = NULL;
  else if(s->list == out)
    s->list = NULL;
  if(erase) {
    shreduler_erase(s, out);
    free_vm_shred(out);
  }
}

ANN void shredule(const Shreduler s, const VM_Shred shred, const m_float wake_time) {
  const m_float time = wake_time + s->vm->sp->pos;
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
      s->list->prev = shred;
      s->list = shred;
    } else {
      shred->next = prev->next;
      shred->prev = prev;
      if(prev->next)
        prev->next->prev = shred;
      prev->next = shred;
    }
  } else
    s->list = shred;
  if(s->curr == shred)
    s->curr = NULL;
}
