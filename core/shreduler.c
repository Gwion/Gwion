#include <stdlib.h>
#include <string.h>
#include "err_msg.h"
#include "shreduler.h"
#ifdef USE_DOUBLE
#undef USE_DOUBLE
#endif
#include "soundpipe.h"
#include "bbq.h"
#include "map.h"
#include "oo.h"
#include "object.h"
#include "array.h"

Shreduler new_shreduler(VM* vm) {
  Shreduler s = (Shreduler)malloc(sizeof(struct Shreduler_));
  s->curr = s->list = NULL;
  s->vm = vm;
  s->n_shred = 0;
  return s;
}

void free_shreduler(Shreduler s) {
  free(s);
}

void shreduler_set_loop(Shreduler s, m_bool loop) {
  s->loop = loop;
}

VM_Shred shreduler_get(Shreduler s) {
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "get");
#endif
  VM_Shred shred = s->list;
  if(!shred) {
    if(!vector_size(&s->vm->shred) && !s->loop) {
      s->vm->is_running = 0;
      s->vm->wakeup();
    }
    return NULL;
  }
  if(shred->wake_time <= (s->vm->bbq->sp->pos + .5)) {
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

void shreduler_remove(Shreduler s, VM_Shred out, m_bool erase) {
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "%s %i", erase ? "erase" : "wait", out ? out->xid : -1);
#endif
  m_uint i, size = out->child.ptr ? vector_size(&out->child) : 0;
  if(erase) {
    vtype index;
    if(out->parent) {
      index = vector_find(&out->parent->child, (vtype)out);
      vector_rem(&out->parent->child, index);
      if(!vector_size(&out->parent->child)) {
        vector_release(&out->parent->child);
        out->parent->child.ptr = NULL;
      }
    }
    if(out->child.ptr) {
      for(i = 0; i < size; i++) {
        VM_Shred child = (VM_Shred)vector_front(&out->child);
        child->prev = NULL;
        child->next = NULL;
        if(child == s->list) // 09/03/17
          s->list = NULL;
        shreduler_remove(s, child, 1);
      }
    }
    index = vector_find(&s->vm->shred, (vtype)out);
    vector_rem(&s->vm->shred, index);
// GC
    if(out->gc.ptr) {
      for(i = 0; i < vector_size(&out->gc); i++) {
        M_Object o = (M_Object)vector_at(&out->gc, i);
        if(o)
          release(o, out);
      }
      vector_release(&out->gc);
    }
  }
  s->curr = (s->curr == out) ? s->curr : NULL;

  if(!out->prev && !out->next && out != s->list) {
//    if(!out->wait && !out->child && erase)
    if(erase && !out->wait && !out->child.ptr && !strstr(out->code->name, "class ")) // if fails in ctor. creates leak
      free_vm_shred(out);
    return;
  }
  out->prev ? (out->prev->next = out->next) : (s->list = out->next);
  if(out->next)
    out->next->prev = out->prev;
  out->next = out->prev = NULL;
  return;
}

m_bool shredule(Shreduler s, VM_Shred shred, m_float wake_time) {
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "shredule shred[%i] at %f", shred->xid, wake_time);
#endif
  VM_Shred curr, prev;

  shred->wake_time = (wake_time += s->vm->bbq->sp->pos);
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
