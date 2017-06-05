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
  struct driver_wrapper* (*func)();
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

Driver* dummy_driver(VM* vm);
#ifdef HAVE_SPA
Driver* spa_driver(VM* vm);
#endif
#ifdef HAVE_SNDFILE
Driver* sndfile_driver(VM* vm);
#endif
Driver* silent_driver(VM* vm);
#ifdef HAVE_ALSA
Driver* alsa_driver(VM* vm);
#include <alsa/asoundlib.h>
#endif
#ifdef HAVE_JACK
Driver* jack_driver(VM* vm);
#endif
#ifdef HAVE_SOUNDIO
#include <soundio/soundio.h>
Driver* sio_driver(VM* vm);
#endif
#ifdef HAVE_PORTAUDIO
Driver* pa_driver(VM* vm);
#include <portaudio.h>
#endif
BBQ new_bbq(VM* vm, DriverInfo* di, Driver** d);
void udp_do(VM* vm);
#define GWION_CTL_SIZE 8192
#define GWION_CTL if(!(sp->pos%GWION_CTL_SIZE))udp_do(vm);
