#ifndef __VARARG
#define __VARARG
struct Vararg_ {
  m_uint* k;      // sizes
  m_bit* d;  // d(ata)
  m_uint o, i, s; // o(ffset), i(ndex), s(ize)
};

#endif
