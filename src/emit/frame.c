//#include <stdlib.h>
//#include <string.h>
#include "defs.h"
//#include "err_msg.h"
//#include "defs.h"
#include "map.h"
//#include "absyn.h"
//#include "oo.h"
//#include "vm.h"
//#include "env.h"
//#include "type.h"
//#include "nspc.h"
//#include "value.h"
//#include "instr.h"
//#include "emit.h"
//#include "object.h"
//#include "array.h"
//#include "func.h"
//#include "traverse.h"
#include "mpool.h"
//#include "escape.h"
//#include "parse.h"
//#include "memoize.h"
//#include "operator.h"

typedef struct Frame_ {
  m_uint curr_offset;
  struct Vector_ stack;
} Frame;

typedef struct Local_ {
  m_uint size;
  m_uint offset;
  m_bool is_obj;
} Local;

ANEW Frame* new_frame() {
  Frame* frame = mp_alloc(Frame);
  vector_init(&frame->stack);
  vector_add(&frame->stack, (vtype)NULL);
  return frame;
}

ANN void free_frame(Frame* a) {
  LOOP_OPTIM
  for(vtype i = vector_size(&a->stack) + 1; --i;)
    if(vector_at(&a->stack, i - 1))
      mp_free(Local, (Local*)vector_at(&a->stack, i - 1));
  vector_release(&a->stack);
  mp_free(Frame, a);
}

ANN m_uint frame_alloc(Frame* frame, const m_uint size, const m_bool is_obj) {
  Local* local = mp_alloc(Local);
  local->size = size;
  local->offset = frame->curr_offset;
  local->is_obj = is_obj;
  frame->curr_offset += (local->size = size);
  vector_add(&frame->stack, (vtype)local);
  return local->offset;
}

ANN void frame_push(Frame* frame) {
  vector_add(&frame->stack, (vtype)NULL);
}

ANN m_int frame_pop(Frame* frame) {
  const Local* l = (Local*)vector_pop(&frame->stack);
  if(!l)
    return -1;
  frame->curr_offset -= l->size;
  return l->is_obj ? (m_int)l->offset : frame_pop(frame);
}

