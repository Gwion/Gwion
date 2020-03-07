#ifndef __VARARG
#define __VARARG
struct Vararg_ {
  m_uint* k;      // sizes
  Type* t;      // types
  m_bit* d;  // d(ata)
  m_uint o, i, s; // o(ffset), i(ndex), s(ize)
};
void free_vararg(MemPool p, struct Vararg_* arg);
#endif
