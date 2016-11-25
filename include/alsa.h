//#ifndef __SSP_ALSA
//#define __SSP_ALSA

//#define SP_ALSA_ACCESS SND_PCM_ACCESS_RW_NONINTERLEAVED
#define SP_ALSA_ACCESS SND_PCM_ACCESS_RW_INTERLEAVED

#include "defs.h"
void* alsa_process(void * arg);
extern m_float* ssp_in;
//#endif
