#include <stdlib.h>
#include "vm.h"
#include "object.h"
Shreduler new_shreduler(VM* vm) {
  Shreduler s = (Shreduler)malloc(sizeof(struct Shreduler_));
  s->curr = s->list = NULL;
  s->vm = vm;
  s->n_shred = 1;
  return s;
}

void free_shreduler(Shreduler s) {
  free(s);
}

void shreduler_set_loop(Shreduler s, m_bool loop) {
  s->loop = loop;
}

VM_Shred shreduler_get(Shreduler s) {
  VM_Shred shred = s->list;
  if(!shred) {
    if(!vector_size(&s->vm->shred) && !s->loop) {
      s->vm->is_running = 0;
      s->vm->wakeup();
    }
    return NULL;
  }
  if(shred->wake_time <= (s->vm->sp->pos + .5)) {
    s->list = shred->next;
    shred->next = NULL;
    shred->prev = NULL;
    if(s->list)
      s->list->prev = NULL;
    s->curr = shred;
    return shred;
  }
  return NULL;
}

static void shreduler_parent(VM_Shred out, Vector v) {
  m_uint index = vector_find(v, (vtype)out);
  vector_rem(v, index);
  if(!vector_size(v)) {
    vector_release(v);
    out->parent->child.ptr = NULL;
  }
}

static void shreduler_child(Shreduler s, Vector v) {
  m_uint i, size = vector_size(v);
  for(i = 0; i < size; i++) {
    VM_Shred child = (VM_Shred)vector_front(v);
    child->prev = child->next = NULL;
    child->wait = NULL;
    if(child == s->list) // 09/03/17
      s->list = NULL;
    else if(s->list && (child == s->list->next)) // 09/03/17
      s->list->next = child->next;
    shreduler_remove(s, child, 1);
  }
}

static void shreduler_gc(VM_Shred out) {
  m_uint i;
  for(i = 0; i < vector_size(&out->gc); i++) {
    M_Object o = (M_Object)vector_at(&out->gc, i);
    if(o)
      release(o, out);
  }
  vector_release(&out->gc);
}

static void shreduler_erase(Shreduler s, VM_Shred out) {
  vtype index;
  if(out->parent)
    shreduler_parent(out, &out->parent->child);
  if(out->child.ptr)
    shreduler_child(s, &out->child);
  index = vector_find(&s->vm->shred, (vtype)out);
  vector_rem(&s->vm->shred, index);
  if(out->gc.ptr)
    shreduler_gc(out);
}

static m_bool shreduler_free_shred(Shreduler s, VM_Shred out, m_bool erase) {
  if(!out->prev && !out->next && out != s->list) {
    if(erase && !out->wait && !out->child.ptr)
      free_vm_shred(out);
    return - 1;
  }
  return 1;
}

void shreduler_remove(Shreduler s, VM_Shred out, m_bool erase) {
  if(erase)
    shreduler_erase(s, out);
  s->curr = (s->curr == out) ? NULL : s->curr;
  if(shreduler_free_shred(s, out, erase) < 0)
    return;
  out->prev ? (out->prev->next = out->next) : (s->list = out->next);
  if(out->next)
    out->next->prev = out->prev;
  out->next = out->prev = NULL;
  return;
}

m_bool shredule(Shreduler s, VM_Shred shred, m_float wake_time) {
  VM_Shred curr, prev;

  shred->wake_time = (wake_time += s->vm->sp->pos);
  if(!s->list)
    s->list = shred;
  else {
    curr = s->list;
    prev = NULL;
    while(curr) {
      if(curr->wake_time > wake_time)
        break;
      prev = curr;
      curr = curr->next;
    }
    if(!prev) {
      shred->next = s->list;
      if(s->list)
        s->list->prev = shred;
      s->list = shred;
    } else {
      shred->next = prev->next;
      shred->prev = prev;
      if(prev->next)
        prev->next->prev = shred;
      prev->next = shred;
    }
  }
  if(s->curr == shred)
    s->curr = NULL;
  return 1;
}
