#include <stdlib.h>
#include "defs.h"
#include "vm.h"
#include "bbq.h"

extern m_bool ssp_is_running;


m_bool jack_ini(VM* vm, m_uint i, m_uint o, m_uint* sr);
m_bool sio_ini(VM* vm, m_uint , char *device_id, m_bool raw, char *stream_name, m_uint* sr);
m_bool raw_ini(VM* vm, m_str id);
m_bool sndfile_ini(VM* v, m_str id, m_uint bufsize, m_uint* nout, m_uint* sr);
m_bool alsa_ini(VM* vm, m_uint *sr, m_uint* nin, m_uint *nout,
	m_uint bufnum, m_uint bufsize, m_str id);
enum a_driver
{
	ALSA_D, JACK_D, SOUNDIO_D, FILE_D, SILENT_D, DUMMY_D
};

BBQ new_BBQ(VM* vm, m_uint sr, m_uint format, m_uint nin, m_uint nout, m_uint bufsize, m_uint bufnum,
  m_str id, m_str name, m_bool raw, enum a_driver driver, m_uint backend)
{
	BBQ a = malloc(sizeof(struct BBQ_));
	int ret;
	switch(driver)
	{
#ifdef HAVE_SOUNDIO
		case SOUNDIO_D:
			ret = sio_ini(vm, backend, id, raw, name, &sr);
			break;
#endif
#ifdef HAVE_JACK
		case JACK_D:
			ret = jack_ini(vm, &nin, &nout, &sr);
			break;
#endif
#ifdef HAVE_ALSA
		case ALSA_D:
			ret = alsa_ini(vm, &sr, &nin, &nout, bufnum, bufsize, id);
#endif
		case FILE_D:
			if(raw)
				ret = raw_ini(vm, id);
			else
				ret = sndfile_ini(vm, id, bufsize, &nout, &sr);
			break;
	}
	if(ret < 0)
	{
		free(a);
		return NULL;
	}
	a->n_in = nin;
	sp_createn(&vm->bbq->sp, nout);
	vm->bbq->sp->sr = sr;
	ssp_is_running = 1;
	return a;
}
