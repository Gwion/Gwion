#ifndef SOUNDPIPE_H
#include <soundpipe.h>
#endif

struct BBQ_
{
//	m_uint buf_size, buf_num;
	unsigned int n_in, n_out;
	SPFLOAT* in;
	sp_data* sp;
	void (*cb)(VM*  vm);
};
