#include <portaudio.h>
#include "defs.h"
#include "err_msg.h"
#include "vm.h"
#include "oo.h"
#include "type.h"
#include "ugen.h"
#include "driver.h"

#define SAMPLE_RATE (48000)
#define FRAMES_PER_BUFFER  (64)

struct PaInfo {
  PaStream *stream;
  PaStreamParameters outputParameters;
  PaStreamParameters  inputParameters;
};

static int callback(const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo __attribute__((unused)),
                    PaStreamCallbackFlags statusFlags __attribute__((unused)), void *userData) {
  VM* vm = (VM*)userData;
  float *in  = (float*)inputBuffer;
  float *out = (float*)outputBuffer;
  m_uint i, j;
  for(i = 0; i < framesPerBuffer; i++) {
    for(j = 0; j < vm->n_in; j++)
      vm->bbq->in[j] = *in++;
//    di->run(vm);
    vm_run(vm);
    for(j = 0; j < (m_uint)vm->bbq->nchan; j++)
      *out++ = vm->bbq->out[j];
    ++vm->bbq->pos;
  }
  return paContinue;
}

static m_bool ini(VM* vm, DriverInfo* di) {
  struct PaInfo* info = (struct PaInfo*)xmalloc(sizeof(struct PaInfo*));
  di->data = info;
  if(Pa_Initialize() != paNoError)
    return -1;
  info->outputParameters.device = Pa_GetDefaultOutputDevice();
  if(info->outputParameters.device == paNoDevice) {
    gw_err("Error: No default output device.\n");
    goto error;
  }
  info->outputParameters.channelCount = 2;
  info->outputParameters.sampleFormat = di->format;
  info->outputParameters.suggestedLatency = Pa_GetDeviceInfo(info->outputParameters.device)->defaultLowOutputLatency;
  info->outputParameters.hostApiSpecificStreamInfo = NULL;

  info->inputParameters.device = Pa_GetDefaultInputDevice(); /* default output device */
  if(info->inputParameters.device == paNoDevice) {
    gw_err("Error: No default input device.\n");
    goto error;
  }
  info->inputParameters.channelCount = 2;
  info->inputParameters.sampleFormat = paFloat32;
  info->inputParameters.suggestedLatency = Pa_GetDeviceInfo(info->inputParameters.device)->defaultLowOutputLatency;
  info->inputParameters.hostApiSpecificStreamInfo = NULL;

  if(Pa_OpenStream(
        &info->stream,
        &info->inputParameters,
        &info->outputParameters,
        di->sr,
        di->bufsize,
        paClipOff,
        callback,
        vm) != paNoError)
    goto error;
  return 1;
error:
  Pa_Terminate();
  return -1;
}

static void del(VM* vm __attribute__((unused)), DriverInfo* di) {
  struct PaInfo* info = (struct PaInfo*)di->data;
  Pa_StopStream(info->stream);
  Pa_Terminate();
}

static void run(VM* vm, DriverInfo* di) {
  struct PaInfo* info = (struct PaInfo*)di->data;
  Pa_StartStream(info->stream);
  while(vm->is_running)
    Pa_Sleep(1);
}

void pa_driver(Driver* d) {
  d->ini = ini;
  d->run = run;
  d->del = del;
}
