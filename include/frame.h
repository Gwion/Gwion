#include "map_private.h"

typedef struct {
  m_uint size;
  m_uint offset;
  m_bool is_ref;
  m_bool is_obj;
} Local;

typedef struct {
  m_uint curr_offset;
  struct Vector_ stack;
} Frame;

Frame* new_frame();
void   free_frame(Frame* frame);

Local* frame_alloc_local(Frame* frame, m_uint size, m_bool is_ref, m_bool is_obj);
void   frame_push_scope(Frame* frame);
void   frame_pop_scope(Frame* frame, Vector v);
