#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include "defs.h"
#include "vm.h"
#include "bbq.h"

#define SP_ALSA_ACCESS SND_PCM_ACCESS_RW_INTERLEAVED

extern m_bool ssp_is_running;
static snd_pcm_t *out,*in;
static m_bool _init(unsigned int* sr, m_uint* chan, const char* device,
	unsigned int bufnum, m_uint bufsize, snd_pcm_t** h, int stream, int mode)
{
  snd_pcm_t* handle;
  snd_pcm_hw_params_t* params;
  int dir = 0;

  if(snd_pcm_open(&handle, device, stream, mode) > 0)
    return -1;

  snd_pcm_hw_params_alloca(&params);
  if(snd_pcm_hw_params_any(handle, params) > 0)
  {
    snd_pcm_close(handle);
    return -1;
  }

  if(!snd_pcm_hw_params_test_access(handle, params, SP_ALSA_ACCESS))
    snd_pcm_hw_params_set_access(handle, params, SP_ALSA_ACCESS);
	else return -1;

  if(!snd_pcm_hw_params_test_format(handle, params, SND_PCM_FORMAT_FLOAT64))
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_FLOAT64);
	else return -1;

  if(!snd_pcm_hw_params_test_channels(handle, params, *chan))
    snd_pcm_hw_params_set_channels(handle, params, *chan);
	else return -1;

	if(snd_pcm_hw_params_set_period_size_near(handle, params, &bufsize, &dir))
		return -1;

  if(snd_pcm_hw_params_set_periods_near(handle, params, &bufnum, &dir))
    return -1;

  snd_pcm_hw_params_get_rate_max(params, sr, &dir);
	snd_pcm_hw_params_set_rate_near(handle, params, sr, &dir);
	 *h = handle;
  return 1;
}


m_bool alsa_ini(VM* vm, m_uint *sr, m_uint nin, m_uint nout,
  m_uint bufnum, m_uint bufsize, m_str id)
{
	unsigned int _sr = *sr;
	if(_init(&_sr, &nout, id, bufnum, bufsize, &out, SND_PCM_STREAM_PLAYBACK, 0) < 0)
		return -1;
	if(_init(&_sr, &nin,  id, bufnum, bufsize, &in, SND_PCM_STREAM_CAPTURE,
			SND_PCM_ASYNC|SND_PCM_NONBLOCK) < 0)
		return -1;
	*sr = _sr;
	return 1;
}
