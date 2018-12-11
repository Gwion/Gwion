#include <alsa/asoundlib.h>
#include <alsa/mixer.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "driver.h"

#ifndef SP_ALSA_ACCESS
#define SP_ALSA_ACCESS SND_PCM_ACCESS_RW_INTERLEAVED
#endif

#ifdef USE_DOUBLE
#define ALSA_FORMAT SND_PCM_FORMAT_FLOAT64
#else
#define ALSA_FORMAT SND_PCM_FORMAT_FLOAT
#endif
/*
struct AlsaNi {
  void** buf;
  void* bufi;
};
*/
struct AlsaInfo {
  snd_pcm_t* handle;
  snd_pcm_t *pcm_in,       *pcm_out;
  SPFLOAT  **in_buf,  **out_buf;
  void     **_in_buf, **_out_buf;
  void *in_bufi, *out_bufi;
};

static int sp_alsa_init(DriverInfo* di, const char* device,
    int stream, int mode) {
  struct AlsaInfo* info = (struct AlsaInfo*)di->data;
  static snd_pcm_t* handle;
  snd_pcm_hw_params_t* params;
  uint num = (uint)di->bufnum;
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

  if(!snd_pcm_hw_params_test_channels(handle, params, (uint)di->chan))
    snd_pcm_hw_params_set_channels(handle, params, (uint)di->chan);
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

static void alsa_run_init(VM* vm __attribute__((unused)), DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*)di->data;
  snd_pcm_hwsync(info->pcm_out);
  snd_pcm_start(info->pcm_out);
  snd_pcm_hwsync(info->pcm_in);
  snd_pcm_start(info->pcm_in);
}

static m_bool alsa_ini(VM* vm __attribute__((unused)), DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*)xcalloc(1, sizeof(struct AlsaInfo));
  di->data = info;
  if(sp_alsa_init(di, di->card, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
    err_msg(0, "problem with playback");
    return -1;
  }
  info->pcm_out = info->handle;
  di->out = di->chan;
  if(sp_alsa_init(di, di->card, SND_PCM_STREAM_CAPTURE, SND_PCM_ASYNC |
      SND_PCM_NONBLOCK) < 0) {
    err_msg(0, "problem with capture");
    return -1;
  }
  info->pcm_in = info->handle;
  di->in = di->chan;
  return 1;
}

static void alsa_run_init_non_interleaved(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  info->in_buf  = (m_float**)xcalloc(vm->bbq->nchan, SZ_FLOAT);
  info->out_buf = (m_float**)xcalloc(vm->bbq->nchan, SZ_FLOAT);
  info->_out_buf   = (void**)xcalloc(vm->bbq->nchan, SZ_INT);
  info->_in_buf    = (void**)xcalloc(vm->bbq->nchan, SZ_INT);
  for(m_uint i = 0; i < (m_uint)vm->bbq->nchan; i++) {
    info->out_buf[i]  = (m_float*)xcalloc(di->bufsize, SZ_FLOAT);
    info->_out_buf[i] = info->out_buf[i];
    info->in_buf[i]   = (m_float*)xcalloc(di->bufsize, SZ_FLOAT);
    info->_in_buf[i]  = info->in_buf[i];
  }
}

static void alsa_run_non_interleaved(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  while(vm->is_running) {
    snd_pcm_readn(info->pcm_in, info->_in_buf, di->bufsize);
    LOOP_OPTIM
    for(m_uint i = 0; i < di->bufsize; i++) {
      for(m_uint chan = 0; chan < (m_uint)vm->bbq->nchan; chan++)
        vm->bbq->in[chan] = ((m_float**)(info->_in_buf))[chan][i];
      di->run(vm);
      LOOP_OPTIM
      for(m_uint chan = 0; chan < (m_uint)vm->bbq->nchan; chan++)
        info->out_buf[chan][i] = vm->bbq->out[chan];
      ++vm->bbq->pos;
    }
    if(snd_pcm_writen(info->pcm_out, info->_out_buf, di->bufsize) < 0)
      snd_pcm_prepare(info->pcm_out);
  }
}

static void alsa_run_interleaved(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  while(vm->is_running) {
    m_int j = 0, k = 0;
    snd_pcm_readi(info->pcm_in, info->in_bufi, di->bufsize);
    LOOP_OPTIM
    for(m_uint i = 0; i < di->bufsize; i++) {
      LOOP_OPTIM
      for(m_uint chan = 0; chan < (m_uint)vm->bbq->nchan; chan++)
        vm->bbq->in[chan] = ((m_float*)(info->in_bufi))[j++];
      di->run(vm);
      LOOP_OPTIM
      for(m_uint chan = 0; chan < (m_uint)vm->bbq->nchan; chan++)
        ((m_float*)info->out_bufi)[k++] = vm->bbq->out[chan];
      ++vm->bbq->pos;
    }
    if(snd_pcm_writei(info->pcm_out, info->out_bufi, di->bufsize) < 0)
      snd_pcm_prepare(info->pcm_out);
  }
}

static void alsa_run(VM* vm, DriverInfo* di) {
  struct AlsaInfo* info = (struct AlsaInfo*) di->data;
  alsa_run_init(vm, di);
  if(SP_ALSA_ACCESS == SND_PCM_ACCESS_RW_NONINTERLEAVED) {
    alsa_run_init_non_interleaved(vm, di);
    alsa_run_non_interleaved(vm, di);
  } else {
    info->in_bufi  = (void*)xcalloc(vm->bbq->nchan * di->bufsize, SZ_FLOAT);
    info->out_bufi = (void*)xcalloc(vm->bbq->nchan * di->bufsize, SZ_FLOAT);
    alsa_run_interleaved(vm, di);
  }
}

static void alsa_del_non_interleaved(VM* vm, struct AlsaInfo* info) {
  if(info->in_buf && info->out_buf) {
    for(m_uint chan = 0; chan < (m_uint)vm->bbq->nchan; chan++) {
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
  if(info->pcm_in)
    snd_pcm_close(info->pcm_in);
  if(info->pcm_out)
    snd_pcm_close(info->pcm_out);
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

