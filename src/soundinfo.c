#include <gwion_util.h>
#include <soundinfo.h>

#ifndef GWION_DEFAULT_NIN
#define GWION_DEFAULT_NIN 2
#endif

#ifndef GWION_DEFAULT_NOUT
#define GWION_DEFAULT_NOUT 2
#endif

#ifndef GWION_DEFAULT_SAMPLERATE
#define GWION_DEFAULT_SAMPLERATE 48000
#endif

struct SoundInfo_ *new_soundinfo(MemPool p) {
  struct SoundInfo_ *si = mp_calloc(p, SoundInfo);
  si->in  = GWION_DEFAULT_NIN;
  si->out = GWION_DEFAULT_NOUT;
  si->sr  = GWION_DEFAULT_SAMPLERATE;
  return si;
}

struct SoundInfo_ *soundinfo_cpy(MemPool p, const struct SoundInfo_ *src) {
  struct SoundInfo_ *si = mp_calloc(p, SoundInfo);
  si->in                = src->in;
  si->out               = src->out;
  si->sr                = src->sr;
  return si;
}
