#include "err_msg.h"
#include "vm.h"
#include "bbq.h"
#include "oo.h"
#include "type.h"
#include "dl.h"
#include "ugen.h"
#include "driver.h"

#include <portaudio.h>
#define SAMPLE_RATE 48000

static PaStream *stream = NULL;
static     PaStreamParameters outputParameters;
static int callback( const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags, void *userData )
{
	VM* vm = (VM*)userData;
	m_float *in  = (m_float*)inputBuffer;
	m_float *out = (m_float*)outputBuffer;
  m_bool i, j;
	for(i = 0; i < framesPerBuffer; i += vm->bbq->n_out)
	{
		for(j = 0; j < vm->bbq->n_in; j++)
			vm->bbq->in[j] = *in++;
		vm_run(vm);
		for(j = 0; j < vm->bbq->n_in; j++)
			*out++ = vm->bbq->sp->out[j];
	}
	return paContinue;
}

static m_bool ini(VM* vm, DriverInfo* di)
{
	if(Pa_Initialize() != paNoError)
exit(2);
//		return -1;
    outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
      fprintf(stderr,"Error: No default output device.\n");
      goto error;
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

if(Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              SAMPLE_RATE,
              //FRAMES_PER_BUFFER,
              paFramesPerBufferUnspecified,
							paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              callback,
              vm ))
exit(3);;
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
	printf("start\n");
	di->out =2;
	di->in =2;
	vm->bbq->n_in = 2;
	vm->bbq->n_out = 2;
vm->bbq->in = malloc(sizeof(m_float) * 2);
	return 1;
	error: Pa_Terminate();
}

static void del(VM* vm, int finish)
{
	if(finish)
	{
		Pa_StopStream(stream);
		Pa_Terminate();
	}
}

static void run(VM* vm, DriverInfo* di)
{
printf("ll\n");
	if(Pa_StartStream(stream) != paNoError)
exit(4);
printf("ll\n");
	Pa_Sleep(48000*50);
printf("ll\n");
sleep(12);
}

Driver* pa_driver()
{
  Driver* d = malloc(sizeof(Driver));
  d->ini = ini;
  d->run = run;
  d->del = del;
}
