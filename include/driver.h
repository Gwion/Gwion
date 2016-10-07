#include <soundio/soundio.h>
m_bool sio_ini(VM* vm, enum SoundIoBackend backend, char *device_id, bool raw, char *stream_name, int* 
sample_rate);
void sio_run();
void sio_del();
BBQ new_BBQ();
