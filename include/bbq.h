#ifdef USE_DOUBLE
#undef USE_DOUBLE
#endif
#ifndef SOUNDPIPE_H
#include <soundpipe.h>
#endif

struct BBQ_ {
  unsigned int n_in;
  SPFLOAT* in;
  sp_data* sp; // holds sample rate and out
};
