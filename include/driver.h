typedef struct
{
  m_uint in;
  m_uint out;
  unsigned int sr;
  unsigned int buf_num;
  m_uint  buf_size;
  m_uint  chan;
} DriverInfo;


typedef struct
{
	m_bool (*ini)(VM* vm, DriverInfo* di);
	void (*run)(VM* vm, DriverInfo* di);
	void (*del)(VM* vm, int finish);
} Driver;

void free_Driver(Driver* d, VM* vm);
#ifdef HAVE_ALSA
Driver* alsa_driver();
#endif
Driver* dummy_driver();
Driver* silent_driver();
Driver* pa_driver();
Driver* soundio_driver();
