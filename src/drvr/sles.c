#include <stdio.h>
#include <string.h>
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>
#include "vm.h"
#include "driver.h"

#define BUFSIZE 1024
#define CHECK_SL(a) if((a) != SL_RESULT_SUCCESS) return -1;

typedef struct {
  long l, r;
} frame_t;

static frame_t out_buf[BUFSIZE];
static SLObjectItf object;
static SLEngineItf engine;
static SLObjectItf player;

static m_bool sles_ini(VM* vm, DriverInfo* di) {
  // create engine

  CHECK_SL(slCreateEngine(&object, 0, NULL, 0, NULL, NULL))
  CHECK_SL((*object)->Realize(object, SL_BOOLEAN_FALSE))

  CHECK_SL((*object)->GetInterface(object, SL_IID_ENGINE, &engine))

  // create output mix
  SLObjectItf outputmixObject;
  CHECK_SL((*engine)->CreateOutputMix(engine, &outputmixObject, 0, NULL, NULL))
  CHECK_SL((*outputmixObject)->Realize(outputmixObject, SL_BOOLEAN_FALSE))

  // create audio player
  SLDataSource audiosrc;
  SLDataSink audiosnk;
  SLDataFormat_PCM pcm;
  SLDataLocator_OutputMix locator_outputmix;
  SLDataLocator_BufferQueue locator_bufferqueue;
  locator_bufferqueue.locatorType = SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE;
  locator_bufferqueue.numBuffers = BUFSIZE;
  locator_outputmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
  locator_outputmix.outputMix = outputmixObject;
  pcm.formatType = SL_DATAFORMAT_PCM;
  pcm.numChannels = 2;
  pcm.samplesPerSec = SL_SAMPLINGRATE_48;
  pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_32;
  pcm.containerSize = 32;
  pcm.channelMask = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
  pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;
  audiosrc.pLocator = &locator_bufferqueue;
  audiosrc.pFormat = &pcm;
  audiosnk.pLocator = &locator_outputmix;
  audiosnk.pFormat = NULL;
  SLObjectItf playerObject;
  SLInterfaceID ids[1] = {SL_IID_ANDROIDSIMPLEBUFFERQUEUE};
  SLboolean flags[1] = {SL_BOOLEAN_TRUE};
  CHECK_SL((*engine)->CreateAudioPlayer(engine, &playerObject,
    &audiosrc, &audiosnk, 1, ids, flags))
  CHECK_SL((*playerObject)->Realize(playerObject, SL_BOOLEAN_FALSE))
  SLPlayItf playerPlay;
  CHECK_SL((*playerObject)->GetInterface(playerObject, SL_IID_PLAY, &playerPlay))
  SLAndroidSimpleBufferQueueItf playerBufferqueue;
  CHECK_SL((*playerObject)->GetInterface(playerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE, &playerBufferqueue))
  CHECK_SL((*playerPlay)->SetPlayState(playerPlay, SL_PLAYSTATE_PLAYING))
  return 1;
}

static void sles_run(VM* vm, DriverInfo* di) {
  while(vm->is_running) {
    for(m_uint i = 0; i < BUFSIZE; i++) {
      di->run(vm);
      out_buf[i].l = vm->sp->out[0];
      out_buf[i].r = vm->sp->out[1];
    }
  }
}

static void sles_del() {
  if(player)
    (*player)->Destroy(player);
//  if(engine)
//    (*engine)->Destroy(engine);
  if(object)
    (*object)->Destroy(object);
}

void sles_driver(Driver* d) {
  d->ini = sles_ini;
  d->run = sles_run;
  d->del = sles_del;
}

