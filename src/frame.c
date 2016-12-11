#include <stdlib.h>
#include "map.h"
#include "frame.h"
Local* new_Local()
{
  Local* local  = calloc(1, sizeof(Local));
  local->size   = 0;
  local->is_ref = 0;
  local->is_obj = 0;
  local->offset = 0;
  return local;
}

void free_Local(Local* local)
{
  free(local);
}

Frame* new_Frame()
{
  Frame* frame = calloc(1, sizeof(Frame));
  frame->curr_offset = 0;
  frame->num_access  = 0;
  frame->stack = new_Vector();
  return frame;
}

void free_Frame(Frame* frame)
{
  free_Vector(frame->stack);
  free(frame);
}

Local* frame_alloc_local(Frame* frame, m_uint size, m_str name, m_bool is_ref, m_bool is_obj)
{
  Local* local = new_Local();
  local->size = size;
  /*  local->size = 1;*/
  local->offset = frame->curr_offset;
  local->is_ref = is_ref;
  local->is_obj = is_obj;
  frame->curr_offset += local->size;
  local->name = name;
  vector_append(frame->stack, local);
  return local;
}

void frame_push_scope(Frame* frame)
{
  vector_append(frame->stack, NULL);
}

/*
void frame_get_scope(Frame* frame, Vector out)
{
  Local* local = NULL;
  m_uint index = vector_size(frame->stack) - 1;
  while(index >=0 && vector_at(frame->stack, index))
  {
    local = vector_at(frame->stack, index);
    index--;
    if(local)
      vector_append(out, local);
  }
}
*/

/*
void frame_pop_scope(Frame* frame)
{
  m_uint i;
  Local* local = NULL;
  while(i = vector_size(frame->stack) && vector_back(frame->stack))
  {
    local = vector_at(frame->stack, i - 1);
    vector_pop(frame->stack);
    if(local)
    {
      frame->curr_offset -= local->size;
      vector_append(frame->stack, local);
    }
  }
  vector_remove(frame->stack, 0);
}
*/

void frame_pop_scope(Frame* frame, Vector v)
{
  m_uint i;
  Local* local = vector_back(frame->stack);

  while((i = vector_size(frame->stack) && vector_back(frame->stack))) {
    local = vector_back(frame->stack);
    vector_pop(frame->stack);
    if(local) {
      frame->curr_offset -= local->size;
      vector_append(v, local);
    }
  }
  vector_pop(frame->stack);
}
m_uint frame_next(Frame* frame)
{
  return vector_size(frame->stack);
}
