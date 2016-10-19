typedef struct containing_driver_info
{
	m_uint in, out;
	m_uint chan;
	unsigned int sr;
	m_uint bufsize;
	unsigned int bufnum;
	m_str card;
	m_bool raw;
} DriverInfo;


typedef struct driver_wrapper
{
	m_bool (*ini)(VM* vm, DriverInfo* info);
	void   (*run)(VM* vm, DriverInfo* info);
	void   (*del)(VM* vm);
} Driver;

static inline void no_wakeup(){};
void free_Driver(Driver* driver, VM* vm);

Driver* sndfile_driver(VM* vm);
Driver* raw_driver(VM* vm);
Driver* dummy_driver(VM* vm);
Driver* silent_driver(VM* vm);
#ifdef HAVE_ALSA
Driver* alsa_driver(VM* vm);
#endif
#ifdef HAVE_JACK
Driver* jack_driver(VM* vm);
#endif
#ifdef HAVE_SOUNDIO
Driver* sio_driver(VM* vm);
#endif
#ifdef HAVE_PORTAUDIO
Driver* pa_driver(VM* vm);
#endif
