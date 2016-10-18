#include "vm.h"
#include "bbq.h"
#include "/home/djay/src/git/Soundpipe/lib/spa/spa.h"
extern m_bool ssp_is_running;
static sp_audio spa;
m_bool raw_ini(VM* vm, m_str id)
{
	if(spa_open(vm->bbq->sp, &spa, vm->bbq->sp->filename, SPA_WRITE) == SP_NOT_OK)
	{
		fprintf(stderr, "Error: could not open file %s.\n", id);
		return -1;
	}
	return 1;
}

void raw_run(VM* vm)
{
	while(ssp_is_running)
	{
		vm_run(vm);
		spa_write_buf(vm->bbq->sp, &spa, vm->bbq->sp->out, vm->bbq->sp->nchan);
		vm->bbq->sp->pos++;
	}
}

void raw_del()
{
	spa_close(&spa);
}
