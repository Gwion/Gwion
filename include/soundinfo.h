#ifndef __SOUNDINFO
#define __SOUNDINFO
struct SoundInfo_ {
  m_str    arg;
  uint32_t sr;
  uint8_t  in, out;
};
struct SoundInfo_ *new_soundinfo(MemPool p);
struct SoundInfo_ *soundinfo_cpy(MemPool p, const struct SoundInfo_ *src);
#endif
