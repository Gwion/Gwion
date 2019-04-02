#ifndef __SOUNDINFO
#define __SOUNDINFO
struct SoundInfo_ {
  uint32_t sr;
  uint8_t in, out;
  m_str arg;
};
struct SoundInfo_ *new_soundinfo(MemPool p);
struct SoundInfo_ *soundinfo_cpy(MemPool p, const struct SoundInfo_ *src);
#endif
