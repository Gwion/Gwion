#ifndef __CODE
#define __CODE

typedef struct Code_ {
  m_str  name;
  m_uint stack_depth;
  struct Vector_ instr;
  struct Vector_ stack_cont, stack_break, stack_return;
  struct Frame_* frame;
  ae_flag flag;
} Code;

ANEW Code* new_code(const m_str, const m_str);
ANN void free_code(Code*);
void code_push(const Code*);
m_int code_pop(const Code*);
m_uint code_offset(const Code*);
ANN m_str code_name_set(const m_str, const m_str);
ANN m_str code_name(const m_str, const m_bool);
ANN m_uint code_alloc(const Code*, const m_uint, const m_bool);
#endif
