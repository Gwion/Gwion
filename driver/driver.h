typedef struct containing_driver_info
{
	m_uint in, out;
	m_uint chan;
	unsigned int sr;
	m_uint bufsize;
	unsigned int bufnum;
	m_str card;
} DriverInfo;


typedef struct driver_wrapper
{
	m_bool (*ini)(VM* vm, DriverInfo* info);
	void   (*run)(VM* vm, DriverInfo* info);
	void   (*del)(VM* vm);
} Driver;

#include <soundio/soundio.h>
//m_bool sio_ini(VM* vm, enum SoundIoBackend backend, char *device_id, m_bool raw,
//m_bool sio_ini(VM* vm, m_uint backend, char *device_id, m_bool raw,
//	char *stream_name, m_uint* sample_rate);
void sio_run();
void sio_del();
