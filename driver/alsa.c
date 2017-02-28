#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include "err_msg.h"
#include "vm.h"
#include "bbq.h"
#include "oo.h"
#include "type.h"
#include "dl.h"
#include "ugen.h"
#include "driver.h"
#include "alsa.h"

#ifdef USE_DOUBLE
#define ALSA_FORMAT SND_PCM_FORMAT_FLOAT64
#else
#define ALSA_FORMAT SND_PCM_FORMAT_FLOAT
#endif

static snd_pcm_t *out = NULL, *in = NULL;
static SPFLOAT  **in_buf = NULL,  **out_buf = NULL;
static void     **_in_buf = NULL, **_out_buf = NULL;

void* in_bufi;
void* out_bufi;

static int sp_alsa_init(DriverInfo* di, snd_pcm_t** h, const char* device, int stream, int mode)
{
  snd_pcm_t* handle = NULL;
  snd_pcm_hw_params_t* params;
  unsigned int num = di->bufnum;
  int dir = 0;

  if(snd_pcm_open(&handle, device, stream, mode) > 0 || !handle)
    return -1;

  snd_pcm_hw_params_alloca(&params);

  if(snd_pcm_hw_params_any(handle, params) > 0) {
    snd_pcm_close(handle);
    return -1;
  }

  if(!snd_pcm_hw_params_test_access(handle, params, SP_ALSA_ACCESS))
    snd_pcm_hw_params_set_access(handle, params, SP_ALSA_ACCESS);

  if(!snd_pcm_hw_params_test_format(handle, params, ALSA_FORMAT))
    snd_pcm_hw_params_set_format(handle, params, ALSA_FORMAT);

  if(!snd_pcm_hw_params_test_rate(handle, params, di->sr, dir))
    snd_pcm_hw_params_set_rate_near(handle, params, &di->sr, &dir);

  if(!snd_pcm_hw_params_test_channels(handle, params, di->chan))
    snd_pcm_hw_params_set_channels(handle, params, di->chan);
  else return -1;;

  if(snd_pcm_hw_params_set_period_size_near(handle, params, &di->bufsize, &dir))
    return -1;

  if(snd_pcm_hw_params_set_periods_near(handle, params, &num, &dir))
    return -1;
  di->bufnum = num;
  if(snd_pcm_hw_params(handle, params))
    return -1;

  snd_pcm_hw_params_get_rate_max(params, &di->sr, &dir);
  snd_pcm_hw_params_set_rate_near(handle, params, &di->sr, &dir);

  *h = handle;
  return 1;
}


static m_bool alsa_ini(VM* vm, DriverInfo* di)
{
  out = NULL;
  if(sp_alsa_init(di, &out, di->card, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
    err_msg(ALSA_, 0, "problem with playback");
    return -1;
  }
  di->out = di->chan;
  in = NULL;
  if(sp_alsa_init(di, &in,  di->card, SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC | SND_PCM_NONBLOCK) < 0) {
    err_msg(ALSA_, 0, "problem with capture");
    return -1;
  }
  di->in = di->chan;
  return 1;
}

static void alsa_run(VM* vm, DriverInfo* di)
{
  m_uint i, chan;
  BBQ bbq = vm->bbq;
  sp_data* sp = vm->bbq->sp;

  snd_pcm_get_params(out, &di->bufsize, &di->bufnum);
  di->bufsize /= di->bufnum;

  if(SP_ALSA_ACCESS == SND_PCM_ACCESS_RW_NONINTERLEAVED) {
    in_buf  = calloc(sp->nchan, sizeof(SPFLOAT*));
    out_buf = calloc(sp->nchan, sizeof(SPFLOAT*));
    _out_buf   = calloc(sp->nchan, sizeof(void*));
    _in_buf    = calloc(sp->nchan, sizeof(void*));

    for(chan = 0; chan < sp->nchan; chan++) {
      out_buf[chan]  = calloc(di->bufsize, sizeof(SPFLOAT));
      _out_buf[chan] = out_buf[chan];
      in_buf[chan]   = calloc(di->bufsize, sizeof(SPFLOAT));
      _in_buf[chan]  = in_buf[chan];
    }

    snd_pcm_hwsync(out);
    snd_pcm_hwsync(in);
    snd_pcm_start(out);
    snd_pcm_start(in);
    while(vm->is_running) {
      snd_pcm_readn(in, _in_buf, di->bufsize);
      for(i = 0; i < di->bufsize; i++) {
        for(chan = 0; chan < sp->nchan; chan++)
          bbq->in[chan] = ((m_float**)(_in_buf))[chan][i];
        vm_run(vm);
        for(chan = 0; chan < sp->nchan; chan++)
          out_buf[chan][i] = sp->out[chan];
        sp->pos++;
      }
      snd_pcm_writen(out, _out_buf, di->bufsize);
    }
  } else { // interleaved
    in_bufi  = calloc(sp->nchan * di->bufsize, sizeof(SPFLOAT));
    out_bufi = calloc(sp->nchan * di->bufsize, sizeof(SPFLOAT));
    snd_pcm_hwsync(out);
    snd_pcm_hwsync(in);
    snd_pcm_start(out);
    snd_pcm_start(in);
    while(vm->is_running) {
      int j = 0;
      int k = 0;
      sp->nchan = 2;
      snd_pcm_readi(in, in_bufi, di->bufsize);
      for(i = 0; i < di->bufsize; i++) {
        for(chan = 0; chan < sp->nchan; chan++) {
          bbq->in[chan] = ((m_float*)(in_bufi))[j];
          j++;
        }
        vm_run(vm);
        for(chan = 0; chan < sp->nchan; chan++) {
          ((m_float*)out_bufi)[k] = sp->out[chan];
          k++;
        }
        sp->pos++;
      }
      snd_pcm_writei(out, out_bufi, di->bufsize);
    }
  }
}

static void alsa_del(VM* vm)
{
  m_uint chan;
  snd_pcm_close(in);
//  snd_pcm_hw_free(in);
  snd_pcm_close(out);
//  snd_pcm_hw_free(out);
  snd_config_update_free_global();
//snd_dlclose(out);
//snd_dlclose(in);
  if(SP_ALSA_ACCESS == SND_PCM_ACCESS_RW_NONINTERLEAVED) {
    if(in_buf && out_buf) {
      for(chan = 0; chan < vm->bbq->sp->nchan; chan++) {
        free(in_buf[chan]);
        free(out_buf[chan]);
      }
      free(in_buf);
      free(out_buf);
    }
    if(_in_buf)
      free(_in_buf);
    if(_out_buf)
      free(_out_buf);
  } else { /* interleaved */
    if(in_bufi)
      free(in_bufi);
    if(out_bufi)
      free(out_bufi);
  }
}

Driver* alsa_driver(VM* vm)
{
  Driver* d = malloc(sizeof(Driver));
  d->ini = alsa_ini;
  d->run = alsa_run;
  d->del = alsa_del;
  vm->wakeup = no_wakeup;
  return d;
}

