#include <stdarg.h>
typedef struct containing_driver_info {
  m_uint in, out;
  m_uint chan;
  unsigned int sr;
  m_uint bufsize;
//	unsigned int bufnum;
  m_uint bufnum;
  m_str card;
  m_bool raw;
  m_uint backend;
  m_uint format;
  struct driver_wrapper* (*func)();
} DriverInfo;


typedef struct driver_wrapper {
  m_bool (*ini)(VM* vm, DriverInfo* info);
  void   (*run)(VM* vm, DriverInfo* info);
  void   (*del)(VM* vm);
} Driver;

static inline void no_wakeup() {}
void free_Driver(Driver* driver, VM* vm);
void select_driver(DriverInfo* di, const m_str d);
void select_backend(DriverInfo* di, const m_str d);
void select_format(DriverInfo* di, const m_str d);

Driver* dummy_driver(VM* vm);
#ifdef HAVE_SPA
Driver* raw_driver(VM* vm);
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
BBQ new_BBQ(VM* vm, DriverInfo* di, Driver** d);
