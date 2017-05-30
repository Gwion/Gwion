typedef struct Code_ {
  m_str name;
  m_str filename;
  m_uint stack_depth;
  m_bool need_this;
  Vector code;
  Vector stack_cont, stack_break, stack_return;
  Frame* frame;
} Code;
