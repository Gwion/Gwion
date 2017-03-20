#ifdef USE_DOUBLE
#undef USE_DOUBLE
#endif
#ifndef SOUNDPIPE_H
#include <soundpipe.h>
#endif
struct BBQ_ {
  m_uint buf_size, buf_num;
  unsigned int n_in;
  SPFLOAT* in;
  sp_data* sp; // holds sample rate and out
  void (*cb)(VM*  vm);
};
