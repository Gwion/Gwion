typedef struct Code_ {
  m_str name;
  m_str filename;
  m_uint stack_depth;
  Vector code;
  Vector stack_cont, stack_break, stack_return;
  Frame* frame;
  m_bool need_this;
} Code;
