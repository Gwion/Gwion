#include <stdlib.h>
#include "map.h"
#include "frame.h"

Frame* new_Frame()
{
  Frame* frame = calloc(1, sizeof(Frame));
  frame->stack = new_Vector();
  return frame;
}

void free_Frame(Frame* a)
{
  vtype i;
  for(i = 0; i < vector_size(a->stack); i++)
    free((Local*)vector_at(a->stack, i));
  free_Vector(a->stack);
  free(a);
}

Local* frame_alloc_local(Frame* frame, m_uint size, m_str name, m_bool is_ref, m_bool is_obj)
{
  Local* local = calloc(1, sizeof(Local));
  local->size = size;
  local->offset = frame->curr_offset;
  local->is_ref = is_ref;
  local->is_obj = is_obj;
  frame->curr_offset += local->size;
  local->name = name;
  vector_append(frame->stack, (vtype)local);
  return local;
}

void frame_push_scope(Frame* frame)
{
  vector_append(frame->stack, (vtype)NULL);
}

void frame_pop_scope(Frame* frame, Vector v)
{
  m_uint i;
  Local* local = (Local*)vector_back(frame->stack);

  while((i = vector_size(frame->stack) && vector_back(frame->stack))) {
    local = (Local*)vector_back(frame->stack);
    vector_pop(frame->stack);
    if(local) {
      frame->curr_offset -= local->size;
      vector_append(v, (vtype)local);
    }
  }
  vector_pop(frame->stack);
}
