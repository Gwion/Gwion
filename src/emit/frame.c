#include <string.h>
#include "defs.h"
#include "map.h"
#include "mpool.h"
#include "code.h"

typedef struct Frame_ {
  m_uint curr_offset;
  struct Vector_ stack;
} Frame;

typedef struct Local_ {
  m_uint size;
  m_uint offset;
  m_bool is_obj;
} Local;

ANEW static Frame* new_frame() {
  Frame* frame = mp_alloc(Frame);
  vector_init(&frame->stack);
  vector_add(&frame->stack, (vtype)NULL);
  return frame;
}

ANN static void free_frame(Frame* a) {
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

ANN m_uint frame_offset(const Frame* frame) {
  return frame->curr_offset;
}

ANEW Code* new_code(const m_str file, const m_str name) {
  Code* code = mp_alloc(Code);
  code->name = code_name_set(name, file);
  code->stack_depth = 0;
  vector_init(&code->instr);
  vector_init(&code->stack_break);
  vector_init(&code->stack_cont);
  vector_init(&code->stack_return);
  code->frame = new_frame();
  return code;
}

ANN void free_code(Code* code) {
  vector_release(&code->instr);
  vector_release(&code->stack_break);
  vector_release(&code->stack_cont);
  vector_release(&code->stack_return);
  free_frame(code->frame);
  free(code->name);
  mp_free(Code, code);
}

void code_push(const Code* code) {
  frame_push(code->frame);
}

m_int code_pop(const Code* code) {
  return frame_pop(code->frame);
}

ANN m_uint code_alloc(const Code* code, const m_uint size, const m_bool is_obj) {
  return frame_alloc(code->frame, size, is_obj);
}

ANN m_uint code_offset(const Code* code) {
  return frame_offset(code->frame);
}

m_str code_name_set(const m_str name, const m_str file) {
  const size_t len = strlen(name) + strlen(file) + 2;
  const m_str str = (const m_str)xcalloc(1, len);
  sprintf(str, "%s$%s", name, file);
  return str;
}

m_str code_name(const m_str name, const m_bool b) {
  const m_str str = strchr(name, '$');
  return (b && str) ?  (str + 1) : name;
}

