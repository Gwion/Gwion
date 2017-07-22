typedef struct Code_ {
  m_str name;
  m_str filename;
  m_uint stack_depth;
  struct Vector_ code;
  struct Vector_ stack_cont, stack_break, stack_return;
  Frame* frame;
  m_bool need_this;
} Code;
