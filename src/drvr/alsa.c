#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include "defs.h"
#include "err_msg.h"
#include "vm.h"
#include "oo.h"
#include "type.h"
#include "ugen.h"
#include "driver.h"

#ifndef SP_ALSA_ACCESS
#define SP_ALSA_ACCESS SND_PCM_ACCESS_RW_INTERLEAVED
#endif

#ifdef USE_DOUBLE
#define ALSA_FORMAT SND_PCM_FORMAT_FLOAT64
#else
#define ALSA_FORMAT SND_PCM_FORMAT_FLOAT
#endif

struct AlsaNi {
  void** bufi;
  void* buf;
};
struct AlsaInfo {
  snd_pcm_t* handle;
  snd_pcm_t *in,       *out;
  SPFLOAT  **in_buf,  **out_buf;
  void     **_in_buf, **_out_buf;
  void *in_bufi, *out_bufi;
};

static int sp_alsa_init(DriverInfo* di, const char* device,
    int stream, int mode) {
  struct AlsaInfo* info = (struct AlsaInfo*)di->data;
  static snd_pcm_t* handle;
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
  else return -1;

  snd_pcm_uframes_t size = di->bufsize;
  if(snd_pcm_hw_params_set_period_size_near(handle, params, &size, &dir))
    return -1;
  di->bufsize = size;
  if(snd_pcm_hw_params_set_periods_near(handle, params, &num, &dir))
    return -1;
  di->bufnum = num;
  if(snd_pcm_hw_params(handle, params))
    return -1;

  snd_pcm_hw_params_get_rate_max(params, &di->sr, &dir);
  snd_pcm_hw_params_set_rate_near(handle, params, &di->sr, &dir);
  info->handle = handle;
  return 1;
}

static void alsa_run_init(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*)di->data;
  snd_pcm_hwsync(info->out);
  snd_pcm_hwsync(info->in);
  snd_pcm_start(info->out);
  snd_pcm_start(info->in);
}

static m_bool alsa_ini(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = calloc(1, sizeof(struct AlsaInfo));
  di->data = info;
  if(sp_alsa_init(di, di->card, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
    err_msg(ALSA_, 0, "problem with playback");
    return -1;
  }
  info->out = info->handle;
  di->out = di->chan;
  if(sp_alsa_init(di, di->card, SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC |
      SND_PCM_NONBLOCK) < 0) {
    err_msg(ALSA_, 0, "problem with capture");
    return -1;
  }
  info->in = info->handle;
  di->in = di->chan;
  return 1;
}

static void alsa_run_init_non_interleaved(sp_data* sp, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  m_uint i;

  info->in_buf  = calloc(sp->nchan, sizeof(SPFLOAT*));
  info->out_buf = calloc(sp->nchan, sizeof(SPFLOAT*));
  info->_out_buf   = calloc(sp->nchan, sizeof(void*));
  info->_in_buf    = calloc(sp->nchan, sizeof(void*));

  for(i = 0; i < sp->nchan; i++) {
    info->out_buf[i]  = calloc(di->bufsize, sizeof(SPFLOAT));
    info->_out_buf[i] = info->out_buf[i];
    info->in_buf[i]   = calloc(di->bufsize, sizeof(SPFLOAT));
    info->_in_buf[i]  = info->in_buf[i];
  }
}

static void alsa_run_non_interleaved(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  sp_data* sp = vm->sp;
  m_uint i, chan;
  while(vm->is_running) {
    snd_pcm_readn(info->in, info->_in_buf, di->bufsize);
    LOOP_OPTIM
    for(i = 0; i < di->bufsize; i++) {
      for(chan = 0; chan < sp->nchan; chan++)
        vm->in[chan] = ((m_float**)(info->_in_buf))[chan][i];
      di->run(vm);
      LOOP_OPTIM
      for(chan = 0; chan < sp->nchan; chan++)
        info->out_buf[chan][i] = sp->out[chan];
      sp->pos++;
    }
    if(snd_pcm_writen(info->out, info->_out_buf, di->bufsize) < 0)
      snd_pcm_prepare(info->out);
  }
}

static void alsa_run_interleaved(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  sp_data* sp = vm->sp;
  while(vm->is_running) {
    m_uint i, chan;
    m_int j = 0, k = 0;
    snd_pcm_readi(info->in, info->in_bufi, di->bufsize);
    LOOP_OPTIM
    for(i = 0; i < di->bufsize; i++) {
      LOOP_OPTIM
      for(chan = 0; chan < sp->nchan; chan++)
        vm->in[chan] = ((m_float*)(info->in_bufi))[j++];
      di->run(vm);
      LOOP_OPTIM
      for(chan = 0; chan < sp->nchan; chan++)
        ((m_float*)info->out_bufi)[k++] = sp->out[chan];
      sp->pos++;
    }
    if(snd_pcm_writei(info->out, info->out_bufi, di->bufsize) < 0)
      snd_pcm_prepare(info->out);
  }
}

static void alsa_run(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  sp_data* sp = vm->sp;
  alsa_run_init(vm, di);
  if(SP_ALSA_ACCESS == SND_PCM_ACCESS_RW_NONINTERLEAVED) {
    alsa_run_init_non_interleaved(sp, di);
    alsa_run_non_interleaved(vm, di);
  } else {
    info->in_bufi  = calloc(sp->nchan * di->bufsize, sizeof(SPFLOAT));
    info->out_bufi = calloc(sp->nchan * di->bufsize, sizeof(SPFLOAT));
    alsa_run_interleaved(vm, di);
  }
}

static void alsa_del_non_interleaved(VM* vm, struct AlsaInfo* info) {
  m_uint chan;
  if(info->in_buf && info->out_buf) {
    for(chan = 0; chan < vm->sp->nchan; chan++) {
      free(info->in_buf[chan]);
      free(info->out_buf[chan]);
    }
    free(info->in_buf);
    free(info->out_buf);
  }
  if(info->_in_buf)
    free(info->_in_buf);
  if(info->_out_buf)
    free(info->_out_buf);
}

static void alsa_del(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  if(info->in)
    snd_pcm_close(info->in);
  if(info->out)
    snd_pcm_close(info->out);
  snd_config_update_free_global();
  if(SP_ALSA_ACCESS == SND_PCM_ACCESS_RW_NONINTERLEAVED)
    alsa_del_non_interleaved(vm, info);
  else {
    if(info->in_bufi)
      free(info->in_bufi);
    if(info->out_bufi)
      free(info->out_bufi);
  }
  free(info);
}

void alsa_driver(Driver* d) {
  d->ini = alsa_ini;
  d->run = alsa_run;
  d->del = alsa_del;
}

