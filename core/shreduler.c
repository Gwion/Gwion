#include <stdlib.h>
#include "err_msg.h"
#include "shreduler.h"
#ifdef USE_DOUBLE
#undef USE_DOUBLE
#endif
#include "soundpipe.h"
#include "bbq.h"
#include "map.h"
#include "oo.h"
#include "array.h"

struct  Shreduler_ {
  m_int til_next;
  VM* vm;
  VM_Shred list;
  VM_Shred curr;
  m_uint n_shred;
  m_bool loop;
};

Shreduler new_Shreduler(VM* vm)
{
  Shreduler s = (Shreduler)malloc(sizeof(struct Shreduler_));
  s->til_next = 0;
  s->list = NULL;
  s->vm = vm;
  s->n_shred = 0;
  return s;
}

void free_Shreduler(Shreduler s)
{
  free(s);
}

m_float get_now(Shreduler s)
{
  return s->vm->bbq->sp->pos;
}

void shreduler_set_loop(Shreduler s, m_bool loop)
{
  s->loop = loop;
}

VM_Shred shreduler_get(Shreduler s)
{
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "get");
#endif
  VM_Shred shred = s->list;
  if(!shred) {
    s->til_next = -1;
    if(!vector_size(s->vm->shred) && ! s->loop) {
      s->vm->is_running = 0;
      s->vm->wakeup();
    }
    return NULL;
  }
  if(shred->wake_time <= (get_now(s) + .5)) {
    s->list = shred->next;
    shred->next = NULL;
    shred->prev = NULL;
    if(s->list) {
      s->list->prev = NULL;
      s->til_next = s->list->wake_time - get_now(s);
      if(s->til_next < 0)
        s->til_next = 0;
    }
    shred->is_running = 1;
    return shred;
  }
  return NULL;
}

m_bool shreduler_remove(Shreduler s, VM_Shred out, m_bool erase)
{
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "%s %i", erase ? "erase" : "wait", out ? out->xid : -1);
#endif
  if(!out)
    return -1;
  m_uint i, size = out->child ? vector_size(out->child) : 0;
  if(erase) {
    vtype index;
    if(out->parent) {
      index = vector_find(out->parent->child, (vtype)out);
      vector_remove(out->parent->child, index);
      if(!vector_size(out->parent->child)) {
        free(out->parent->child);
        out->parent->child = NULL;
      }
    }
    if(out->child) {
      for(i = 0; i < size; i++) {
        printf("fiund child!\n");
        VM_Shred child = (VM_Shred)vector_front(out->child);
        child->prev = NULL;
        child->next = NULL;
        if(child == s->list) // 09/03/17
          s->list = NULL;
        shreduler_remove(s, child, 1);
      }
    }
    index = vector_find(s->vm->shred, (vtype)out);
    vector_remove(s->vm->shred, index);
  }
  out->is_running = 0;

  if(!out->prev && !out->next && out != s->list) {
//    release(out->me, out);
    if(!out->wait && !out->child)
      free_VM_Shred(out);
    return -1;
  }
  if(!out->prev)
    s->list = out->next;
  else
    out->prev->next = out->next;
  if(out->next)
    out->next->prev = out->prev;
  out->next = out->prev = NULL;
  if(!erase) {
    out->next = NULL;
    out->prev = NULL;
  }
  return 1;
}

m_bool shredule(Shreduler s, VM_Shred shred, m_float wake_time)
{
#ifdef DEBUG_SHREDULER
  debug_msg("clock", "shredule shred[%i] at %f", shred->xid, wake_time);
#endif
  m_float diff;
  VM_Shred curr, prev;
  if(shred->prev || shred->next) {
    err_msg(VM_, 0, "[chuck](VM): internal sanity check failed in shredule()");
    err_msg(VM_, 0, "[chuck](VM): (shred shreduled while shreduled)");
  }

  if(wake_time < get_now(s) + .5) {
    err_msg(VM_, 0,  "[chuck](VM): internal sanity check failed in shredule()");
    err_msg(VM_, 0,  "[chuck](VM): (wake time is past) - %f : %f for shred %i", wake_time, get_now(s), shred->xid);
    /*    wake_time = get_now(s) + .5; // hack*/
  }
  // assign id if new shred
  if(shred->xid == -1) {
    shred->xid = s->n_shred;
    s->n_shred++;
  }

  shred->wake_time = wake_time;
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
  diff = s->list->wake_time - get_now(s);
  if(diff < 0)
    diff = 0;
  s->til_next = diff;
  return 1;
}
