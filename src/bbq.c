#include "defs.h"
#include "vm.h"
#include "bbq.h"

#include <stdlib.h> // malloc

BBQ new_BBQ(m_uint n_chan, m_uint sr)
{
	BBQ a = malloc(sizeof(struct BBQ_));
	sp_createn(&a->sp, n_chan);
	a->in = calloc(n_chan, sizeof(m_float));
	a->sp->sr =sr;
	return a;
}
