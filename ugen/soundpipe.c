#include "vm.h"
#include "type.h"
#include "dl.h"
#include "err_msg.h"
#include "import.h"
#include "ugen.h"
#include "bbq.h"
#include "array.h"
#include "func.h"
#include "lang.h"
m_uint o_ftbl_data;
#define FTBL(o) *((sp_ftbl**)((M_Object)o)->d.data + o_ftbl_data)
#define CHECK_SIZE(size)	if(size <= 0){fprintf(stderr, "'gen_ftbl' size argument must be more than 0");return;}

DTOR(ftbl_dtor)
{
	if(FTBL(o))
		sp_ftbl_destroy(&FTBL(o));
}

typedef struct
{
	sp_data* sp;
	sp_adsr* osc;
} GW_adsr;

TICK(adsr_tick)
{
	GW_adsr* ug = (GW_adsr*)u->ug;
	base_tick(u);
	sp_adsr_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(adsr_ctor)
{
	GW_adsr* ug = malloc(sizeof(GW_adsr));
	ug->sp = shred->vm_ref->bbq->sp;
sp_adsr_create(&ug->osc);
sp_adsr_init(ug->sp, ug->osc);
	o->ugen->tick = adsr_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(adsr_dtor)
{
	GW_adsr* ug = o->ugen->ug;
	sp_adsr_destroy(&ug->osc);
	free(ug);
}

MFUN(adsr_get_atk)
{
	GW_adsr* ug = (GW_adsr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->atk;
}

MFUN(adsr_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_adsr* ug = (GW_adsr*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->atk = atk);
}

MFUN(adsr_get_dec)
{
	GW_adsr* ug = (GW_adsr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dec;
}

MFUN(adsr_set_dec)
{
	m_uint gw_offset = SZ_INT;
	GW_adsr* ug = (GW_adsr*)o->ugen->ug;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dec = dec);
}

MFUN(adsr_get_sus)
{
	GW_adsr* ug = (GW_adsr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->sus;
}

MFUN(adsr_set_sus)
{
	m_uint gw_offset = SZ_INT;
	GW_adsr* ug = (GW_adsr*)o->ugen->ug;
	m_float sus = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->sus = sus);
}

MFUN(adsr_get_rel)
{
	GW_adsr* ug = (GW_adsr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->rel;
}

MFUN(adsr_set_rel)
{
	m_uint gw_offset = SZ_INT;
	GW_adsr* ug = (GW_adsr*)o->ugen->ug;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->rel = rel);
}

typedef struct
{
	sp_data* sp;
	sp_allpass* osc;
	m_bool is_init;
} GW_allpass;

TICK(allpass_tick)
{
	GW_allpass* ug = (GW_allpass*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_allpass_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(allpass_ctor)
{
	GW_allpass* ug = malloc(sizeof(GW_allpass));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = allpass_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(allpass_dtor)
{
	GW_allpass* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_allpass_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(allpass_init)
{
	m_uint gw_offset = SZ_INT;
	GW_allpass* ug = (GW_allpass*)o->ugen->ug;
	if(ug->osc) {
		sp_allpass_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float looptime = *(m_float*)(shred->mem + gw_offset);
	if(sp_allpass_create(&ug->osc) == SP_NOT_OK || sp_allpass_init(ug->sp, ug->osc, looptime) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(allpass_get_revtime)
{
	GW_allpass* ug = (GW_allpass*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->revtime;
}

MFUN(allpass_set_revtime)
{
	m_uint gw_offset = SZ_INT;
	GW_allpass* ug = (GW_allpass*)o->ugen->ug;
	m_float revtime = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->revtime = revtime);
}

typedef struct
{
	sp_data* sp;
	sp_atone* osc;
} GW_atone;

TICK(atone_tick)
{
	GW_atone* ug = (GW_atone*)u->ug;
	base_tick(u);
	sp_atone_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(atone_ctor)
{
	GW_atone* ug = malloc(sizeof(GW_atone));
	ug->sp = shred->vm_ref->bbq->sp;
sp_atone_create(&ug->osc);
sp_atone_init(ug->sp, ug->osc);
	o->ugen->tick = atone_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(atone_dtor)
{
	GW_atone* ug = o->ugen->ug;
	sp_atone_destroy(&ug->osc);
	free(ug);
}

MFUN(atone_get_hp)
{
	GW_atone* ug = (GW_atone*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->hp;
}

MFUN(atone_set_hp)
{
	m_uint gw_offset = SZ_INT;
	GW_atone* ug = (GW_atone*)o->ugen->ug;
	m_float hp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->hp = hp);
}

typedef struct
{
	sp_data* sp;
	sp_autowah* osc;
} GW_autowah;

TICK(autowah_tick)
{
	GW_autowah* ug = (GW_autowah*)u->ug;
	base_tick(u);
	sp_autowah_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(autowah_ctor)
{
	GW_autowah* ug = malloc(sizeof(GW_autowah));
	ug->sp = shred->vm_ref->bbq->sp;
sp_autowah_create(&ug->osc);
sp_autowah_init(ug->sp, ug->osc);
	o->ugen->tick = autowah_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(autowah_dtor)
{
	GW_autowah* ug = o->ugen->ug;
	sp_autowah_destroy(&ug->osc);
	free(ug);
}

MFUN(autowah_get_level)
{
	GW_autowah* ug = (GW_autowah*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->level;
}

MFUN(autowah_set_level)
{
	m_uint gw_offset = SZ_INT;
	GW_autowah* ug = (GW_autowah*)o->ugen->ug;
	m_float level = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->level = level);
}

MFUN(autowah_get_wah)
{
	GW_autowah* ug = (GW_autowah*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->wah;
}

MFUN(autowah_set_wah)
{
	m_uint gw_offset = SZ_INT;
	GW_autowah* ug = (GW_autowah*)o->ugen->ug;
	m_float wah = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->wah = wah);
}

MFUN(autowah_get_mix)
{
	GW_autowah* ug = (GW_autowah*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->mix;
}

MFUN(autowah_set_mix)
{
	m_uint gw_offset = SZ_INT;
	GW_autowah* ug = (GW_autowah*)o->ugen->ug;
	m_float mix = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->mix = mix);
}

typedef struct
{
	sp_data* sp;
	sp_bal* osc;
} GW_bal;

TICK(bal_tick)
{
	GW_bal* ug = (GW_bal*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	sp_bal_compute(ug->sp, ug->osc, &u->channel[0]->ugen->in, &u->channel[1]->ugen->in, &u->out);
	return 1;
}

CTOR(bal_ctor)
{
	GW_bal* ug = malloc(sizeof(GW_bal));
	ug->sp = shred->vm_ref->bbq->sp;
sp_bal_create(&ug->osc);
sp_bal_init(ug->sp, ug->osc);
	o->ugen->tick = bal_tick;
	assign_ugen(o->ugen, 2, 1, 0, ug);
}

DTOR(bal_dtor)
{
	GW_bal* ug = o->ugen->ug;
	sp_bal_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_bar* osc;
	m_bool is_init;
} GW_bar;

TICK(bar_tick)
{
	GW_bar* ug = (GW_bar*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_bar_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(bar_ctor)
{
	GW_bar* ug = malloc(sizeof(GW_bar));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = bar_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(bar_dtor)
{
	GW_bar* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_bar_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(bar_init)
{
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	if(ug->osc) {
		sp_bar_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float iK = *(m_float*)(shred->mem + gw_offset);
	gw_offset +=SZ_FLOAT;
	m_float ib = *(m_float*)(shred->mem + gw_offset);
	if(sp_bar_create(&ug->osc) == SP_NOT_OK || sp_bar_init(ug->sp, ug->osc, iK, ib) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(bar_get_bcL)
{
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bcL;
}

MFUN(bar_set_bcL)
{
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	m_float bcL = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bcL = bcL);
}

MFUN(bar_get_bcR)
{
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bcR;
}

MFUN(bar_set_bcR)
{
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	m_float bcR = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bcR = bcR);
}

MFUN(bar_get_T30)
{
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->T30;
}

MFUN(bar_set_T30)
{
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	m_float T30 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->T30 = T30);
}

MFUN(bar_get_scan)
{
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->scan;
}

MFUN(bar_set_scan)
{
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	m_float scan = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->scan = scan);
}

MFUN(bar_get_pos)
{
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->pos;
}

MFUN(bar_set_pos)
{
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	m_float pos = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->pos = pos);
}

MFUN(bar_get_vel)
{
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->vel;
}

MFUN(bar_set_vel)
{
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	m_float vel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->vel = vel);
}

MFUN(bar_get_wid)
{
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->wid;
}

MFUN(bar_set_wid)
{
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)o->ugen->ug;
	m_float wid = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->wid = wid);
}

typedef struct
{
	sp_data* sp;
	sp_biquad* osc;
} GW_biquad;

TICK(biquad_tick)
{
	GW_biquad* ug = (GW_biquad*)u->ug;
	base_tick(u);
	sp_biquad_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(biquad_ctor)
{
	GW_biquad* ug = malloc(sizeof(GW_biquad));
	ug->sp = shred->vm_ref->bbq->sp;
sp_biquad_create(&ug->osc);
sp_biquad_init(ug->sp, ug->osc);
	o->ugen->tick = biquad_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(biquad_dtor)
{
	GW_biquad* ug = o->ugen->ug;
	sp_biquad_destroy(&ug->osc);
	free(ug);
}

MFUN(biquad_get_b0)
{
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->b0;
}

MFUN(biquad_set_b0)
{
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	m_float b0 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->b0 = b0);
}

MFUN(biquad_get_b1)
{
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->b1;
}

MFUN(biquad_set_b1)
{
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	m_float b1 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->b1 = b1);
}

MFUN(biquad_get_b2)
{
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->b2;
}

MFUN(biquad_set_b2)
{
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	m_float b2 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->b2 = b2);
}

MFUN(biquad_get_a0)
{
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->a0;
}

MFUN(biquad_set_a0)
{
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	m_float a0 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->a0 = a0);
}

MFUN(biquad_get_a1)
{
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->a1;
}

MFUN(biquad_set_a1)
{
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	m_float a1 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->a1 = a1);
}

MFUN(biquad_get_a2)
{
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->a2;
}

MFUN(biquad_set_a2)
{
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)o->ugen->ug;
	m_float a2 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->a2 = a2);
}

typedef struct
{
	sp_data* sp;
	sp_biscale* osc;
} GW_biscale;

TICK(biscale_tick)
{
	GW_biscale* ug = (GW_biscale*)u->ug;
	base_tick(u);
	sp_biscale_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(biscale_ctor)
{
	GW_biscale* ug = malloc(sizeof(GW_biscale));
	ug->sp = shred->vm_ref->bbq->sp;
sp_biscale_create(&ug->osc);
sp_biscale_init(ug->sp, ug->osc);
	o->ugen->tick = biscale_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(biscale_dtor)
{
	GW_biscale* ug = o->ugen->ug;
	sp_biscale_destroy(&ug->osc);
	free(ug);
}

MFUN(biscale_get_min)
{
	GW_biscale* ug = (GW_biscale*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->min;
}

MFUN(biscale_set_min)
{
	m_uint gw_offset = SZ_INT;
	GW_biscale* ug = (GW_biscale*)o->ugen->ug;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->min = min);
}

MFUN(biscale_get_max)
{
	GW_biscale* ug = (GW_biscale*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->max;
}

MFUN(biscale_set_max)
{
	m_uint gw_offset = SZ_INT;
	GW_biscale* ug = (GW_biscale*)o->ugen->ug;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->max = max);
}

typedef struct
{
	sp_data* sp;
	sp_bitcrush* osc;
} GW_bitcrush;

TICK(bitcrush_tick)
{
	GW_bitcrush* ug = (GW_bitcrush*)u->ug;
	base_tick(u);
	sp_bitcrush_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(bitcrush_ctor)
{
	GW_bitcrush* ug = malloc(sizeof(GW_bitcrush));
	ug->sp = shred->vm_ref->bbq->sp;
sp_bitcrush_create(&ug->osc);
sp_bitcrush_init(ug->sp, ug->osc);
	o->ugen->tick = bitcrush_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(bitcrush_dtor)
{
	GW_bitcrush* ug = o->ugen->ug;
	sp_bitcrush_destroy(&ug->osc);
	free(ug);
}

MFUN(bitcrush_get_bitdepth)
{
	GW_bitcrush* ug = (GW_bitcrush*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bitdepth;
}

MFUN(bitcrush_set_bitdepth)
{
	m_uint gw_offset = SZ_INT;
	GW_bitcrush* ug = (GW_bitcrush*)o->ugen->ug;
	m_float bitdepth = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bitdepth = bitdepth);
}

MFUN(bitcrush_get_srate)
{
	GW_bitcrush* ug = (GW_bitcrush*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->srate;
}

MFUN(bitcrush_set_srate)
{
	m_uint gw_offset = SZ_INT;
	GW_bitcrush* ug = (GW_bitcrush*)o->ugen->ug;
	m_float srate = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->srate = srate);
}

typedef struct
{
	sp_data* sp;
	sp_blsaw* osc;
} GW_blsaw;

TICK(blsaw_tick)
{
	GW_blsaw* ug = (GW_blsaw*)u->ug;
	sp_blsaw_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(blsaw_ctor)
{
	GW_blsaw* ug = malloc(sizeof(GW_blsaw));
	ug->sp = shred->vm_ref->bbq->sp;
sp_blsaw_create(&ug->osc);
sp_blsaw_init(ug->sp, ug->osc);
	o->ugen->tick = blsaw_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(blsaw_dtor)
{
	GW_blsaw* ug = o->ugen->ug;
	sp_blsaw_destroy(&ug->osc);
	free(ug);
}

MFUN(blsaw_get_freq)
{
	GW_blsaw* ug = (GW_blsaw*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->freq;
}

MFUN(blsaw_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_blsaw* ug = (GW_blsaw*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->freq = freq);
}

MFUN(blsaw_get_amp)
{
	GW_blsaw* ug = (GW_blsaw*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->amp;
}

MFUN(blsaw_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_blsaw* ug = (GW_blsaw*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->amp = amp);
}

typedef struct
{
	sp_data* sp;
	sp_blsquare* osc;
} GW_blsquare;

TICK(blsquare_tick)
{
	GW_blsquare* ug = (GW_blsquare*)u->ug;
	sp_blsquare_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(blsquare_ctor)
{
	GW_blsquare* ug = malloc(sizeof(GW_blsquare));
	ug->sp = shred->vm_ref->bbq->sp;
sp_blsquare_create(&ug->osc);
sp_blsquare_init(ug->sp, ug->osc);
	o->ugen->tick = blsquare_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(blsquare_dtor)
{
	GW_blsquare* ug = o->ugen->ug;
	sp_blsquare_destroy(&ug->osc);
	free(ug);
}

MFUN(blsquare_get_freq)
{
	GW_blsquare* ug = (GW_blsquare*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->freq;
}

MFUN(blsquare_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_blsquare* ug = (GW_blsquare*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->freq = freq);
}

MFUN(blsquare_get_amp)
{
	GW_blsquare* ug = (GW_blsquare*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->amp;
}

MFUN(blsquare_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_blsquare* ug = (GW_blsquare*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->amp = amp);
}

MFUN(blsquare_get_width)
{
	GW_blsquare* ug = (GW_blsquare*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->width;
}

MFUN(blsquare_set_width)
{
	m_uint gw_offset = SZ_INT;
	GW_blsquare* ug = (GW_blsquare*)o->ugen->ug;
	m_float width = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->width = width);
}

typedef struct
{
	sp_data* sp;
	sp_bltriangle* osc;
} GW_bltriangle;

TICK(bltriangle_tick)
{
	GW_bltriangle* ug = (GW_bltriangle*)u->ug;
	sp_bltriangle_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(bltriangle_ctor)
{
	GW_bltriangle* ug = malloc(sizeof(GW_bltriangle));
	ug->sp = shred->vm_ref->bbq->sp;
sp_bltriangle_create(&ug->osc);
sp_bltriangle_init(ug->sp, ug->osc);
	o->ugen->tick = bltriangle_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(bltriangle_dtor)
{
	GW_bltriangle* ug = o->ugen->ug;
	sp_bltriangle_destroy(&ug->osc);
	free(ug);
}

MFUN(bltriangle_get_freq)
{
	GW_bltriangle* ug = (GW_bltriangle*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->freq;
}

MFUN(bltriangle_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_bltriangle* ug = (GW_bltriangle*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->freq = freq);
}

MFUN(bltriangle_get_amp)
{
	GW_bltriangle* ug = (GW_bltriangle*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->amp;
}

MFUN(bltriangle_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_bltriangle* ug = (GW_bltriangle*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->amp = amp);
}

typedef struct
{
	sp_data* sp;
	sp_brown* osc;
} GW_brown;

TICK(brown_tick)
{
	GW_brown* ug = (GW_brown*)u->ug;
	sp_brown_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(brown_ctor)
{
	GW_brown* ug = malloc(sizeof(GW_brown));
	ug->sp = shred->vm_ref->bbq->sp;
sp_brown_create(&ug->osc);
sp_brown_init(ug->sp, ug->osc);
	o->ugen->tick = brown_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(brown_dtor)
{
	GW_brown* ug = o->ugen->ug;
	sp_brown_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_butbp* osc;
} GW_butbp;

TICK(butbp_tick)
{
	GW_butbp* ug = (GW_butbp*)u->ug;
	base_tick(u);
	sp_butbp_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(butbp_ctor)
{
	GW_butbp* ug = malloc(sizeof(GW_butbp));
	ug->sp = shred->vm_ref->bbq->sp;
sp_butbp_create(&ug->osc);
sp_butbp_init(ug->sp, ug->osc);
	o->ugen->tick = butbp_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(butbp_dtor)
{
	GW_butbp* ug = o->ugen->ug;
	sp_butbp_destroy(&ug->osc);
	free(ug);
}

MFUN(butbp_get_freq)
{
	GW_butbp* ug = (GW_butbp*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(butbp_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_butbp* ug = (GW_butbp*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(butbp_get_bw)
{
	GW_butbp* ug = (GW_butbp*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bw;
}

MFUN(butbp_set_bw)
{
	m_uint gw_offset = SZ_INT;
	GW_butbp* ug = (GW_butbp*)o->ugen->ug;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bw = bw);
}

typedef struct
{
	sp_data* sp;
	sp_butbr* osc;
} GW_butbr;

TICK(butbr_tick)
{
	GW_butbr* ug = (GW_butbr*)u->ug;
	base_tick(u);
	sp_butbr_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(butbr_ctor)
{
	GW_butbr* ug = malloc(sizeof(GW_butbr));
	ug->sp = shred->vm_ref->bbq->sp;
sp_butbr_create(&ug->osc);
sp_butbr_init(ug->sp, ug->osc);
	o->ugen->tick = butbr_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(butbr_dtor)
{
	GW_butbr* ug = o->ugen->ug;
	sp_butbr_destroy(&ug->osc);
	free(ug);
}

MFUN(butbr_get_freq)
{
	GW_butbr* ug = (GW_butbr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(butbr_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_butbr* ug = (GW_butbr*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(butbr_get_bw)
{
	GW_butbr* ug = (GW_butbr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bw;
}

MFUN(butbr_set_bw)
{
	m_uint gw_offset = SZ_INT;
	GW_butbr* ug = (GW_butbr*)o->ugen->ug;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bw = bw);
}

typedef struct
{
	sp_data* sp;
	sp_buthp* osc;
} GW_buthp;

TICK(buthp_tick)
{
	GW_buthp* ug = (GW_buthp*)u->ug;
	base_tick(u);
	sp_buthp_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(buthp_ctor)
{
	GW_buthp* ug = malloc(sizeof(GW_buthp));
	ug->sp = shred->vm_ref->bbq->sp;
sp_buthp_create(&ug->osc);
sp_buthp_init(ug->sp, ug->osc);
	o->ugen->tick = buthp_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(buthp_dtor)
{
	GW_buthp* ug = o->ugen->ug;
	sp_buthp_destroy(&ug->osc);
	free(ug);
}

MFUN(buthp_get_freq)
{
	GW_buthp* ug = (GW_buthp*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(buthp_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_buthp* ug = (GW_buthp*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

typedef struct
{
	sp_data* sp;
	sp_butlp* osc;
} GW_butlp;

TICK(butlp_tick)
{
	GW_butlp* ug = (GW_butlp*)u->ug;
	base_tick(u);
	sp_butlp_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(butlp_ctor)
{
	GW_butlp* ug = malloc(sizeof(GW_butlp));
	ug->sp = shred->vm_ref->bbq->sp;
sp_butlp_create(&ug->osc);
sp_butlp_init(ug->sp, ug->osc);
	o->ugen->tick = butlp_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(butlp_dtor)
{
	GW_butlp* ug = o->ugen->ug;
	sp_butlp_destroy(&ug->osc);
	free(ug);
}

MFUN(butlp_get_freq)
{
	GW_butlp* ug = (GW_butlp*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(butlp_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_butlp* ug = (GW_butlp*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

typedef struct
{
	sp_data* sp;
	sp_clip* osc;
} GW_clip;

TICK(clip_tick)
{
	GW_clip* ug = (GW_clip*)u->ug;
	base_tick(u);
	sp_clip_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(clip_ctor)
{
	GW_clip* ug = malloc(sizeof(GW_clip));
	ug->sp = shred->vm_ref->bbq->sp;
sp_clip_create(&ug->osc);
sp_clip_init(ug->sp, ug->osc);
	o->ugen->tick = clip_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(clip_dtor)
{
	GW_clip* ug = o->ugen->ug;
	sp_clip_destroy(&ug->osc);
	free(ug);
}

MFUN(clip_get_lim)
{
	GW_clip* ug = (GW_clip*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->lim;
}

MFUN(clip_set_lim)
{
	m_uint gw_offset = SZ_INT;
	GW_clip* ug = (GW_clip*)o->ugen->ug;
	m_float lim = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->lim = lim);
}

typedef struct
{
	sp_data* sp;
	sp_clock* osc;
} GW_clock;

TICK(clock_tick)
{
	GW_clock* ug = (GW_clock*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_clock_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(clock_ctor)
{
	GW_clock* ug = malloc(sizeof(GW_clock));
	ug->sp = shred->vm_ref->bbq->sp;
sp_clock_create(&ug->osc);
sp_clock_init(ug->sp, ug->osc);
	o->ugen->tick = clock_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(clock_dtor)
{
	GW_clock* ug = o->ugen->ug;
	sp_clock_destroy(&ug->osc);
	free(ug);
}

MFUN(clock_get_bpm)
{
	GW_clock* ug = (GW_clock*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bpm;
}

MFUN(clock_set_bpm)
{
	m_uint gw_offset = SZ_INT;
	GW_clock* ug = (GW_clock*)o->ugen->ug;
	m_float bpm = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bpm = bpm);
}

MFUN(clock_get_subdiv)
{
	GW_clock* ug = (GW_clock*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->subdiv;
}

MFUN(clock_set_subdiv)
{
	m_uint gw_offset = SZ_INT;
	GW_clock* ug = (GW_clock*)o->ugen->ug;
	m_float subdiv = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->subdiv = subdiv);
}

typedef struct
{
	sp_data* sp;
	sp_comb* osc;
	m_bool is_init;
} GW_comb;

TICK(comb_tick)
{
	GW_comb* ug = (GW_comb*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_comb_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(comb_ctor)
{
	GW_comb* ug = malloc(sizeof(GW_comb));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = comb_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(comb_dtor)
{
	GW_comb* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_comb_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(comb_init)
{
	m_uint gw_offset = SZ_INT;
	GW_comb* ug = (GW_comb*)o->ugen->ug;
	if(ug->osc) {
		sp_comb_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float looptime = *(m_float*)(shred->mem + gw_offset);
	if(sp_comb_create(&ug->osc) == SP_NOT_OK || sp_comb_init(ug->sp, ug->osc, looptime) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(comb_get_revtime)
{
	GW_comb* ug = (GW_comb*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->revtime;
}

MFUN(comb_set_revtime)
{
	m_uint gw_offset = SZ_INT;
	GW_comb* ug = (GW_comb*)o->ugen->ug;
	m_float revtime = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->revtime = revtime);
}

typedef struct
{
	sp_data* sp;
	sp_compressor* osc;
} GW_compressor;

TICK(compressor_tick)
{
	GW_compressor* ug = (GW_compressor*)u->ug;
	base_tick(u);
	sp_compressor_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(compressor_ctor)
{
	GW_compressor* ug = malloc(sizeof(GW_compressor));
	ug->sp = shred->vm_ref->bbq->sp;
sp_compressor_create(&ug->osc);
sp_compressor_init(ug->sp, ug->osc);
	o->ugen->tick = compressor_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(compressor_dtor)
{
	GW_compressor* ug = o->ugen->ug;
	sp_compressor_destroy(&ug->osc);
	free(ug);
}

MFUN(compressor_get_ratio)
{
	GW_compressor* ug = (GW_compressor*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->ratio;
}

MFUN(compressor_set_ratio)
{
	m_uint gw_offset = SZ_INT;
	GW_compressor* ug = (GW_compressor*)o->ugen->ug;
	m_float ratio = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->ratio = ratio);
}

MFUN(compressor_get_thresh)
{
	GW_compressor* ug = (GW_compressor*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->thresh;
}

MFUN(compressor_set_thresh)
{
	m_uint gw_offset = SZ_INT;
	GW_compressor* ug = (GW_compressor*)o->ugen->ug;
	m_float thresh = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->thresh = thresh);
}

MFUN(compressor_get_atk)
{
	GW_compressor* ug = (GW_compressor*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->atk;
}

MFUN(compressor_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_compressor* ug = (GW_compressor*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->atk = atk);
}

MFUN(compressor_get_rel)
{
	GW_compressor* ug = (GW_compressor*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->rel;
}

MFUN(compressor_set_rel)
{
	m_uint gw_offset = SZ_INT;
	GW_compressor* ug = (GW_compressor*)o->ugen->ug;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->rel = rel);
}

typedef struct
{
	sp_data* sp;
	sp_conv* osc;
	m_bool is_init;
} GW_conv;

TICK(conv_tick)
{
	GW_conv* ug = (GW_conv*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_conv_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(conv_ctor)
{
	GW_conv* ug = malloc(sizeof(GW_conv));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = conv_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(conv_dtor)
{
	GW_conv* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_conv_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(conv_init)
{
	m_uint gw_offset = SZ_INT;
	GW_conv* ug = (GW_conv*)o->ugen->ug;
	if(ug->osc) {
		sp_conv_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* ft = FTBL(ft_obj);
	release(ft_obj, shred);
	gw_offset +=SZ_INT;
	m_float iPartLen = *(m_float*)(shred->mem + gw_offset);
	if(sp_conv_create(&ug->osc) == SP_NOT_OK || sp_conv_init(ug->sp, ug->osc, ft, iPartLen) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_count* osc;
} GW_count;

TICK(count_tick)
{
	GW_count* ug = (GW_count*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_count_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(count_ctor)
{
	GW_count* ug = malloc(sizeof(GW_count));
	ug->sp = shred->vm_ref->bbq->sp;
sp_count_create(&ug->osc);
sp_count_init(ug->sp, ug->osc);
	o->ugen->tick = count_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(count_dtor)
{
	GW_count* ug = o->ugen->ug;
	sp_count_destroy(&ug->osc);
	free(ug);
}

MFUN(count_get_count)
{
	GW_count* ug = (GW_count*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->count;
}

MFUN(count_set_count)
{
	m_uint gw_offset = SZ_INT;
	GW_count* ug = (GW_count*)o->ugen->ug;
	m_float count = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->count = count);
}

MFUN(count_get_mode)
{
	GW_count* ug = (GW_count*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->mode;
}

MFUN(count_set_mode)
{
	m_uint gw_offset = SZ_INT;
	GW_count* ug = (GW_count*)o->ugen->ug;
	m_float mode = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->mode = mode);
}

typedef struct
{
	sp_data* sp;
	sp_crossfade* osc;
} GW_crossfade;

TICK(crossfade_tick)
{
	GW_crossfade* ug = (GW_crossfade*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	sp_crossfade_compute(ug->sp, ug->osc, &u->channel[0]->ugen->in, &u->channel[1]->ugen->in, &u->out);
	return 1;
}

CTOR(crossfade_ctor)
{
	GW_crossfade* ug = malloc(sizeof(GW_crossfade));
	ug->sp = shred->vm_ref->bbq->sp;
sp_crossfade_create(&ug->osc);
sp_crossfade_init(ug->sp, ug->osc);
	o->ugen->tick = crossfade_tick;
	assign_ugen(o->ugen, 2, 1, 0, ug);
}

DTOR(crossfade_dtor)
{
	GW_crossfade* ug = o->ugen->ug;
	sp_crossfade_destroy(&ug->osc);
	free(ug);
}

MFUN(crossfade_get_pos)
{
	GW_crossfade* ug = (GW_crossfade*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->pos;
}

MFUN(crossfade_set_pos)
{
	m_uint gw_offset = SZ_INT;
	GW_crossfade* ug = (GW_crossfade*)o->ugen->ug;
	m_float pos = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->pos = pos);
}

typedef struct
{
	sp_data* sp;
	sp_dcblock* osc;
} GW_dcblock;

TICK(dcblock_tick)
{
	GW_dcblock* ug = (GW_dcblock*)u->ug;
	base_tick(u);
	sp_dcblock_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(dcblock_ctor)
{
	GW_dcblock* ug = malloc(sizeof(GW_dcblock));
	ug->sp = shred->vm_ref->bbq->sp;
sp_dcblock_create(&ug->osc);
sp_dcblock_init(ug->sp, ug->osc);
	o->ugen->tick = dcblock_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(dcblock_dtor)
{
	GW_dcblock* ug = o->ugen->ug;
	sp_dcblock_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_delay* osc;
	m_bool is_init;
} GW_delay;

TICK(delay_tick)
{
	GW_delay* ug = (GW_delay*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_delay_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(delay_ctor)
{
	GW_delay* ug = malloc(sizeof(GW_delay));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = delay_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(delay_dtor)
{
	GW_delay* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_delay_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(delay_init)
{
	m_uint gw_offset = SZ_INT;
	GW_delay* ug = (GW_delay*)o->ugen->ug;
	if(ug->osc) {
		sp_delay_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float time = *(m_float*)(shred->mem + gw_offset);
	if(sp_delay_create(&ug->osc) == SP_NOT_OK || sp_delay_init(ug->sp, ug->osc, time) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(delay_get_feedback)
{
	GW_delay* ug = (GW_delay*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->feedback;
}

MFUN(delay_set_feedback)
{
	m_uint gw_offset = SZ_INT;
	GW_delay* ug = (GW_delay*)o->ugen->ug;
	m_float feedback = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->feedback = feedback);
}

typedef struct
{
	sp_data* sp;
	sp_diode* osc;
} GW_diode;

TICK(diode_tick)
{
	GW_diode* ug = (GW_diode*)u->ug;
	base_tick(u);
	sp_diode_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(diode_ctor)
{
	GW_diode* ug = malloc(sizeof(GW_diode));
	ug->sp = shred->vm_ref->bbq->sp;
sp_diode_create(&ug->osc);
sp_diode_init(ug->sp, ug->osc);
	o->ugen->tick = diode_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(diode_dtor)
{
	GW_diode* ug = o->ugen->ug;
	sp_diode_destroy(&ug->osc);
	free(ug);
}

MFUN(diode_get_freq)
{
	GW_diode* ug = (GW_diode*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(diode_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_diode* ug = (GW_diode*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(diode_get_res)
{
	GW_diode* ug = (GW_diode*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->res;
}

MFUN(diode_set_res)
{
	m_uint gw_offset = SZ_INT;
	GW_diode* ug = (GW_diode*)o->ugen->ug;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->res = res);
}

typedef struct
{
	sp_data* sp;
	sp_diskin* osc;
	m_bool is_init;
} GW_diskin;

TICK(diskin_tick)
{
	GW_diskin* ug = (GW_diskin*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_diskin_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(diskin_ctor)
{
	GW_diskin* ug = malloc(sizeof(GW_diskin));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = diskin_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(diskin_dtor)
{
	GW_diskin* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_diskin_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(diskin_init)
{
	m_uint gw_offset = SZ_INT;
	GW_diskin* ug = (GW_diskin*)o->ugen->ug;
	if(ug->osc) {
		sp_diskin_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str filename = STRING(filename_obj);
	release(filename_obj, shred);
	if(sp_diskin_create(&ug->osc) == SP_NOT_OK || sp_diskin_init(ug->sp, ug->osc, filename) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_dist* osc;
} GW_dist;

TICK(dist_tick)
{
	GW_dist* ug = (GW_dist*)u->ug;
	base_tick(u);
	sp_dist_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(dist_ctor)
{
	GW_dist* ug = malloc(sizeof(GW_dist));
	ug->sp = shred->vm_ref->bbq->sp;
sp_dist_create(&ug->osc);
sp_dist_init(ug->sp, ug->osc);
	o->ugen->tick = dist_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(dist_dtor)
{
	GW_dist* ug = o->ugen->ug;
	sp_dist_destroy(&ug->osc);
	free(ug);
}

MFUN(dist_get_pregain)
{
	GW_dist* ug = (GW_dist*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->pregain;
}

MFUN(dist_set_pregain)
{
	m_uint gw_offset = SZ_INT;
	GW_dist* ug = (GW_dist*)o->ugen->ug;
	m_float pregain = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->pregain = pregain);
}

MFUN(dist_get_postgain)
{
	GW_dist* ug = (GW_dist*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->postgain;
}

MFUN(dist_set_postgain)
{
	m_uint gw_offset = SZ_INT;
	GW_dist* ug = (GW_dist*)o->ugen->ug;
	m_float postgain = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->postgain = postgain);
}

MFUN(dist_get_shape1)
{
	GW_dist* ug = (GW_dist*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->shape1;
}

MFUN(dist_set_shape1)
{
	m_uint gw_offset = SZ_INT;
	GW_dist* ug = (GW_dist*)o->ugen->ug;
	m_float shape1 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->shape1 = shape1);
}

MFUN(dist_get_shape2)
{
	GW_dist* ug = (GW_dist*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->shape2;
}

MFUN(dist_set_shape2)
{
	m_uint gw_offset = SZ_INT;
	GW_dist* ug = (GW_dist*)o->ugen->ug;
	m_float shape2 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->shape2 = shape2);
}

typedef struct
{
	sp_data* sp;
	sp_dmetro* osc;
} GW_dmetro;

TICK(dmetro_tick)
{
	GW_dmetro* ug = (GW_dmetro*)u->ug;
	sp_dmetro_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(dmetro_ctor)
{
	GW_dmetro* ug = malloc(sizeof(GW_dmetro));
	ug->sp = shred->vm_ref->bbq->sp;
sp_dmetro_create(&ug->osc);
sp_dmetro_init(ug->sp, ug->osc);
	o->ugen->tick = dmetro_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(dmetro_dtor)
{
	GW_dmetro* ug = o->ugen->ug;
	sp_dmetro_destroy(&ug->osc);
	free(ug);
}

MFUN(dmetro_get_time)
{
	GW_dmetro* ug = (GW_dmetro*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->time;
}

MFUN(dmetro_set_time)
{
	m_uint gw_offset = SZ_INT;
	GW_dmetro* ug = (GW_dmetro*)o->ugen->ug;
	m_float time = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->time = time);
}

typedef struct
{
	sp_data* sp;
	sp_drip* osc;
	m_bool is_init;
} GW_drip;

TICK(drip_tick)
{
	GW_drip* ug = (GW_drip*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_drip_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(drip_ctor)
{
	GW_drip* ug = malloc(sizeof(GW_drip));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = drip_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(drip_dtor)
{
	GW_drip* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_drip_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(drip_init)
{
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	if(ug->osc) {
		sp_drip_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float dettack = *(m_float*)(shred->mem + gw_offset);
	if(sp_drip_create(&ug->osc) == SP_NOT_OK || sp_drip_init(ug->sp, ug->osc, dettack) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(drip_get_num_tubes)
{
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->num_tubes;
}

MFUN(drip_set_num_tubes)
{
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	m_float num_tubes = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->num_tubes = num_tubes);
}

MFUN(drip_get_amp)
{
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(drip_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(drip_get_damp)
{
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->damp;
}

MFUN(drip_set_damp)
{
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	m_float damp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->damp = damp);
}

MFUN(drip_get_shake_max)
{
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->shake_max;
}

MFUN(drip_set_shake_max)
{
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	m_float shake_max = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->shake_max = shake_max);
}

MFUN(drip_get_freq)
{
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(drip_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(drip_get_freq1)
{
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq1;
}

MFUN(drip_set_freq1)
{
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	m_float freq1 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq1 = freq1);
}

MFUN(drip_get_freq2)
{
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq2;
}

MFUN(drip_set_freq2)
{
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)o->ugen->ug;
	m_float freq2 = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq2 = freq2);
}

typedef struct
{
	sp_data* sp;
	sp_dtrig* osc;
	m_bool is_init;
} GW_dtrig;

TICK(dtrig_tick)
{
	GW_dtrig* ug = (GW_dtrig*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_dtrig_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(dtrig_ctor)
{
	GW_dtrig* ug = malloc(sizeof(GW_dtrig));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = dtrig_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(dtrig_dtor)
{
	GW_dtrig* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_dtrig_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(dtrig_init)
{
	m_uint gw_offset = SZ_INT;
	GW_dtrig* ug = (GW_dtrig*)o->ugen->ug;
	if(ug->osc) {
		sp_dtrig_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* ft = FTBL(ft_obj);
	release(ft_obj, shred);
	if(sp_dtrig_create(&ug->osc) == SP_NOT_OK || sp_dtrig_init(ug->sp, ug->osc, ft) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(dtrig_get_loop)
{
	GW_dtrig* ug = (GW_dtrig*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->loop;
}

MFUN(dtrig_set_loop)
{
	m_uint gw_offset = SZ_INT;
	GW_dtrig* ug = (GW_dtrig*)o->ugen->ug;
	m_int loop = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->loop = loop);
}

MFUN(dtrig_get_delay)
{
	GW_dtrig* ug = (GW_dtrig*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->delay;
}

MFUN(dtrig_set_delay)
{
	m_uint gw_offset = SZ_INT;
	GW_dtrig* ug = (GW_dtrig*)o->ugen->ug;
	m_float delay = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->delay = delay);
}

MFUN(dtrig_get_scale)
{
	GW_dtrig* ug = (GW_dtrig*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->scale;
}

MFUN(dtrig_set_scale)
{
	m_uint gw_offset = SZ_INT;
	GW_dtrig* ug = (GW_dtrig*)o->ugen->ug;
	m_float scale = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->scale = scale);
}

typedef struct
{
	sp_data* sp;
	sp_dust* osc;
} GW_dust;

TICK(dust_tick)
{
	GW_dust* ug = (GW_dust*)u->ug;
	sp_dust_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(dust_ctor)
{
	GW_dust* ug = malloc(sizeof(GW_dust));
	ug->sp = shred->vm_ref->bbq->sp;
sp_dust_create(&ug->osc);
sp_dust_init(ug->sp, ug->osc);
	o->ugen->tick = dust_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(dust_dtor)
{
	GW_dust* ug = o->ugen->ug;
	sp_dust_destroy(&ug->osc);
	free(ug);
}

MFUN(dust_get_amp)
{
	GW_dust* ug = (GW_dust*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(dust_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_dust* ug = (GW_dust*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(dust_get_density)
{
	GW_dust* ug = (GW_dust*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->density;
}

MFUN(dust_set_density)
{
	m_uint gw_offset = SZ_INT;
	GW_dust* ug = (GW_dust*)o->ugen->ug;
	m_float density = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->density = density);
}

MFUN(dust_get_bipolar)
{
	GW_dust* ug = (GW_dust*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->bipolar;
}

MFUN(dust_set_bipolar)
{
	m_uint gw_offset = SZ_INT;
	GW_dust* ug = (GW_dust*)o->ugen->ug;
	m_int bipolar = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->bipolar = bipolar);
}

typedef struct
{
	sp_data* sp;
	sp_eqfil* osc;
} GW_eqfil;

TICK(eqfil_tick)
{
	GW_eqfil* ug = (GW_eqfil*)u->ug;
	base_tick(u);
	sp_eqfil_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(eqfil_ctor)
{
	GW_eqfil* ug = malloc(sizeof(GW_eqfil));
	ug->sp = shred->vm_ref->bbq->sp;
sp_eqfil_create(&ug->osc);
sp_eqfil_init(ug->sp, ug->osc);
	o->ugen->tick = eqfil_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(eqfil_dtor)
{
	GW_eqfil* ug = o->ugen->ug;
	sp_eqfil_destroy(&ug->osc);
	free(ug);
}

MFUN(eqfil_get_freq)
{
	GW_eqfil* ug = (GW_eqfil*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(eqfil_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_eqfil* ug = (GW_eqfil*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(eqfil_get_bw)
{
	GW_eqfil* ug = (GW_eqfil*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bw;
}

MFUN(eqfil_set_bw)
{
	m_uint gw_offset = SZ_INT;
	GW_eqfil* ug = (GW_eqfil*)o->ugen->ug;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bw = bw);
}

MFUN(eqfil_get_gain)
{
	GW_eqfil* ug = (GW_eqfil*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->gain;
}

MFUN(eqfil_set_gain)
{
	m_uint gw_offset = SZ_INT;
	GW_eqfil* ug = (GW_eqfil*)o->ugen->ug;
	m_float gain = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->gain = gain);
}

typedef struct
{
	sp_data* sp;
	sp_expon* osc;
} GW_expon;

TICK(expon_tick)
{
	GW_expon* ug = (GW_expon*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_expon_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(expon_ctor)
{
	GW_expon* ug = malloc(sizeof(GW_expon));
	ug->sp = shred->vm_ref->bbq->sp;
sp_expon_create(&ug->osc);
sp_expon_init(ug->sp, ug->osc);
	o->ugen->tick = expon_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(expon_dtor)
{
	GW_expon* ug = o->ugen->ug;
	sp_expon_destroy(&ug->osc);
	free(ug);
}

MFUN(expon_get_a)
{
	GW_expon* ug = (GW_expon*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->a;
}

MFUN(expon_set_a)
{
	m_uint gw_offset = SZ_INT;
	GW_expon* ug = (GW_expon*)o->ugen->ug;
	m_float a = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->a = a);
}

MFUN(expon_get_dur)
{
	GW_expon* ug = (GW_expon*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dur;
}

MFUN(expon_set_dur)
{
	m_uint gw_offset = SZ_INT;
	GW_expon* ug = (GW_expon*)o->ugen->ug;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dur = dur);
}

MFUN(expon_get_b)
{
	GW_expon* ug = (GW_expon*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->b;
}

MFUN(expon_set_b)
{
	m_uint gw_offset = SZ_INT;
	GW_expon* ug = (GW_expon*)o->ugen->ug;
	m_float b = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->b = b);
}

typedef struct
{
	sp_data* sp;
	sp_fof* osc;
	m_bool is_init;
} GW_fof;

TICK(fof_tick)
{
	GW_fof* ug = (GW_fof*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_fof_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(fof_ctor)
{
	GW_fof* ug = malloc(sizeof(GW_fof));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = fof_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(fof_dtor)
{
	GW_fof* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_fof_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(fof_init)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	if(ug->osc) {
		sp_fof_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object sine_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* sine = FTBL(sine_obj);
	release(sine_obj, shred);
	gw_offset +=SZ_INT;
	M_Object win_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* win = FTBL(win_obj);
	release(win_obj, shred);
	gw_offset +=SZ_INT;
	m_int iolaps = *(m_int*)(shred->mem + gw_offset);
	gw_offset +=SZ_INT;
	m_float iphs = *(m_float*)(shred->mem + gw_offset);
	if(sp_fof_create(&ug->osc) == SP_NOT_OK || sp_fof_init(ug->sp, ug->osc, sine, win, iolaps, iphs) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(fof_get_amp)
{
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(fof_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(fof_get_fund)
{
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->fund;
}

MFUN(fof_set_fund)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	m_float fund = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->fund = fund);
}

MFUN(fof_get_form)
{
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->form;
}

MFUN(fof_set_form)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	m_float form = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->form = form);
}

MFUN(fof_get_oct)
{
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->oct;
}

MFUN(fof_set_oct)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	m_float oct = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->oct = oct);
}

MFUN(fof_get_band)
{
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->band;
}

MFUN(fof_set_band)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	m_float band = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->band = band);
}

MFUN(fof_get_ris)
{
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->ris;
}

MFUN(fof_set_ris)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	m_float ris = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->ris = ris);
}

MFUN(fof_get_dec)
{
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dec;
}

MFUN(fof_set_dec)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dec = dec);
}

MFUN(fof_get_dur)
{
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dur;
}

MFUN(fof_set_dur)
{
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)o->ugen->ug;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dur = dur);
}

typedef struct
{
	sp_data* sp;
	sp_fofilt* osc;
} GW_fofilt;

TICK(fofilt_tick)
{
	GW_fofilt* ug = (GW_fofilt*)u->ug;
	base_tick(u);
	sp_fofilt_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(fofilt_ctor)
{
	GW_fofilt* ug = malloc(sizeof(GW_fofilt));
	ug->sp = shred->vm_ref->bbq->sp;
sp_fofilt_create(&ug->osc);
sp_fofilt_init(ug->sp, ug->osc);
	o->ugen->tick = fofilt_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(fofilt_dtor)
{
	GW_fofilt* ug = o->ugen->ug;
	sp_fofilt_destroy(&ug->osc);
	free(ug);
}

MFUN(fofilt_get_freq)
{
	GW_fofilt* ug = (GW_fofilt*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(fofilt_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_fofilt* ug = (GW_fofilt*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(fofilt_get_atk)
{
	GW_fofilt* ug = (GW_fofilt*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->atk;
}

MFUN(fofilt_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_fofilt* ug = (GW_fofilt*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->atk = atk);
}

MFUN(fofilt_get_dec)
{
	GW_fofilt* ug = (GW_fofilt*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dec;
}

MFUN(fofilt_set_dec)
{
	m_uint gw_offset = SZ_INT;
	GW_fofilt* ug = (GW_fofilt*)o->ugen->ug;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dec = dec);
}

typedef struct
{
	sp_data* sp;
	sp_fog* osc;
	m_bool is_init;
} GW_fog;

TICK(fog_tick)
{
	GW_fog* ug = (GW_fog*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_fog_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(fog_ctor)
{
	GW_fog* ug = malloc(sizeof(GW_fog));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = fog_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(fog_dtor)
{
	GW_fog* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_fog_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(fog_init)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	if(ug->osc) {
		sp_fog_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object wav_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* wav = FTBL(wav_obj);
	release(wav_obj, shred);
	gw_offset +=SZ_INT;
	M_Object win_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* win = FTBL(win_obj);
	release(win_obj, shred);
	gw_offset +=SZ_INT;
	m_int iolaps = *(m_int*)(shred->mem + gw_offset);
	gw_offset +=SZ_INT;
	m_float iphs = *(m_float*)(shred->mem + gw_offset);
	if(sp_fog_create(&ug->osc) == SP_NOT_OK || sp_fog_init(ug->sp, ug->osc, wav, win, iolaps, iphs) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(fog_get_amp)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(fog_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(fog_get_dens)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dens;
}

MFUN(fog_set_dens)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float dens = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dens = dens);
}

MFUN(fog_get_trans)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->trans;
}

MFUN(fog_set_trans)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float trans = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->trans = trans);
}

MFUN(fog_get_spd)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->spd;
}

MFUN(fog_set_spd)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float spd = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->spd = spd);
}

MFUN(fog_get_oct)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->oct;
}

MFUN(fog_set_oct)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float oct = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->oct = oct);
}

MFUN(fog_get_band)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->band;
}

MFUN(fog_set_band)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float band = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->band = band);
}

MFUN(fog_get_ris)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->ris;
}

MFUN(fog_set_ris)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float ris = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->ris = ris);
}

MFUN(fog_get_dec)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dec;
}

MFUN(fog_set_dec)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dec = dec);
}

MFUN(fog_get_dur)
{
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dur;
}

MFUN(fog_set_dur)
{
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)o->ugen->ug;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dur = dur);
}

typedef struct
{
	sp_data* sp;
	sp_fold* osc;
} GW_fold;

TICK(fold_tick)
{
	GW_fold* ug = (GW_fold*)u->ug;
	base_tick(u);
	sp_fold_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(fold_ctor)
{
	GW_fold* ug = malloc(sizeof(GW_fold));
	ug->sp = shred->vm_ref->bbq->sp;
sp_fold_create(&ug->osc);
sp_fold_init(ug->sp, ug->osc);
	o->ugen->tick = fold_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(fold_dtor)
{
	GW_fold* ug = o->ugen->ug;
	sp_fold_destroy(&ug->osc);
	free(ug);
}

MFUN(fold_get_incr)
{
	GW_fold* ug = (GW_fold*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->incr;
}

MFUN(fold_set_incr)
{
	m_uint gw_offset = SZ_INT;
	GW_fold* ug = (GW_fold*)o->ugen->ug;
	m_float incr = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->incr = incr);
}

typedef struct
{
	sp_data* sp;
	sp_fosc* osc;
	m_bool is_init;
} GW_fosc;

TICK(fosc_tick)
{
	GW_fosc* ug = (GW_fosc*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_fosc_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(fosc_ctor)
{
	GW_fosc* ug = malloc(sizeof(GW_fosc));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = fosc_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(fosc_dtor)
{
	GW_fosc* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_fosc_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(fosc_init)
{
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	if(ug->osc) {
		sp_fosc_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object tbl_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* tbl = FTBL(tbl_obj);
	release(tbl_obj, shred);
	if(sp_fosc_create(&ug->osc) == SP_NOT_OK || sp_fosc_init(ug->sp, ug->osc, tbl) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(fosc_get_freq)
{
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(fosc_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(fosc_get_amp)
{
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(fosc_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(fosc_get_car)
{
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->car;
}

MFUN(fosc_set_car)
{
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	m_float car = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->car = car);
}

MFUN(fosc_get_mod)
{
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->mod;
}

MFUN(fosc_set_mod)
{
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	m_float mod = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->mod = mod);
}

MFUN(fosc_get_indx)
{
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->indx;
}

MFUN(fosc_set_indx)
{
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)o->ugen->ug;
	m_float indx = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->indx = indx);
}

typedef struct
{
	sp_data* sp;
	sp_gbuzz* osc;
	m_bool is_init;
} GW_gbuzz;

TICK(gbuzz_tick)
{
	GW_gbuzz* ug = (GW_gbuzz*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_gbuzz_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(gbuzz_ctor)
{
	GW_gbuzz* ug = malloc(sizeof(GW_gbuzz));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = gbuzz_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(gbuzz_dtor)
{
	GW_gbuzz* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_gbuzz_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(gbuzz_init)
{
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	if(ug->osc) {
		sp_gbuzz_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* ft = FTBL(ft_obj);
	release(ft_obj, shred);
	gw_offset +=SZ_INT;
	m_float iphs = *(m_float*)(shred->mem + gw_offset);
	if(sp_gbuzz_create(&ug->osc) == SP_NOT_OK || sp_gbuzz_init(ug->sp, ug->osc, ft, iphs) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(gbuzz_get_freq)
{
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(gbuzz_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(gbuzz_get_amp)
{
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(gbuzz_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(gbuzz_get_nharm)
{
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->nharm;
}

MFUN(gbuzz_set_nharm)
{
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	m_float nharm = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->nharm = nharm);
}

MFUN(gbuzz_get_lharm)
{
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->lharm;
}

MFUN(gbuzz_set_lharm)
{
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	m_float lharm = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->lharm = lharm);
}

MFUN(gbuzz_get_mul)
{
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->mul;
}

MFUN(gbuzz_set_mul)
{
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)o->ugen->ug;
	m_float mul = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->mul = mul);
}

MFUN(ftbl_gen_composite)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_composite(shred->vm_ref->bbq->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_file)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str filename = STRING(filename_obj);
	release(filename_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_file(shred->vm_ref->bbq->sp, ftbl, filename);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_gauss)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	m_float scale = *(m_float*)(shred->mem + gw_offset);
	gw_offset +=SZ_FLOAT;
	m_int seed = *(m_int*)(shred->mem + gw_offset);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_gauss(shred->vm_ref->bbq->sp, ftbl, scale, seed);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_line)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_line(shred->vm_ref->bbq->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_padsynth)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object amps_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* amps = FTBL(amps_obj);
	release(amps_obj, shred);
	gw_offset +=SZ_INT;
	m_float f = *(m_float*)(shred->mem + gw_offset);
	gw_offset +=SZ_FLOAT;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_padsynth(shred->vm_ref->bbq->sp, ftbl, amps, f, bw);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_rand)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_rand(shred->vm_ref->bbq->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_scrambler)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object dest_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl** dest = &FTBL(dest_obj);
	release(dest_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_scrambler(shred->vm_ref->bbq->sp, ftbl, dest);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_sine)
{
	sp_ftbl* ftbl = FTBL(o);
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_sine(shred->vm_ref->bbq->sp, ftbl);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_sinesum)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_sinesum(shred->vm_ref->bbq->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_xline)
{
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->bbq->sp, &ftbl, size);
	sp_gen_xline(shred->vm_ref->bbq->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

typedef struct
{
	sp_data* sp;
	sp_hilbert* osc;
} GW_hilbert;

TICK(hilbert_tick)
{
	GW_hilbert* ug = (GW_hilbert*)u->ug;
	sp_hilbert_compute(ug->sp, ug->osc, &u->in, &u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
	return 1;
}

CTOR(hilbert_ctor)
{
	GW_hilbert* ug = malloc(sizeof(GW_hilbert));
	ug->sp = shred->vm_ref->bbq->sp;
sp_hilbert_create(&ug->osc);
sp_hilbert_init(ug->sp, ug->osc);
	o->ugen->tick = hilbert_tick;
	assign_ugen(o->ugen, 1, 2, 0, ug);
}

DTOR(hilbert_dtor)
{
	GW_hilbert* ug = o->ugen->ug;
	sp_hilbert_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_in* osc;
} GW_in;

TICK(in_tick)
{
	GW_in* ug = (GW_in*)u->ug;
	sp_in_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(in_ctor)
{
	GW_in* ug = malloc(sizeof(GW_in));
	ug->sp = shred->vm_ref->bbq->sp;
sp_in_create(&ug->osc);
sp_in_init(ug->sp, ug->osc);
	o->ugen->tick = in_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(in_dtor)
{
	GW_in* ug = o->ugen->ug;
	sp_in_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_incr* osc;
	m_bool is_init;
} GW_incr;

TICK(incr_tick)
{
	GW_incr* ug = (GW_incr*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_incr_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(incr_ctor)
{
	GW_incr* ug = malloc(sizeof(GW_incr));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = incr_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(incr_dtor)
{
	GW_incr* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_incr_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(incr_init)
{
	m_uint gw_offset = SZ_INT;
	GW_incr* ug = (GW_incr*)o->ugen->ug;
	if(ug->osc) {
		sp_incr_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float val = *(m_float*)(shred->mem + gw_offset);
	if(sp_incr_create(&ug->osc) == SP_NOT_OK || sp_incr_init(ug->sp, ug->osc, val) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(incr_get_step)
{
	GW_incr* ug = (GW_incr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->step;
}

MFUN(incr_set_step)
{
	m_uint gw_offset = SZ_INT;
	GW_incr* ug = (GW_incr*)o->ugen->ug;
	m_float step = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->step = step);
}

MFUN(incr_get_min)
{
	GW_incr* ug = (GW_incr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->min;
}

MFUN(incr_set_min)
{
	m_uint gw_offset = SZ_INT;
	GW_incr* ug = (GW_incr*)o->ugen->ug;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->min = min);
}

MFUN(incr_get_max)
{
	GW_incr* ug = (GW_incr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->max;
}

MFUN(incr_set_max)
{
	m_uint gw_offset = SZ_INT;
	GW_incr* ug = (GW_incr*)o->ugen->ug;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->max = max);
}

typedef struct
{
	sp_data* sp;
	sp_jcrev* osc;
} GW_jcrev;

TICK(jcrev_tick)
{
	GW_jcrev* ug = (GW_jcrev*)u->ug;
	base_tick(u);
	sp_jcrev_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(jcrev_ctor)
{
	GW_jcrev* ug = malloc(sizeof(GW_jcrev));
	ug->sp = shred->vm_ref->bbq->sp;
sp_jcrev_create(&ug->osc);
sp_jcrev_init(ug->sp, ug->osc);
	o->ugen->tick = jcrev_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(jcrev_dtor)
{
	GW_jcrev* ug = o->ugen->ug;
	sp_jcrev_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_jitter* osc;
} GW_jitter;

TICK(jitter_tick)
{
	GW_jitter* ug = (GW_jitter*)u->ug;
	sp_jitter_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(jitter_ctor)
{
	GW_jitter* ug = malloc(sizeof(GW_jitter));
	ug->sp = shred->vm_ref->bbq->sp;
sp_jitter_create(&ug->osc);
sp_jitter_init(ug->sp, ug->osc);
	o->ugen->tick = jitter_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(jitter_dtor)
{
	GW_jitter* ug = o->ugen->ug;
	sp_jitter_destroy(&ug->osc);
	free(ug);
}

MFUN(jitter_get_amp)
{
	GW_jitter* ug = (GW_jitter*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(jitter_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_jitter* ug = (GW_jitter*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(jitter_get_cpsMin)
{
	GW_jitter* ug = (GW_jitter*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->cpsMin;
}

MFUN(jitter_set_cpsMin)
{
	m_uint gw_offset = SZ_INT;
	GW_jitter* ug = (GW_jitter*)o->ugen->ug;
	m_float cpsMin = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->cpsMin = cpsMin);
}

MFUN(jitter_get_cpsMax)
{
	GW_jitter* ug = (GW_jitter*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->cpsMax;
}

MFUN(jitter_set_cpsMax)
{
	m_uint gw_offset = SZ_INT;
	GW_jitter* ug = (GW_jitter*)o->ugen->ug;
	m_float cpsMax = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->cpsMax = cpsMax);
}

typedef struct
{
	sp_data* sp;
	sp_line* osc;
} GW_line;

TICK(line_tick)
{
	GW_line* ug = (GW_line*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_line_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(line_ctor)
{
	GW_line* ug = malloc(sizeof(GW_line));
	ug->sp = shred->vm_ref->bbq->sp;
sp_line_create(&ug->osc);
sp_line_init(ug->sp, ug->osc);
	o->ugen->tick = line_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(line_dtor)
{
	GW_line* ug = o->ugen->ug;
	sp_line_destroy(&ug->osc);
	free(ug);
}

MFUN(line_get_a)
{
	GW_line* ug = (GW_line*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->a;
}

MFUN(line_set_a)
{
	m_uint gw_offset = SZ_INT;
	GW_line* ug = (GW_line*)o->ugen->ug;
	m_float a = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->a = a);
}

MFUN(line_get_dur)
{
	GW_line* ug = (GW_line*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dur;
}

MFUN(line_set_dur)
{
	m_uint gw_offset = SZ_INT;
	GW_line* ug = (GW_line*)o->ugen->ug;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dur = dur);
}

MFUN(line_get_b)
{
	GW_line* ug = (GW_line*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->b;
}

MFUN(line_set_b)
{
	m_uint gw_offset = SZ_INT;
	GW_line* ug = (GW_line*)o->ugen->ug;
	m_float b = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->b = b);
}

typedef struct
{
	sp_data* sp;
	sp_lpf18* osc;
} GW_lpf18;

TICK(lpf18_tick)
{
	GW_lpf18* ug = (GW_lpf18*)u->ug;
	base_tick(u);
	sp_lpf18_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(lpf18_ctor)
{
	GW_lpf18* ug = malloc(sizeof(GW_lpf18));
	ug->sp = shred->vm_ref->bbq->sp;
sp_lpf18_create(&ug->osc);
sp_lpf18_init(ug->sp, ug->osc);
	o->ugen->tick = lpf18_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(lpf18_dtor)
{
	GW_lpf18* ug = o->ugen->ug;
	sp_lpf18_destroy(&ug->osc);
	free(ug);
}

MFUN(lpf18_get_cutoff)
{
	GW_lpf18* ug = (GW_lpf18*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->cutoff;
}

MFUN(lpf18_set_cutoff)
{
	m_uint gw_offset = SZ_INT;
	GW_lpf18* ug = (GW_lpf18*)o->ugen->ug;
	m_float cutoff = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->cutoff = cutoff);
}

MFUN(lpf18_get_res)
{
	GW_lpf18* ug = (GW_lpf18*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->res;
}

MFUN(lpf18_set_res)
{
	m_uint gw_offset = SZ_INT;
	GW_lpf18* ug = (GW_lpf18*)o->ugen->ug;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->res = res);
}

MFUN(lpf18_get_dist)
{
	GW_lpf18* ug = (GW_lpf18*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dist;
}

MFUN(lpf18_set_dist)
{
	m_uint gw_offset = SZ_INT;
	GW_lpf18* ug = (GW_lpf18*)o->ugen->ug;
	m_float dist = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dist = dist);
}

typedef struct
{
	sp_data* sp;
	sp_maygate* osc;
} GW_maygate;

TICK(maygate_tick)
{
	GW_maygate* ug = (GW_maygate*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_maygate_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(maygate_ctor)
{
	GW_maygate* ug = malloc(sizeof(GW_maygate));
	ug->sp = shred->vm_ref->bbq->sp;
sp_maygate_create(&ug->osc);
sp_maygate_init(ug->sp, ug->osc);
	o->ugen->tick = maygate_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(maygate_dtor)
{
	GW_maygate* ug = o->ugen->ug;
	sp_maygate_destroy(&ug->osc);
	free(ug);
}

MFUN(maygate_get_prob)
{
	GW_maygate* ug = (GW_maygate*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->prob;
}

MFUN(maygate_set_prob)
{
	m_uint gw_offset = SZ_INT;
	GW_maygate* ug = (GW_maygate*)o->ugen->ug;
	m_float prob = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->prob = prob);
}

MFUN(maygate_get_mode)
{
	GW_maygate* ug = (GW_maygate*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->mode;
}

MFUN(maygate_set_mode)
{
	m_uint gw_offset = SZ_INT;
	GW_maygate* ug = (GW_maygate*)o->ugen->ug;
	m_int mode = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->mode = mode);
}

typedef struct
{
	sp_data* sp;
	sp_metro* osc;
} GW_metro;

TICK(metro_tick)
{
	GW_metro* ug = (GW_metro*)u->ug;
	sp_metro_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(metro_ctor)
{
	GW_metro* ug = malloc(sizeof(GW_metro));
	ug->sp = shred->vm_ref->bbq->sp;
sp_metro_create(&ug->osc);
sp_metro_init(ug->sp, ug->osc);
	o->ugen->tick = metro_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(metro_dtor)
{
	GW_metro* ug = o->ugen->ug;
	sp_metro_destroy(&ug->osc);
	free(ug);
}

MFUN(metro_get_freq)
{
	GW_metro* ug = (GW_metro*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(metro_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_metro* ug = (GW_metro*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

typedef struct
{
	sp_data* sp;
	sp_mincer* osc;
	m_bool is_init;
} GW_mincer;

TICK(mincer_tick)
{
	GW_mincer* ug = (GW_mincer*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_mincer_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(mincer_ctor)
{
	GW_mincer* ug = malloc(sizeof(GW_mincer));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = mincer_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(mincer_dtor)
{
	GW_mincer* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_mincer_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(mincer_init)
{
	m_uint gw_offset = SZ_INT;
	GW_mincer* ug = (GW_mincer*)o->ugen->ug;
	if(ug->osc) {
		sp_mincer_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* ft = FTBL(ft_obj);
	release(ft_obj, shred);
	gw_offset +=SZ_INT;
	m_int winsize = *(m_int*)(shred->mem + gw_offset);
	if(sp_mincer_create(&ug->osc) == SP_NOT_OK || sp_mincer_init(ug->sp, ug->osc, ft, winsize) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(mincer_get_time)
{
	GW_mincer* ug = (GW_mincer*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->time;
}

MFUN(mincer_set_time)
{
	m_uint gw_offset = SZ_INT;
	GW_mincer* ug = (GW_mincer*)o->ugen->ug;
	m_float time = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->time = time);
}

MFUN(mincer_get_amp)
{
	GW_mincer* ug = (GW_mincer*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(mincer_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_mincer* ug = (GW_mincer*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(mincer_get_pitch)
{
	GW_mincer* ug = (GW_mincer*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->pitch;
}

MFUN(mincer_set_pitch)
{
	m_uint gw_offset = SZ_INT;
	GW_mincer* ug = (GW_mincer*)o->ugen->ug;
	m_float pitch = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->pitch = pitch);
}

typedef struct
{
	sp_data* sp;
	sp_mode* osc;
} GW_mode;

TICK(mode_tick)
{
	GW_mode* ug = (GW_mode*)u->ug;
	base_tick(u);
	sp_mode_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(mode_ctor)
{
	GW_mode* ug = malloc(sizeof(GW_mode));
	ug->sp = shred->vm_ref->bbq->sp;
sp_mode_create(&ug->osc);
sp_mode_init(ug->sp, ug->osc);
	o->ugen->tick = mode_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(mode_dtor)
{
	GW_mode* ug = o->ugen->ug;
	sp_mode_destroy(&ug->osc);
	free(ug);
}

MFUN(mode_get_freq)
{
	GW_mode* ug = (GW_mode*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(mode_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_mode* ug = (GW_mode*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(mode_get_q)
{
	GW_mode* ug = (GW_mode*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->q;
}

MFUN(mode_set_q)
{
	m_uint gw_offset = SZ_INT;
	GW_mode* ug = (GW_mode*)o->ugen->ug;
	m_float q = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->q = q);
}

typedef struct
{
	sp_data* sp;
	sp_moogladder* osc;
} GW_moogladder;

TICK(moogladder_tick)
{
	GW_moogladder* ug = (GW_moogladder*)u->ug;
	base_tick(u);
	sp_moogladder_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(moogladder_ctor)
{
	GW_moogladder* ug = malloc(sizeof(GW_moogladder));
	ug->sp = shred->vm_ref->bbq->sp;
sp_moogladder_create(&ug->osc);
sp_moogladder_init(ug->sp, ug->osc);
	o->ugen->tick = moogladder_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(moogladder_dtor)
{
	GW_moogladder* ug = o->ugen->ug;
	sp_moogladder_destroy(&ug->osc);
	free(ug);
}

MFUN(moogladder_get_freq)
{
	GW_moogladder* ug = (GW_moogladder*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(moogladder_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_moogladder* ug = (GW_moogladder*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(moogladder_get_res)
{
	GW_moogladder* ug = (GW_moogladder*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->res;
}

MFUN(moogladder_set_res)
{
	m_uint gw_offset = SZ_INT;
	GW_moogladder* ug = (GW_moogladder*)o->ugen->ug;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->res = res);
}

typedef struct
{
	sp_data* sp;
	sp_noise* osc;
} GW_noise;

TICK(noise_tick)
{
	GW_noise* ug = (GW_noise*)u->ug;
	sp_noise_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(noise_ctor)
{
	GW_noise* ug = malloc(sizeof(GW_noise));
	ug->sp = shred->vm_ref->bbq->sp;
sp_noise_create(&ug->osc);
sp_noise_init(ug->sp, ug->osc);
	o->ugen->tick = noise_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(noise_dtor)
{
	GW_noise* ug = o->ugen->ug;
	sp_noise_destroy(&ug->osc);
	free(ug);
}

MFUN(noise_get_amp)
{
	GW_noise* ug = (GW_noise*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(noise_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_noise* ug = (GW_noise*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

typedef struct
{
	sp_data* sp;
	sp_nsmp* osc;
	m_bool is_init;
} GW_nsmp;

TICK(nsmp_tick)
{
	GW_nsmp* ug = (GW_nsmp*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_nsmp_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(nsmp_ctor)
{
	GW_nsmp* ug = malloc(sizeof(GW_nsmp));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = nsmp_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(nsmp_dtor)
{
	GW_nsmp* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_nsmp_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(nsmp_init)
{
	m_uint gw_offset = SZ_INT;
	GW_nsmp* ug = (GW_nsmp*)o->ugen->ug;
	if(ug->osc) {
		sp_nsmp_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* ft = FTBL(ft_obj);
	release(ft_obj, shred);
	gw_offset +=SZ_INT;
	m_int sr = *(m_int*)(shred->mem + gw_offset);
	gw_offset +=SZ_INT;
	M_Object init_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str init = STRING(init_obj);
	release(init_obj, shred);
	if(sp_nsmp_create(&ug->osc) == SP_NOT_OK || sp_nsmp_init(ug->sp, ug->osc, ft, sr, init) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(nsmp_get_index)
{
	GW_nsmp* ug = (GW_nsmp*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->index;
}

MFUN(nsmp_set_index)
{
	m_uint gw_offset = SZ_INT;
	GW_nsmp* ug = (GW_nsmp*)o->ugen->ug;
	m_int index = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->index = index);
}

typedef struct
{
	sp_data* sp;
	sp_osc* osc;
	m_bool is_init;
} GW_osc;

TICK(osc_tick)
{
	GW_osc* ug = (GW_osc*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_osc_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(osc_ctor)
{
	GW_osc* ug = malloc(sizeof(GW_osc));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = osc_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(osc_dtor)
{
	GW_osc* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_osc_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(osc_init)
{
	m_uint gw_offset = SZ_INT;
	GW_osc* ug = (GW_osc*)o->ugen->ug;
	if(ug->osc) {
		sp_osc_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object tbl_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* tbl = FTBL(tbl_obj);
	release(tbl_obj, shred);
	gw_offset +=SZ_INT;
	m_float phase = *(m_float*)(shred->mem + gw_offset);
	if(sp_osc_create(&ug->osc) == SP_NOT_OK || sp_osc_init(ug->sp, ug->osc, tbl, phase) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(osc_get_freq)
{
	GW_osc* ug = (GW_osc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(osc_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_osc* ug = (GW_osc*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(osc_get_amp)
{
	GW_osc* ug = (GW_osc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(osc_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_osc* ug = (GW_osc*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

typedef struct
{
	sp_data* sp;
	sp_oscmorph* osc;
	m_bool is_init;
	sp_ftbl** tbl;

} GW_oscmorph;

TICK(oscmorph_tick)
{
	GW_oscmorph* ug = (GW_oscmorph*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_oscmorph_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(oscmorph_ctor)
{
	GW_oscmorph* ug = malloc(sizeof(GW_oscmorph));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = oscmorph_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(oscmorph_dtor)
{
	GW_oscmorph* ug = o->ugen->ug;
	if(ug->is_init) {

		free(ug->osc->tbl);

		sp_oscmorph_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(oscmorph_init)
{
	m_uint gw_offset = SZ_INT;
	GW_oscmorph* ug = (GW_oscmorph*)o->ugen->ug;
	if(ug->osc) {
		sp_oscmorph_destroy(&ug->osc);
		free(ug->tbl);
		ug->osc = NULL;
	}
	M_Object tbl_ptr = *(M_Object*)(shred->mem + gw_offset);
	m_uint tbl_iter;
	sp_ftbl** tbl = malloc(m_vector_size(tbl_ptr->d.array) * SZ_INT);
	for(tbl_iter = 0; tbl_iter < m_vector_size(tbl_ptr->d.array); tbl_iter++)
		tbl[tbl_iter] = FTBL((M_Object)i_vector_at(tbl_ptr->d.array, tbl_iter));
	release(tbl_ptr, shred);
	gw_offset +=SZ_INT;
	m_int nft = *(m_int*)(shred->mem + gw_offset);
	gw_offset +=SZ_INT;
	m_float phase = *(m_float*)(shred->mem + gw_offset);
	if(sp_oscmorph_create(&ug->osc) == SP_NOT_OK || sp_oscmorph_init(ug->sp, ug->osc, tbl, nft, phase) == SP_NOT_OK) {
		free(tbl); // LCOV_EXCL_LINE
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->tbl = tbl;
	ug->is_init = 1;
}

MFUN(oscmorph_get_freq)
{
	GW_oscmorph* ug = (GW_oscmorph*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(oscmorph_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_oscmorph* ug = (GW_oscmorph*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(oscmorph_get_amp)
{
	GW_oscmorph* ug = (GW_oscmorph*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(oscmorph_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_oscmorph* ug = (GW_oscmorph*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

MFUN(oscmorph_get_wtpos)
{
	GW_oscmorph* ug = (GW_oscmorph*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->wtpos;
}

MFUN(oscmorph_set_wtpos)
{
	m_uint gw_offset = SZ_INT;
	GW_oscmorph* ug = (GW_oscmorph*)o->ugen->ug;
	m_float wtpos = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->wtpos = wtpos);
}

typedef struct
{
	sp_data* sp;
	sp_pan2* osc;
} GW_pan2;

TICK(pan2_tick)
{
	GW_pan2* ug = (GW_pan2*)u->ug;
	sp_pan2_compute(ug->sp, ug->osc, &u->in, &u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
	return 1;
}

CTOR(pan2_ctor)
{
	GW_pan2* ug = malloc(sizeof(GW_pan2));
	ug->sp = shred->vm_ref->bbq->sp;
sp_pan2_create(&ug->osc);
sp_pan2_init(ug->sp, ug->osc);
	o->ugen->tick = pan2_tick;
	assign_ugen(o->ugen, 1, 2, 0, ug);
}

DTOR(pan2_dtor)
{
	GW_pan2* ug = o->ugen->ug;
	sp_pan2_destroy(&ug->osc);
	free(ug);
}

MFUN(pan2_get_type)
{
	GW_pan2* ug = (GW_pan2*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->type;
}

MFUN(pan2_set_type)
{
	m_uint gw_offset = SZ_INT;
	GW_pan2* ug = (GW_pan2*)o->ugen->ug;
	m_int type = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->type = type);
}

MFUN(pan2_get_pan)
{
	GW_pan2* ug = (GW_pan2*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->pan;
}

MFUN(pan2_set_pan)
{
	m_uint gw_offset = SZ_INT;
	GW_pan2* ug = (GW_pan2*)o->ugen->ug;
	m_float pan = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->pan = pan);
}

typedef struct
{
	sp_data* sp;
	sp_panst* osc;
} GW_panst;

TICK(panst_tick)
{
	GW_panst* ug = (GW_panst*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	sp_panst_compute(ug->sp, ug->osc, &u->channel[0]->ugen->in, &u->channel[1]->ugen->in, &u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
	return 1;
}

CTOR(panst_ctor)
{
	GW_panst* ug = malloc(sizeof(GW_panst));
	ug->sp = shred->vm_ref->bbq->sp;
sp_panst_create(&ug->osc);
sp_panst_init(ug->sp, ug->osc);
	o->ugen->tick = panst_tick;
	assign_ugen(o->ugen, 2, 2, 0, ug);
}

DTOR(panst_dtor)
{
	GW_panst* ug = o->ugen->ug;
	sp_panst_destroy(&ug->osc);
	free(ug);
}

MFUN(panst_get_type)
{
	GW_panst* ug = (GW_panst*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->type;
}

MFUN(panst_set_type)
{
	m_uint gw_offset = SZ_INT;
	GW_panst* ug = (GW_panst*)o->ugen->ug;
	m_int type = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->type = type);
}

MFUN(panst_get_pan)
{
	GW_panst* ug = (GW_panst*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->pan;
}

MFUN(panst_set_pan)
{
	m_uint gw_offset = SZ_INT;
	GW_panst* ug = (GW_panst*)o->ugen->ug;
	m_float pan = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->pan = pan);
}

typedef struct
{
	sp_data* sp;
	sp_pareq* osc;
} GW_pareq;

TICK(pareq_tick)
{
	GW_pareq* ug = (GW_pareq*)u->ug;
	base_tick(u);
	sp_pareq_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(pareq_ctor)
{
	GW_pareq* ug = malloc(sizeof(GW_pareq));
	ug->sp = shred->vm_ref->bbq->sp;
sp_pareq_create(&ug->osc);
sp_pareq_init(ug->sp, ug->osc);
	o->ugen->tick = pareq_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(pareq_dtor)
{
	GW_pareq* ug = o->ugen->ug;
	sp_pareq_destroy(&ug->osc);
	free(ug);
}

MFUN(pareq_get_fc)
{
	GW_pareq* ug = (GW_pareq*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->fc;
}

MFUN(pareq_set_fc)
{
	m_uint gw_offset = SZ_INT;
	GW_pareq* ug = (GW_pareq*)o->ugen->ug;
	m_float fc = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->fc = fc);
}

MFUN(pareq_get_v)
{
	GW_pareq* ug = (GW_pareq*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->v;
}

MFUN(pareq_set_v)
{
	m_uint gw_offset = SZ_INT;
	GW_pareq* ug = (GW_pareq*)o->ugen->ug;
	m_float v = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->v = v);
}

MFUN(pareq_get_q)
{
	GW_pareq* ug = (GW_pareq*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->q;
}

MFUN(pareq_set_q)
{
	m_uint gw_offset = SZ_INT;
	GW_pareq* ug = (GW_pareq*)o->ugen->ug;
	m_float q = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->q = q);
}

MFUN(pareq_get_mode)
{
	GW_pareq* ug = (GW_pareq*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->mode;
}

MFUN(pareq_set_mode)
{
	m_uint gw_offset = SZ_INT;
	GW_pareq* ug = (GW_pareq*)o->ugen->ug;
	m_float mode = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->mode = mode);
}

typedef struct
{
	sp_data* sp;
	sp_paulstretch* osc;
	m_bool is_init;
} GW_paulstretch;

TICK(paulstretch_tick)
{
	GW_paulstretch* ug = (GW_paulstretch*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_paulstretch_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(paulstretch_ctor)
{
	GW_paulstretch* ug = malloc(sizeof(GW_paulstretch));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = paulstretch_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(paulstretch_dtor)
{
	GW_paulstretch* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_paulstretch_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(paulstretch_init)
{
	m_uint gw_offset = SZ_INT;
	GW_paulstretch* ug = (GW_paulstretch*)o->ugen->ug;
	if(ug->osc) {
		sp_paulstretch_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* ft = FTBL(ft_obj);
	release(ft_obj, shred);
	gw_offset +=SZ_INT;
	m_float windowsize = *(m_float*)(shred->mem + gw_offset);
	gw_offset +=SZ_FLOAT;
	m_float stretch = *(m_float*)(shred->mem + gw_offset);
	if(sp_paulstretch_create(&ug->osc) == SP_NOT_OK || sp_paulstretch_init(ug->sp, ug->osc, ft, windowsize, stretch) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_pdhalf* osc;
} GW_pdhalf;

TICK(pdhalf_tick)
{
	GW_pdhalf* ug = (GW_pdhalf*)u->ug;
	base_tick(u);
	sp_pdhalf_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(pdhalf_ctor)
{
	GW_pdhalf* ug = malloc(sizeof(GW_pdhalf));
	ug->sp = shred->vm_ref->bbq->sp;
sp_pdhalf_create(&ug->osc);
sp_pdhalf_init(ug->sp, ug->osc);
	o->ugen->tick = pdhalf_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(pdhalf_dtor)
{
	GW_pdhalf* ug = o->ugen->ug;
	sp_pdhalf_destroy(&ug->osc);
	free(ug);
}

MFUN(pdhalf_get_amount)
{
	GW_pdhalf* ug = (GW_pdhalf*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amount;
}

MFUN(pdhalf_set_amount)
{
	m_uint gw_offset = SZ_INT;
	GW_pdhalf* ug = (GW_pdhalf*)o->ugen->ug;
	m_float amount = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amount = amount);
}

typedef struct
{
	sp_data* sp;
	sp_peaklim* osc;
} GW_peaklim;

TICK(peaklim_tick)
{
	GW_peaklim* ug = (GW_peaklim*)u->ug;
	base_tick(u);
	sp_peaklim_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(peaklim_ctor)
{
	GW_peaklim* ug = malloc(sizeof(GW_peaklim));
	ug->sp = shred->vm_ref->bbq->sp;
sp_peaklim_create(&ug->osc);
sp_peaklim_init(ug->sp, ug->osc);
	o->ugen->tick = peaklim_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(peaklim_dtor)
{
	GW_peaklim* ug = o->ugen->ug;
	sp_peaklim_destroy(&ug->osc);
	free(ug);
}

MFUN(peaklim_get_atk)
{
	GW_peaklim* ug = (GW_peaklim*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->atk;
}

MFUN(peaklim_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_peaklim* ug = (GW_peaklim*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->atk = atk);
}

MFUN(peaklim_get_rel)
{
	GW_peaklim* ug = (GW_peaklim*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->rel;
}

MFUN(peaklim_set_rel)
{
	m_uint gw_offset = SZ_INT;
	GW_peaklim* ug = (GW_peaklim*)o->ugen->ug;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->rel = rel);
}

MFUN(peaklim_get_thresh)
{
	GW_peaklim* ug = (GW_peaklim*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->thresh;
}

MFUN(peaklim_set_thresh)
{
	m_uint gw_offset = SZ_INT;
	GW_peaklim* ug = (GW_peaklim*)o->ugen->ug;
	m_float thresh = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->thresh = thresh);
}

typedef struct
{
	sp_data* sp;
	sp_phaser* osc;
} GW_phaser;

TICK(phaser_tick)
{
	GW_phaser* ug = (GW_phaser*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	sp_phaser_compute(ug->sp, ug->osc, &u->channel[0]->ugen->in, &u->channel[1]->ugen->in, &u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
	return 1;
}

CTOR(phaser_ctor)
{
	GW_phaser* ug = malloc(sizeof(GW_phaser));
	ug->sp = shred->vm_ref->bbq->sp;
sp_phaser_create(&ug->osc);
sp_phaser_init(ug->sp, ug->osc);
	o->ugen->tick = phaser_tick;
	assign_ugen(o->ugen, 2, 2, 0, ug);
}

DTOR(phaser_dtor)
{
	GW_phaser* ug = o->ugen->ug;
	sp_phaser_destroy(&ug->osc);
	free(ug);
}

MFUN(phaser_get_MaxNotch1Freq)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->MaxNotch1Freq;
}

MFUN(phaser_set_MaxNotch1Freq)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float MaxNotch1Freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->MaxNotch1Freq = MaxNotch1Freq);
}

MFUN(phaser_get_MinNotch1Freq)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->MinNotch1Freq;
}

MFUN(phaser_set_MinNotch1Freq)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float MinNotch1Freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->MinNotch1Freq = MinNotch1Freq);
}

MFUN(phaser_get_Notch_width)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->Notch_width;
}

MFUN(phaser_set_Notch_width)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float Notch_width = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->Notch_width = Notch_width);
}

MFUN(phaser_get_NotchFreq)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->NotchFreq;
}

MFUN(phaser_set_NotchFreq)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float NotchFreq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->NotchFreq = NotchFreq);
}

MFUN(phaser_get_VibratoMode)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->VibratoMode;
}

MFUN(phaser_set_VibratoMode)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float VibratoMode = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->VibratoMode = VibratoMode);
}

MFUN(phaser_get_depth)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->depth;
}

MFUN(phaser_set_depth)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float depth = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->depth = depth);
}

MFUN(phaser_get_feedback_gain)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->feedback_gain;
}

MFUN(phaser_set_feedback_gain)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float feedback_gain = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->feedback_gain = feedback_gain);
}

MFUN(phaser_get_invert)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->invert;
}

MFUN(phaser_set_invert)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float invert = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->invert = invert);
}

MFUN(phaser_get_level)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->level;
}

MFUN(phaser_set_level)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float level = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->level = level);
}

MFUN(phaser_get_lfobpm)
{
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->lfobpm;
}

MFUN(phaser_set_lfobpm)
{
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)o->ugen->ug;
	m_float lfobpm = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->lfobpm = lfobpm);
}

typedef struct
{
	sp_data* sp;
	sp_phasor* osc;
	m_bool is_init;
} GW_phasor;

TICK(phasor_tick)
{
	GW_phasor* ug = (GW_phasor*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_phasor_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(phasor_ctor)
{
	GW_phasor* ug = malloc(sizeof(GW_phasor));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = phasor_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(phasor_dtor)
{
	GW_phasor* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_phasor_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(phasor_init)
{
	m_uint gw_offset = SZ_INT;
	GW_phasor* ug = (GW_phasor*)o->ugen->ug;
	if(ug->osc) {
		sp_phasor_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float iphs = *(m_float*)(shred->mem + gw_offset);
	if(sp_phasor_create(&ug->osc) == SP_NOT_OK || sp_phasor_init(ug->sp, ug->osc, iphs) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(phasor_get_freq)
{
	GW_phasor* ug = (GW_phasor*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(phasor_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_phasor* ug = (GW_phasor*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

typedef struct
{
	sp_data* sp;
	sp_pinknoise* osc;
} GW_pinknoise;

TICK(pinknoise_tick)
{
	GW_pinknoise* ug = (GW_pinknoise*)u->ug;
	sp_pinknoise_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(pinknoise_ctor)
{
	GW_pinknoise* ug = malloc(sizeof(GW_pinknoise));
	ug->sp = shred->vm_ref->bbq->sp;
sp_pinknoise_create(&ug->osc);
sp_pinknoise_init(ug->sp, ug->osc);
	o->ugen->tick = pinknoise_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(pinknoise_dtor)
{
	GW_pinknoise* ug = o->ugen->ug;
	sp_pinknoise_destroy(&ug->osc);
	free(ug);
}

MFUN(pinknoise_get_amp)
{
	GW_pinknoise* ug = (GW_pinknoise*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(pinknoise_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_pinknoise* ug = (GW_pinknoise*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

typedef struct
{
	sp_data* sp;
	sp_pitchamdf* osc;
	m_bool is_init;
} GW_pitchamdf;

TICK(pitchamdf_tick)
{
	GW_pitchamdf* ug = (GW_pitchamdf*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_pitchamdf_compute(ug->sp, ug->osc, &u->in, &u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
	return 1;
}

CTOR(pitchamdf_ctor)
{
	GW_pitchamdf* ug = malloc(sizeof(GW_pitchamdf));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = pitchamdf_tick;
	assign_ugen(o->ugen, 1, 2, 0, ug);
}

DTOR(pitchamdf_dtor)
{
	GW_pitchamdf* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_pitchamdf_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(pitchamdf_init)
{
	m_uint gw_offset = SZ_INT;
	GW_pitchamdf* ug = (GW_pitchamdf*)o->ugen->ug;
	if(ug->osc) {
		sp_pitchamdf_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float min = *(m_float*)(shred->mem + gw_offset);
	gw_offset +=SZ_FLOAT;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	if(sp_pitchamdf_create(&ug->osc) == SP_NOT_OK || sp_pitchamdf_init(ug->sp, ug->osc, min, max) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_pluck* osc;
	m_bool is_init;
} GW_pluck;

TICK(pluck_tick)
{
	GW_pluck* ug = (GW_pluck*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_pluck_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(pluck_ctor)
{
	GW_pluck* ug = malloc(sizeof(GW_pluck));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = pluck_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(pluck_dtor)
{
	GW_pluck* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_pluck_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(pluck_init)
{
	m_uint gw_offset = SZ_INT;
	GW_pluck* ug = (GW_pluck*)o->ugen->ug;
	if(ug->osc) {
		sp_pluck_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float ifreq = *(m_float*)(shred->mem + gw_offset);
	if(sp_pluck_create(&ug->osc) == SP_NOT_OK || sp_pluck_init(ug->sp, ug->osc, ifreq) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(pluck_get_freq)
{
	GW_pluck* ug = (GW_pluck*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(pluck_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_pluck* ug = (GW_pluck*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(pluck_get_amp)
{
	GW_pluck* ug = (GW_pluck*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(pluck_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_pluck* ug = (GW_pluck*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

typedef struct
{
	sp_data* sp;
	sp_port* osc;
	m_bool is_init;
} GW_port;

TICK(port_tick)
{
	GW_port* ug = (GW_port*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_port_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(port_ctor)
{
	GW_port* ug = malloc(sizeof(GW_port));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = port_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(port_dtor)
{
	GW_port* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_port_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(port_init)
{
	m_uint gw_offset = SZ_INT;
	GW_port* ug = (GW_port*)o->ugen->ug;
	if(ug->osc) {
		sp_port_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float htime = *(m_float*)(shred->mem + gw_offset);
	if(sp_port_create(&ug->osc) == SP_NOT_OK || sp_port_init(ug->sp, ug->osc, htime) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_posc3* osc;
	m_bool is_init;
} GW_posc3;

TICK(posc3_tick)
{
	GW_posc3* ug = (GW_posc3*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_posc3_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(posc3_ctor)
{
	GW_posc3* ug = malloc(sizeof(GW_posc3));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = posc3_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(posc3_dtor)
{
	GW_posc3* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_posc3_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(posc3_init)
{
	m_uint gw_offset = SZ_INT;
	GW_posc3* ug = (GW_posc3*)o->ugen->ug;
	if(ug->osc) {
		sp_posc3_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object tbl_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* tbl = FTBL(tbl_obj);
	release(tbl_obj, shred);
	if(sp_posc3_create(&ug->osc) == SP_NOT_OK || sp_posc3_init(ug->sp, ug->osc, tbl) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(posc3_get_freq)
{
	GW_posc3* ug = (GW_posc3*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(posc3_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_posc3* ug = (GW_posc3*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(posc3_get_amp)
{
	GW_posc3* ug = (GW_posc3*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->amp;
}

MFUN(posc3_set_amp)
{
	m_uint gw_offset = SZ_INT;
	GW_posc3* ug = (GW_posc3*)o->ugen->ug;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->amp = amp);
}

typedef struct
{
	sp_data* sp;
	sp_progress* osc;
} GW_progress;

TICK(progress_tick)
{
	GW_progress* ug = (GW_progress*)u->ug;
	sp_progress_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(progress_ctor)
{
	GW_progress* ug = malloc(sizeof(GW_progress));
	ug->sp = shred->vm_ref->bbq->sp;
sp_progress_create(&ug->osc);
sp_progress_init(ug->sp, ug->osc);
	o->ugen->tick = progress_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(progress_dtor)
{
	GW_progress* ug = o->ugen->ug;
	sp_progress_destroy(&ug->osc);
	free(ug);
}

MFUN(progress_get_nbars)
{
	GW_progress* ug = (GW_progress*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->nbars;
}

MFUN(progress_set_nbars)
{
	m_uint gw_offset = SZ_INT;
	GW_progress* ug = (GW_progress*)o->ugen->ug;
	m_int nbars = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->nbars = nbars);
}

MFUN(progress_get_skip)
{
	GW_progress* ug = (GW_progress*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->skip;
}

MFUN(progress_set_skip)
{
	m_uint gw_offset = SZ_INT;
	GW_progress* ug = (GW_progress*)o->ugen->ug;
	m_int skip = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->skip = skip);
}

typedef struct
{
	sp_data* sp;
	sp_prop* osc;
	m_bool is_init;
} GW_prop;

TICK(prop_tick)
{
	GW_prop* ug = (GW_prop*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_prop_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(prop_ctor)
{
	GW_prop* ug = malloc(sizeof(GW_prop));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = prop_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(prop_dtor)
{
	GW_prop* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_prop_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(prop_init)
{
	m_uint gw_offset = SZ_INT;
	GW_prop* ug = (GW_prop*)o->ugen->ug;
	if(ug->osc) {
		sp_prop_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object str_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str str = STRING(str_obj);
	release(str_obj, shred);
	if(sp_prop_create(&ug->osc) == SP_NOT_OK || sp_prop_init(ug->sp, ug->osc, str) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(prop_get_bpm)
{
	GW_prop* ug = (GW_prop*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bpm;
}

MFUN(prop_set_bpm)
{
	m_uint gw_offset = SZ_INT;
	GW_prop* ug = (GW_prop*)o->ugen->ug;
	m_float bpm = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bpm = bpm);
}

typedef struct
{
	sp_data* sp;
	sp_pshift* osc;
} GW_pshift;

TICK(pshift_tick)
{
	GW_pshift* ug = (GW_pshift*)u->ug;
	base_tick(u);
	sp_pshift_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(pshift_ctor)
{
	GW_pshift* ug = malloc(sizeof(GW_pshift));
	ug->sp = shred->vm_ref->bbq->sp;
sp_pshift_create(&ug->osc);
sp_pshift_init(ug->sp, ug->osc);
	o->ugen->tick = pshift_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(pshift_dtor)
{
	GW_pshift* ug = o->ugen->ug;
	sp_pshift_destroy(&ug->osc);
	free(ug);
}

MFUN(pshift_get_shift)
{
	GW_pshift* ug = (GW_pshift*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->shift;
}

MFUN(pshift_set_shift)
{
	m_uint gw_offset = SZ_INT;
	GW_pshift* ug = (GW_pshift*)o->ugen->ug;
	m_float shift = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->shift = shift);
}

MFUN(pshift_get_window)
{
	GW_pshift* ug = (GW_pshift*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->window;
}

MFUN(pshift_set_window)
{
	m_uint gw_offset = SZ_INT;
	GW_pshift* ug = (GW_pshift*)o->ugen->ug;
	m_float window = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->window = window);
}

MFUN(pshift_get_xfade)
{
	GW_pshift* ug = (GW_pshift*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->xfade;
}

MFUN(pshift_set_xfade)
{
	m_uint gw_offset = SZ_INT;
	GW_pshift* ug = (GW_pshift*)o->ugen->ug;
	m_float xfade = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->xfade = xfade);
}

typedef struct
{
	sp_data* sp;
	sp_ptrack* osc;
	m_bool is_init;
} GW_ptrack;

TICK(ptrack_tick)
{
	GW_ptrack* ug = (GW_ptrack*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_ptrack_compute(ug->sp, ug->osc, &u->in, &u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
	return 1;
}

CTOR(ptrack_ctor)
{
	GW_ptrack* ug = malloc(sizeof(GW_ptrack));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = ptrack_tick;
	assign_ugen(o->ugen, 1, 2, 0, ug);
}

DTOR(ptrack_dtor)
{
	GW_ptrack* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_ptrack_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(ptrack_init)
{
	m_uint gw_offset = SZ_INT;
	GW_ptrack* ug = (GW_ptrack*)o->ugen->ug;
	if(ug->osc) {
		sp_ptrack_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_int ihopsize = *(m_int*)(shred->mem + gw_offset);
	gw_offset +=SZ_INT;
	m_int ipeaks = *(m_int*)(shred->mem + gw_offset);
	if(sp_ptrack_create(&ug->osc) == SP_NOT_OK || sp_ptrack_init(ug->sp, ug->osc, ihopsize, ipeaks) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_randh* osc;
} GW_randh;

TICK(randh_tick)
{
	GW_randh* ug = (GW_randh*)u->ug;
	sp_randh_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(randh_ctor)
{
	GW_randh* ug = malloc(sizeof(GW_randh));
	ug->sp = shred->vm_ref->bbq->sp;
sp_randh_create(&ug->osc);
sp_randh_init(ug->sp, ug->osc);
	o->ugen->tick = randh_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(randh_dtor)
{
	GW_randh* ug = o->ugen->ug;
	sp_randh_destroy(&ug->osc);
	free(ug);
}

MFUN(randh_get_min)
{
	GW_randh* ug = (GW_randh*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->min;
}

MFUN(randh_set_min)
{
	m_uint gw_offset = SZ_INT;
	GW_randh* ug = (GW_randh*)o->ugen->ug;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->min = min);
}

MFUN(randh_get_max)
{
	GW_randh* ug = (GW_randh*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->max;
}

MFUN(randh_set_max)
{
	m_uint gw_offset = SZ_INT;
	GW_randh* ug = (GW_randh*)o->ugen->ug;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->max = max);
}

MFUN(randh_get_freq)
{
	GW_randh* ug = (GW_randh*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(randh_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_randh* ug = (GW_randh*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

typedef struct
{
	sp_data* sp;
	sp_randi* osc;
} GW_randi;

TICK(randi_tick)
{
	GW_randi* ug = (GW_randi*)u->ug;
	sp_randi_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(randi_ctor)
{
	GW_randi* ug = malloc(sizeof(GW_randi));
	ug->sp = shred->vm_ref->bbq->sp;
sp_randi_create(&ug->osc);
sp_randi_init(ug->sp, ug->osc);
	o->ugen->tick = randi_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(randi_dtor)
{
	GW_randi* ug = o->ugen->ug;
	sp_randi_destroy(&ug->osc);
	free(ug);
}

MFUN(randi_get_min)
{
	GW_randi* ug = (GW_randi*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->min;
}

MFUN(randi_set_min)
{
	m_uint gw_offset = SZ_INT;
	GW_randi* ug = (GW_randi*)o->ugen->ug;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->min = min);
}

MFUN(randi_get_max)
{
	GW_randi* ug = (GW_randi*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->max;
}

MFUN(randi_set_max)
{
	m_uint gw_offset = SZ_INT;
	GW_randi* ug = (GW_randi*)o->ugen->ug;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->max = max);
}

MFUN(randi_get_cps)
{
	GW_randi* ug = (GW_randi*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->cps;
}

MFUN(randi_set_cps)
{
	m_uint gw_offset = SZ_INT;
	GW_randi* ug = (GW_randi*)o->ugen->ug;
	m_float cps = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->cps = cps);
}

MFUN(randi_get_mode)
{
	GW_randi* ug = (GW_randi*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->mode;
}

MFUN(randi_set_mode)
{
	m_uint gw_offset = SZ_INT;
	GW_randi* ug = (GW_randi*)o->ugen->ug;
	m_float mode = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->mode = mode);
}

typedef struct
{
	sp_data* sp;
	sp_random* osc;
} GW_random;

TICK(random_tick)
{
	GW_random* ug = (GW_random*)u->ug;
	sp_random_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(random_ctor)
{
	GW_random* ug = malloc(sizeof(GW_random));
	ug->sp = shred->vm_ref->bbq->sp;
sp_random_create(&ug->osc);
sp_random_init(ug->sp, ug->osc);
	o->ugen->tick = random_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(random_dtor)
{
	GW_random* ug = o->ugen->ug;
	sp_random_destroy(&ug->osc);
	free(ug);
}

MFUN(random_get_min)
{
	GW_random* ug = (GW_random*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->min;
}

MFUN(random_set_min)
{
	m_uint gw_offset = SZ_INT;
	GW_random* ug = (GW_random*)o->ugen->ug;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->min = min);
}

MFUN(random_get_max)
{
	GW_random* ug = (GW_random*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->max;
}

MFUN(random_set_max)
{
	m_uint gw_offset = SZ_INT;
	GW_random* ug = (GW_random*)o->ugen->ug;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->max = max);
}

typedef struct
{
	sp_data* sp;
	sp_reson* osc;
} GW_reson;

TICK(reson_tick)
{
	GW_reson* ug = (GW_reson*)u->ug;
	base_tick(u);
	sp_reson_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(reson_ctor)
{
	GW_reson* ug = malloc(sizeof(GW_reson));
	ug->sp = shred->vm_ref->bbq->sp;
sp_reson_create(&ug->osc);
sp_reson_init(ug->sp, ug->osc);
	o->ugen->tick = reson_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(reson_dtor)
{
	GW_reson* ug = o->ugen->ug;
	sp_reson_destroy(&ug->osc);
	free(ug);
}

MFUN(reson_get_freq)
{
	GW_reson* ug = (GW_reson*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(reson_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_reson* ug = (GW_reson*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(reson_get_bw)
{
	GW_reson* ug = (GW_reson*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->bw;
}

MFUN(reson_set_bw)
{
	m_uint gw_offset = SZ_INT;
	GW_reson* ug = (GW_reson*)o->ugen->ug;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->bw = bw);
}

typedef struct
{
	sp_data* sp;
	sp_reverse* osc;
	m_bool is_init;
} GW_reverse;

TICK(reverse_tick)
{
	GW_reverse* ug = (GW_reverse*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_reverse_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(reverse_ctor)
{
	GW_reverse* ug = malloc(sizeof(GW_reverse));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = reverse_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(reverse_dtor)
{
	GW_reverse* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_reverse_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(reverse_init)
{
	m_uint gw_offset = SZ_INT;
	GW_reverse* ug = (GW_reverse*)o->ugen->ug;
	if(ug->osc) {
		sp_reverse_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float delay = *(m_float*)(shred->mem + gw_offset);
	if(sp_reverse_create(&ug->osc) == SP_NOT_OK || sp_reverse_init(ug->sp, ug->osc, delay) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_revsc* osc;
} GW_revsc;

TICK(revsc_tick)
{
	GW_revsc* ug = (GW_revsc*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	sp_revsc_compute(ug->sp, ug->osc, &u->channel[0]->ugen->in, &u->channel[1]->ugen->in, &u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
	return 1;
}

CTOR(revsc_ctor)
{
	GW_revsc* ug = malloc(sizeof(GW_revsc));
	ug->sp = shred->vm_ref->bbq->sp;
sp_revsc_create(&ug->osc);
sp_revsc_init(ug->sp, ug->osc);
	o->ugen->tick = revsc_tick;
	assign_ugen(o->ugen, 2, 2, 0, ug);
}

DTOR(revsc_dtor)
{
	GW_revsc* ug = o->ugen->ug;
	sp_revsc_destroy(&ug->osc);
	free(ug);
}

MFUN(revsc_get_feedback)
{
	GW_revsc* ug = (GW_revsc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->feedback;
}

MFUN(revsc_set_feedback)
{
	m_uint gw_offset = SZ_INT;
	GW_revsc* ug = (GW_revsc*)o->ugen->ug;
	m_float feedback = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->feedback = feedback);
}

MFUN(revsc_get_lpfreq)
{
	GW_revsc* ug = (GW_revsc*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->lpfreq;
}

MFUN(revsc_set_lpfreq)
{
	m_uint gw_offset = SZ_INT;
	GW_revsc* ug = (GW_revsc*)o->ugen->ug;
	m_float lpfreq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->lpfreq = lpfreq);
}

typedef struct
{
	sp_data* sp;
	sp_rms* osc;
} GW_rms;

TICK(rms_tick)
{
	GW_rms* ug = (GW_rms*)u->ug;
	base_tick(u);
	sp_rms_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(rms_ctor)
{
	GW_rms* ug = malloc(sizeof(GW_rms));
	ug->sp = shred->vm_ref->bbq->sp;
sp_rms_create(&ug->osc);
sp_rms_init(ug->sp, ug->osc);
	o->ugen->tick = rms_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(rms_dtor)
{
	GW_rms* ug = o->ugen->ug;
	sp_rms_destroy(&ug->osc);
	free(ug);
}

MFUN(rms_get_ihp)
{
	GW_rms* ug = (GW_rms*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->ihp;
}

MFUN(rms_set_ihp)
{
	m_uint gw_offset = SZ_INT;
	GW_rms* ug = (GW_rms*)o->ugen->ug;
	m_float ihp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->ihp = ihp);
}

typedef struct
{
	sp_data* sp;
	sp_rpt* osc;
	m_bool is_init;
} GW_rpt;

TICK(rpt_tick)
{
	GW_rpt* ug = (GW_rpt*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	ugen_compute(u->trig->ugen);
	sp_rpt_compute(ug->sp, ug->osc, &u->in, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(rpt_ctor)
{
	GW_rpt* ug = malloc(sizeof(GW_rpt));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = rpt_tick;
	assign_ugen(o->ugen, 2, 1, 1, ug);
}

DTOR(rpt_dtor)
{
	GW_rpt* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_rpt_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(rpt_init)
{
	m_uint gw_offset = SZ_INT;
	GW_rpt* ug = (GW_rpt*)o->ugen->ug;
	if(ug->osc) {
		sp_rpt_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float maxdur = *(m_float*)(shred->mem + gw_offset);
	if(sp_rpt_create(&ug->osc) == SP_NOT_OK || sp_rpt_init(ug->sp, ug->osc, maxdur) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_samphold* osc;
} GW_samphold;

TICK(samphold_tick)
{
	GW_samphold* ug = (GW_samphold*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	ugen_compute(u->trig->ugen);
	sp_samphold_compute(ug->sp, ug->osc, &u->in, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(samphold_ctor)
{
	GW_samphold* ug = malloc(sizeof(GW_samphold));
	ug->sp = shred->vm_ref->bbq->sp;
sp_samphold_create(&ug->osc);
sp_samphold_init(ug->sp, ug->osc);
	o->ugen->tick = samphold_tick;
	assign_ugen(o->ugen, 2, 1, 1, ug);
}

DTOR(samphold_dtor)
{
	GW_samphold* ug = o->ugen->ug;
	sp_samphold_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_saturator* osc;
} GW_saturator;

TICK(saturator_tick)
{
	GW_saturator* ug = (GW_saturator*)u->ug;
	base_tick(u);
	sp_saturator_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(saturator_ctor)
{
	GW_saturator* ug = malloc(sizeof(GW_saturator));
	ug->sp = shred->vm_ref->bbq->sp;
sp_saturator_create(&ug->osc);
sp_saturator_init(ug->sp, ug->osc);
	o->ugen->tick = saturator_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(saturator_dtor)
{
	GW_saturator* ug = o->ugen->ug;
	sp_saturator_destroy(&ug->osc);
	free(ug);
}

MFUN(saturator_get_drive)
{
	GW_saturator* ug = (GW_saturator*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->drive;
}

MFUN(saturator_set_drive)
{
	m_uint gw_offset = SZ_INT;
	GW_saturator* ug = (GW_saturator*)o->ugen->ug;
	m_float drive = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->drive = drive);
}

MFUN(saturator_get_dcoffset)
{
	GW_saturator* ug = (GW_saturator*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dcoffset;
}

MFUN(saturator_set_dcoffset)
{
	m_uint gw_offset = SZ_INT;
	GW_saturator* ug = (GW_saturator*)o->ugen->ug;
	m_float dcoffset = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dcoffset = dcoffset);
}

typedef struct
{
	sp_data* sp;
	sp_scale* osc;
} GW_scale;

TICK(scale_tick)
{
	GW_scale* ug = (GW_scale*)u->ug;
	base_tick(u);
	sp_scale_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(scale_ctor)
{
	GW_scale* ug = malloc(sizeof(GW_scale));
	ug->sp = shred->vm_ref->bbq->sp;
sp_scale_create(&ug->osc);
sp_scale_init(ug->sp, ug->osc);
	o->ugen->tick = scale_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(scale_dtor)
{
	GW_scale* ug = o->ugen->ug;
	sp_scale_destroy(&ug->osc);
	free(ug);
}

MFUN(scale_get_min)
{
	GW_scale* ug = (GW_scale*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->min;
}

MFUN(scale_set_min)
{
	m_uint gw_offset = SZ_INT;
	GW_scale* ug = (GW_scale*)o->ugen->ug;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->min = min);
}

MFUN(scale_get_max)
{
	GW_scale* ug = (GW_scale*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->max;
}

MFUN(scale_set_max)
{
	m_uint gw_offset = SZ_INT;
	GW_scale* ug = (GW_scale*)o->ugen->ug;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->max = max);
}

typedef struct
{
	sp_data* sp;
	sp_sdelay* osc;
	m_bool is_init;
} GW_sdelay;

TICK(sdelay_tick)
{
	GW_sdelay* ug = (GW_sdelay*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_sdelay_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(sdelay_ctor)
{
	GW_sdelay* ug = malloc(sizeof(GW_sdelay));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = sdelay_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(sdelay_dtor)
{
	GW_sdelay* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_sdelay_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(sdelay_init)
{
	m_uint gw_offset = SZ_INT;
	GW_sdelay* ug = (GW_sdelay*)o->ugen->ug;
	if(ug->osc) {
		sp_sdelay_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float size = *(m_float*)(shred->mem + gw_offset);
	if(sp_sdelay_create(&ug->osc) == SP_NOT_OK || sp_sdelay_init(ug->sp, ug->osc, size) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_slice* osc;
	m_bool is_init;
} GW_slice;

TICK(slice_tick)
{
	GW_slice* ug = (GW_slice*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_slice_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(slice_ctor)
{
	GW_slice* ug = malloc(sizeof(GW_slice));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = slice_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(slice_dtor)
{
	GW_slice* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_slice_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(slice_init)
{
	m_uint gw_offset = SZ_INT;
	GW_slice* ug = (GW_slice*)o->ugen->ug;
	if(ug->osc) {
		sp_slice_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object vals_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* vals = FTBL(vals_obj);
	release(vals_obj, shred);
	gw_offset +=SZ_INT;
	M_Object buf_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* buf = FTBL(buf_obj);
	release(buf_obj, shred);
	if(sp_slice_create(&ug->osc) == SP_NOT_OK || sp_slice_init(ug->sp, ug->osc, vals, buf) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(slice_get_id)
{
	GW_slice* ug = (GW_slice*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->id;
}

MFUN(slice_set_id)
{
	m_uint gw_offset = SZ_INT;
	GW_slice* ug = (GW_slice*)o->ugen->ug;
	m_float id = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->id = id);
}

typedef struct
{
	sp_data* sp;
	sp_smoothdelay* osc;
	m_bool is_init;
} GW_smoothdelay;

TICK(smoothdelay_tick)
{
	GW_smoothdelay* ug = (GW_smoothdelay*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_smoothdelay_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(smoothdelay_ctor)
{
	GW_smoothdelay* ug = malloc(sizeof(GW_smoothdelay));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = smoothdelay_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(smoothdelay_dtor)
{
	GW_smoothdelay* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_smoothdelay_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(smoothdelay_init)
{
	m_uint gw_offset = SZ_INT;
	GW_smoothdelay* ug = (GW_smoothdelay*)o->ugen->ug;
	if(ug->osc) {
		sp_smoothdelay_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float maxdel = *(m_float*)(shred->mem + gw_offset);
	gw_offset +=SZ_FLOAT;
	m_int interp = *(m_int*)(shred->mem + gw_offset);
	if(sp_smoothdelay_create(&ug->osc) == SP_NOT_OK || sp_smoothdelay_init(ug->sp, ug->osc, maxdel, interp) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(smoothdelay_get_feedback)
{
	GW_smoothdelay* ug = (GW_smoothdelay*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->feedback;
}

MFUN(smoothdelay_set_feedback)
{
	m_uint gw_offset = SZ_INT;
	GW_smoothdelay* ug = (GW_smoothdelay*)o->ugen->ug;
	m_float feedback = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->feedback = feedback);
}

MFUN(smoothdelay_get_del)
{
	GW_smoothdelay* ug = (GW_smoothdelay*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->del;
}

MFUN(smoothdelay_set_del)
{
	m_uint gw_offset = SZ_INT;
	GW_smoothdelay* ug = (GW_smoothdelay*)o->ugen->ug;
	m_float del = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->del = del);
}

typedef struct
{
	sp_data* sp;
	sp_spa* osc;
	m_bool is_init;
} GW_spa;

TICK(spa_tick)
{
	GW_spa* ug = (GW_spa*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_spa_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(spa_ctor)
{
	GW_spa* ug = malloc(sizeof(GW_spa));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = spa_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(spa_dtor)
{
	GW_spa* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_spa_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(spa_init)
{
	m_uint gw_offset = SZ_INT;
	GW_spa* ug = (GW_spa*)o->ugen->ug;
	if(ug->osc) {
		sp_spa_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str filename = STRING(filename_obj);
	release(filename_obj, shred);
	if(sp_spa_create(&ug->osc) == SP_NOT_OK || sp_spa_init(ug->sp, ug->osc, filename) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_sparec* osc;
	m_bool is_init;
} GW_sparec;

TICK(sparec_tick)
{
	GW_sparec* ug = (GW_sparec*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_sparec_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(sparec_ctor)
{
	GW_sparec* ug = malloc(sizeof(GW_sparec));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = sparec_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(sparec_dtor)
{
	GW_sparec* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_sparec_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(sparec_init)
{
	m_uint gw_offset = SZ_INT;
	GW_sparec* ug = (GW_sparec*)o->ugen->ug;
	if(ug->osc) {
		sp_sparec_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str filename = STRING(filename_obj);
	release(filename_obj, shred);
	if(sp_sparec_create(&ug->osc) == SP_NOT_OK || sp_sparec_init(ug->sp, ug->osc, filename) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_streson* osc;
} GW_streson;

TICK(streson_tick)
{
	GW_streson* ug = (GW_streson*)u->ug;
	base_tick(u);
	sp_streson_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(streson_ctor)
{
	GW_streson* ug = malloc(sizeof(GW_streson));
	ug->sp = shred->vm_ref->bbq->sp;
sp_streson_create(&ug->osc);
sp_streson_init(ug->sp, ug->osc);
	o->ugen->tick = streson_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(streson_dtor)
{
	GW_streson* ug = o->ugen->ug;
	sp_streson_destroy(&ug->osc);
	free(ug);
}

MFUN(streson_get_freq)
{
	GW_streson* ug = (GW_streson*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->freq;
}

MFUN(streson_set_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_streson* ug = (GW_streson*)o->ugen->ug;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->freq = freq);
}

MFUN(streson_get_fdbgain)
{
	GW_streson* ug = (GW_streson*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->fdbgain;
}

MFUN(streson_set_fdbgain)
{
	m_uint gw_offset = SZ_INT;
	GW_streson* ug = (GW_streson*)o->ugen->ug;
	m_float fdbgain = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->fdbgain = fdbgain);
}

typedef struct
{
	sp_data* sp;
	sp_switch* osc;
} GW_switch;

TICK(switch_tick)
{
	GW_switch* ug = (GW_switch*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	base_tick(u->channel[2]->ugen);
	ugen_compute(u->trig->ugen);
	sp_switch_compute(ug->sp, ug->osc, &u->channel[0]->ugen->in, &u->channel[1]->ugen->in, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(switch_ctor)
{
	GW_switch* ug = malloc(sizeof(GW_switch));
	ug->sp = shred->vm_ref->bbq->sp;
sp_switch_create(&ug->osc);
sp_switch_init(ug->sp, ug->osc);
	o->ugen->tick = switch_tick;
	assign_ugen(o->ugen, 3, 1, 1, ug);
}

DTOR(switch_dtor)
{
	GW_switch* ug = o->ugen->ug;
	sp_switch_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_tabread* osc;
	m_bool is_init;
} GW_tabread;

TICK(tabread_tick)
{
	GW_tabread* ug = (GW_tabread*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	sp_tabread_compute(ug->sp, ug->osc, NULL, &u->out);
	return 1;
}

CTOR(tabread_ctor)
{
	GW_tabread* ug = malloc(sizeof(GW_tabread));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = tabread_tick;
	assign_ugen(o->ugen, 0, 1, 0, ug);
}

DTOR(tabread_dtor)
{
	GW_tabread* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_tabread_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(tabread_init)
{
	m_uint gw_offset = SZ_INT;
	GW_tabread* ug = (GW_tabread*)o->ugen->ug;
	if(ug->osc) {
		sp_tabread_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* ft = FTBL(ft_obj);
	release(ft_obj, shred);
	gw_offset +=SZ_INT;
	m_float mode = *(m_float*)(shred->mem + gw_offset);
	if(sp_tabread_create(&ug->osc) == SP_NOT_OK || sp_tabread_init(ug->sp, ug->osc, ft, mode) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(tabread_get_index)
{
	GW_tabread* ug = (GW_tabread*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->index;
}

MFUN(tabread_set_index)
{
	m_uint gw_offset = SZ_INT;
	GW_tabread* ug = (GW_tabread*)o->ugen->ug;
	m_float index = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->index = index);
}

MFUN(tabread_get_offset)
{
	GW_tabread* ug = (GW_tabread*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->offset;
}

MFUN(tabread_set_offset)
{
	m_uint gw_offset = SZ_INT;
	GW_tabread* ug = (GW_tabread*)o->ugen->ug;
	m_float offset = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->offset = offset);
}

MFUN(tabread_get_wrap)
{
	GW_tabread* ug = (GW_tabread*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->wrap;
}

MFUN(tabread_set_wrap)
{
	m_uint gw_offset = SZ_INT;
	GW_tabread* ug = (GW_tabread*)o->ugen->ug;
	m_float wrap = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->wrap = wrap);
}

typedef struct
{
	sp_data* sp;
	sp_tadsr* osc;
} GW_tadsr;

TICK(tadsr_tick)
{
	GW_tadsr* ug = (GW_tadsr*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tadsr_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tadsr_ctor)
{
	GW_tadsr* ug = malloc(sizeof(GW_tadsr));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tadsr_create(&ug->osc);
sp_tadsr_init(ug->sp, ug->osc);
	o->ugen->tick = tadsr_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tadsr_dtor)
{
	GW_tadsr* ug = o->ugen->ug;
	sp_tadsr_destroy(&ug->osc);
	free(ug);
}

MFUN(tadsr_get_atk)
{
	GW_tadsr* ug = (GW_tadsr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->atk;
}

MFUN(tadsr_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_tadsr* ug = (GW_tadsr*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->atk = atk);
}

MFUN(tadsr_get_dec)
{
	GW_tadsr* ug = (GW_tadsr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dec;
}

MFUN(tadsr_set_dec)
{
	m_uint gw_offset = SZ_INT;
	GW_tadsr* ug = (GW_tadsr*)o->ugen->ug;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dec = dec);
}

MFUN(tadsr_get_sus)
{
	GW_tadsr* ug = (GW_tadsr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->sus;
}

MFUN(tadsr_set_sus)
{
	m_uint gw_offset = SZ_INT;
	GW_tadsr* ug = (GW_tadsr*)o->ugen->ug;
	m_float sus = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->sus = sus);
}

MFUN(tadsr_get_rel)
{
	GW_tadsr* ug = (GW_tadsr*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->rel;
}

MFUN(tadsr_set_rel)
{
	m_uint gw_offset = SZ_INT;
	GW_tadsr* ug = (GW_tadsr*)o->ugen->ug;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->rel = rel);
}

typedef struct
{
	sp_data* sp;
	sp_tblrec* osc;
	m_bool is_init;
} GW_tblrec;

TICK(tblrec_tick)
{
	GW_tblrec* ug = (GW_tblrec*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	ugen_compute(u->trig->ugen);
	sp_tblrec_compute(ug->sp, ug->osc, &u->in, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tblrec_ctor)
{
	GW_tblrec* ug = malloc(sizeof(GW_tblrec));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = tblrec_tick;
	assign_ugen(o->ugen, 2, 1, 1, ug);
}

DTOR(tblrec_dtor)
{
	GW_tblrec* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_tblrec_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(tblrec_init)
{
	m_uint gw_offset = SZ_INT;
	GW_tblrec* ug = (GW_tblrec*)o->ugen->ug;
	if(ug->osc) {
		sp_tblrec_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object bar_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* bar = FTBL(bar_obj);
	release(bar_obj, shred);
	if(sp_tblrec_create(&ug->osc) == SP_NOT_OK || sp_tblrec_init(ug->sp, ug->osc, bar) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct
{
	sp_data* sp;
	sp_tbvcf* osc;
} GW_tbvcf;

TICK(tbvcf_tick)
{
	GW_tbvcf* ug = (GW_tbvcf*)u->ug;
	base_tick(u);
	sp_tbvcf_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(tbvcf_ctor)
{
	GW_tbvcf* ug = malloc(sizeof(GW_tbvcf));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tbvcf_create(&ug->osc);
sp_tbvcf_init(ug->sp, ug->osc);
	o->ugen->tick = tbvcf_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(tbvcf_dtor)
{
	GW_tbvcf* ug = o->ugen->ug;
	sp_tbvcf_destroy(&ug->osc);
	free(ug);
}

MFUN(tbvcf_get_fco)
{
	GW_tbvcf* ug = (GW_tbvcf*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->fco;
}

MFUN(tbvcf_set_fco)
{
	m_uint gw_offset = SZ_INT;
	GW_tbvcf* ug = (GW_tbvcf*)o->ugen->ug;
	m_float fco = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->fco = fco);
}

MFUN(tbvcf_get_res)
{
	GW_tbvcf* ug = (GW_tbvcf*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->res;
}

MFUN(tbvcf_set_res)
{
	m_uint gw_offset = SZ_INT;
	GW_tbvcf* ug = (GW_tbvcf*)o->ugen->ug;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->res = res);
}

MFUN(tbvcf_get_dist)
{
	GW_tbvcf* ug = (GW_tbvcf*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dist;
}

MFUN(tbvcf_set_dist)
{
	m_uint gw_offset = SZ_INT;
	GW_tbvcf* ug = (GW_tbvcf*)o->ugen->ug;
	m_float dist = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dist = dist);
}

MFUN(tbvcf_get_asym)
{
	GW_tbvcf* ug = (GW_tbvcf*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->asym;
}

MFUN(tbvcf_set_asym)
{
	m_uint gw_offset = SZ_INT;
	GW_tbvcf* ug = (GW_tbvcf*)o->ugen->ug;
	m_float asym = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->asym = asym);
}

typedef struct
{
	sp_data* sp;
	sp_tdiv* osc;
} GW_tdiv;

TICK(tdiv_tick)
{
	GW_tdiv* ug = (GW_tdiv*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tdiv_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tdiv_ctor)
{
	GW_tdiv* ug = malloc(sizeof(GW_tdiv));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tdiv_create(&ug->osc);
sp_tdiv_init(ug->sp, ug->osc);
	o->ugen->tick = tdiv_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tdiv_dtor)
{
	GW_tdiv* ug = o->ugen->ug;
	sp_tdiv_destroy(&ug->osc);
	free(ug);
}

MFUN(tdiv_get_num)
{
	GW_tdiv* ug = (GW_tdiv*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->num;
}

MFUN(tdiv_set_num)
{
	m_uint gw_offset = SZ_INT;
	GW_tdiv* ug = (GW_tdiv*)o->ugen->ug;
	m_float num = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->num = num);
}

MFUN(tdiv_get_offset)
{
	GW_tdiv* ug = (GW_tdiv*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->offset;
}

MFUN(tdiv_set_offset)
{
	m_uint gw_offset = SZ_INT;
	GW_tdiv* ug = (GW_tdiv*)o->ugen->ug;
	m_float offset = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->offset = offset);
}

typedef struct
{
	sp_data* sp;
	sp_tenv* osc;
} GW_tenv;

TICK(tenv_tick)
{
	GW_tenv* ug = (GW_tenv*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tenv_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tenv_ctor)
{
	GW_tenv* ug = malloc(sizeof(GW_tenv));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tenv_create(&ug->osc);
sp_tenv_init(ug->sp, ug->osc);
	o->ugen->tick = tenv_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tenv_dtor)
{
	GW_tenv* ug = o->ugen->ug;
	sp_tenv_destroy(&ug->osc);
	free(ug);
}

MFUN(tenv_get_atk)
{
	GW_tenv* ug = (GW_tenv*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->atk;
}

MFUN(tenv_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_tenv* ug = (GW_tenv*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->atk = atk);
}

MFUN(tenv_get_hold)
{
	GW_tenv* ug = (GW_tenv*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->hold;
}

MFUN(tenv_set_hold)
{
	m_uint gw_offset = SZ_INT;
	GW_tenv* ug = (GW_tenv*)o->ugen->ug;
	m_float hold = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->hold = hold);
}

MFUN(tenv_get_rel)
{
	GW_tenv* ug = (GW_tenv*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->rel;
}

MFUN(tenv_set_rel)
{
	m_uint gw_offset = SZ_INT;
	GW_tenv* ug = (GW_tenv*)o->ugen->ug;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->rel = rel);
}

typedef struct
{
	sp_data* sp;
	sp_tenv2* osc;
} GW_tenv2;

TICK(tenv2_tick)
{
	GW_tenv2* ug = (GW_tenv2*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tenv2_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tenv2_ctor)
{
	GW_tenv2* ug = malloc(sizeof(GW_tenv2));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tenv2_create(&ug->osc);
sp_tenv2_init(ug->sp, ug->osc);
	o->ugen->tick = tenv2_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tenv2_dtor)
{
	GW_tenv2* ug = o->ugen->ug;
	sp_tenv2_destroy(&ug->osc);
	free(ug);
}

MFUN(tenv2_get_atk)
{
	GW_tenv2* ug = (GW_tenv2*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->atk;
}

MFUN(tenv2_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_tenv2* ug = (GW_tenv2*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->atk = atk);
}

MFUN(tenv2_get_rel)
{
	GW_tenv2* ug = (GW_tenv2*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->rel;
}

MFUN(tenv2_set_rel)
{
	m_uint gw_offset = SZ_INT;
	GW_tenv2* ug = (GW_tenv2*)o->ugen->ug;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->rel = rel);
}

typedef struct
{
	sp_data* sp;
	sp_tenvx* osc;
} GW_tenvx;

TICK(tenvx_tick)
{
	GW_tenvx* ug = (GW_tenvx*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tenvx_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tenvx_ctor)
{
	GW_tenvx* ug = malloc(sizeof(GW_tenvx));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tenvx_create(&ug->osc);
sp_tenvx_init(ug->sp, ug->osc);
	o->ugen->tick = tenvx_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tenvx_dtor)
{
	GW_tenvx* ug = o->ugen->ug;
	sp_tenvx_destroy(&ug->osc);
	free(ug);
}

MFUN(tenvx_get_atk)
{
	GW_tenvx* ug = (GW_tenvx*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->atk;
}

MFUN(tenvx_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_tenvx* ug = (GW_tenvx*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->atk = atk);
}

MFUN(tenvx_get_hold)
{
	GW_tenvx* ug = (GW_tenvx*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->hold;
}

MFUN(tenvx_set_hold)
{
	m_uint gw_offset = SZ_INT;
	GW_tenvx* ug = (GW_tenvx*)o->ugen->ug;
	m_float hold = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->hold = hold);
}

MFUN(tenvx_get_rel)
{
	GW_tenvx* ug = (GW_tenvx*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->rel;
}

MFUN(tenvx_set_rel)
{
	m_uint gw_offset = SZ_INT;
	GW_tenvx* ug = (GW_tenvx*)o->ugen->ug;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->rel = rel);
}

typedef struct
{
	sp_data* sp;
	sp_tgate* osc;
} GW_tgate;

TICK(tgate_tick)
{
	GW_tgate* ug = (GW_tgate*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tgate_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tgate_ctor)
{
	GW_tgate* ug = malloc(sizeof(GW_tgate));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tgate_create(&ug->osc);
sp_tgate_init(ug->sp, ug->osc);
	o->ugen->tick = tgate_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tgate_dtor)
{
	GW_tgate* ug = o->ugen->ug;
	sp_tgate_destroy(&ug->osc);
	free(ug);
}

MFUN(tgate_get_time)
{
	GW_tgate* ug = (GW_tgate*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->time;
}

MFUN(tgate_set_time)
{
	m_uint gw_offset = SZ_INT;
	GW_tgate* ug = (GW_tgate*)o->ugen->ug;
	m_float time = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->time = time);
}

typedef struct
{
	sp_data* sp;
	sp_thresh* osc;
} GW_thresh;

TICK(thresh_tick)
{
	GW_thresh* ug = (GW_thresh*)u->ug;
	base_tick(u);
	sp_thresh_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(thresh_ctor)
{
	GW_thresh* ug = malloc(sizeof(GW_thresh));
	ug->sp = shred->vm_ref->bbq->sp;
sp_thresh_create(&ug->osc);
sp_thresh_init(ug->sp, ug->osc);
	o->ugen->tick = thresh_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(thresh_dtor)
{
	GW_thresh* ug = o->ugen->ug;
	sp_thresh_destroy(&ug->osc);
	free(ug);
}

MFUN(thresh_get_thresh)
{
	GW_thresh* ug = (GW_thresh*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->thresh;
}

MFUN(thresh_set_thresh)
{
	m_uint gw_offset = SZ_INT;
	GW_thresh* ug = (GW_thresh*)o->ugen->ug;
	m_float thresh = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->thresh = thresh);
}

MFUN(thresh_get_mode)
{
	GW_thresh* ug = (GW_thresh*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->mode;
}

MFUN(thresh_set_mode)
{
	m_uint gw_offset = SZ_INT;
	GW_thresh* ug = (GW_thresh*)o->ugen->ug;
	m_int mode = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->mode = mode);
}

typedef struct
{
	sp_data* sp;
	sp_timer* osc;
} GW_timer;

TICK(timer_tick)
{
	GW_timer* ug = (GW_timer*)u->ug;
	base_tick(u);
	sp_timer_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(timer_ctor)
{
	GW_timer* ug = malloc(sizeof(GW_timer));
	ug->sp = shred->vm_ref->bbq->sp;
sp_timer_create(&ug->osc);
sp_timer_init(ug->sp, ug->osc);
	o->ugen->tick = timer_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(timer_dtor)
{
	GW_timer* ug = o->ugen->ug;
	sp_timer_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_tin* osc;
} GW_tin;

TICK(tin_tick)
{
	GW_tin* ug = (GW_tin*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tin_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tin_ctor)
{
	GW_tin* ug = malloc(sizeof(GW_tin));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tin_create(&ug->osc);
sp_tin_init(ug->sp, ug->osc);
	o->ugen->tick = tin_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tin_dtor)
{
	GW_tin* ug = o->ugen->ug;
	sp_tin_destroy(&ug->osc);
	free(ug);
}

typedef struct
{
	sp_data* sp;
	sp_tone* osc;
} GW_tone;

TICK(tone_tick)
{
	GW_tone* ug = (GW_tone*)u->ug;
	base_tick(u);
	sp_tone_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(tone_ctor)
{
	GW_tone* ug = malloc(sizeof(GW_tone));
	ug->sp = shred->vm_ref->bbq->sp;
sp_tone_create(&ug->osc);
sp_tone_init(ug->sp, ug->osc);
	o->ugen->tick = tone_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(tone_dtor)
{
	GW_tone* ug = o->ugen->ug;
	sp_tone_destroy(&ug->osc);
	free(ug);
}

MFUN(tone_get_hp)
{
	GW_tone* ug = (GW_tone*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->hp;
}

MFUN(tone_set_hp)
{
	m_uint gw_offset = SZ_INT;
	GW_tone* ug = (GW_tone*)o->ugen->ug;
	m_float hp = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->hp = hp);
}

typedef struct
{
	sp_data* sp;
	sp_trand* osc;
} GW_trand;

TICK(trand_tick)
{
	GW_trand* ug = (GW_trand*)u->ug;
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_trand_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(trand_ctor)
{
	GW_trand* ug = malloc(sizeof(GW_trand));
	ug->sp = shred->vm_ref->bbq->sp;
sp_trand_create(&ug->osc);
sp_trand_init(ug->sp, ug->osc);
	o->ugen->tick = trand_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(trand_dtor)
{
	GW_trand* ug = o->ugen->ug;
	sp_trand_destroy(&ug->osc);
	free(ug);
}

MFUN(trand_get_min)
{
	GW_trand* ug = (GW_trand*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->min;
}

MFUN(trand_set_min)
{
	m_uint gw_offset = SZ_INT;
	GW_trand* ug = (GW_trand*)o->ugen->ug;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->min = min);
}

MFUN(trand_get_max)
{
	GW_trand* ug = (GW_trand*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->max;
}

MFUN(trand_set_max)
{
	m_uint gw_offset = SZ_INT;
	GW_trand* ug = (GW_trand*)o->ugen->ug;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->max = max);
}

typedef struct
{
	sp_data* sp;
	sp_tseg* osc;
	m_bool is_init;
} GW_tseg;

TICK(tseg_tick)
{
	GW_tseg* ug = (GW_tseg*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tseg_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tseg_ctor)
{
	GW_tseg* ug = malloc(sizeof(GW_tseg));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = tseg_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tseg_dtor)
{
	GW_tseg* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_tseg_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(tseg_init)
{
	m_uint gw_offset = SZ_INT;
	GW_tseg* ug = (GW_tseg*)o->ugen->ug;
	if(ug->osc) {
		sp_tseg_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float ibeg = *(m_float*)(shred->mem + gw_offset);
	if(sp_tseg_create(&ug->osc) == SP_NOT_OK || sp_tseg_init(ug->sp, ug->osc, ibeg) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(tseg_get_end)
{
	GW_tseg* ug = (GW_tseg*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->end;
}

MFUN(tseg_set_end)
{
	m_uint gw_offset = SZ_INT;
	GW_tseg* ug = (GW_tseg*)o->ugen->ug;
	m_float end = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->end = end);
}

MFUN(tseg_get_dur)
{
	GW_tseg* ug = (GW_tseg*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->dur;
}

MFUN(tseg_set_dur)
{
	m_uint gw_offset = SZ_INT;
	GW_tseg* ug = (GW_tseg*)o->ugen->ug;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->dur = dur);
}

MFUN(tseg_get_type)
{
	GW_tseg* ug = (GW_tseg*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->type;
}

MFUN(tseg_set_type)
{
	m_uint gw_offset = SZ_INT;
	GW_tseg* ug = (GW_tseg*)o->ugen->ug;
	m_float type = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->type = type);
}

typedef struct
{
	sp_data* sp;
	sp_tseq* osc;
	m_bool is_init;
} GW_tseq;

TICK(tseq_tick)
{
	GW_tseq* ug = (GW_tseq*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	ugen_compute(u->trig->ugen);
	sp_tseq_compute(ug->sp, ug->osc, &u->trig->ugen->out, &u->out);
	return 1;
}

CTOR(tseq_ctor)
{
	GW_tseq* ug = malloc(sizeof(GW_tseq));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = tseq_tick;
	assign_ugen(o->ugen, 1, 1, 1, ug);
}

DTOR(tseq_dtor)
{
	GW_tseq* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_tseq_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(tseq_init)
{
	m_uint gw_offset = SZ_INT;
	GW_tseq* ug = (GW_tseq*)o->ugen->ug;
	if(ug->osc) {
		sp_tseq_destroy(&ug->osc);
		ug->osc = NULL;
	}
	M_Object ft_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl* ft = FTBL(ft_obj);
	release(ft_obj, shred);
	if(sp_tseq_create(&ug->osc) == SP_NOT_OK || sp_tseq_init(ug->sp, ug->osc, ft) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(tseq_get_shuf)
{
	GW_tseq* ug = (GW_tseq*)o->ugen->ug;
	RETURN->d.v_uint = ug->osc->shuf;
}

MFUN(tseq_set_shuf)
{
	m_uint gw_offset = SZ_INT;
	GW_tseq* ug = (GW_tseq*)o->ugen->ug;
	m_int shuf = *(m_int*)(shred->mem + gw_offset);
	RETURN->d.v_uint = (ug->osc->shuf = shuf);
}

typedef struct
{
	sp_data* sp;
	sp_vdelay* osc;
	m_bool is_init;
} GW_vdelay;

TICK(vdelay_tick)
{
	GW_vdelay* ug = (GW_vdelay*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_vdelay_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(vdelay_ctor)
{
	GW_vdelay* ug = malloc(sizeof(GW_vdelay));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = vdelay_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(vdelay_dtor)
{
	GW_vdelay* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_vdelay_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(vdelay_init)
{
	m_uint gw_offset = SZ_INT;
	GW_vdelay* ug = (GW_vdelay*)o->ugen->ug;
	if(ug->osc) {
		sp_vdelay_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float maxdel = *(m_float*)(shred->mem + gw_offset);
	if(sp_vdelay_create(&ug->osc) == SP_NOT_OK || sp_vdelay_init(ug->sp, ug->osc, maxdel) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(vdelay_get_del)
{
	GW_vdelay* ug = (GW_vdelay*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->del;
}

MFUN(vdelay_set_del)
{
	m_uint gw_offset = SZ_INT;
	GW_vdelay* ug = (GW_vdelay*)o->ugen->ug;
	m_float del = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->del = del);
}

typedef struct
{
	sp_data* sp;
	sp_vocoder* osc;
} GW_vocoder;

TICK(vocoder_tick)
{
	GW_vocoder* ug = (GW_vocoder*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	sp_vocoder_compute(ug->sp, ug->osc, &u->channel[0]->ugen->in, &u->channel[1]->ugen->in, &u->out);
	return 1;
}

CTOR(vocoder_ctor)
{
	GW_vocoder* ug = malloc(sizeof(GW_vocoder));
	ug->sp = shred->vm_ref->bbq->sp;
sp_vocoder_create(&ug->osc);
sp_vocoder_init(ug->sp, ug->osc);
	o->ugen->tick = vocoder_tick;
	assign_ugen(o->ugen, 2, 1, 0, ug);
}

DTOR(vocoder_dtor)
{
	GW_vocoder* ug = o->ugen->ug;
	sp_vocoder_destroy(&ug->osc);
	free(ug);
}

MFUN(vocoder_get_atk)
{
	GW_vocoder* ug = (GW_vocoder*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->atk;
}

MFUN(vocoder_set_atk)
{
	m_uint gw_offset = SZ_INT;
	GW_vocoder* ug = (GW_vocoder*)o->ugen->ug;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->atk = atk);
}

MFUN(vocoder_get_rel)
{
	GW_vocoder* ug = (GW_vocoder*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->rel;
}

MFUN(vocoder_set_rel)
{
	m_uint gw_offset = SZ_INT;
	GW_vocoder* ug = (GW_vocoder*)o->ugen->ug;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->rel = rel);
}

MFUN(vocoder_get_bwratio)
{
	GW_vocoder* ug = (GW_vocoder*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->bwratio;
}

MFUN(vocoder_set_bwratio)
{
	m_uint gw_offset = SZ_INT;
	GW_vocoder* ug = (GW_vocoder*)o->ugen->ug;
	m_float bwratio = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->bwratio = bwratio);
}

typedef struct
{
	sp_data* sp;
	sp_waveset* osc;
	m_bool is_init;
} GW_waveset;

TICK(waveset_tick)
{
	GW_waveset* ug = (GW_waveset*)u->ug;
	if(!ug->is_init)
	{ // LCOV_EXCL_START
		u->out = 0;
		return 1;
	} // LCOV_EXCL_STOP
	base_tick(u);
	sp_waveset_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(waveset_ctor)
{
	GW_waveset* ug = malloc(sizeof(GW_waveset));
	ug->sp = shred->vm_ref->bbq->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	o->ugen->tick = waveset_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(waveset_dtor)
{
	GW_waveset* ug = o->ugen->ug;
	if(ug->is_init) {

		sp_waveset_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(waveset_init)
{
	m_uint gw_offset = SZ_INT;
	GW_waveset* ug = (GW_waveset*)o->ugen->ug;
	if(ug->osc) {
		sp_waveset_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_float ilen = *(m_float*)(shred->mem + gw_offset);
	if(sp_waveset_create(&ug->osc) == SP_NOT_OK || sp_waveset_init(ug->sp, ug->osc, ilen) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

MFUN(waveset_get_rep)
{
	GW_waveset* ug = (GW_waveset*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->rep;
}

MFUN(waveset_set_rep)
{
	m_uint gw_offset = SZ_INT;
	GW_waveset* ug = (GW_waveset*)o->ugen->ug;
	m_float rep = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->rep = rep);
}

typedef struct
{
	sp_data* sp;
	sp_wpkorg35* osc;
} GW_wpkorg35;

TICK(wpkorg35_tick)
{
	GW_wpkorg35* ug = (GW_wpkorg35*)u->ug;
	base_tick(u);
	sp_wpkorg35_compute(ug->sp, ug->osc, &u->in, &u->out);
	return 1;
}

CTOR(wpkorg35_ctor)
{
	GW_wpkorg35* ug = malloc(sizeof(GW_wpkorg35));
	ug->sp = shred->vm_ref->bbq->sp;
sp_wpkorg35_create(&ug->osc);
sp_wpkorg35_init(ug->sp, ug->osc);
	o->ugen->tick = wpkorg35_tick;
	assign_ugen(o->ugen, 1, 1, 0, ug);
}

DTOR(wpkorg35_dtor)
{
	GW_wpkorg35* ug = o->ugen->ug;
	sp_wpkorg35_destroy(&ug->osc);
	free(ug);
}

MFUN(wpkorg35_get_cutoff)
{
	GW_wpkorg35* ug = (GW_wpkorg35*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->cutoff;
}

MFUN(wpkorg35_set_cutoff)
{
	m_uint gw_offset = SZ_INT;
	GW_wpkorg35* ug = (GW_wpkorg35*)o->ugen->ug;
	m_float cutoff = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->cutoff = cutoff);
}

MFUN(wpkorg35_get_res)
{
	GW_wpkorg35* ug = (GW_wpkorg35*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->res;
}

MFUN(wpkorg35_set_res)
{
	m_uint gw_offset = SZ_INT;
	GW_wpkorg35* ug = (GW_wpkorg35*)o->ugen->ug;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->res = res);
}

MFUN(wpkorg35_get_saturation)
{
	GW_wpkorg35* ug = (GW_wpkorg35*)o->ugen->ug;
	RETURN->d.v_float = ug->osc->saturation;
}

MFUN(wpkorg35_set_saturation)
{
	m_uint gw_offset = SZ_INT;
	GW_wpkorg35* ug = (GW_wpkorg35*)o->ugen->ug;
	m_float saturation = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (ug->osc->saturation = saturation);
}

typedef struct
{
	sp_data* sp;
	sp_zitarev* osc;
} GW_zitarev;

TICK(zitarev_tick)
{
	GW_zitarev* ug = (GW_zitarev*)u->ug;
	base_tick(u->channel[0]->ugen);
	base_tick(u->channel[1]->ugen);
	sp_zitarev_compute(ug->sp, ug->osc, &u->channel[0]->ugen->in, &u->channel[1]->ugen->in, &u->channel[0]->ugen->out, &u->channel[1]->ugen->out);
	return 1;
}

CTOR(zitarev_ctor)
{
	GW_zitarev* ug = malloc(sizeof(GW_zitarev));
	ug->sp = shred->vm_ref->bbq->sp;
sp_zitarev_create(&ug->osc);
sp_zitarev_init(ug->sp, ug->osc);
	o->ugen->tick = zitarev_tick;
	assign_ugen(o->ugen, 2, 2, 0, ug);
}

DTOR(zitarev_dtor)
{
	GW_zitarev* ug = o->ugen->ug;
	sp_zitarev_destroy(&ug->osc);
	free(ug);
}

MFUN(zitarev_get_in_delay)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->in_delay;
}

MFUN(zitarev_set_in_delay)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float in_delay = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->in_delay = in_delay);
}

MFUN(zitarev_get_lf_x)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->lf_x;
}

MFUN(zitarev_set_lf_x)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float lf_x = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->lf_x = lf_x);
}

MFUN(zitarev_get_rt60_low)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->rt60_low;
}

MFUN(zitarev_set_rt60_low)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float rt60_low = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->rt60_low = rt60_low);
}

MFUN(zitarev_get_rt60_mid)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->rt60_mid;
}

MFUN(zitarev_set_rt60_mid)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float rt60_mid = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->rt60_mid = rt60_mid);
}

MFUN(zitarev_get_hf_damping)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->hf_damping;
}

MFUN(zitarev_set_hf_damping)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float hf_damping = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->hf_damping = hf_damping);
}

MFUN(zitarev_get_eq1_freq)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->eq1_freq;
}

MFUN(zitarev_set_eq1_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float eq1_freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->eq1_freq = eq1_freq);
}

MFUN(zitarev_get_eq1_level)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->eq1_level;
}

MFUN(zitarev_set_eq1_level)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float eq1_level = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->eq1_level = eq1_level);
}

MFUN(zitarev_get_eq2_freq)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->eq2_freq;
}

MFUN(zitarev_set_eq2_freq)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float eq2_freq = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->eq2_freq = eq2_freq);
}

MFUN(zitarev_get_eq2_level)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->eq2_level;
}

MFUN(zitarev_set_eq2_level)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float eq2_level = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->eq2_level = eq2_level);
}

MFUN(zitarev_get_mix)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->mix;
}

MFUN(zitarev_set_mix)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float mix = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->mix = mix);
}

MFUN(zitarev_get_level)
{
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	RETURN->d.v_float = *ug->osc->level;
}

MFUN(zitarev_set_level)
{
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)o->ugen->ug;
	m_float level = *(m_float*)(shred->mem + gw_offset);
	RETURN->d.v_float = (*ug->osc->level = level);
}

struct Type_ t_ftbl = {"ftbl", SZ_INT, &t_object};
struct Type_ t_adsr = {"Adsr", SZ_INT, &t_ugen};
struct Type_ t_allpass = {"Allpass", SZ_INT, &t_ugen};
struct Type_ t_atone = {"Atone", SZ_INT, &t_ugen};
struct Type_ t_autowah = {"Autowah", SZ_INT, &t_ugen};
struct Type_ t_bal = {"Bal", SZ_INT, &t_ugen};
struct Type_ t_bar = {"Bar", SZ_INT, &t_ugen};
struct Type_ t_biquad = {"Biquad", SZ_INT, &t_ugen};
struct Type_ t_biscale = {"Biscale", SZ_INT, &t_ugen};
struct Type_ t_bitcrush = {"Bitcrush", SZ_INT, &t_ugen};
struct Type_ t_blsaw = {"Blsaw", SZ_INT, &t_ugen};
struct Type_ t_blsquare = {"Blsquare", SZ_INT, &t_ugen};
struct Type_ t_bltriangle = {"Bltriangle", SZ_INT, &t_ugen};
struct Type_ t_brown = {"Brown", SZ_INT, &t_ugen};
struct Type_ t_butbp = {"Butbp", SZ_INT, &t_ugen};
struct Type_ t_butbr = {"Butbr", SZ_INT, &t_ugen};
struct Type_ t_buthp = {"Buthp", SZ_INT, &t_ugen};
struct Type_ t_butlp = {"Butlp", SZ_INT, &t_ugen};
struct Type_ t_clip = {"Clip", SZ_INT, &t_ugen};
struct Type_ t_clock = {"Clock", SZ_INT, &t_ugen};
struct Type_ t_comb = {"Comb", SZ_INT, &t_ugen};
struct Type_ t_compressor = {"Compressor", SZ_INT, &t_ugen};
struct Type_ t_conv = {"Conv", SZ_INT, &t_ugen};
struct Type_ t_count = {"Count", SZ_INT, &t_ugen};
struct Type_ t_crossfade = {"Crossfade", SZ_INT, &t_ugen};
struct Type_ t_dcblock = {"Dcblock", SZ_INT, &t_ugen};
struct Type_ t_delay = {"Delay", SZ_INT, &t_ugen};
struct Type_ t_diode = {"Diode", SZ_INT, &t_ugen};
struct Type_ t_diskin = {"Diskin", SZ_INT, &t_ugen};
struct Type_ t_dist = {"Dist", SZ_INT, &t_ugen};
struct Type_ t_dmetro = {"Dmetro", SZ_INT, &t_ugen};
struct Type_ t_drip = {"Drip", SZ_INT, &t_ugen};
struct Type_ t_dtrig = {"Dtrig", SZ_INT, &t_ugen};
struct Type_ t_dust = {"Dust", SZ_INT, &t_ugen};
struct Type_ t_eqfil = {"Eqfil", SZ_INT, &t_ugen};
struct Type_ t_expon = {"Expon", SZ_INT, &t_ugen};
struct Type_ t_fof = {"Fof", SZ_INT, &t_ugen};
struct Type_ t_fofilt = {"Fofilt", SZ_INT, &t_ugen};
struct Type_ t_fog = {"Fog", SZ_INT, &t_ugen};
struct Type_ t_fold = {"Fold", SZ_INT, &t_ugen};
struct Type_ t_fosc = {"Fosc", SZ_INT, &t_ugen};
struct Type_ t_gbuzz = {"Gbuzz", SZ_INT, &t_ugen};
struct Type_ t_hilbert = {"Hilbert", SZ_INT, &t_ugen};
struct Type_ t_in = {"In", SZ_INT, &t_ugen};
struct Type_ t_incr = {"Incr", SZ_INT, &t_ugen};
struct Type_ t_jcrev = {"Jcrev", SZ_INT, &t_ugen};
struct Type_ t_jitter = {"Jitter", SZ_INT, &t_ugen};
struct Type_ t_line = {"Line", SZ_INT, &t_ugen};
struct Type_ t_lpf18 = {"Lpf18", SZ_INT, &t_ugen};
struct Type_ t_maygate = {"Maygate", SZ_INT, &t_ugen};
struct Type_ t_metro = {"Metro", SZ_INT, &t_ugen};
struct Type_ t_mincer = {"Mincer", SZ_INT, &t_ugen};
struct Type_ t_mode = {"Mode", SZ_INT, &t_ugen};
struct Type_ t_moogladder = {"Moogladder", SZ_INT, &t_ugen};
struct Type_ t_noise = {"Noise", SZ_INT, &t_ugen};
struct Type_ t_nsmp = {"Nsmp", SZ_INT, &t_ugen};
struct Type_ t_osc = {"Osc", SZ_INT, &t_ugen};
struct Type_ t_oscmorph = {"Oscmorph", SZ_INT, &t_ugen};
struct Type_ t_pan2 = {"Pan2", SZ_INT, &t_ugen};
struct Type_ t_panst = {"Panst", SZ_INT, &t_ugen};
struct Type_ t_pareq = {"Pareq", SZ_INT, &t_ugen};
struct Type_ t_paulstretch = {"Paulstretch", SZ_INT, &t_ugen};
struct Type_ t_pdhalf = {"Pdhalf", SZ_INT, &t_ugen};
struct Type_ t_peaklim = {"Peaklim", SZ_INT, &t_ugen};
struct Type_ t_phaser = {"Phaser", SZ_INT, &t_ugen};
struct Type_ t_phasor = {"Phasor", SZ_INT, &t_ugen};
struct Type_ t_pinknoise = {"Pinknoise", SZ_INT, &t_ugen};
struct Type_ t_pitchamdf = {"Pitchamdf", SZ_INT, &t_ugen};
struct Type_ t_pluck = {"Pluck", SZ_INT, &t_ugen};
struct Type_ t_port = {"Port", SZ_INT, &t_ugen};
struct Type_ t_posc3 = {"Posc3", SZ_INT, &t_ugen};
struct Type_ t_progress = {"Progress", SZ_INT, &t_ugen};
struct Type_ t_prop = {"Prop", SZ_INT, &t_ugen};
struct Type_ t_pshift = {"Pshift", SZ_INT, &t_ugen};
struct Type_ t_ptrack = {"Ptrack", SZ_INT, &t_ugen};
struct Type_ t_randh = {"Randh", SZ_INT, &t_ugen};
struct Type_ t_randi = {"Randi", SZ_INT, &t_ugen};
struct Type_ t_random = {"Random", SZ_INT, &t_ugen};
struct Type_ t_reson = {"Reson", SZ_INT, &t_ugen};
struct Type_ t_reverse = {"Reverse", SZ_INT, &t_ugen};
struct Type_ t_revsc = {"Revsc", SZ_INT, &t_ugen};
struct Type_ t_rms = {"Rms", SZ_INT, &t_ugen};
struct Type_ t_rpt = {"Rpt", SZ_INT, &t_ugen};
struct Type_ t_samphold = {"Samphold", SZ_INT, &t_ugen};
struct Type_ t_saturator = {"Saturator", SZ_INT, &t_ugen};
struct Type_ t_scale = {"Scale", SZ_INT, &t_ugen};
struct Type_ t_sdelay = {"Sdelay", SZ_INT, &t_ugen};
struct Type_ t_slice = {"Slice", SZ_INT, &t_ugen};
struct Type_ t_smoothdelay = {"Smoothdelay", SZ_INT, &t_ugen};
struct Type_ t_spa = {"Spa", SZ_INT, &t_ugen};
struct Type_ t_sparec = {"Sparec", SZ_INT, &t_ugen};
struct Type_ t_streson = {"Streson", SZ_INT, &t_ugen};
struct Type_ t_switch = {"Switch", SZ_INT, &t_ugen};
struct Type_ t_tabread = {"Tabread", SZ_INT, &t_ugen};
struct Type_ t_tadsr = {"Tadsr", SZ_INT, &t_ugen};
struct Type_ t_tblrec = {"Tblrec", SZ_INT, &t_ugen};
struct Type_ t_tbvcf = {"Tbvcf", SZ_INT, &t_ugen};
struct Type_ t_tdiv = {"Tdiv", SZ_INT, &t_ugen};
struct Type_ t_tenv = {"Tenv", SZ_INT, &t_ugen};
struct Type_ t_tenv2 = {"Tenv2", SZ_INT, &t_ugen};
struct Type_ t_tenvx = {"Tenvx", SZ_INT, &t_ugen};
struct Type_ t_tgate = {"Tgate", SZ_INT, &t_ugen};
struct Type_ t_thresh = {"Thresh", SZ_INT, &t_ugen};
struct Type_ t_timer = {"Timer", SZ_INT, &t_ugen};
struct Type_ t_tin = {"Tin", SZ_INT, &t_ugen};
struct Type_ t_tone = {"Tone", SZ_INT, &t_ugen};
struct Type_ t_trand = {"Trand", SZ_INT, &t_ugen};
struct Type_ t_tseg = {"Tseg", SZ_INT, &t_ugen};
struct Type_ t_tseq = {"Tseq", SZ_INT, &t_ugen};
struct Type_ t_vdelay = {"Vdelay", SZ_INT, &t_ugen};
struct Type_ t_vocoder = {"Vocoder", SZ_INT, &t_ugen};
struct Type_ t_waveset = {"Waveset", SZ_INT, &t_ugen};
struct Type_ t_wpkorg35 = {"Wpkorg35", SZ_INT, &t_ugen};
struct Type_ t_zitarev = {"Zitarev", SZ_INT, &t_ugen};

m_bool import_soundpipe(Env env)
{
	DL_Func* fun;
	DL_Value* arg;
	Func f;

	CHECK_BB(add_global_type(env, &t_ftbl))
	CHECK_OB(import_class_begin(env, &t_ftbl, env->global_nspc, NULL, ftbl_dtor))
	o_ftbl_data = import_mvar(env, "int", "@ftbl", 1, 0, "internal data for ftbl");
	fun = new_dl_func("void", "gen_composite", (m_uint)ftbl_gen_composite);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "string", "argstring");
		arg->doc = "a string of space-separated parameters, in groups of four:arg 1 is the partial number. must be positive, but it doesn't need to be a whole number.arg 2 is the strength.arg 3 is the initial phase (expressed in degrees)arg 4 is the dc offset. A dc offset of 2 will put a 2-strength sinusoid in the rangefrom (-2,2) to (0, 4)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Generate a composite waveform of sinusoids";
	fun = new_dl_func("void", "gen_file", (m_uint)ftbl_gen_file);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "string", "filename");
		arg->doc = "filename";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Reads from a wav file    This will only load as many samples as the length of the ftable.";
	fun = new_dl_func("void", "gen_gauss", (m_uint)ftbl_gen_gauss);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "float", "scale");
		arg->doc = "The scale of the distribution, in the range of -/+scale";
		arg = dl_func_add_arg(fun, "int", "seed");
		arg->doc = "Random seed.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Gaussian distribution";
	fun = new_dl_func("void", "gen_line", (m_uint)ftbl_gen_line);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "string", "argstring");
		arg->doc = "A list of ordered xy pairs. X expects whole number integers, as they correlate to index positions in the ftable.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "A series of line segments";
	fun = new_dl_func("void", "gen_padsynth", (m_uint)ftbl_gen_padsynth);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "ftbl", "amps");
		arg->doc = "ftable of amplitudes to use";
		arg = dl_func_add_arg(fun, "float", "f");
		arg->doc = "Base frequency.";
		arg = dl_func_add_arg(fun, "float", "bw");
		arg->doc = "Bandwidth.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "An implementation of the Padsynth Algorithm by Paul Nasca. This is a basic implemenation of the padsynth algorithm. More information can be found here:http://zynaddsubfx.sourceforge.net/doc/PADsynth/PADsynth.htmThis gen routine requires libfftw, and is not compiled by default. See config.mk for more info.        ";
	fun = new_dl_func("void", "gen_rand", (m_uint)ftbl_gen_rand);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "string", "argstring");
		arg->doc = "A string of value pairs. The first value is the value, then the probability.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Generates a user defined random number distribution.";
	fun = new_dl_func("void", "gen_scrambler", (m_uint)ftbl_gen_scrambler);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "ftbl", "dest");
		arg->doc = "destination ftable";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Scrambles phase of ftable.This gen routine will copy the ftable, apply an FFT, applya random phase, and then do an inverse FFT. This effect is ideal for creating pad-like sounds. ";
	fun = new_dl_func("void", "gen_sine", (m_uint)ftbl_gen_sine);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "generates a sampled sinusoid";
	fun = new_dl_func("void", "gen_sinesum", (m_uint)ftbl_gen_sinesum);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "string", "argstring");
		arg->doc = "A list of amplitudes, in the range 0-1, separated by spaces.Each position coordinates to their partial number. Position 1 is the fundamental amplitude (1 * freq). Position 2 is the first overtone (2 * freq), 3 is the second (3 * freq), etc...";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Waveform as a sum of harmonically related sine waves ";
	fun = new_dl_func("void", "gen_xline", (m_uint)ftbl_gen_xline);
		arg = dl_func_add_arg(fun, "int", "size");
		arg->doc = "size of the ftbl";
		arg = dl_func_add_arg(fun, "string", "argstring");
		arg->doc = "A list of ordered xy pairs. X expects whole number integers, as they correlate to index positions in the ftable.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "A series of exponential segments";
env->class_def->doc = "soudpipe float array type";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_adsr))
	CHECK_OB(import_class_begin(env, &t_adsr, env->global_nspc, adsr_ctor, adsr_dtor))
	fun = new_dl_func("float", "atk", (m_uint)adsr_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time (in seconds)";
	fun = new_dl_func("float", "atk", (m_uint)adsr_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Attack time (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time (in seconds)";
	fun = new_dl_func("float", "dec", (m_uint)adsr_get_dec);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Decay time (in seconds)";
	fun = new_dl_func("float", "dec", (m_uint)adsr_set_dec);
		arg = dl_func_add_arg(fun, "float", "dec");
		arg->doc = "Decay time (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Decay time (in seconds)";
	fun = new_dl_func("float", "sus", (m_uint)adsr_get_sus);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sustain (in range 0-1)";
	fun = new_dl_func("float", "sus", (m_uint)adsr_set_sus);
		arg = dl_func_add_arg(fun, "float", "sus");
		arg->doc = "Sustain (in range 0-1)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sustain (in range 0-1)";
	fun = new_dl_func("float", "rel", (m_uint)adsr_get_rel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time (in seconds)";
	fun = new_dl_func("float", "rel", (m_uint)adsr_set_rel);
		arg = dl_func_add_arg(fun, "float", "rel");
		arg->doc = "Release time (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time (in seconds)";
	env->class_def->doc = "ADSR generatorThis is an ADSR generator whose curves are created using a one-pole low pass filter.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_allpass))
	CHECK_OB(import_class_begin(env, &t_allpass, env->global_nspc, allpass_ctor, allpass_dtor))
	fun = new_dl_func("void", "init", (m_uint)allpass_init);
		arg = dl_func_add_arg(fun, "float", "looptime");
		arg->doc = "The loop time of the filter, in seconds. This can also be thought of as the delay time.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Allpass filter    Often used for the creation of reverb modules.";
	fun = new_dl_func("float", "revtime", (m_uint)allpass_get_revtime);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The reverberation time, in seconds (RT-60).";
	fun = new_dl_func("float", "revtime", (m_uint)allpass_set_revtime);
		arg = dl_func_add_arg(fun, "float", "revtime");
		arg->doc = "The reverberation time, in seconds (RT-60).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The reverberation time, in seconds (RT-60).";
	env->class_def->doc = "Allpass filter    Often used for the creation of reverb modules.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_atone))
	CHECK_OB(import_class_begin(env, &t_atone, env->global_nspc, atone_ctor, atone_dtor))
	fun = new_dl_func("float", "hp", (m_uint)atone_get_hp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The response curve's half power point (cutoff frequency).";
	fun = new_dl_func("float", "hp", (m_uint)atone_set_hp);
		arg = dl_func_add_arg(fun, "float", "hp");
		arg->doc = "The response curve's half power point (cutoff frequency).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The response curve's half power point (cutoff frequency).";
	env->class_def->doc = "First-order recursive highpass filter	This is the complement to the tone module.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_autowah))
	CHECK_OB(import_class_begin(env, &t_autowah, env->global_nspc, autowah_ctor, autowah_dtor))
	fun = new_dl_func("float", "level", (m_uint)autowah_get_level);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Overall level (between 0 and 1)";
	fun = new_dl_func("float", "level", (m_uint)autowah_set_level);
		arg = dl_func_add_arg(fun, "float", "level");
		arg->doc = "Overall level (between 0 and 1)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Overall level (between 0 and 1)";
	fun = new_dl_func("float", "wah", (m_uint)autowah_get_wah);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "wah amount";
	fun = new_dl_func("float", "wah", (m_uint)autowah_set_wah);
		arg = dl_func_add_arg(fun, "float", "wah");
		arg->doc = "wah amount";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "wah amount";
	fun = new_dl_func("float", "mix", (m_uint)autowah_get_mix);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Wet/dry amount (100 = wet, 0 = dry)";
	fun = new_dl_func("float", "mix", (m_uint)autowah_set_mix);
		arg = dl_func_add_arg(fun, "float", "mix");
		arg->doc = "Wet/dry amount (100 = wet, 0 = dry)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Wet/dry amount (100 = wet, 0 = dry)";
	env->class_def->doc = "Automatic wah pedal	An automatic wah effect, ported from Guitarix via Faust. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_bal))
	CHECK_OB(import_class_begin(env, &t_bal, env->global_nspc, bal_ctor, bal_dtor))
	env->class_def->doc = "Balance the gain of one signal based on another signal    This is often used to restore gain lost in the output of a filter.In the source code, the value \"ihp\" is set to a static 10hz. This is the default value in Csound, and should not often need to be changed.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_bar))
	CHECK_OB(import_class_begin(env, &t_bar, env->global_nspc, bar_ctor, bar_dtor))
	fun = new_dl_func("void", "init", (m_uint)bar_init);
		arg = dl_func_add_arg(fun, "float", "iK");
		arg->doc = "Dimensionless stiffness parameter";
		arg = dl_func_add_arg(fun, "float", "ib");
		arg->doc = "High-frequency loss parameter. Keep this small";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Physical model approximating the sound of a struck metal bar";
	fun = new_dl_func("float", "bcL", (m_uint)bar_get_bcL);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Boundary condition at left end of bar. 1 = clamped, 2 = pivoting, 3 = free";
	fun = new_dl_func("float", "bcL", (m_uint)bar_set_bcL);
		arg = dl_func_add_arg(fun, "float", "bcL");
		arg->doc = "Boundary condition at left end of bar. 1 = clamped, 2 = pivoting, 3 = free";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Boundary condition at left end of bar. 1 = clamped, 2 = pivoting, 3 = free";
	fun = new_dl_func("float", "bcR", (m_uint)bar_get_bcR);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Boundary condition at right end of bar. 1 = clamped, 2 = pivoting, 3 = free";
	fun = new_dl_func("float", "bcR", (m_uint)bar_set_bcR);
		arg = dl_func_add_arg(fun, "float", "bcR");
		arg->doc = "Boundary condition at right end of bar. 1 = clamped, 2 = pivoting, 3 = free";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Boundary condition at right end of bar. 1 = clamped, 2 = pivoting, 3 = free";
	fun = new_dl_func("float", "T30", (m_uint)bar_get_T30);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "30db decay time (in seconds).";
	fun = new_dl_func("float", "T30", (m_uint)bar_set_T30);
		arg = dl_func_add_arg(fun, "float", "T30");
		arg->doc = "30db decay time (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "30db decay time (in seconds).";
	fun = new_dl_func("float", "scan", (m_uint)bar_get_scan);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Speed of scanning the output location.";
	fun = new_dl_func("float", "scan", (m_uint)bar_set_scan);
		arg = dl_func_add_arg(fun, "float", "scan");
		arg->doc = "Speed of scanning the output location.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Speed of scanning the output location.";
	fun = new_dl_func("float", "pos", (m_uint)bar_get_pos);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Position along bar that strike occurs.";
	fun = new_dl_func("float", "pos", (m_uint)bar_set_pos);
		arg = dl_func_add_arg(fun, "float", "pos");
		arg->doc = "Position along bar that strike occurs.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Position along bar that strike occurs.";
	fun = new_dl_func("float", "vel", (m_uint)bar_get_vel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Normalized strike velocity";
	fun = new_dl_func("float", "vel", (m_uint)bar_set_vel);
		arg = dl_func_add_arg(fun, "float", "vel");
		arg->doc = "Normalized strike velocity";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Normalized strike velocity";
	fun = new_dl_func("float", "wid", (m_uint)bar_get_wid);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Spatial width of strike.";
	fun = new_dl_func("float", "wid", (m_uint)bar_set_wid);
		arg = dl_func_add_arg(fun, "float", "wid");
		arg->doc = "Spatial width of strike.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Spatial width of strike.";
	env->class_def->doc = "Physical model approximating the sound of a struck metal bar";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_biquad))
	CHECK_OB(import_class_begin(env, &t_biquad, env->global_nspc, biquad_ctor, biquad_dtor))
	fun = new_dl_func("float", "b0", (m_uint)biquad_get_b0);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "b0", (m_uint)biquad_set_b0);
		arg = dl_func_add_arg(fun, "float", "b0");
		arg->doc = "biquad coefficient.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "b1", (m_uint)biquad_get_b1);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "b1", (m_uint)biquad_set_b1);
		arg = dl_func_add_arg(fun, "float", "b1");
		arg->doc = "biquad coefficient.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "b2", (m_uint)biquad_get_b2);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "b2", (m_uint)biquad_set_b2);
		arg = dl_func_add_arg(fun, "float", "b2");
		arg->doc = "biquad coefficient.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "a0", (m_uint)biquad_get_a0);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "a0", (m_uint)biquad_set_a0);
		arg = dl_func_add_arg(fun, "float", "a0");
		arg->doc = "biquad coefficient.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "a1", (m_uint)biquad_get_a1);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "a1", (m_uint)biquad_set_a1);
		arg = dl_func_add_arg(fun, "float", "a1");
		arg->doc = "biquad coefficient.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "a2", (m_uint)biquad_get_a2);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	fun = new_dl_func("float", "a2", (m_uint)biquad_set_a2);
		arg = dl_func_add_arg(fun, "float", "a2");
		arg->doc = "biquad coefficient.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "biquad coefficient.";
	env->class_def->doc = "A sweepable biquadratic general purpose filter    ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_biscale))
	CHECK_OB(import_class_begin(env, &t_biscale, env->global_nspc, biscale_ctor, biscale_dtor))
	fun = new_dl_func("float", "min", (m_uint)biscale_get_min);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value to scale to.";
	fun = new_dl_func("float", "min", (m_uint)biscale_set_min);
		arg = dl_func_add_arg(fun, "float", "min");
		arg->doc = "Minimum value to scale to.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value to scale to.";
	fun = new_dl_func("float", "max", (m_uint)biscale_get_max);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value to scale to.";
	fun = new_dl_func("float", "max", (m_uint)biscale_set_max);
		arg = dl_func_add_arg(fun, "float", "max");
		arg->doc = "Maximum value to scale to.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value to scale to.";
	env->class_def->doc = "Bipolar Scale    This module scales from bipolar [-1, 1] to another range defined by min and max.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_bitcrush))
	CHECK_OB(import_class_begin(env, &t_bitcrush, env->global_nspc, bitcrush_ctor, bitcrush_dtor))
	fun = new_dl_func("float", "bitdepth", (m_uint)bitcrush_get_bitdepth);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bit depth. Expects an integer in the range of 1-16. Fractional values will be truncated.";
	fun = new_dl_func("float", "bitdepth", (m_uint)bitcrush_set_bitdepth);
		arg = dl_func_add_arg(fun, "float", "bitdepth");
		arg->doc = "Bit depth. Expects an integer in the range of 1-16. Fractional values will be truncated.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bit depth. Expects an integer in the range of 1-16. Fractional values will be truncated.";
	fun = new_dl_func("float", "srate", (m_uint)bitcrush_get_srate);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sampling rate.";
	fun = new_dl_func("float", "srate", (m_uint)bitcrush_set_srate);
		arg = dl_func_add_arg(fun, "float", "srate");
		arg->doc = "Sampling rate.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sampling rate.";
	env->class_def->doc = "Digital signal degradation    Bitcrusher will digitally degrade a signal by altering the bitdepth and sampling-rate. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_blsaw))
	CHECK_OB(import_class_begin(env, &t_blsaw, env->global_nspc, blsaw_ctor, blsaw_dtor))
	fun = new_dl_func("float", "freq", (m_uint)blsaw_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency, (range 0-20000)";
	fun = new_dl_func("float", "freq", (m_uint)blsaw_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency, (range 0-20000)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency, (range 0-20000)";
	fun = new_dl_func("float", "amp", (m_uint)blsaw_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (range 0-1).";
	fun = new_dl_func("float", "amp", (m_uint)blsaw_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude (range 0-1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (range 0-1).";
	env->class_def->doc = "Bandlimited sawtooth oscillatorThis is a bandlimited sawtooth oscillator ported from the \"sawtooth\" function from the Faustprogramming language.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_blsquare))
	CHECK_OB(import_class_begin(env, &t_blsquare, env->global_nspc, blsquare_ctor, blsquare_dtor))
	fun = new_dl_func("float", "freq", (m_uint)blsquare_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency, (range 0-20000)";
	fun = new_dl_func("float", "freq", (m_uint)blsquare_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency, (range 0-20000)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency, (range 0-20000)";
	fun = new_dl_func("float", "amp", (m_uint)blsquare_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (range 0-1).";
	fun = new_dl_func("float", "amp", (m_uint)blsquare_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude (range 0-1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (range 0-1).";
	fun = new_dl_func("float", "width", (m_uint)blsquare_get_width);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Duty cycle width (range 0-1).";
	fun = new_dl_func("float", "width", (m_uint)blsquare_set_width);
		arg = dl_func_add_arg(fun, "float", "width");
		arg->doc = "Duty cycle width (range 0-1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Duty cycle width (range 0-1).";
	env->class_def->doc = "Bandlimited square oscillatorThis is a bandlimited square oscillator ported from the \"squaretooth\" function from the Faustprogramming language.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_bltriangle))
	CHECK_OB(import_class_begin(env, &t_bltriangle, env->global_nspc, bltriangle_ctor, bltriangle_dtor))
	fun = new_dl_func("float", "freq", (m_uint)bltriangle_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency, (range 0-20000)";
	fun = new_dl_func("float", "freq", (m_uint)bltriangle_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency, (range 0-20000)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency, (range 0-20000)";
	fun = new_dl_func("float", "amp", (m_uint)bltriangle_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (range 0-1).";
	fun = new_dl_func("float", "amp", (m_uint)bltriangle_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude (range 0-1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (range 0-1).";
	env->class_def->doc = "Bandlimited triangle oscillatorThis is a bandlimited triangle oscillator ported from the \"triangletooth\" function from the Faustprogramming language.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_brown))
	CHECK_OB(import_class_begin(env, &t_brown, env->global_nspc, brown_ctor, brown_dtor))
	env->class_def->doc = "Brownian noise generator.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_butbp))
	CHECK_OB(import_class_begin(env, &t_butbp, env->global_nspc, butbp_ctor, butbp_dtor))
	fun = new_dl_func("float", "freq", (m_uint)butbp_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center Frequency. (in Hertz)";
	fun = new_dl_func("float", "freq", (m_uint)butbp_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Center Frequency. (in Hertz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center Frequency. (in Hertz)";
	fun = new_dl_func("float", "bw", (m_uint)butbp_get_bw);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth. (in Hertz)";
	fun = new_dl_func("float", "bw", (m_uint)butbp_set_bw);
		arg = dl_func_add_arg(fun, "float", "bw");
		arg->doc = "Bandwidth. (in Hertz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth. (in Hertz)";
	env->class_def->doc = "Bandpass Butterworth filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_butbr))
	CHECK_OB(import_class_begin(env, &t_butbr, env->global_nspc, butbr_ctor, butbr_dtor))
	fun = new_dl_func("float", "freq", (m_uint)butbr_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center Frequency. (in Hertz)";
	fun = new_dl_func("float", "freq", (m_uint)butbr_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Center Frequency. (in Hertz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center Frequency. (in Hertz)";
	fun = new_dl_func("float", "bw", (m_uint)butbr_get_bw);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth. (in Hertz)";
	fun = new_dl_func("float", "bw", (m_uint)butbr_set_bw);
		arg = dl_func_add_arg(fun, "float", "bw");
		arg->doc = "Bandwidth. (in Hertz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth. (in Hertz)";
	env->class_def->doc = "Band-reject Butterworth filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_buthp))
	CHECK_OB(import_class_begin(env, &t_buthp, env->global_nspc, buthp_ctor, buthp_dtor))
	fun = new_dl_func("float", "freq", (m_uint)buthp_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Cutoff Frequency.";
	fun = new_dl_func("float", "freq", (m_uint)buthp_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Cutoff Frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Cutoff Frequency.";
	env->class_def->doc = "Highpass Butterworth filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_butlp))
	CHECK_OB(import_class_begin(env, &t_butlp, env->global_nspc, butlp_ctor, butlp_dtor))
	fun = new_dl_func("float", "freq", (m_uint)butlp_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Cutoff Frequency.";
	fun = new_dl_func("float", "freq", (m_uint)butlp_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Cutoff Frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Cutoff Frequency.";
	env->class_def->doc = "Lowpass Butterworth filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_clip))
	CHECK_OB(import_class_begin(env, &t_clip, env->global_nspc, clip_ctor, clip_dtor))
	fun = new_dl_func("float", "lim", (m_uint)clip_get_lim);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "threshold / limiting value.";
	fun = new_dl_func("float", "lim", (m_uint)clip_set_lim);
		arg = dl_func_add_arg(fun, "float", "lim");
		arg->doc = "threshold / limiting value.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "threshold / limiting value.";
	env->class_def->doc = "Applies clip-limiting to a signal";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_clock))
	CHECK_OB(import_class_begin(env, &t_clock, env->global_nspc, clock_ctor, clock_dtor))
	fun = new_dl_func("float", "bpm", (m_uint)clock_get_bpm);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Clock tempo, in beats per minute.";
	fun = new_dl_func("float", "bpm", (m_uint)clock_set_bpm);
		arg = dl_func_add_arg(fun, "float", "bpm");
		arg->doc = "Clock tempo, in beats per minute.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Clock tempo, in beats per minute.";
	fun = new_dl_func("float", "subdiv", (m_uint)clock_get_subdiv);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Clock subdivision. 2 = eighths, 4 = 16ths, etc.";
	fun = new_dl_func("float", "subdiv", (m_uint)clock_set_subdiv);
		arg = dl_func_add_arg(fun, "float", "subdiv");
		arg->doc = "Clock subdivision. 2 = eighths, 4 = 16ths, etc.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Clock subdivision. 2 = eighths, 4 = 16ths, etc.";
	env->class_def->doc = "Resettable clock with subdivisions";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_comb))
	CHECK_OB(import_class_begin(env, &t_comb, env->global_nspc, comb_ctor, comb_dtor))
	fun = new_dl_func("void", "init", (m_uint)comb_init);
		arg = dl_func_add_arg(fun, "float", "looptime");
		arg->doc = "The loop time of the filter, in seconds. This can also be thought of as the delay time.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Comb filter";
	fun = new_dl_func("float", "revtime", (m_uint)comb_get_revtime);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Reverberation time, in seconds (RT-60).";
	fun = new_dl_func("float", "revtime", (m_uint)comb_set_revtime);
		arg = dl_func_add_arg(fun, "float", "revtime");
		arg->doc = "Reverberation time, in seconds (RT-60).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Reverberation time, in seconds (RT-60).";
	env->class_def->doc = "Comb filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_compressor))
	CHECK_OB(import_class_begin(env, &t_compressor, env->global_nspc, compressor_ctor, compressor_dtor))
	fun = new_dl_func("float", "ratio", (m_uint)compressor_get_ratio);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Ratio to compress with, a value > 1 will compress";
	fun = new_dl_func("float", "ratio", (m_uint)compressor_set_ratio);
		arg = dl_func_add_arg(fun, "float", "ratio");
		arg->doc = "Ratio to compress with, a value > 1 will compress";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Ratio to compress with, a value > 1 will compress";
	fun = new_dl_func("float", "thresh", (m_uint)compressor_get_thresh);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Threshold (in dB) 0 = max";
	fun = new_dl_func("float", "thresh", (m_uint)compressor_set_thresh);
		arg = dl_func_add_arg(fun, "float", "thresh");
		arg->doc = "Threshold (in dB) 0 = max";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Threshold (in dB) 0 = max";
	fun = new_dl_func("float", "atk", (m_uint)compressor_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Compressor attack";
	fun = new_dl_func("float", "atk", (m_uint)compressor_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Compressor attack";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Compressor attack";
	fun = new_dl_func("float", "rel", (m_uint)compressor_get_rel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Compressor release";
	fun = new_dl_func("float", "rel", (m_uint)compressor_set_rel);
		arg = dl_func_add_arg(fun, "float", "rel");
		arg->doc = "Compressor release";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Compressor release";
	env->class_def->doc = "Compressor";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_conv))
	CHECK_OB(import_class_begin(env, &t_conv, env->global_nspc, conv_ctor, conv_dtor))
	fun = new_dl_func("void", "init", (m_uint)conv_init);
		arg = dl_func_add_arg(fun, "ftbl", "ft");
		arg->doc = "Ftable used as the impulse response. ";
		arg = dl_func_add_arg(fun, "float", "iPartLen");
		arg->doc = "Partition length (in samples). Must be a power of 2. Lower values will add less latency, at the cost of requiring more CPU power. ";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Partioned convolution. This module will perform partitioned convolution on an input signal usingan ftable as an impulse response.";
	env->class_def->doc = "Partioned convolution. This module will perform partitioned convolution on an input signal usingan ftable as an impulse response.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_count))
	CHECK_OB(import_class_begin(env, &t_count, env->global_nspc, count_ctor, count_dtor))
	fun = new_dl_func("float", "count", (m_uint)count_get_count);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Number to count up to (count - 1). Decimal points will be truncated.";
	fun = new_dl_func("float", "count", (m_uint)count_set_count);
		arg = dl_func_add_arg(fun, "float", "count");
		arg->doc = "Number to count up to (count - 1). Decimal points will be truncated.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Number to count up to (count - 1). Decimal points will be truncated.";
	fun = new_dl_func("float", "mode", (m_uint)count_get_mode);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Counting mode. 0 = wrap-around, 1 = count up to N -1, then stop and spit out -1";
	fun = new_dl_func("float", "mode", (m_uint)count_set_mode);
		arg = dl_func_add_arg(fun, "float", "mode");
		arg->doc = "Counting mode. 0 = wrap-around, 1 = count up to N -1, then stop and spit out -1";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Counting mode. 0 = wrap-around, 1 = count up to N -1, then stop and spit out -1";
	env->class_def->doc = "Trigger-based fixed counter    The signal output will count from 0 to [N-1], and thenrepeat itself. Count will start when it has been triggered, otherwise it will be -1.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_crossfade))
	CHECK_OB(import_class_begin(env, &t_crossfade, env->global_nspc, crossfade_ctor, crossfade_dtor))
	fun = new_dl_func("float", "pos", (m_uint)crossfade_get_pos);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Crossfade position. 0 = all signal 1, 1 = all signal 2";
	fun = new_dl_func("float", "pos", (m_uint)crossfade_set_pos);
		arg = dl_func_add_arg(fun, "float", "pos");
		arg->doc = "Crossfade position. 0 = all signal 1, 1 = all signal 2";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Crossfade position. 0 = all signal 1, 1 = all signal 2";
	env->class_def->doc = "Crossfade two signals. This module will perform a linear crossfade between two input signals.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_dcblock))
	CHECK_OB(import_class_begin(env, &t_dcblock, env->global_nspc, dcblock_ctor, dcblock_dtor))
	env->class_def->doc = "A simple DC block filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_delay))
	CHECK_OB(import_class_begin(env, &t_delay, env->global_nspc, delay_ctor, delay_dtor))
	fun = new_dl_func("void", "init", (m_uint)delay_init);
		arg = dl_func_add_arg(fun, "float", "time");
		arg->doc = "Delay time, in seconds.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Adds a delay to an incoming signal with optional feedback.";
	fun = new_dl_func("float", "feedback", (m_uint)delay_get_feedback);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Feedback amount. Should be a value between 0-1.";
	fun = new_dl_func("float", "feedback", (m_uint)delay_set_feedback);
		arg = dl_func_add_arg(fun, "float", "feedback");
		arg->doc = "Feedback amount. Should be a value between 0-1.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Feedback amount. Should be a value between 0-1.";
	env->class_def->doc = "Adds a delay to an incoming signal with optional feedback.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_diode))
	CHECK_OB(import_class_begin(env, &t_diode, env->global_nspc, diode_ctor, diode_dtor))
	fun = new_dl_func("float", "freq", (m_uint)diode_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "freq", (m_uint)diode_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "res", (m_uint)diode_get_res);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "res", (m_uint)diode_set_res);
		arg = dl_func_add_arg(fun, "float", "res");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	env->class_def->doc = "Diode-ladder virtual analogue low-pass filterThis is a diode-ladder filter, designed by Will Pirkle. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_diskin))
	CHECK_OB(import_class_begin(env, &t_diskin, env->global_nspc, diskin_ctor, diskin_dtor))
	fun = new_dl_func("void", "init", (m_uint)diskin_init);
		arg = dl_func_add_arg(fun, "string", "filename");
		arg->doc = "Filename of the audio file.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Read from an audio file    Expects a 1-channel file matching the project samplerate. Diskin should be able to read any file format that libsndfile supports.";
	env->class_def->doc = "Read from an audio file    Expects a 1-channel file matching the project samplerate. Diskin should be able to read any file format that libsndfile supports.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_dist))
	CHECK_OB(import_class_begin(env, &t_dist, env->global_nspc, dist_ctor, dist_dtor))
	fun = new_dl_func("float", "pregain", (m_uint)dist_get_pregain);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Gain applied before waveshaping.";
	fun = new_dl_func("float", "pregain", (m_uint)dist_set_pregain);
		arg = dl_func_add_arg(fun, "float", "pregain");
		arg->doc = "Gain applied before waveshaping.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Gain applied before waveshaping.";
	fun = new_dl_func("float", "postgain", (m_uint)dist_get_postgain);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Gain applied after waveshaping";
	fun = new_dl_func("float", "postgain", (m_uint)dist_set_postgain);
		arg = dl_func_add_arg(fun, "float", "postgain");
		arg->doc = "Gain applied after waveshaping";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Gain applied after waveshaping";
	fun = new_dl_func("float", "shape1", (m_uint)dist_get_shape1);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Shape of the positive part of the signal. A value of 0 gets a flat clip.";
	fun = new_dl_func("float", "shape1", (m_uint)dist_set_shape1);
		arg = dl_func_add_arg(fun, "float", "shape1");
		arg->doc = "Shape of the positive part of the signal. A value of 0 gets a flat clip.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Shape of the positive part of the signal. A value of 0 gets a flat clip.";
	fun = new_dl_func("float", "shape2", (m_uint)dist_get_shape2);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Like shape1, only for the negative part of the signal.";
	fun = new_dl_func("float", "shape2", (m_uint)dist_set_shape2);
		arg = dl_func_add_arg(fun, "float", "shape2");
		arg->doc = "Like shape1, only for the negative part of the signal.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Like shape1, only for the negative part of the signal.";
	env->class_def->doc = "Distortion using a modified hyperbolic tangent function";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_dmetro))
	CHECK_OB(import_class_begin(env, &t_dmetro, env->global_nspc, dmetro_ctor, dmetro_dtor))
	fun = new_dl_func("float", "time", (m_uint)dmetro_get_time);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Time between triggers (in seconds). This will update at the start of each trigger.";
	fun = new_dl_func("float", "time", (m_uint)dmetro_set_time);
		arg = dl_func_add_arg(fun, "float", "time");
		arg->doc = "Time between triggers (in seconds). This will update at the start of each trigger.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Time between triggers (in seconds). This will update at the start of each trigger.";
	env->class_def->doc = "Delta Metro    Produce a set of triggers spaced apart by time.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_drip))
	CHECK_OB(import_class_begin(env, &t_drip, env->global_nspc, drip_ctor, drip_dtor))
	fun = new_dl_func("void", "init", (m_uint)drip_init);
		arg = dl_func_add_arg(fun, "float", "dettack");
		arg->doc = "Period of time over which all sound is stopped.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Water drop physical model    Physical model of the sound of dripping water. When triggered, it will produce a droplet of water.";
	fun = new_dl_func("float", "num_tubes", (m_uint)drip_get_num_tubes);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Number of units.";
	fun = new_dl_func("float", "num_tubes", (m_uint)drip_set_num_tubes);
		arg = dl_func_add_arg(fun, "float", "num_tubes");
		arg->doc = "Number of units.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Number of units.";
	fun = new_dl_func("float", "amp", (m_uint)drip_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude.";
	fun = new_dl_func("float", "amp", (m_uint)drip_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude.";
	fun = new_dl_func("float", "damp", (m_uint)drip_get_damp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The damping factor. Maximum value is 2.0.";
	fun = new_dl_func("float", "damp", (m_uint)drip_set_damp);
		arg = dl_func_add_arg(fun, "float", "damp");
		arg->doc = "The damping factor. Maximum value is 2.0.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The damping factor. Maximum value is 2.0.";
	fun = new_dl_func("float", "shake_max", (m_uint)drip_get_shake_max);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The amount of energy to add back into the system.";
	fun = new_dl_func("float", "shake_max", (m_uint)drip_set_shake_max);
		arg = dl_func_add_arg(fun, "float", "shake_max");
		arg->doc = "The amount of energy to add back into the system.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The amount of energy to add back into the system.";
	fun = new_dl_func("float", "freq", (m_uint)drip_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Main resonant frequency.";
	fun = new_dl_func("float", "freq", (m_uint)drip_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Main resonant frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Main resonant frequency.";
	fun = new_dl_func("float", "freq1", (m_uint)drip_get_freq1);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The first resonant frequency.";
	fun = new_dl_func("float", "freq1", (m_uint)drip_set_freq1);
		arg = dl_func_add_arg(fun, "float", "freq1");
		arg->doc = "The first resonant frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The first resonant frequency.";
	fun = new_dl_func("float", "freq2", (m_uint)drip_get_freq2);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The second resonant frequency.";
	fun = new_dl_func("float", "freq2", (m_uint)drip_set_freq2);
		arg = dl_func_add_arg(fun, "float", "freq2");
		arg->doc = "The second resonant frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The second resonant frequency.";
	env->class_def->doc = "Water drop physical model    Physical model of the sound of dripping water. When triggered, it will produce a droplet of water.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_dtrig))
	CHECK_OB(import_class_begin(env, &t_dtrig, env->global_nspc, dtrig_ctor, dtrig_dtor))
	fun = new_dl_func("void", "init", (m_uint)dtrig_init);
		arg = dl_func_add_arg(fun, "ftbl", "ft");
		arg->doc = "An ftable containing times in seconds.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = " Delta trigger    This is able to create spaced out triggers. It is set off by a single trigger.";
	fun = new_dl_func("int", "loop", (m_uint)dtrig_get_loop);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "When set to 1, dtrig will wrap around and loop again.";
	fun = new_dl_func("int", "loop", (m_uint)dtrig_set_loop);
		arg = dl_func_add_arg(fun, "int", "loop");
		arg->doc = "When set to 1, dtrig will wrap around and loop again.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "When set to 1, dtrig will wrap around and loop again.";
	fun = new_dl_func("float", "delay", (m_uint)dtrig_get_delay);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "This sets a delay (in seconds) on the triggered output when it is initially triggered. This is useful for rhythmic sequences with rests in the beginnings.";
	fun = new_dl_func("float", "delay", (m_uint)dtrig_set_delay);
		arg = dl_func_add_arg(fun, "float", "delay");
		arg->doc = "This sets a delay (in seconds) on the triggered output when it is initially triggered. This is useful for rhythmic sequences with rests in the beginnings.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "This sets a delay (in seconds) on the triggered output when it is initially triggered. This is useful for rhythmic sequences with rests in the beginnings.";
	fun = new_dl_func("float", "scale", (m_uint)dtrig_get_scale);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Scales the timing signal. A scale of 1 is normal, a scale of 2 will double the duration, and a scale of 0.5 will halve it.";
	fun = new_dl_func("float", "scale", (m_uint)dtrig_set_scale);
		arg = dl_func_add_arg(fun, "float", "scale");
		arg->doc = "Scales the timing signal. A scale of 1 is normal, a scale of 2 will double the duration, and a scale of 0.5 will halve it.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Scales the timing signal. A scale of 1 is normal, a scale of 2 will double the duration, and a scale of 0.5 will halve it.";
	env->class_def->doc = " Delta trigger    This is able to create spaced out triggers. It is set off by a single trigger.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_dust))
	CHECK_OB(import_class_begin(env, &t_dust, env->global_nspc, dust_ctor, dust_dtor))
	fun = new_dl_func("float", "amp", (m_uint)dust_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "amp", (m_uint)dust_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "density", (m_uint)dust_get_density);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "density", (m_uint)dust_set_density);
		arg = dl_func_add_arg(fun, "float", "density");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("int", "bipolar", (m_uint)dust_get_bipolar);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bipolar flag. A non-zero makes the signal bipolar as opposed to unipolar. ";
	fun = new_dl_func("int", "bipolar", (m_uint)dust_set_bipolar);
		arg = dl_func_add_arg(fun, "int", "bipolar");
		arg->doc = "Bipolar flag. A non-zero makes the signal bipolar as opposed to unipolar. ";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bipolar flag. A non-zero makes the signal bipolar as opposed to unipolar. ";
	env->class_def->doc = "A series of random impulses";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_eqfil))
	CHECK_OB(import_class_begin(env, &t_eqfil, env->global_nspc, eqfil_ctor, eqfil_dtor))
	fun = new_dl_func("float", "freq", (m_uint)eqfil_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The center frequency of the filter";
	fun = new_dl_func("float", "freq", (m_uint)eqfil_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "The center frequency of the filter";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The center frequency of the filter";
	fun = new_dl_func("float", "bw", (m_uint)eqfil_get_bw);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The peak/notch bandwidth in Hertz";
	fun = new_dl_func("float", "bw", (m_uint)eqfil_set_bw);
		arg = dl_func_add_arg(fun, "float", "bw");
		arg->doc = "The peak/notch bandwidth in Hertz";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The peak/notch bandwidth in Hertz";
	fun = new_dl_func("float", "gain", (m_uint)eqfil_get_gain);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The peak/notch gain";
	fun = new_dl_func("float", "gain", (m_uint)eqfil_set_gain);
		arg = dl_func_add_arg(fun, "float", "gain");
		arg->doc = "The peak/notch gain";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The peak/notch gain";
	env->class_def->doc = "2nd order tunable equalization filter    This provides a peak/notch filter for building parametric/graphic equalizers. With gain above 1, there will be a peak at the center frequency with a width dependent on bw. If gain is less than 1, a notch is formed around the center frequency (freq).    ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_expon))
	CHECK_OB(import_class_begin(env, &t_expon, env->global_nspc, expon_ctor, expon_dtor))
	fun = new_dl_func("float", "a", (m_uint)expon_get_a);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Inital point.";
	fun = new_dl_func("float", "a", (m_uint)expon_set_a);
		arg = dl_func_add_arg(fun, "float", "a");
		arg->doc = "Inital point.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Inital point.";
	fun = new_dl_func("float", "dur", (m_uint)expon_get_dur);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Duration (in seconds)";
	fun = new_dl_func("float", "dur", (m_uint)expon_set_dur);
		arg = dl_func_add_arg(fun, "float", "dur");
		arg->doc = "Duration (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Duration (in seconds)";
	fun = new_dl_func("float", "b", (m_uint)expon_get_b);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "End point";
	fun = new_dl_func("float", "b", (m_uint)expon_set_b);
		arg = dl_func_add_arg(fun, "float", "b");
		arg->doc = "End point";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "End point";
	env->class_def->doc = "Produce a line segment with exponential slopeThis will generate a line from value A to value B in given amount of time. When it reaches it's target, it will stay at that value. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_fof))
	CHECK_OB(import_class_begin(env, &t_fof, env->global_nspc, fof_ctor, fof_dtor))
	fun = new_dl_func("void", "init", (m_uint)fof_init);
		arg = dl_func_add_arg(fun, "ftbl", "sine");
		arg->doc = "ftable for sine wave.";
		arg = dl_func_add_arg(fun, "ftbl", "win");
		arg->doc = "Ftable for envelope function (use either gen_line or gen_sinecomp)";
		arg = dl_func_add_arg(fun, "int", "iolaps");
		arg->doc = "Maximum number of foflet overlaps.";
		arg = dl_func_add_arg(fun, "float", "iphs");
		arg->doc = "Phase";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Produces sinusoid bursts for granular and formant synthesis";
	fun = new_dl_func("float", "amp", (m_uint)fof_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Overall amplitude";
	fun = new_dl_func("float", "amp", (m_uint)fof_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Overall amplitude";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Overall amplitude";
	fun = new_dl_func("float", "fund", (m_uint)fof_get_fund);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Fundamental frequency";
	fun = new_dl_func("float", "fund", (m_uint)fof_set_fund);
		arg = dl_func_add_arg(fun, "float", "fund");
		arg->doc = "Fundamental frequency";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Fundamental frequency";
	fun = new_dl_func("float", "form", (m_uint)fof_get_form);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Formant frequency.";
	fun = new_dl_func("float", "form", (m_uint)fof_set_form);
		arg = dl_func_add_arg(fun, "float", "form");
		arg->doc = "Formant frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Formant frequency.";
	fun = new_dl_func("float", "oct", (m_uint)fof_get_oct);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";
	fun = new_dl_func("float", "oct", (m_uint)fof_set_oct);
		arg = dl_func_add_arg(fun, "float", "oct");
		arg->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";
	fun = new_dl_func("float", "band", (m_uint)fof_get_band);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";
	fun = new_dl_func("float", "band", (m_uint)fof_set_band);
		arg = dl_func_add_arg(fun, "float", "band");
		arg->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";
	fun = new_dl_func("float", "ris", (m_uint)fof_get_ris);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Rise of sinusoid burst (in seconds)";
	fun = new_dl_func("float", "ris", (m_uint)fof_set_ris);
		arg = dl_func_add_arg(fun, "float", "ris");
		arg->doc = "Rise of sinusoid burst (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Rise of sinusoid burst (in seconds)";
	fun = new_dl_func("float", "dec", (m_uint)fof_get_dec);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Decay of the sinusoid burst (in seconds).";
	fun = new_dl_func("float", "dec", (m_uint)fof_set_dec);
		arg = dl_func_add_arg(fun, "float", "dec");
		arg->doc = "Decay of the sinusoid burst (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Decay of the sinusoid burst (in seconds).";
	fun = new_dl_func("float", "dur", (m_uint)fof_get_dur);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "OVerall duration of sinusoid burst (in seconds).";
	fun = new_dl_func("float", "dur", (m_uint)fof_set_dur);
		arg = dl_func_add_arg(fun, "float", "dur");
		arg->doc = "OVerall duration of sinusoid burst (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "OVerall duration of sinusoid burst (in seconds).";
	env->class_def->doc = "Produces sinusoid bursts for granular and formant synthesis";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_fofilt))
	CHECK_OB(import_class_begin(env, &t_fofilt, env->global_nspc, fofilt_ctor, fofilt_dtor))
	fun = new_dl_func("float", "freq", (m_uint)fofilt_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency.";
	fun = new_dl_func("float", "freq", (m_uint)fofilt_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Center frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency.";
	fun = new_dl_func("float", "atk", (m_uint)fofilt_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Impulse response attack time (in seconds).";
	fun = new_dl_func("float", "atk", (m_uint)fofilt_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Impulse response attack time (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Impulse response attack time (in seconds).";
	fun = new_dl_func("float", "dec", (m_uint)fofilt_get_dec);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Impulse reponse decay time (in seconds)";
	fun = new_dl_func("float", "dec", (m_uint)fofilt_set_dec);
		arg = dl_func_add_arg(fun, "float", "dec");
		arg->doc = "Impulse reponse decay time (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Impulse reponse decay time (in seconds)";
	env->class_def->doc = "Formant filter    When fed with a pulse train, it will generate a series of overlapping grains. Overlapping will occur when 1/freq < dec, but there is no upper limit on the number of overlaps. (cited from www.csounds.com/manual/html/fofilter.html)";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_fog))
	CHECK_OB(import_class_begin(env, &t_fog, env->global_nspc, fog_ctor, fog_dtor))
	fun = new_dl_func("void", "init", (m_uint)fog_init);
		arg = dl_func_add_arg(fun, "ftbl", "wav");
		arg->doc = "ftable for sample.";
		arg = dl_func_add_arg(fun, "ftbl", "win");
		arg->doc = "Ftable for envelope function (use either gen_line or gen_sinecomp)";
		arg = dl_func_add_arg(fun, "int", "iolaps");
		arg->doc = "Maximum number of foglet overlaps.";
		arg = dl_func_add_arg(fun, "float", "iphs");
		arg->doc = "Phase";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sucession of grains from data in a stored function table";
	fun = new_dl_func("float", "amp", (m_uint)fog_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Overall amplitude";
	fun = new_dl_func("float", "amp", (m_uint)fog_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Overall amplitude";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Overall amplitude";
	fun = new_dl_func("float", "dens", (m_uint)fog_get_dens);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Density. The frequency of grains/second.";
	fun = new_dl_func("float", "dens", (m_uint)fog_set_dens);
		arg = dl_func_add_arg(fun, "float", "dens");
		arg->doc = "Density. The frequency of grains/second.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Density. The frequency of grains/second.";
	fun = new_dl_func("float", "trans", (m_uint)fog_get_trans);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Transposition, in terms of playback speed";
	fun = new_dl_func("float", "trans", (m_uint)fog_set_trans);
		arg = dl_func_add_arg(fun, "float", "trans");
		arg->doc = "Transposition, in terms of playback speed";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Transposition, in terms of playback speed";
	fun = new_dl_func("float", "spd", (m_uint)fog_get_spd);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Starting sample index, normalized 0-1.";
	fun = new_dl_func("float", "spd", (m_uint)fog_set_spd);
		arg = dl_func_add_arg(fun, "float", "spd");
		arg->doc = "Starting sample index, normalized 0-1.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Starting sample index, normalized 0-1.";
	fun = new_dl_func("float", "oct", (m_uint)fog_get_oct);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";
	fun = new_dl_func("float", "oct", (m_uint)fog_set_oct);
		arg = dl_func_add_arg(fun, "float", "oct");
		arg->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Octaviation index, if greater than zero, lowers the effective fund frequency by attenuating odd-numbered sine bursts. whole numbers are full octaves. fractions transpositional.";
	fun = new_dl_func("float", "band", (m_uint)fog_get_band);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";
	fun = new_dl_func("float", "band", (m_uint)fog_set_band);
		arg = dl_func_add_arg(fun, "float", "band");
		arg->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth (in -6db) expressed in Hz. The bandwidth determines the rate of exponential decay throughout the sineburst, before the enveloping is applied.";
	fun = new_dl_func("float", "ris", (m_uint)fog_get_ris);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Rise of sinusoid burst (in seconds)";
	fun = new_dl_func("float", "ris", (m_uint)fog_set_ris);
		arg = dl_func_add_arg(fun, "float", "ris");
		arg->doc = "Rise of sinusoid burst (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Rise of sinusoid burst (in seconds)";
	fun = new_dl_func("float", "dec", (m_uint)fog_get_dec);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Decay of the sinusoid burst (in seconds).";
	fun = new_dl_func("float", "dec", (m_uint)fog_set_dec);
		arg = dl_func_add_arg(fun, "float", "dec");
		arg->doc = "Decay of the sinusoid burst (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Decay of the sinusoid burst (in seconds).";
	fun = new_dl_func("float", "dur", (m_uint)fog_get_dur);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "OVerall duration of sinusoid burst (in seconds).";
	fun = new_dl_func("float", "dur", (m_uint)fog_set_dur);
		arg = dl_func_add_arg(fun, "float", "dur");
		arg->doc = "OVerall duration of sinusoid burst (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "OVerall duration of sinusoid burst (in seconds).";
	env->class_def->doc = "Sucession of grains from data in a stored function table";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_fold))
	CHECK_OB(import_class_begin(env, &t_fold, env->global_nspc, fold_ctor, fold_dtor))
	fun = new_dl_func("float", "incr", (m_uint)fold_get_incr);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Increment";
	fun = new_dl_func("float", "incr", (m_uint)fold_set_incr);
		arg = dl_func_add_arg(fun, "float", "incr");
		arg->doc = "Increment";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Increment";
	env->class_def->doc = "Adds artificial foldover to an audio signal    Primarily created for use with Decimator.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_fosc))
	CHECK_OB(import_class_begin(env, &t_fosc, env->global_nspc, fosc_ctor, fosc_dtor))
	fun = new_dl_func("void", "init", (m_uint)fosc_init);
		arg = dl_func_add_arg(fun, "ftbl", "tbl");
		arg->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "FM oscilator pair with linear interpolation";
	fun = new_dl_func("float", "freq", (m_uint)fosc_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (in Hz)";
	fun = new_dl_func("float", "freq", (m_uint)fosc_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency (in Hz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (in Hz)";
	fun = new_dl_func("float", "amp", (m_uint)fosc_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (typically a value between 0 and 1).";
	fun = new_dl_func("float", "amp", (m_uint)fosc_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude (typically a value between 0 and 1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (typically a value between 0 and 1).";
	fun = new_dl_func("float", "car", (m_uint)fosc_get_car);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Carrier frequency, expressed as a ratio number in C:M ratio. Typically an integer.";
	fun = new_dl_func("float", "car", (m_uint)fosc_set_car);
		arg = dl_func_add_arg(fun, "float", "car");
		arg->doc = "Carrier frequency, expressed as a ratio number in C:M ratio. Typically an integer.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Carrier frequency, expressed as a ratio number in C:M ratio. Typically an integer.";
	fun = new_dl_func("float", "mod", (m_uint)fosc_get_mod);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Modulator frequency, expressed as a ratio number in C:M ratio. Typically an integer.";
	fun = new_dl_func("float", "mod", (m_uint)fosc_set_mod);
		arg = dl_func_add_arg(fun, "float", "mod");
		arg->doc = "Modulator frequency, expressed as a ratio number in C:M ratio. Typically an integer.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Modulator frequency, expressed as a ratio number in C:M ratio. Typically an integer.";
	fun = new_dl_func("float", "indx", (m_uint)fosc_get_indx);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Modulation index. Most commercial synthesizers have a range from 0-8, but there's nothing stopping you from going beyond that.";
	fun = new_dl_func("float", "indx", (m_uint)fosc_set_indx);
		arg = dl_func_add_arg(fun, "float", "indx");
		arg->doc = "Modulation index. Most commercial synthesizers have a range from 0-8, but there's nothing stopping you from going beyond that.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Modulation index. Most commercial synthesizers have a range from 0-8, but there's nothing stopping you from going beyond that.";
	env->class_def->doc = "FM oscilator pair with linear interpolation";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_gbuzz))
	CHECK_OB(import_class_begin(env, &t_gbuzz, env->global_nspc, gbuzz_ctor, gbuzz_dtor))
	fun = new_dl_func("void", "init", (m_uint)gbuzz_init);
		arg = dl_func_add_arg(fun, "ftbl", "ft");
		arg->doc = "Soundpipe function table used internally. This should be a sine wave.";
		arg = dl_func_add_arg(fun, "float", "iphs");
		arg->doc = "Phase to start on (in the range 0-1)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Series of partials from the harmonic series    GBuzz comes from the \"buzz\" family of Csound opcodes, and is capable of producing a rich spectrum of harmonic content, useful for subtractive synthesis implementation.";
	fun = new_dl_func("float", "freq", (m_uint)gbuzz_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency, in Hertz.";
	fun = new_dl_func("float", "freq", (m_uint)gbuzz_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency, in Hertz.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency, in Hertz.";
	fun = new_dl_func("float", "amp", (m_uint)gbuzz_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (Typically a value between 0 and 1).";
	fun = new_dl_func("float", "amp", (m_uint)gbuzz_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude (Typically a value between 0 and 1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (Typically a value between 0 and 1).";
	fun = new_dl_func("float", "nharm", (m_uint)gbuzz_get_nharm);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Number of harmonics.";
	fun = new_dl_func("float", "nharm", (m_uint)gbuzz_set_nharm);
		arg = dl_func_add_arg(fun, "float", "nharm");
		arg->doc = "Number of harmonics.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Number of harmonics.";
	fun = new_dl_func("float", "lharm", (m_uint)gbuzz_get_lharm);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Lowest harmonic present. This should be a whole number integer.";
	fun = new_dl_func("float", "lharm", (m_uint)gbuzz_set_lharm);
		arg = dl_func_add_arg(fun, "float", "lharm");
		arg->doc = "Lowest harmonic present. This should be a whole number integer.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Lowest harmonic present. This should be a whole number integer.";
	fun = new_dl_func("float", "mul", (m_uint)gbuzz_get_mul);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Multiplier. This determines the relative strength of each harmonic.";
	fun = new_dl_func("float", "mul", (m_uint)gbuzz_set_mul);
		arg = dl_func_add_arg(fun, "float", "mul");
		arg->doc = "Multiplier. This determines the relative strength of each harmonic.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Multiplier. This determines the relative strength of each harmonic.";
	env->class_def->doc = "Series of partials from the harmonic series    GBuzz comes from the \"buzz\" family of Csound opcodes, and is capable of producing a rich spectrum of harmonic content, useful for subtractive synthesis implementation.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_hilbert))
	CHECK_OB(import_class_begin(env, &t_hilbert, env->global_nspc, hilbert_ctor, hilbert_dtor))
	env->class_def->doc = "Hilbert transform";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_in))
	CHECK_OB(import_class_begin(env, &t_in, env->global_nspc, in_ctor, in_dtor))
	env->class_def->doc = "Reads from standard input.    Expects type of SPFLOAT, which by default is a float. If the input data is larger than the number of samples, you will get a complaint about a broken pipe (but it will still work). If there is no input data from STDIN, it will hang.The expected use case of sp_in is to utilize pipes from the commandline, like so:cat /dev/urandom | ./my_programAssuming my_program is using sp_in, this will write /dev/urandom (essentially white noise) to an audio file.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_incr))
	CHECK_OB(import_class_begin(env, &t_incr, env->global_nspc, incr_ctor, incr_dtor))
	fun = new_dl_func("void", "init", (m_uint)incr_init);
		arg = dl_func_add_arg(fun, "float", "val");
		arg->doc = "Initial value";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Trigger-based IncrementorWhen triggered, this module will increment and decrement a value bounded between a minand max. Initially, this was designed for the specific use case of interfacing with thegriffin knob. ";
	fun = new_dl_func("float", "step", (m_uint)incr_get_step);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Step value to increment by.";
	fun = new_dl_func("float", "step", (m_uint)incr_set_step);
		arg = dl_func_add_arg(fun, "float", "step");
		arg->doc = "Step value to increment by.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Step value to increment by.";
	fun = new_dl_func("float", "min", (m_uint)incr_get_min);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value";
	fun = new_dl_func("float", "min", (m_uint)incr_set_min);
		arg = dl_func_add_arg(fun, "float", "min");
		arg->doc = "Minimum value";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value";
	fun = new_dl_func("float", "max", (m_uint)incr_get_max);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value";
	fun = new_dl_func("float", "max", (m_uint)incr_set_max);
		arg = dl_func_add_arg(fun, "float", "max");
		arg->doc = "Maximum value";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value";
	env->class_def->doc = "Trigger-based IncrementorWhen triggered, this module will increment and decrement a value bounded between a minand max. Initially, this was designed for the specific use case of interfacing with thegriffin knob. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_jcrev))
	CHECK_OB(import_class_begin(env, &t_jcrev, env->global_nspc, jcrev_ctor, jcrev_dtor))
	env->class_def->doc = "John Chowning reverberator    This is was built using the JC reverb implentation found in FAUST. According to the source code, the specifications forthis implementation were found on an old SAIL DART backup tape.  This class is derived from the CLM JCRev function, which is based on the use of  networks of simple allpass and comb delay filters.  This class implements three series  allpass units, followed by four parallel comb filters, and two decorrelation delay lines in  parallel at the output.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_jitter))
	CHECK_OB(import_class_begin(env, &t_jitter, env->global_nspc, jitter_ctor, jitter_dtor))
	fun = new_dl_func("float", "amp", (m_uint)jitter_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The amplitude of the line. Will produce values in the range of (+/-)amp.";
	fun = new_dl_func("float", "amp", (m_uint)jitter_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "The amplitude of the line. Will produce values in the range of (+/-)amp.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The amplitude of the line. Will produce values in the range of (+/-)amp.";
	fun = new_dl_func("float", "cpsMin", (m_uint)jitter_get_cpsMin);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The minimum frequency of change in Hz.";
	fun = new_dl_func("float", "cpsMin", (m_uint)jitter_set_cpsMin);
		arg = dl_func_add_arg(fun, "float", "cpsMin");
		arg->doc = "The minimum frequency of change in Hz.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The minimum frequency of change in Hz.";
	fun = new_dl_func("float", "cpsMax", (m_uint)jitter_get_cpsMax);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The maximum frequency of change in Hz.";
	fun = new_dl_func("float", "cpsMax", (m_uint)jitter_set_cpsMax);
		arg = dl_func_add_arg(fun, "float", "cpsMax");
		arg->doc = "The maximum frequency of change in Hz.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The maximum frequency of change in Hz.";
	env->class_def->doc = "A signal with random fluctuations     This is useful for emulating jitter found in analogue equipment. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_line))
	CHECK_OB(import_class_begin(env, &t_line, env->global_nspc, line_ctor, line_dtor))
	fun = new_dl_func("float", "a", (m_uint)line_get_a);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Inital point.";
	fun = new_dl_func("float", "a", (m_uint)line_set_a);
		arg = dl_func_add_arg(fun, "float", "a");
		arg->doc = "Inital point.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Inital point.";
	fun = new_dl_func("float", "dur", (m_uint)line_get_dur);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Duration (in seconds)";
	fun = new_dl_func("float", "dur", (m_uint)line_set_dur);
		arg = dl_func_add_arg(fun, "float", "dur");
		arg->doc = "Duration (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Duration (in seconds)";
	fun = new_dl_func("float", "b", (m_uint)line_get_b);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "End point";
	fun = new_dl_func("float", "b", (m_uint)line_set_b);
		arg = dl_func_add_arg(fun, "float", "b");
		arg->doc = "End point";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "End point";
	env->class_def->doc = "Produce a line segment with linear slopeThis will generate a line from value A to value B in given amount of time. When it reaches it's target, it will stay at that value. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_lpf18))
	CHECK_OB(import_class_begin(env, &t_lpf18, env->global_nspc, lpf18_ctor, lpf18_dtor))
	fun = new_dl_func("float", "cutoff", (m_uint)lpf18_get_cutoff);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter cutoff frequency, in Hertz";
	fun = new_dl_func("float", "cutoff", (m_uint)lpf18_set_cutoff);
		arg = dl_func_add_arg(fun, "float", "cutoff");
		arg->doc = "Filter cutoff frequency, in Hertz";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter cutoff frequency, in Hertz";
	fun = new_dl_func("float", "res", (m_uint)lpf18_get_res);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Resonance. Expects a value in the range 0-1. A value of 1.0 will self oscillate at the cutoff frequency.";
	fun = new_dl_func("float", "res", (m_uint)lpf18_set_res);
		arg = dl_func_add_arg(fun, "float", "res");
		arg->doc = "Resonance. Expects a value in the range 0-1. A value of 1.0 will self oscillate at the cutoff frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Resonance. Expects a value in the range 0-1. A value of 1.0 will self oscillate at the cutoff frequency.";
	fun = new_dl_func("float", "dist", (m_uint)lpf18_get_dist);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Distortion amount.";
	fun = new_dl_func("float", "dist", (m_uint)lpf18_set_dist);
		arg = dl_func_add_arg(fun, "float", "dist");
		arg->doc = "Distortion amount.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Distortion amount.";
	env->class_def->doc = "3-pole (18 db/oct slope) Low-Pass filter with resonance and tanh distortion";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_maygate))
	CHECK_OB(import_class_begin(env, &t_maygate, env->global_nspc, maygate_ctor, maygate_dtor))
	fun = new_dl_func("float", "prob", (m_uint)maygate_get_prob);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Probability of maygate. This is a value between 0-1. The closer to 1, the more likely the maygate will let a signal through.";
	fun = new_dl_func("float", "prob", (m_uint)maygate_set_prob);
		arg = dl_func_add_arg(fun, "float", "prob");
		arg->doc = "Probability of maygate. This is a value between 0-1. The closer to 1, the more likely the maygate will let a signal through.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Probability of maygate. This is a value between 0-1. The closer to 1, the more likely the maygate will let a signal through.";
	fun = new_dl_func("int", "mode", (m_uint)maygate_get_mode);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "If mode is nonzero, maygate will output one sample triggers instead of a gate signal.";
	fun = new_dl_func("int", "mode", (m_uint)maygate_set_mode);
		arg = dl_func_add_arg(fun, "int", "mode");
		arg->doc = "If mode is nonzero, maygate will output one sample triggers instead of a gate signal.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "If mode is nonzero, maygate will output one sample triggers instead of a gate signal.";
	env->class_def->doc = "A randomly open or closed \"maybe gate\"    It takes in a trigger, and then it will randomly decide to turn the gate on or not. One particular application for maygate is to arbitrarily turn on/off sends to effects. One specific example of this could be a randomized reverb throw on a snare.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_metro))
	CHECK_OB(import_class_begin(env, &t_metro, env->global_nspc, metro_ctor, metro_dtor))
	fun = new_dl_func("float", "freq", (m_uint)metro_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The frequency to repeat.";
	fun = new_dl_func("float", "freq", (m_uint)metro_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "The frequency to repeat.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The frequency to repeat.";
	env->class_def->doc = "Metronome    Metro produces a series of 1-sample ticks at a regular rate. Typically, this is used alongside trigger-driven modules.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_mincer))
	CHECK_OB(import_class_begin(env, &t_mincer, env->global_nspc, mincer_ctor, mincer_dtor))
	fun = new_dl_func("void", "init", (m_uint)mincer_init);
		arg = dl_func_add_arg(fun, "ftbl", "ft");
		arg->doc = "ftable containing an audio file.";
		arg = dl_func_add_arg(fun, "int", "winsize");
		arg->doc = "FFT window size. Should be a power of 2.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Phase-locked vocoder.    Mincer is a phase-locked vocoder. It has the ability to play back an audio file loaded into an ftable like a sampler would. Unlike a typical sampler, mincer allowstime and pitch to be controlled separately. ";
	fun = new_dl_func("float", "time", (m_uint)mincer_get_time);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Position in time. When non-changing it will do a spectral freeze of a the current point in time.";
	fun = new_dl_func("float", "time", (m_uint)mincer_set_time);
		arg = dl_func_add_arg(fun, "float", "time");
		arg->doc = "Position in time. When non-changing it will do a spectral freeze of a the current point in time.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Position in time. When non-changing it will do a spectral freeze of a the current point in time.";
	fun = new_dl_func("float", "amp", (m_uint)mincer_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude.";
	fun = new_dl_func("float", "amp", (m_uint)mincer_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude.";
	fun = new_dl_func("float", "pitch", (m_uint)mincer_get_pitch);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Pitch ratio. A value of. 1  normal, 2 is double speed, 0.5 is halfspeed, etc.";
	fun = new_dl_func("float", "pitch", (m_uint)mincer_set_pitch);
		arg = dl_func_add_arg(fun, "float", "pitch");
		arg->doc = "Pitch ratio. A value of. 1  normal, 2 is double speed, 0.5 is halfspeed, etc.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Pitch ratio. A value of. 1  normal, 2 is double speed, 0.5 is halfspeed, etc.";
	env->class_def->doc = "Phase-locked vocoder.    Mincer is a phase-locked vocoder. It has the ability to play back an audio file loaded into an ftable like a sampler would. Unlike a typical sampler, mincer allowstime and pitch to be controlled separately. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_mode))
	CHECK_OB(import_class_begin(env, &t_mode, env->global_nspc, mode_ctor, mode_dtor))
	fun = new_dl_func("float", "freq", (m_uint)mode_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Resonant frequency of the filter.";
	fun = new_dl_func("float", "freq", (m_uint)mode_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Resonant frequency of the filter.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Resonant frequency of the filter.";
	fun = new_dl_func("float", "q", (m_uint)mode_get_q);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Quality factor of the filter. Roughly equal to q/freq.";
	fun = new_dl_func("float", "q", (m_uint)mode_set_q);
		arg = dl_func_add_arg(fun, "float", "q");
		arg->doc = "Quality factor of the filter. Roughly equal to q/freq.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Quality factor of the filter. Roughly equal to q/freq.";
	env->class_def->doc = "Resonance filter used for modal synthesis    Plucked and bell sounds can be created by passing an impulse through a combination of modal filters. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_moogladder))
	CHECK_OB(import_class_begin(env, &t_moogladder, env->global_nspc, moogladder_ctor, moogladder_dtor))
	fun = new_dl_func("float", "freq", (m_uint)moogladder_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter cutoff frequency.";
	fun = new_dl_func("float", "freq", (m_uint)moogladder_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Filter cutoff frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter cutoff frequency.";
	fun = new_dl_func("float", "res", (m_uint)moogladder_get_res);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter resonance";
	fun = new_dl_func("float", "res", (m_uint)moogladder_set_res);
		arg = dl_func_add_arg(fun, "float", "res");
		arg->doc = "Filter resonance";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter resonance";
	env->class_def->doc = "Low pass resonant filter based on the Moogladder filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_noise))
	CHECK_OB(import_class_begin(env, &t_noise, env->global_nspc, noise_ctor, noise_dtor))
	fun = new_dl_func("float", "amp", (m_uint)noise_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude. (Value between 0-1).";
	fun = new_dl_func("float", "amp", (m_uint)noise_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude. (Value between 0-1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude. (Value between 0-1).";
	env->class_def->doc = "White noise generator";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_nsmp))
	CHECK_OB(import_class_begin(env, &t_nsmp, env->global_nspc, nsmp_ctor, nsmp_dtor))
	fun = new_dl_func("void", "init", (m_uint)nsmp_init);
		arg = dl_func_add_arg(fun, "ftbl", "ft");
		arg->doc = "ftbl of the audio file. It should be mono.";
		arg = dl_func_add_arg(fun, "int", "sr");
		arg->doc = "samplerate.";
		arg = dl_func_add_arg(fun, "string", "init");
		arg->doc = "ini file.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Nanosamp: a tiny sampler built for Soundpipe    A nanosamp file is comprised of a mono audio file and an ini file. Nanosamp is geared towards percussive and found sound sample players, and is intended to be combined with soundpipe modules.The ini file contains mappings that correspond to the audio file. Such an entry would look like this:[keyword]pos = 2.3size = 3</pre>In this instance, an entry called \"keyword\" has been made, starting at 2.3 seconds in theaudio file, with a length of 3 seconds. An example file oneart.ini has been created in theexamples folder.The SoundPipe implementation of nanosamp will automatically index the entriesin the order they appear in the INI file and must be selected this way by changing the indexparameter. Soundpipe will only select the new entry when the trigger input is a non-zero value.";
	fun = new_dl_func("int", "index", (m_uint)nsmp_get_index);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "This is an optional parameter. These values are always set to a value by default, and can be set after the init function has been called.";
	fun = new_dl_func("int", "index", (m_uint)nsmp_set_index);
		arg = dl_func_add_arg(fun, "int", "index");
		arg->doc = "This is an optional parameter. These values are always set to a value by default, and can be set after the init function has been called.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "This is an optional parameter. These values are always set to a value by default, and can be set after the init function has been called.";
	env->class_def->doc = "Nanosamp: a tiny sampler built for Soundpipe    A nanosamp file is comprised of a mono audio file and an ini file. Nanosamp is geared towards percussive and found sound sample players, and is intended to be combined with soundpipe modules.The ini file contains mappings that correspond to the audio file. Such an entry would look like this:[keyword]pos = 2.3size = 3</pre>In this instance, an entry called \"keyword\" has been made, starting at 2.3 seconds in theaudio file, with a length of 3 seconds. An example file oneart.ini has been created in theexamples folder.The SoundPipe implementation of nanosamp will automatically index the entriesin the order they appear in the INI file and must be selected this way by changing the indexparameter. Soundpipe will only select the new entry when the trigger input is a non-zero value.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_osc))
	CHECK_OB(import_class_begin(env, &t_osc, env->global_nspc, osc_ctor, osc_dtor))
	fun = new_dl_func("void", "init", (m_uint)osc_init);
		arg = dl_func_add_arg(fun, "ftbl", "tbl");
		arg->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";
		arg = dl_func_add_arg(fun, "float", "phase");
		arg->doc = "Initial phase of waveform, expects a value 0-1";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = " Table-lookup oscilator with linear interpolation";
	fun = new_dl_func("float", "freq", (m_uint)osc_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (in Hz)";
	fun = new_dl_func("float", "freq", (m_uint)osc_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency (in Hz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (in Hz)";
	fun = new_dl_func("float", "amp", (m_uint)osc_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (typically a value between 0 and 1).";
	fun = new_dl_func("float", "amp", (m_uint)osc_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude (typically a value between 0 and 1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (typically a value between 0 and 1).";
	env->class_def->doc = " Table-lookup oscilator with linear interpolation";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_oscmorph))
	CHECK_OB(import_class_begin(env, &t_oscmorph, env->global_nspc, oscmorph_ctor, oscmorph_dtor))
	fun = new_dl_func("void", "init", (m_uint)oscmorph_init);
		arg = dl_func_add_arg(fun, "ftbl[]", "tbl");
		arg->doc = "An array of ftables to read from. Note: the size of these tables must be a power of 2 (and the same size as well).";
		arg = dl_func_add_arg(fun, "int", "nft");
		arg->doc = "Number of ftbls";
		arg = dl_func_add_arg(fun, "float", "phase");
		arg->doc = "Initial phase of waveform, expects a value 0-1";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Wavetable morphing oscillatorThis is an oscillator with linear interpolation that is capable of morphing between an arbitrary number of wavetables. ";
	fun = new_dl_func("float", "freq", (m_uint)oscmorph_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (in Hz)";
	fun = new_dl_func("float", "freq", (m_uint)oscmorph_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency (in Hz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (in Hz)";
	fun = new_dl_func("float", "amp", (m_uint)oscmorph_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (typically a value between 0 and 1).";
	fun = new_dl_func("float", "amp", (m_uint)oscmorph_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude (typically a value between 0 and 1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (typically a value between 0 and 1).";
	fun = new_dl_func("float", "wtpos", (m_uint)oscmorph_get_wtpos);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Wavetable position. (scaled 0-1)";
	fun = new_dl_func("float", "wtpos", (m_uint)oscmorph_set_wtpos);
		arg = dl_func_add_arg(fun, "float", "wtpos");
		arg->doc = "Wavetable position. (scaled 0-1)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Wavetable position. (scaled 0-1)";
	env->class_def->doc = "Wavetable morphing oscillatorThis is an oscillator with linear interpolation that is capable of morphing between an arbitrary number of wavetables. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_pan2))
	CHECK_OB(import_class_begin(env, &t_pan2, env->global_nspc, pan2_ctor, pan2_dtor))
	fun = new_dl_func("int", "type", (m_uint)pan2_get_type);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";
	fun = new_dl_func("int", "type", (m_uint)pan2_set_type);
		arg = dl_func_add_arg(fun, "int", "type");
		arg->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";
	fun = new_dl_func("float", "pan", (m_uint)pan2_get_pan);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right, and 0 is center.";
	fun = new_dl_func("float", "pan", (m_uint)pan2_set_pan);
		arg = dl_func_add_arg(fun, "float", "pan");
		arg->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right, and 0 is center.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right, and 0 is center.";
	env->class_def->doc = "Panner";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_panst))
	CHECK_OB(import_class_begin(env, &t_panst, env->global_nspc, panst_ctor, panst_dtor))
	fun = new_dl_func("int", "type", (m_uint)panst_get_type);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";
	fun = new_dl_func("int", "type", (m_uint)panst_set_type);
		arg = dl_func_add_arg(fun, "int", "type");
		arg->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Panning type. 0 = equal power, 1 = square root, 2 = linear,3 = alternative equal power. Values outside this range will wrap. ";
	fun = new_dl_func("float", "pan", (m_uint)panst_get_pan);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right.";
	fun = new_dl_func("float", "pan", (m_uint)panst_set_pan);
		arg = dl_func_add_arg(fun, "float", "pan");
		arg->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Panning. A value of -1 is hard left, and a value of 1 is hard right.";
	env->class_def->doc = "Stereo Panner";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_pareq))
	CHECK_OB(import_class_begin(env, &t_pareq, env->global_nspc, pareq_ctor, pareq_dtor))
	fun = new_dl_func("float", "fc", (m_uint)pareq_get_fc);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency in peak mode, corner frequency in shelving mode.";
	fun = new_dl_func("float", "fc", (m_uint)pareq_set_fc);
		arg = dl_func_add_arg(fun, "float", "fc");
		arg->doc = "Center frequency in peak mode, corner frequency in shelving mode.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency in peak mode, corner frequency in shelving mode.";
	fun = new_dl_func("float", "v", (m_uint)pareq_get_v);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amount at which the center frequency value shall be increased or decreased. A value of 1 is a flat response.";
	fun = new_dl_func("float", "v", (m_uint)pareq_set_v);
		arg = dl_func_add_arg(fun, "float", "v");
		arg->doc = "Amount at which the center frequency value shall be increased or decreased. A value of 1 is a flat response.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amount at which the center frequency value shall be increased or decreased. A value of 1 is a flat response.";
	fun = new_dl_func("float", "q", (m_uint)pareq_get_q);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Q of the filter. sqrt(0.5) is no resonance.";
	fun = new_dl_func("float", "q", (m_uint)pareq_set_q);
		arg = dl_func_add_arg(fun, "float", "q");
		arg->doc = "Q of the filter. sqrt(0.5) is no resonance.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Q of the filter. sqrt(0.5) is no resonance.";
	fun = new_dl_func("float", "mode", (m_uint)pareq_get_mode);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "EQ mode. 0 = peak, 1 = low shelving, 2 = high shelving";
	fun = new_dl_func("float", "mode", (m_uint)pareq_set_mode);
		arg = dl_func_add_arg(fun, "float", "mode");
		arg->doc = "EQ mode. 0 = peak, 1 = low shelving, 2 = high shelving";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "EQ mode. 0 = peak, 1 = low shelving, 2 = high shelving";
	env->class_def->doc = "Parametric EqualizerThis is an implementation of Zoelzer's parametric equalizer filter.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_paulstretch))
	CHECK_OB(import_class_begin(env, &t_paulstretch, env->global_nspc, paulstretch_ctor, paulstretch_dtor))
	fun = new_dl_func("void", "init", (m_uint)paulstretch_init);
		arg = dl_func_add_arg(fun, "ftbl", "ft");
		arg->doc = "ftable containing audio data";
		arg = dl_func_add_arg(fun, "float", "windowsize");
		arg->doc = "Window size, in seconds.";
		arg = dl_func_add_arg(fun, "float", "stretch");
		arg->doc = "Stretch factor, 1.0 is no stretch.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "An extreme time-stretching algorithm by Paul Nasca OctavianThis is an implementation of the popular paulstretch algorithm used for timestretching an audio signal to create ambient textures. Ideally, this algorithm is best used for stretching signals by very very long amounts. This version of paulstretch will take an ftable and loop through it, makeit an ideal means for creating sustained pads. ";
	env->class_def->doc = "An extreme time-stretching algorithm by Paul Nasca OctavianThis is an implementation of the popular paulstretch algorithm used for timestretching an audio signal to create ambient textures. Ideally, this algorithm is best used for stretching signals by very very long amounts. This version of paulstretch will take an ftable and loop through it, makeit an ideal means for creating sustained pads. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_pdhalf))
	CHECK_OB(import_class_begin(env, &t_pdhalf, env->global_nspc, pdhalf_ctor, pdhalf_dtor))
	fun = new_dl_func("float", "amount", (m_uint)pdhalf_get_amount);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amount of distortion, within the range [-1, 1]. 0 is no distortion.";
	fun = new_dl_func("float", "amount", (m_uint)pdhalf_set_amount);
		arg = dl_func_add_arg(fun, "float", "amount");
		arg->doc = "Amount of distortion, within the range [-1, 1]. 0 is no distortion.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amount of distortion, within the range [-1, 1]. 0 is no distortion.";
	env->class_def->doc = "Casio-style phase distortion with \"pivot point\" on the X axisThis module is designed to emulate the classic phase distortion synthesis technique.From the mid 90's. The technique reads the first and second halves of the ftblat different rates in order to warp the waveform. For example, pdhalf can smoothly transition a sinewave into something approximating a sawtooth wave.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_peaklim))
	CHECK_OB(import_class_begin(env, &t_peaklim, env->global_nspc, peaklim_ctor, peaklim_dtor))
	fun = new_dl_func("float", "atk", (m_uint)peaklim_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time, in seconds";
	fun = new_dl_func("float", "atk", (m_uint)peaklim_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Attack time, in seconds";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time, in seconds";
	fun = new_dl_func("float", "rel", (m_uint)peaklim_get_rel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time, in seconds";
	fun = new_dl_func("float", "rel", (m_uint)peaklim_set_rel);
		arg = dl_func_add_arg(fun, "float", "rel");
		arg->doc = "Release time, in seconds";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time, in seconds";
	fun = new_dl_func("float", "thresh", (m_uint)peaklim_get_thresh);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Threshold, in dB";
	fun = new_dl_func("float", "thresh", (m_uint)peaklim_set_thresh);
		arg = dl_func_add_arg(fun, "float", "thresh");
		arg->doc = "Threshold, in dB";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Threshold, in dB";
	env->class_def->doc = "Peak limiter This is a simple peak limiting algorithm, based off code from the StanfordMusic-424 class.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_phaser))
	CHECK_OB(import_class_begin(env, &t_phaser, env->global_nspc, phaser_ctor, phaser_dtor))
	fun = new_dl_func("float", "MaxNotch1Freq", (m_uint)phaser_get_MaxNotch1Freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 20 and 10000";
	fun = new_dl_func("float", "MaxNotch1Freq", (m_uint)phaser_set_MaxNotch1Freq);
		arg = dl_func_add_arg(fun, "float", "MaxNotch1Freq");
		arg->doc = "Between 20 and 10000";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 20 and 10000";
	fun = new_dl_func("float", "MinNotch1Freq", (m_uint)phaser_get_MinNotch1Freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 20 and 5000";
	fun = new_dl_func("float", "MinNotch1Freq", (m_uint)phaser_set_MinNotch1Freq);
		arg = dl_func_add_arg(fun, "float", "MinNotch1Freq");
		arg->doc = "Between 20 and 5000";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 20 and 5000";
	fun = new_dl_func("float", "Notch_width", (m_uint)phaser_get_Notch_width);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 10 and 5000";
	fun = new_dl_func("float", "Notch_width", (m_uint)phaser_set_Notch_width);
		arg = dl_func_add_arg(fun, "float", "Notch_width");
		arg->doc = "Between 10 and 5000";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 10 and 5000";
	fun = new_dl_func("float", "NotchFreq", (m_uint)phaser_get_NotchFreq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 1.1 and 4";
	fun = new_dl_func("float", "NotchFreq", (m_uint)phaser_set_NotchFreq);
		arg = dl_func_add_arg(fun, "float", "NotchFreq");
		arg->doc = "Between 1.1 and 4";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 1.1 and 4";
	fun = new_dl_func("float", "VibratoMode", (m_uint)phaser_get_VibratoMode);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "1 or 0";
	fun = new_dl_func("float", "VibratoMode", (m_uint)phaser_set_VibratoMode);
		arg = dl_func_add_arg(fun, "float", "VibratoMode");
		arg->doc = "1 or 0";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "1 or 0";
	fun = new_dl_func("float", "depth", (m_uint)phaser_get_depth);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 0 and 1";
	fun = new_dl_func("float", "depth", (m_uint)phaser_set_depth);
		arg = dl_func_add_arg(fun, "float", "depth");
		arg->doc = "Between 0 and 1";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 0 and 1";
	fun = new_dl_func("float", "feedback_gain", (m_uint)phaser_get_feedback_gain);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 0 and 1";
	fun = new_dl_func("float", "feedback_gain", (m_uint)phaser_set_feedback_gain);
		arg = dl_func_add_arg(fun, "float", "feedback_gain");
		arg->doc = "Between 0 and 1";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 0 and 1";
	fun = new_dl_func("float", "invert", (m_uint)phaser_get_invert);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "1 or 0";
	fun = new_dl_func("float", "invert", (m_uint)phaser_set_invert);
		arg = dl_func_add_arg(fun, "float", "invert");
		arg->doc = "1 or 0";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "1 or 0";
	fun = new_dl_func("float", "level", (m_uint)phaser_get_level);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "-60 to 10 dB";
	fun = new_dl_func("float", "level", (m_uint)phaser_set_level);
		arg = dl_func_add_arg(fun, "float", "level");
		arg->doc = "-60 to 10 dB";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "-60 to 10 dB";
	fun = new_dl_func("float", "lfobpm", (m_uint)phaser_get_lfobpm);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 24 and 360";
	fun = new_dl_func("float", "lfobpm", (m_uint)phaser_set_lfobpm);
		arg = dl_func_add_arg(fun, "float", "lfobpm");
		arg->doc = "Between 24 and 360";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Between 24 and 360";
	env->class_def->doc = "A stereo phaser	This is a stereo phaser, generated from Faust code taken from the Guitarix project.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_phasor))
	CHECK_OB(import_class_begin(env, &t_phasor, env->global_nspc, phasor_ctor, phasor_dtor))
	fun = new_dl_func("void", "init", (m_uint)phasor_init);
		arg = dl_func_add_arg(fun, "float", "iphs");
		arg->doc = "initial phase";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Normalized sawtooth wave from 0 to 1    Phasors are often used when building table-lookup oscillators.";
	fun = new_dl_func("float", "freq", (m_uint)phasor_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency.";
	fun = new_dl_func("float", "freq", (m_uint)phasor_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency.";
	env->class_def->doc = "Normalized sawtooth wave from 0 to 1    Phasors are often used when building table-lookup oscillators.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_pinknoise))
	CHECK_OB(import_class_begin(env, &t_pinknoise, env->global_nspc, pinknoise_ctor, pinknoise_dtor))
	fun = new_dl_func("float", "amp", (m_uint)pinknoise_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude. (Value between 0-1).";
	fun = new_dl_func("float", "amp", (m_uint)pinknoise_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude. (Value between 0-1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude. (Value between 0-1).";
	env->class_def->doc = "Pink pinknoise generator";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_pitchamdf))
	CHECK_OB(import_class_begin(env, &t_pitchamdf, env->global_nspc, pitchamdf_ctor, pitchamdf_dtor))
	fun = new_dl_func("void", "init", (m_uint)pitchamdf_init);
		arg = dl_func_add_arg(fun, "float", "min");
		arg->doc = "Minimum expected frequency to detect";
		arg = dl_func_add_arg(fun, "float", "max");
		arg->doc = "Maximum expected frequency to detect";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Pitch detection using AMDF method.Pitchamdf tracks the pitch of signal using the AMDF (Average Magnitude Difference Function) method of Pitch following. ";
	env->class_def->doc = "Pitch detection using AMDF method.Pitchamdf tracks the pitch of signal using the AMDF (Average Magnitude Difference Function) method of Pitch following. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_pluck))
	CHECK_OB(import_class_begin(env, &t_pluck, env->global_nspc, pluck_ctor, pluck_dtor))
	fun = new_dl_func("void", "init", (m_uint)pluck_init);
		arg = dl_func_add_arg(fun, "float", "ifreq");
		arg->doc = "Sets the initial frequency. This frequency is used to allocate all the buffers needed for the delay. This should be the lowest frequency you plan on using.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Karplus-Strong plucked string instrument.";
	fun = new_dl_func("float", "freq", (m_uint)pluck_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Variable frequency. Values less than the initial frequency (ifreq) will be doubled until it is greater than or equal to ifreq.";
	fun = new_dl_func("float", "freq", (m_uint)pluck_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Variable frequency. Values less than the initial frequency (ifreq) will be doubled until it is greater than or equal to ifreq.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Variable frequency. Values less than the initial frequency (ifreq) will be doubled until it is greater than or equal to ifreq.";
	fun = new_dl_func("float", "amp", (m_uint)pluck_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude";
	fun = new_dl_func("float", "amp", (m_uint)pluck_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude";
	env->class_def->doc = "Karplus-Strong plucked string instrument.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_port))
	CHECK_OB(import_class_begin(env, &t_port, env->global_nspc, port_ctor, port_dtor))
	fun = new_dl_func("void", "init", (m_uint)port_init);
		arg = dl_func_add_arg(fun, "float", "htime");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = " Portamento-style control signal smoothing    Useful for smoothing out low-resolution signals and applying glissando to filters.";
	env->class_def->doc = " Portamento-style control signal smoothing    Useful for smoothing out low-resolution signals and applying glissando to filters.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_posc3))
	CHECK_OB(import_class_begin(env, &t_posc3, env->global_nspc, posc3_ctor, posc3_dtor))
	fun = new_dl_func("void", "init", (m_uint)posc3_init);
		arg = dl_func_add_arg(fun, "ftbl", "tbl");
		arg->doc = "Wavetable to read from. Note: the size of this table must be a power of 2.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = " High-precision table-lookup posc3ilator with cubic interpolation";
	fun = new_dl_func("float", "freq", (m_uint)posc3_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (in Hz)";
	fun = new_dl_func("float", "freq", (m_uint)posc3_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency (in Hz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (in Hz)";
	fun = new_dl_func("float", "amp", (m_uint)posc3_get_amp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (typically a value between 0 and 1).";
	fun = new_dl_func("float", "amp", (m_uint)posc3_set_amp);
		arg = dl_func_add_arg(fun, "float", "amp");
		arg->doc = "Amplitude (typically a value between 0 and 1).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Amplitude (typically a value between 0 and 1).";
	env->class_def->doc = " High-precision table-lookup posc3ilator with cubic interpolation";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_progress))
	CHECK_OB(import_class_begin(env, &t_progress, env->global_nspc, progress_ctor, progress_dtor))
	fun = new_dl_func("int", "nbars", (m_uint)progress_get_nbars);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("int", "nbars", (m_uint)progress_set_nbars);
		arg = dl_func_add_arg(fun, "int", "nbars");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("int", "skip", (m_uint)progress_get_skip);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "How many samples to skip before checking";
	fun = new_dl_func("int", "skip", (m_uint)progress_set_skip);
		arg = dl_func_add_arg(fun, "int", "skip");
		arg->doc = "How many samples to skip before checking";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "How many samples to skip before checking";
	env->class_def->doc = "A simple progress bar for the commandline    Mostly ideal for offline renderings and programs with finite length. Escape characters are used to show/hide the cursor. Interruption before finishing may cause the cursor to disappear.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_prop))
	CHECK_OB(import_class_begin(env, &t_prop, env->global_nspc, prop_ctor, prop_dtor))
	fun = new_dl_func("void", "init", (m_uint)prop_init);
		arg = dl_func_add_arg(fun, "string", "str");
		arg->doc = "Prop string to be parsed.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Simple rhythmic notation gate generator    Creates a gate using a simple rhythmic notation system called prop. When it reaches the end of the prop string, it will loop back to the beginning.Prop has a few basic rules:1. A '+' denotes a on. A '-' denotes an off (rest). They each have an equal duration of a quarter note.2. On and off values can be strung together to create equally spaced gates: +-+--3. When notes are enclosed in parantheses '()' following a positive integer N, their duration is reduced N times: ++2(+-)4. When notes are enclosed in brackets '[]' following a positive integer N, their duration is scaled by a factor of N: ++2[++]5. Parenthesis and brackets can be nested: +- 2[3(+2(++)+)]2(++)";
	fun = new_dl_func("float", "bpm", (m_uint)prop_get_bpm);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Beats per minute of the prop string.";
	fun = new_dl_func("float", "bpm", (m_uint)prop_set_bpm);
		arg = dl_func_add_arg(fun, "float", "bpm");
		arg->doc = "Beats per minute of the prop string.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Beats per minute of the prop string.";
	env->class_def->doc = "Simple rhythmic notation gate generator    Creates a gate using a simple rhythmic notation system called prop. When it reaches the end of the prop string, it will loop back to the beginning.Prop has a few basic rules:1. A '+' denotes a on. A '-' denotes an off (rest). They each have an equal duration of a quarter note.2. On and off values can be strung together to create equally spaced gates: +-+--3. When notes are enclosed in parantheses '()' following a positive integer N, their duration is reduced N times: ++2(+-)4. When notes are enclosed in brackets '[]' following a positive integer N, their duration is scaled by a factor of N: ++2[++]5. Parenthesis and brackets can be nested: +- 2[3(+2(++)+)]2(++)";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_pshift))
	CHECK_OB(import_class_begin(env, &t_pshift, env->global_nspc, pshift_ctor, pshift_dtor))
	fun = new_dl_func("float", "shift", (m_uint)pshift_get_shift);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Pitch shift (in semitones), range -24/24.";
	fun = new_dl_func("float", "shift", (m_uint)pshift_set_shift);
		arg = dl_func_add_arg(fun, "float", "shift");
		arg->doc = "Pitch shift (in semitones), range -24/24.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Pitch shift (in semitones), range -24/24.";
	fun = new_dl_func("float", "window", (m_uint)pshift_get_window);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Window size (in samples), max 10000";
	fun = new_dl_func("float", "window", (m_uint)pshift_set_window);
		arg = dl_func_add_arg(fun, "float", "window");
		arg->doc = "Window size (in samples), max 10000";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Window size (in samples), max 10000";
	fun = new_dl_func("float", "xfade", (m_uint)pshift_get_xfade);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Crossfade (in samples), max 10000";
	fun = new_dl_func("float", "xfade", (m_uint)pshift_set_xfade);
		arg = dl_func_add_arg(fun, "float", "xfade");
		arg->doc = "Crossfade (in samples), max 10000";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Crossfade (in samples), max 10000";
	env->class_def->doc = "Time-domain pitch shifter.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_ptrack))
	CHECK_OB(import_class_begin(env, &t_ptrack, env->global_nspc, ptrack_ctor, ptrack_dtor))
	fun = new_dl_func("void", "init", (m_uint)ptrack_init);
		arg = dl_func_add_arg(fun, "int", "ihopsize");
		arg->doc = "hop size.";
		arg = dl_func_add_arg(fun, "int", "ipeaks");
		arg->doc = "Number of peaks.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Track pitch of a signal.Ptrack is a pitch tracker, based on an algorithm originally created byMiller Puckette.";
	env->class_def->doc = "Track pitch of a signal.Ptrack is a pitch tracker, based on an algorithm originally created byMiller Puckette.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_randh))
	CHECK_OB(import_class_begin(env, &t_randh, env->global_nspc, randh_ctor, randh_dtor))
	fun = new_dl_func("float", "min", (m_uint)randh_get_min);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value to use.";
	fun = new_dl_func("float", "min", (m_uint)randh_set_min);
		arg = dl_func_add_arg(fun, "float", "min");
		arg->doc = "Minimum value to use.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value to use.";
	fun = new_dl_func("float", "max", (m_uint)randh_get_max);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value to use.";
	fun = new_dl_func("float", "max", (m_uint)randh_set_max);
		arg = dl_func_add_arg(fun, "float", "max");
		arg->doc = "Maximum value to use.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value to use.";
	fun = new_dl_func("float", "freq", (m_uint)randh_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency of randomization (in Hz)";
	fun = new_dl_func("float", "freq", (m_uint)randh_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Frequency of randomization (in Hz)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency of randomization (in Hz)";
	env->class_def->doc = "Random number generator with hold time.Randh is loosely based off of the Csound opcode randomh. The design is equivalentto scaled noise sent through a classic sample and hold module.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_randi))
	CHECK_OB(import_class_begin(env, &t_randi, env->global_nspc, randi_ctor, randi_dtor))
	fun = new_dl_func("float", "min", (m_uint)randi_get_min);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value";
	fun = new_dl_func("float", "min", (m_uint)randi_set_min);
		arg = dl_func_add_arg(fun, "float", "min");
		arg->doc = "Minimum value";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value";
	fun = new_dl_func("float", "max", (m_uint)randi_get_max);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value";
	fun = new_dl_func("float", "max", (m_uint)randi_set_max);
		arg = dl_func_add_arg(fun, "float", "max");
		arg->doc = "Maximum value";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value";
	fun = new_dl_func("float", "cps", (m_uint)randi_get_cps);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency to change values.";
	fun = new_dl_func("float", "cps", (m_uint)randi_set_cps);
		arg = dl_func_add_arg(fun, "float", "cps");
		arg->doc = "Frequency to change values.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency to change values.";
	fun = new_dl_func("float", "mode", (m_uint)randi_get_mode);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Randi mode (not yet implemented yet.)";
	fun = new_dl_func("float", "mode", (m_uint)randi_set_mode);
		arg = dl_func_add_arg(fun, "float", "mode");
		arg->doc = "Randi mode (not yet implemented yet.)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Randi mode (not yet implemented yet.)";
	env->class_def->doc = "Line segments between random values within a range";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_random))
	CHECK_OB(import_class_begin(env, &t_random, env->global_nspc, random_ctor, random_dtor))
	fun = new_dl_func("float", "min", (m_uint)random_get_min);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value.";
	fun = new_dl_func("float", "min", (m_uint)random_set_min);
		arg = dl_func_add_arg(fun, "float", "min");
		arg->doc = "Minimum value.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value.";
	fun = new_dl_func("float", "max", (m_uint)random_get_max);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value.";
	fun = new_dl_func("float", "max", (m_uint)random_set_max);
		arg = dl_func_add_arg(fun, "float", "max");
		arg->doc = "Maximum value.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value.";
	env->class_def->doc = "Random values within a range";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_reson))
	CHECK_OB(import_class_begin(env, &t_reson, env->global_nspc, reson_ctor, reson_dtor))
	fun = new_dl_func("float", "freq", (m_uint)reson_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency of the filter, or frequency position of the peak response.";
	fun = new_dl_func("float", "freq", (m_uint)reson_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Center frequency of the filter, or frequency position of the peak response.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency of the filter, or frequency position of the peak response.";
	fun = new_dl_func("float", "bw", (m_uint)reson_get_bw);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth of the filter.";
	fun = new_dl_func("float", "bw", (m_uint)reson_set_bw);
		arg = dl_func_add_arg(fun, "float", "bw");
		arg->doc = "Bandwidth of the filter.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Bandwidth of the filter.";
	env->class_def->doc = "A second-order resonant filter. NOTE: The output for reson appears to be very hot, so take caution when using this module.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_reverse))
	CHECK_OB(import_class_begin(env, &t_reverse, env->global_nspc, reverse_ctor, reverse_dtor))
	fun = new_dl_func("void", "init", (m_uint)reverse_init);
		arg = dl_func_add_arg(fun, "float", "delay");
		arg->doc = "Delay time in seconds.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Signal reverser	Reverse will store a signal inside a buffer and play it back reversed.";
	env->class_def->doc = "Signal reverser	Reverse will store a signal inside a buffer and play it back reversed.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_revsc))
	CHECK_OB(import_class_begin(env, &t_revsc, env->global_nspc, revsc_ctor, revsc_dtor))
	fun = new_dl_func("float", "feedback", (m_uint)revsc_get_feedback);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Value between 0-1 that sets feedback value. The larger the value, the longer the decay.";
	fun = new_dl_func("float", "feedback", (m_uint)revsc_set_feedback);
		arg = dl_func_add_arg(fun, "float", "feedback");
		arg->doc = "Value between 0-1 that sets feedback value. The larger the value, the longer the decay.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Value between 0-1 that sets feedback value. The larger the value, the longer the decay.";
	fun = new_dl_func("float", "lpfreq", (m_uint)revsc_get_lpfreq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "low pass cutoff frequency.";
	fun = new_dl_func("float", "lpfreq", (m_uint)revsc_set_lpfreq);
		arg = dl_func_add_arg(fun, "float", "lpfreq");
		arg->doc = "low pass cutoff frequency.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "low pass cutoff frequency.";
	env->class_def->doc = " 8 FDN stereo reverb";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_rms))
	CHECK_OB(import_class_begin(env, &t_rms, env->global_nspc, rms_ctor, rms_dtor))
	fun = new_dl_func("float", "ihp", (m_uint)rms_get_ihp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Half-power point (in Hz) of internal lowpass filter. This parameter is fixed at 10Hz and is not yet mutable.";
	fun = new_dl_func("float", "ihp", (m_uint)rms_set_ihp);
		arg = dl_func_add_arg(fun, "float", "ihp");
		arg->doc = "Half-power point (in Hz) of internal lowpass filter. This parameter is fixed at 10Hz and is not yet mutable.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Half-power point (in Hz) of internal lowpass filter. This parameter is fixed at 10Hz and is not yet mutable.";
	env->class_def->doc = "RMS-averaged signal amplitude    Perform \"root-mean-square\" on a signal to get overall amplitude of a signal. The output signal looks similar to that of a classic VU meter.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_rpt))
	CHECK_OB(import_class_begin(env, &t_rpt, env->global_nspc, rpt_ctor, rpt_dtor))
	fun = new_dl_func("void", "init", (m_uint)rpt_init);
		arg = dl_func_add_arg(fun, "float", "maxdur");
		arg->doc = "Maximum delay duration in seconds. This will set the buffer size.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Trigger based beat-repeat stuttering effect    When the input is a non-zero value, rpt will load up the buffer and loop a certain number of times. Speed and repeat amounts can be set with the sp_rpt_set function.";
	env->class_def->doc = "Trigger based beat-repeat stuttering effect    When the input is a non-zero value, rpt will load up the buffer and loop a certain number of times. Speed and repeat amounts can be set with the sp_rpt_set function.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_samphold))
	CHECK_OB(import_class_begin(env, &t_samphold, env->global_nspc, samphold_ctor, samphold_dtor))
	env->class_def->doc = "Classic sample and hold";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_saturator))
	CHECK_OB(import_class_begin(env, &t_saturator, env->global_nspc, saturator_ctor, saturator_dtor))
	fun = new_dl_func("float", "drive", (m_uint)saturator_get_drive);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Input gain into the distortion section, in decibels. Controls overall amount of distortion.";
	fun = new_dl_func("float", "drive", (m_uint)saturator_set_drive);
		arg = dl_func_add_arg(fun, "float", "drive");
		arg->doc = "Input gain into the distortion section, in decibels. Controls overall amount of distortion.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Input gain into the distortion section, in decibels. Controls overall amount of distortion.";
	fun = new_dl_func("float", "dcoffset", (m_uint)saturator_get_dcoffset);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Constant linear offset applied to the signal. A small offset will introduce odd harmonics into the distoration spectrum, whereas a zero offset will have only even harmonics.";
	fun = new_dl_func("float", "dcoffset", (m_uint)saturator_set_dcoffset);
		arg = dl_func_add_arg(fun, "float", "dcoffset");
		arg->doc = "Constant linear offset applied to the signal. A small offset will introduce odd harmonics into the distoration spectrum, whereas a zero offset will have only even harmonics.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Constant linear offset applied to the signal. A small offset will introduce odd harmonics into the distoration spectrum, whereas a zero offset will have only even harmonics.";
	env->class_def->doc = "Soft clip saturating distortion, based on examples from Abel/Berners' Music 424 course at Stanford.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_scale))
	CHECK_OB(import_class_begin(env, &t_scale, env->global_nspc, scale_ctor, scale_dtor))
	fun = new_dl_func("float", "min", (m_uint)scale_get_min);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value to scale to.";
	fun = new_dl_func("float", "min", (m_uint)scale_set_min);
		arg = dl_func_add_arg(fun, "float", "min");
		arg->doc = "Minimum value to scale to.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value to scale to.";
	fun = new_dl_func("float", "max", (m_uint)scale_get_max);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value to scale to.";
	fun = new_dl_func("float", "max", (m_uint)scale_set_max);
		arg = dl_func_add_arg(fun, "float", "max");
		arg->doc = "Maximum value to scale to.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value to scale to.";
	env->class_def->doc = "Bipolar Scale    This module scales from unipolar [0, 1] to another range defined by min and max.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_sdelay))
	CHECK_OB(import_class_begin(env, &t_sdelay, env->global_nspc, sdelay_ctor, sdelay_dtor))
	fun = new_dl_func("void", "init", (m_uint)sdelay_init);
		arg = dl_func_add_arg(fun, "float", "size");
		arg->doc = "Size of delay (in samples)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Delays a signal by a number of samples.";
	env->class_def->doc = "Delays a signal by a number of samples.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_slice))
	CHECK_OB(import_class_begin(env, &t_slice, env->global_nspc, slice_ctor, slice_dtor))
	fun = new_dl_func("void", "init", (m_uint)slice_init);
		arg = dl_func_add_arg(fun, "ftbl", "vals");
		arg->doc = "A table containing slice points, in samples";
		arg = dl_func_add_arg(fun, "ftbl", "buf");
		arg->doc = "The buffer containing the audio samples.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Yet another slice-based sample player.This module takes in an audio buffer and a table with slice points. ";
	fun = new_dl_func("float", "id", (m_uint)slice_get_id);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Value position.";
	fun = new_dl_func("float", "id", (m_uint)slice_set_id);
		arg = dl_func_add_arg(fun, "float", "id");
		arg->doc = "Value position.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Value position.";
	env->class_def->doc = "Yet another slice-based sample player.This module takes in an audio buffer and a table with slice points. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_smoothdelay))
	CHECK_OB(import_class_begin(env, &t_smoothdelay, env->global_nspc, smoothdelay_ctor, smoothdelay_dtor))
	fun = new_dl_func("void", "init", (m_uint)smoothdelay_init);
		arg = dl_func_add_arg(fun, "float", "maxdel");
		arg->doc = "Maximum delay time (in seconds)";
		arg = dl_func_add_arg(fun, "int", "interp");
		arg->doc = "interpolation time (in samples)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Smooth variable delay line without varispeed pitch.Smooth delay is based off the sdelay module in Faust. The smooth delay algorithm involves a double delay line. Any time the delay time changes, the delay time of buffer not heard changes, then is crossfaded to that buffer.";
	fun = new_dl_func("float", "feedback", (m_uint)smoothdelay_get_feedback);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "feedback", (m_uint)smoothdelay_set_feedback);
		arg = dl_func_add_arg(fun, "float", "feedback");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "del", (m_uint)smoothdelay_get_del);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	fun = new_dl_func("float", "del", (m_uint)smoothdelay_set_del);
		arg = dl_func_add_arg(fun, "float", "del");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "";
	env->class_def->doc = "Smooth variable delay line without varispeed pitch.Smooth delay is based off the sdelay module in Faust. The smooth delay algorithm involves a double delay line. Any time the delay time changes, the delay time of buffer not heard changes, then is crossfaded to that buffer.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_spa))
	CHECK_OB(import_class_begin(env, &t_spa, env->global_nspc, spa_ctor, spa_dtor))
	fun = new_dl_func("void", "init", (m_uint)spa_init);
		arg = dl_func_add_arg(fun, "string", "filename");
		arg->doc = "Filename of SPA file";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Stream a Soundpipe Audio FileSimilar to sp_diskin, sp_spa will stream a file in the internal soundpipeaudio format. Such a format is useful for instances where you need to readaudio files, but can't use libsndfile. ";
	env->class_def->doc = "Stream a Soundpipe Audio FileSimilar to sp_diskin, sp_spa will stream a file in the internal soundpipeaudio format. Such a format is useful for instances where you need to readaudio files, but can't use libsndfile. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_sparec))
	CHECK_OB(import_class_begin(env, &t_sparec, env->global_nspc, sparec_ctor, sparec_dtor))
	fun = new_dl_func("void", "init", (m_uint)sparec_init);
		arg = dl_func_add_arg(fun, "string", "filename");
		arg->doc = "Filename to write to";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Writes signal to spa file.";
	env->class_def->doc = "Writes signal to spa file.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_streson))
	CHECK_OB(import_class_begin(env, &t_streson, env->global_nspc, streson_ctor, streson_dtor))
	fun = new_dl_func("float", "freq", (m_uint)streson_get_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Fundamental frequency of string.";
	fun = new_dl_func("float", "freq", (m_uint)streson_set_freq);
		arg = dl_func_add_arg(fun, "float", "freq");
		arg->doc = "Fundamental frequency of string.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Fundamental frequency of string.";
	fun = new_dl_func("float", "fdbgain", (m_uint)streson_get_fdbgain);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Feedback amount (value between 0-1)";
	fun = new_dl_func("float", "fdbgain", (m_uint)streson_set_fdbgain);
		arg = dl_func_add_arg(fun, "float", "fdbgain");
		arg->doc = "Feedback amount (value between 0-1)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Feedback amount (value between 0-1)";
	env->class_def->doc = "String resonator filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_switch))
	CHECK_OB(import_class_begin(env, &t_switch, env->global_nspc, switch_ctor, switch_dtor))
	env->class_def->doc = "Switch between two signals    By default, the incoming first signal is selected. When triggered, the output signal will switch to the other signal.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tabread))
	CHECK_OB(import_class_begin(env, &t_tabread, env->global_nspc, tabread_ctor, tabread_dtor))
	fun = new_dl_func("void", "init", (m_uint)tabread_init);
		arg = dl_func_add_arg(fun, "ftbl", "ft");
		arg->doc = "A properly allocated table (using a function like sp_gen_file).";
		arg = dl_func_add_arg(fun, "float", "mode");
		arg->doc = "1 = scaled index, 0 = unscaled index";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Table         Read through an sp_ftbl with linear interpolation.    ";
	fun = new_dl_func("float", "index", (m_uint)tabread_get_index);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "index position, either scaled or unscaled with mode parameter";
	fun = new_dl_func("float", "index", (m_uint)tabread_set_index);
		arg = dl_func_add_arg(fun, "float", "index");
		arg->doc = "index position, either scaled or unscaled with mode parameter";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "index position, either scaled or unscaled with mode parameter";
	fun = new_dl_func("float", "offset", (m_uint)tabread_get_offset);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Offset from beginning of ftable. If the mode is scaled, then it is in range 0-1, other wise it is the index position.";
	fun = new_dl_func("float", "offset", (m_uint)tabread_set_offset);
		arg = dl_func_add_arg(fun, "float", "offset");
		arg->doc = "Offset from beginning of ftable. If the mode is scaled, then it is in range 0-1, other wise it is the index position.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Offset from beginning of ftable. If the mode is scaled, then it is in range 0-1, other wise it is the index position.";
	fun = new_dl_func("float", "wrap", (m_uint)tabread_get_wrap);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Enable wraparound. 1 = on; 0 = 0ff.";
	fun = new_dl_func("float", "wrap", (m_uint)tabread_set_wrap);
		arg = dl_func_add_arg(fun, "float", "wrap");
		arg->doc = "Enable wraparound. 1 = on; 0 = 0ff.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Enable wraparound. 1 = on; 0 = 0ff.";
	env->class_def->doc = "Table         Read through an sp_ftbl with linear interpolation.    ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tadsr))
	CHECK_OB(import_class_begin(env, &t_tadsr, env->global_nspc, tadsr_ctor, tadsr_dtor))
	fun = new_dl_func("float", "atk", (m_uint)tadsr_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time";
	fun = new_dl_func("float", "atk", (m_uint)tadsr_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Attack time";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time";
	fun = new_dl_func("float", "dec", (m_uint)tadsr_get_dec);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Decay time";
	fun = new_dl_func("float", "dec", (m_uint)tadsr_set_dec);
		arg = dl_func_add_arg(fun, "float", "dec");
		arg->doc = "Decay time";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Decay time";
	fun = new_dl_func("float", "sus", (m_uint)tadsr_get_sus);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sustain Level";
	fun = new_dl_func("float", "sus", (m_uint)tadsr_set_sus);
		arg = dl_func_add_arg(fun, "float", "sus");
		arg->doc = "Sustain Level";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sustain Level";
	fun = new_dl_func("float", "rel", (m_uint)tadsr_get_rel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "release";
	fun = new_dl_func("float", "rel", (m_uint)tadsr_set_rel);
		arg = dl_func_add_arg(fun, "float", "rel");
		arg->doc = "release";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "release";
	env->class_def->doc = "Triggerable classic ADSR envelope";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tblrec))
	CHECK_OB(import_class_begin(env, &t_tblrec, env->global_nspc, tblrec_ctor, tblrec_dtor))
	fun = new_dl_func("void", "init", (m_uint)tblrec_init);
		arg = dl_func_add_arg(fun, "ftbl", "bar");
		arg->doc = "";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Record a signal to an ftable.This module will write audio-rate signals to a preallocated soundpipe ftable. Every time record is enabled, it will got back to index 0 overwrite anyprevious information that was on it. ";
	env->class_def->doc = "Record a signal to an ftable.This module will write audio-rate signals to a preallocated soundpipe ftable. Every time record is enabled, it will got back to index 0 overwrite anyprevious information that was on it. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tbvcf))
	CHECK_OB(import_class_begin(env, &t_tbvcf, env->global_nspc, tbvcf_ctor, tbvcf_dtor))
	fun = new_dl_func("float", "fco", (m_uint)tbvcf_get_fco);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter cutoff frequency";
	fun = new_dl_func("float", "fco", (m_uint)tbvcf_set_fco);
		arg = dl_func_add_arg(fun, "float", "fco");
		arg->doc = "Filter cutoff frequency";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter cutoff frequency";
	fun = new_dl_func("float", "res", (m_uint)tbvcf_get_res);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Resonance";
	fun = new_dl_func("float", "res", (m_uint)tbvcf_set_res);
		arg = dl_func_add_arg(fun, "float", "res");
		arg->doc = "Resonance";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Resonance";
	fun = new_dl_func("float", "dist", (m_uint)tbvcf_get_dist);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Distortion. Value is typically 2.0; deviation from this can cause stability issues. ";
	fun = new_dl_func("float", "dist", (m_uint)tbvcf_set_dist);
		arg = dl_func_add_arg(fun, "float", "dist");
		arg->doc = "Distortion. Value is typically 2.0; deviation from this can cause stability issues. ";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Distortion. Value is typically 2.0; deviation from this can cause stability issues. ";
	fun = new_dl_func("float", "asym", (m_uint)tbvcf_get_asym);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Asymmetry of resonance. Value is between 0-1";
	fun = new_dl_func("float", "asym", (m_uint)tbvcf_set_asym);
		arg = dl_func_add_arg(fun, "float", "asym");
		arg->doc = "Asymmetry of resonance. Value is between 0-1";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Asymmetry of resonance. Value is between 0-1";
	env->class_def->doc = "Emulation of the Roland TB-303 filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tdiv))
	CHECK_OB(import_class_begin(env, &t_tdiv, env->global_nspc, tdiv_ctor, tdiv_dtor))
	fun = new_dl_func("float", "num", (m_uint)tdiv_get_num);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Triggers every N times.";
	fun = new_dl_func("float", "num", (m_uint)tdiv_set_num);
		arg = dl_func_add_arg(fun, "float", "num");
		arg->doc = "Triggers every N times.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Triggers every N times.";
	fun = new_dl_func("float", "offset", (m_uint)tdiv_get_offset);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Offset amoutn.";
	fun = new_dl_func("float", "offset", (m_uint)tdiv_set_offset);
		arg = dl_func_add_arg(fun, "float", "offset");
		arg->doc = "Offset amoutn.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Offset amoutn.";
	env->class_def->doc = "Trigger divider.This module will take in a trigger signal, and output a trigger signalevery N times.For instance, when N = 3:in: * * * * * * * * *out *     *     *   ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tenv))
	CHECK_OB(import_class_begin(env, &t_tenv, env->global_nspc, tenv_ctor, tenv_dtor))
	fun = new_dl_func("float", "atk", (m_uint)tenv_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time, in seconds.";
	fun = new_dl_func("float", "atk", (m_uint)tenv_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Attack time, in seconds.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time, in seconds.";
	fun = new_dl_func("float", "hold", (m_uint)tenv_get_hold);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Hold time, in seconds.";
	fun = new_dl_func("float", "hold", (m_uint)tenv_set_hold);
		arg = dl_func_add_arg(fun, "float", "hold");
		arg->doc = "Hold time, in seconds.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Hold time, in seconds.";
	fun = new_dl_func("float", "rel", (m_uint)tenv_get_rel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time, in seconds.";
	fun = new_dl_func("float", "rel", (m_uint)tenv_set_rel);
		arg = dl_func_add_arg(fun, "float", "rel");
		arg->doc = "Release time, in seconds.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time, in seconds.";
	env->class_def->doc = "Trigger based linear AHD envelope generator";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tenv2))
	CHECK_OB(import_class_begin(env, &t_tenv2, env->global_nspc, tenv2_ctor, tenv2_dtor))
	fun = new_dl_func("float", "atk", (m_uint)tenv2_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time (in seconds).";
	fun = new_dl_func("float", "atk", (m_uint)tenv2_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Attack time (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time (in seconds).";
	fun = new_dl_func("float", "rel", (m_uint)tenv2_get_rel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time (in seconds).";
	fun = new_dl_func("float", "rel", (m_uint)tenv2_set_rel);
		arg = dl_func_add_arg(fun, "float", "rel");
		arg->doc = "Release time (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time (in seconds).";
	env->class_def->doc = "Linear 2-stage Attack/Release envelope generator    This envelope takes 2 triggers. When triggered once,the envelope will rise to 1 according to the attack time. When triggered again, it will decay to 0 according tothe decay time.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tenvx))
	CHECK_OB(import_class_begin(env, &t_tenvx, env->global_nspc, tenvx_ctor, tenvx_dtor))
	fun = new_dl_func("float", "atk", (m_uint)tenvx_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Tau attack time, in seconds. Must be non-zero.";
	fun = new_dl_func("float", "atk", (m_uint)tenvx_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Tau attack time, in seconds. Must be non-zero.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Tau attack time, in seconds. Must be non-zero.";
	fun = new_dl_func("float", "hold", (m_uint)tenvx_get_hold);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Hold time, in seconds. The duration of the gate signal.";
	fun = new_dl_func("float", "hold", (m_uint)tenvx_set_hold);
		arg = dl_func_add_arg(fun, "float", "hold");
		arg->doc = "Hold time, in seconds. The duration of the gate signal.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Hold time, in seconds. The duration of the gate signal.";
	fun = new_dl_func("float", "rel", (m_uint)tenvx_get_rel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Tau release time, in seconds. Must be non-zero.";
	fun = new_dl_func("float", "rel", (m_uint)tenvx_set_rel);
		arg = dl_func_add_arg(fun, "float", "rel");
		arg->doc = "Tau release time, in seconds. Must be non-zero.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Tau release time, in seconds. Must be non-zero.";
	env->class_def->doc = "Trigger based exponential AHD envelope generator.    This envelope generator emulates the exponential behavior of analogue envelope generators by passing a gate signal (whose duration is specified viathe hold parameter) through a one-pole filter, whose filter coefficeints arecalculated in terms of tau.      ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tgate))
	CHECK_OB(import_class_begin(env, &t_tgate, env->global_nspc, tgate_ctor, tgate_dtor))
	fun = new_dl_func("float", "time", (m_uint)tgate_get_time);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Duration of the gate (in seconds)";
	fun = new_dl_func("float", "time", (m_uint)tgate_set_time);
		arg = dl_func_add_arg(fun, "float", "time");
		arg->doc = "Duration of the gate (in seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Duration of the gate (in seconds)";
	env->class_def->doc = "A triggerable gate.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_thresh))
	CHECK_OB(import_class_begin(env, &t_thresh, env->global_nspc, thresh_ctor, thresh_dtor))
	fun = new_dl_func("float", "thresh", (m_uint)thresh_get_thresh);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Threshold level";
	fun = new_dl_func("float", "thresh", (m_uint)thresh_set_thresh);
		arg = dl_func_add_arg(fun, "float", "thresh");
		arg->doc = "Threshold level";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Threshold level";
	fun = new_dl_func("int", "mode", (m_uint)thresh_get_mode);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sets when to detect theshold crossings. 0 = from below. 1 = from above. 2 = from above/below";
	fun = new_dl_func("int", "mode", (m_uint)thresh_set_mode);
		arg = dl_func_add_arg(fun, "int", "mode");
		arg->doc = "Sets when to detect theshold crossings. 0 = from below. 1 = from above. 2 = from above/below";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Sets when to detect theshold crossings. 0 = from below. 1 = from above. 2 = from above/below";
	env->class_def->doc = "Trigger generator for signals that cross a given threshold. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_timer))
	CHECK_OB(import_class_begin(env, &t_timer, env->global_nspc, timer_ctor, timer_dtor))
	env->class_def->doc = "Tap-tempo like timerWhen triggered, timer will begin an internal stopwatch until it is triggered again.The output of the timer will be the time elapsed in seconds.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tin))
	CHECK_OB(import_class_begin(env, &t_tin, env->global_nspc, tin_ctor, tin_dtor))
	env->class_def->doc = "Similar to in, tin reads SPFLOATs (by default, this is a 4 byte binary float) from standard input every time it is triggered. behaves like a sample and hold, retaining the previous value (initial set to 0) until triggered. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tone))
	CHECK_OB(import_class_begin(env, &t_tone, env->global_nspc, tone_ctor, tone_dtor))
	fun = new_dl_func("float", "hp", (m_uint)tone_get_hp);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The response curve's half power point (aka cutoff frequency).";
	fun = new_dl_func("float", "hp", (m_uint)tone_set_hp);
		arg = dl_func_add_arg(fun, "float", "hp");
		arg->doc = "The response curve's half power point (aka cutoff frequency).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The response curve's half power point (aka cutoff frequency).";
	env->class_def->doc = "First-order recursive lowpass filter";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_trand))
	CHECK_OB(import_class_begin(env, &t_trand, env->global_nspc, trand_ctor, trand_dtor))
	fun = new_dl_func("float", "min", (m_uint)trand_get_min);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value to use.";
	fun = new_dl_func("float", "min", (m_uint)trand_set_min);
		arg = dl_func_add_arg(fun, "float", "min");
		arg->doc = "Minimum value to use.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Minimum value to use.";
	fun = new_dl_func("float", "max", (m_uint)trand_get_max);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value to use.";
	fun = new_dl_func("float", "max", (m_uint)trand_set_max);
		arg = dl_func_add_arg(fun, "float", "max");
		arg->doc = "Maximum value to use.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Maximum value to use.";
	env->class_def->doc = "Triggered random number generator.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tseg))
	CHECK_OB(import_class_begin(env, &t_tseg, env->global_nspc, tseg_ctor, tseg_dtor))
	fun = new_dl_func("void", "init", (m_uint)tseg_init);
		arg = dl_func_add_arg(fun, "float", "ibeg");
		arg->doc = "Beginning value.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "This module creates a series of line segments. ";
	fun = new_dl_func("float", "end", (m_uint)tseg_get_end);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "End parameter to go to.";
	fun = new_dl_func("float", "end", (m_uint)tseg_set_end);
		arg = dl_func_add_arg(fun, "float", "end");
		arg->doc = "End parameter to go to.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "End parameter to go to.";
	fun = new_dl_func("float", "dur", (m_uint)tseg_get_dur);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "duration to rise to (in seconds).";
	fun = new_dl_func("float", "dur", (m_uint)tseg_set_dur);
		arg = dl_func_add_arg(fun, "float", "dur");
		arg->doc = "duration to rise to (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "duration to rise to (in seconds).";
	fun = new_dl_func("float", "type", (m_uint)tseg_get_type);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The type of line, which determines slope of line";
	fun = new_dl_func("float", "type", (m_uint)tseg_set_type);
		arg = dl_func_add_arg(fun, "float", "type");
		arg->doc = "The type of line, which determines slope of line";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "The type of line, which determines slope of line";
	env->class_def->doc = "This module creates a series of line segments. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_tseq))
	CHECK_OB(import_class_begin(env, &t_tseq, env->global_nspc, tseq_ctor, tseq_dtor))
	fun = new_dl_func("void", "init", (m_uint)tseq_init);
		arg = dl_func_add_arg(fun, "ftbl", "ft");
		arg->doc = "An ftable of values";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Function table looper    TSeq runs through values in an ftable. It will change values when the trigger input is a non-zero value, and wrap around when it reaches the end.";
	fun = new_dl_func("int", "shuf", (m_uint)tseq_get_shuf);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "When shuf is non-zero, randomly pick a value rather than cycle through sequentially.";
	fun = new_dl_func("int", "shuf", (m_uint)tseq_set_shuf);
		arg = dl_func_add_arg(fun, "int", "shuf");
		arg->doc = "When shuf is non-zero, randomly pick a value rather than cycle through sequentially.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "When shuf is non-zero, randomly pick a value rather than cycle through sequentially.";
	env->class_def->doc = "Function table looper    TSeq runs through values in an ftable. It will change values when the trigger input is a non-zero value, and wrap around when it reaches the end.";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_vdelay))
	CHECK_OB(import_class_begin(env, &t_vdelay, env->global_nspc, vdelay_ctor, vdelay_dtor))
	fun = new_dl_func("void", "init", (m_uint)vdelay_init);
		arg = dl_func_add_arg(fun, "float", "maxdel");
		arg->doc = "The maximum delay time, in seconds.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Delay line with cubic interpolation";
	fun = new_dl_func("float", "del", (m_uint)vdelay_get_del);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Delay time (in seconds) that can be changed during performance. This value must not exceed the maximum delay time.";
	fun = new_dl_func("float", "del", (m_uint)vdelay_set_del);
		arg = dl_func_add_arg(fun, "float", "del");
		arg->doc = "Delay time (in seconds) that can be changed during performance. This value must not exceed the maximum delay time.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Delay time (in seconds) that can be changed during performance. This value must not exceed the maximum delay time.";
	env->class_def->doc = "Delay line with cubic interpolation";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_vocoder))
	CHECK_OB(import_class_begin(env, &t_vocoder, env->global_nspc, vocoder_ctor, vocoder_dtor))
	fun = new_dl_func("float", "atk", (m_uint)vocoder_get_atk);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time . (Range 0.001 and 0.5 seconds)";
	fun = new_dl_func("float", "atk", (m_uint)vocoder_set_atk);
		arg = dl_func_add_arg(fun, "float", "atk");
		arg->doc = "Attack time . (Range 0.001 and 0.5 seconds)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Attack time . (Range 0.001 and 0.5 seconds)";
	fun = new_dl_func("float", "rel", (m_uint)vocoder_get_rel);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time";
	fun = new_dl_func("float", "rel", (m_uint)vocoder_set_rel);
		arg = dl_func_add_arg(fun, "float", "rel");
		arg->doc = "Release time";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Release time";
	fun = new_dl_func("float", "bwratio", (m_uint)vocoder_get_bwratio);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Coeffecient to adjust the bandwidth of each band (0.1 - 2)";
	fun = new_dl_func("float", "bwratio", (m_uint)vocoder_set_bwratio);
		arg = dl_func_add_arg(fun, "float", "bwratio");
		arg->doc = "Coeffecient to adjust the bandwidth of each band (0.1 - 2)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Coeffecient to adjust the bandwidth of each band (0.1 - 2)";
	env->class_def->doc = "16-band channel vocoder";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_waveset))
	CHECK_OB(import_class_begin(env, &t_waveset, env->global_nspc, waveset_ctor, waveset_dtor))
	fun = new_dl_func("void", "init", (m_uint)waveset_init);
		arg = dl_func_add_arg(fun, "float", "ilen");
		arg->doc = "Length of buffer (in seconds).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Simple Time-stretching from repeating wavecylesThis module looks for zero-crossings and repeats them by a integer factor.While a crude means for time stretching, it is a very aesthetically pleasing effect to use on sounds and often produces at \"wet\" sound.The waveset algorithm was originally created by Trevor Wishart for the ComposerDesktop Project (CDP), and was then ported to Csound. ";
	fun = new_dl_func("float", "rep", (m_uint)waveset_get_rep);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Number of repeats.";
	fun = new_dl_func("float", "rep", (m_uint)waveset_set_rep);
		arg = dl_func_add_arg(fun, "float", "rep");
		arg->doc = "Number of repeats.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Number of repeats.";
	env->class_def->doc = "Simple Time-stretching from repeating wavecylesThis module looks for zero-crossings and repeats them by a integer factor.While a crude means for time stretching, it is a very aesthetically pleasing effect to use on sounds and often produces at \"wet\" sound.The waveset algorithm was originally created by Trevor Wishart for the ComposerDesktop Project (CDP), and was then ported to Csound. ";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_wpkorg35))
	CHECK_OB(import_class_begin(env, &t_wpkorg35, env->global_nspc, wpkorg35_ctor, wpkorg35_dtor))
	fun = new_dl_func("float", "cutoff", (m_uint)wpkorg35_get_cutoff);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter cutoff";
	fun = new_dl_func("float", "cutoff", (m_uint)wpkorg35_set_cutoff);
		arg = dl_func_add_arg(fun, "float", "cutoff");
		arg->doc = "Filter cutoff";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter cutoff";
	fun = new_dl_func("float", "res", (m_uint)wpkorg35_get_res);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter resonance (should be between 0-2)";
	fun = new_dl_func("float", "res", (m_uint)wpkorg35_set_res);
		arg = dl_func_add_arg(fun, "float", "res");
		arg->doc = "Filter resonance (should be between 0-2)";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter resonance (should be between 0-2)";
	fun = new_dl_func("float", "saturation", (m_uint)wpkorg35_get_saturation);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter saturation.";
	fun = new_dl_func("float", "saturation", (m_uint)wpkorg35_set_saturation);
		arg = dl_func_add_arg(fun, "float", "saturation");
		arg->doc = "Filter saturation.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Filter saturation.";
	env->class_def->doc = " Analogue model of the Korg 35 Lowpass FilterOriginal port done by Will Pirkle:http://www.willpirkle.com/Downloads/AN-5Korg35_V3.pdf";
	CHECK_BB(import_class_end(env))

	CHECK_BB(add_global_type(env, &t_zitarev))
	CHECK_OB(import_class_begin(env, &t_zitarev, env->global_nspc, zitarev_ctor, zitarev_dtor))
	fun = new_dl_func("float", "in_delay", (m_uint)zitarev_get_in_delay);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Delay in ms before reverberation begins.";
	fun = new_dl_func("float", "in_delay", (m_uint)zitarev_set_in_delay);
		arg = dl_func_add_arg(fun, "float", "in_delay");
		arg->doc = "Delay in ms before reverberation begins.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Delay in ms before reverberation begins.";
	fun = new_dl_func("float", "lf_x", (m_uint)zitarev_get_lf_x);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Crossover frequency separating low and middle frequencies (Hz).";
	fun = new_dl_func("float", "lf_x", (m_uint)zitarev_set_lf_x);
		arg = dl_func_add_arg(fun, "float", "lf_x");
		arg->doc = "Crossover frequency separating low and middle frequencies (Hz).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Crossover frequency separating low and middle frequencies (Hz).";
	fun = new_dl_func("float", "rt60_low", (m_uint)zitarev_get_rt60_low);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Time (in seconds) to decay 60db in low-frequency band.";
	fun = new_dl_func("float", "rt60_low", (m_uint)zitarev_set_rt60_low);
		arg = dl_func_add_arg(fun, "float", "rt60_low");
		arg->doc = "Time (in seconds) to decay 60db in low-frequency band.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Time (in seconds) to decay 60db in low-frequency band.";
	fun = new_dl_func("float", "rt60_mid", (m_uint)zitarev_get_rt60_mid);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Time (in seconds) to decay 60db in mid-frequency band.";
	fun = new_dl_func("float", "rt60_mid", (m_uint)zitarev_set_rt60_mid);
		arg = dl_func_add_arg(fun, "float", "rt60_mid");
		arg->doc = "Time (in seconds) to decay 60db in mid-frequency band.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Time (in seconds) to decay 60db in mid-frequency band.";
	fun = new_dl_func("float", "hf_damping", (m_uint)zitarev_get_hf_damping);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (Hz) at which the high-frequency T60 is half the middle-band's T60.";
	fun = new_dl_func("float", "hf_damping", (m_uint)zitarev_set_hf_damping);
		arg = dl_func_add_arg(fun, "float", "hf_damping");
		arg->doc = "Frequency (Hz) at which the high-frequency T60 is half the middle-band's T60.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Frequency (Hz) at which the high-frequency T60 is half the middle-band's T60.";
	fun = new_dl_func("float", "eq1_freq", (m_uint)zitarev_get_eq1_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 1.";
	fun = new_dl_func("float", "eq1_freq", (m_uint)zitarev_set_eq1_freq);
		arg = dl_func_add_arg(fun, "float", "eq1_freq");
		arg->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 1.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 1.";
	fun = new_dl_func("float", "eq1_level", (m_uint)zitarev_get_eq1_level);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 1";
	fun = new_dl_func("float", "eq1_level", (m_uint)zitarev_set_eq1_level);
		arg = dl_func_add_arg(fun, "float", "eq1_level");
		arg->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 1";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 1";
	fun = new_dl_func("float", "eq2_freq", (m_uint)zitarev_get_eq2_freq);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 2.";
	fun = new_dl_func("float", "eq2_freq", (m_uint)zitarev_set_eq2_freq);
		arg = dl_func_add_arg(fun, "float", "eq2_freq");
		arg->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 2.";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Center frequency of second-order Regalia Mitra peaking equalizer section 2.";
	fun = new_dl_func("float", "eq2_level", (m_uint)zitarev_get_eq2_level);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 2";
	fun = new_dl_func("float", "eq2_level", (m_uint)zitarev_set_eq2_level);
		arg = dl_func_add_arg(fun, "float", "eq2_level");
		arg->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 2";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Peak level in dB of second-order Regalia-Mitra peaking equalizer section 2";
	fun = new_dl_func("float", "mix", (m_uint)zitarev_get_mix);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "0 = all dry, 1 = all wet";
	fun = new_dl_func("float", "mix", (m_uint)zitarev_set_mix);
		arg = dl_func_add_arg(fun, "float", "mix");
		arg->doc = "0 = all dry, 1 = all wet";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "0 = all dry, 1 = all wet";
	fun = new_dl_func("float", "level", (m_uint)zitarev_get_level);
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Output scale factor (in dB).";
	fun = new_dl_func("float", "level", (m_uint)zitarev_set_level);
		arg = dl_func_add_arg(fun, "float", "level");
		arg->doc = "Output scale factor (in dB).";
	CHECK_OB((f = import_mfun(env, fun)))
	f->doc = "Output scale factor (in dB).";
	env->class_def->doc = "8 FDN stereo zitareverb algorithm, imported from Faust.";
	CHECK_BB(import_class_end(env))

	return 1;
}
