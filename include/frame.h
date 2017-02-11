#ifndef FRAME_H
#define FRAME_H
#include "defs.h"
typedef struct {
  m_str name;
  m_uint size;
  m_bool is_ref;
  m_bool is_obj;
  m_uint offset;
} Local;
Local* new_Local();
void free_Local(Local* local);

typedef struct {
  m_str name;
  m_uint curr_offset;
  m_uint num_access;
  Vector stack;
} Frame;

Frame* new_Frame();
void   free_Frame(Frame* frame);
Local* frame_alloc_local(Frame* frame, m_uint size, m_str name, m_bool is_ref, m_bool is_obj);
void   frame_push_scope(Frame* frame);
//void   frame_pop_scope(Frame* frame);
void   frame_pop_scope(Frame* frame, Vector v);
//void   frame_get_scope(Frame* frame, Vector out);
m_uint frame_next(Frame* frame);
#endif
