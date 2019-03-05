#ifndef __DRIVER
#define __DRIVER
typedef struct driver_wrapper* Driver_;
typedef struct containing_driver_info {
  uint8_t in, out;
  uint32_t   sr;
  m_str arg;
  void (*func)(Driver_);
  void (*run)(const VM*);
  void* data;
} DriverInfo;


typedef struct driver_wrapper {
  m_bool (*ini)(VM* vm, DriverInfo* info);
  void   (*run)(VM* vm, DriverInfo* info);
  void   (*del)(VM* vm, DriverInfo* info);
} Driver;

extern void select_driver(DriverInfo* di, const m_str d);
extern void select_backend(DriverInfo* di, const m_str d);
extern void select_format(DriverInfo* di, const m_str d);

void dummy_driver(Driver* d);
#ifdef HAVE_SPA
void spa_driver(Driver* d);
#endif
#ifdef HAVE_SNDFILE
void sndfile_driver(Driver* d);
#endif
void silent_driver(Driver* d);
#ifdef HAVE_ALSA
void alsa_driver(Driver* d);
#include <alsa/asoundlib.h>
#endif
#ifdef HAVE_JACK
void jack_driver(Driver* d);
#endif
#ifdef HAVE_SOUNDIO
#include <soundio/soundio.h>
void sio_driver(Driver* d);
#endif
#ifdef HAVE_PORTAUDIO
void pa_driver(Driver* d);
#include <portaudio.h>
#endif
#ifdef HAVE_PULSE
void pulse_driver(Driver* d);
#endif
#ifdef HAVE_PLOT
void plot_driver(Driver* d);
#endif
#ifdef HAVE_SLES
void sles_driver(Driver* d);
#endif
ANN struct BBQ_* new_bbq(DriverInfo* di);
#endif
