#include <gwion_util.h>
#include <soundinfo.h>

struct SoundInfo_ *new_soundinfo(void) {
  struct SoundInfo_ *si = mp_alloc(SoundInfo);
  si->in = si->out = 2;
  si->sr = 48000;
  return si;
}

struct SoundInfo_ *soundinfo_cpy(const struct SoundInfo_ *src) {
  struct SoundInfo_ *si = mp_alloc(SoundInfo);
  si->in  = src->in;
  si->out = src->out;
  si->sr  = src->sr;
  return si;
}
