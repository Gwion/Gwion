#include <portaudio.h>
#include "err_msg.h"
#include "vm.h"
#include "oo.h"
#include "type.h"
#include "ugen.h"
#include "driver.h"

#define SAMPLE_RATE (48000)
#define FRAMES_PER_BUFFER  (64)

static m_uint bufsize;
static PaStream *stream = NULL;
static     PaStreamParameters outputParameters;
static     PaStreamParameters  inputParameters;

static int callback(const void *inputBuffer, void *outputBuffer,
                    unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
                    PaStreamCallbackFlags statusFlags, void *userData) {
  VM* vm = (VM*)userData;
  sp_data* sp = vm->sp;
  float *in  = (float*)inputBuffer;
  float *out = (float*)outputBuffer;
  m_bool i, j;
  for(i = 0; i < framesPerBuffer; i++) {
    for(j = 0; j < vm->n_in; j++)
      vm->in[j] = *in++;
    vm_run(vm);
    for(j = 0; j < sp->nchan; j++)
      *out++ = sp->out[j];
    sp->pos++;
  }
  GWION_CTL
  return paContinue;
}

static m_bool ini(VM* vm, DriverInfo* di) {
  if(Pa_Initialize() != paNoError)
    return -1;
  outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
  if(outputParameters.device == paNoDevice) {
    fprintf(stderr, "Error: No default output device.\n");
    goto error;
  }
  bufsize = di->bufsize;
  outputParameters.channelCount = 2;
  outputParameters.sampleFormat = di->format;
  outputParameters.suggestedLatency = Pa_GetDeviceInfo(outputParameters.device)->defaultLowOutputLatency;
  outputParameters.hostApiSpecificStreamInfo = NULL;

  inputParameters.device = Pa_GetDefaultInputDevice(); /* default output device */
  if(inputParameters.device == paNoDevice) {
    fprintf(stderr, "Error: No default input device.\n");
    goto error;
  }
  inputParameters.channelCount = 2;
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowOutputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  if(Pa_OpenStream(
        &stream,
        &inputParameters,
        &outputParameters,
        di->sr,
        di->bufsize,
        paClipOff,
        callback,
        vm) != paNoError)
    goto error;
  /*
  if(Pa_OpenDefaultStream(&stream,
                                  2,2,
                                  paFloat32,
                                  SAMPLE_RATE,
                                  paFramesPerBufferUnspecified,
  //                                256,
                                  callback,
                                  vm ) != paNoError)
  exit(3);
  */
//		return -1;
  //return -1;


//Pa_SetStreamFinishedCallback( stream, NULL);
//Pa_SetStreamFinishedCallback( stream, &StreamFinished );
  return 1;

error:
  Pa_Terminate();
  return -1;
}

static void del(VM* vm) {
  Pa_StopStream(stream);
  Pa_Terminate();
}

static void run(VM* vm, DriverInfo* di) {
  Pa_StartStream(stream);
  while(vm->is_running)
    Pa_Sleep(1);
}

void pa_driver(Driver* d, VM* vm) {
  d->ini = ini;
  d->run = run;
  d->del = del;
  vm->wakeup = no_wakeup;
}
