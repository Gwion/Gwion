#include <stdarg.h>
typedef struct containing_driver_info {
  m_uint in, out;
  m_uint chan;
  unsigned int sr;
  m_uint bufsize;
  m_uint bufnum;
  m_str card;
  m_uint backend;
  m_uint format;
  void (*func)();
  void (*run)(VM*);
  m_bool raw;
} DriverInfo;


typedef struct driver_wrapper {
  m_bool(*ini)(VM* vm, DriverInfo* info);
  void (*run)(VM* vm, DriverInfo* info);
  void (*del)(VM* vm);
} Driver;

static inline void no_wakeup() {}
void free_driver(Driver* driver, VM* vm);
void select_driver(DriverInfo* di, const m_str d);
void select_backend(DriverInfo* di, const m_str d);
void select_format(DriverInfo* di, const m_str d);

void dummy_driver(Driver* d, VM* vm);
#ifdef HAVE_SPA
void spa_driver(Driver* d, VM* vm);
#endif
#ifdef HAVE_SNDFILE
void sndfile_driver(Driver* d, VM* vm);
#endif
void silent_driver(Driver* d, VM* vm);
#ifdef HAVE_ALSA
void alsa_driver(Driver* d, VM* vm);
#include <alsa/asoundlib.h>
#endif
#ifdef HAVE_JACK
void jack_driver(Driver* d, VM* vm);
#endif
#ifdef HAVE_SOUNDIO
#include <soundio/soundio.h>
void sio_driver(Driver* d, VM* vm);
#endif
#ifdef HAVE_PORTAUDIO
void pa_driver(Driver* d, VM* vm);
#include <portaudio.h>
#endif
#ifdef HAVE_PULSE
void pulse_driver(Driver* d, VM* vm);
#endif
m_bool init_bbq(VM* vm, DriverInfo* di, Driver* d);
void udp_do(VM* vm);
#define GWION_CTL_SIZE 8192
#define GWION_CTL if(!(sp->pos%GWION_CTL_SIZE))udp_do(vm);
