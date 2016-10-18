#include <soundio/soundio.h>
//m_bool sio_ini(VM* vm, enum SoundIoBackend backend, char *device_id, m_bool raw,
m_bool sio_ini(VM* vm, m_uint backend, char *device_id, m_bool raw,
	char *stream_name, m_uint* sample_rate);
void sio_run();
void sio_del();
