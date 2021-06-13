#ifndef __VARARG
#define __VARARG
struct Vararg_ {
  struct Vector_      t; // types
  m_bit *             d; // d(ata)
  m_uint /*o, i, s,*/ l; // o(ffset), i(ndex), s(ize), l(en)
  m_uint              pc;
};
ANN void emit_vararg_end(const Emitter emit, const m_uint pc);
void     free_vararg(MemPool p, struct Vararg_ *arg);
#endif
