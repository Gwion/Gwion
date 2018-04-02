#include <stdlib.h>
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "import.h"
#include "ugen.h"
#include "func.h"
#include "lang.h"
m_uint o_ftbl_data;
#define FTBL(o) *((sp_ftbl**)((M_Object)o)->data + o_ftbl_data)
#define CHECK_SIZE(size)	if(size <= 0){fprintf(stderr, "'gen_ftbl' size argument must be more than 0");return;}

DTOR(ftbl_dtor) {
	if(FTBL(o))
		sp_ftbl_destroy(&FTBL(o));
}

typedef struct {
	sp_data* sp;
	sp_adsr* osc;
} GW_adsr;

static TICK(adsr_tick) {
	const GW_adsr* ug = (GW_adsr*)u->module.gen.data;
	sp_adsr_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(adsr_ctor) {
	GW_adsr* ug = (GW_adsr*)xcalloc(1, sizeof(GW_adsr));
	ug->sp = shred->vm_ref->sp;
	sp_adsr_create(&ug->osc);
	sp_adsr_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), adsr_tick, ug, 0);
}

DTOR(adsr_dtor) {
	GW_adsr* ug = UGEN(o)->module.gen.data;
	sp_adsr_destroy(&ug->osc);
	free(ug);
}

MFUN(adsr_get_atk) {
	GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->atk;
}

MFUN(adsr_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->atk = atk);
}

MFUN(adsr_get_dec) {
	GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dec;
}

MFUN(adsr_set_dec) {
	m_uint gw_offset = SZ_INT;
	GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dec = dec);
}

MFUN(adsr_get_sus) {
	GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->sus;
}

MFUN(adsr_set_sus) {
	m_uint gw_offset = SZ_INT;
	GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
	m_float sus = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->sus = sus);
}

MFUN(adsr_get_rel) {
	GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->rel;
}

MFUN(adsr_set_rel) {
	m_uint gw_offset = SZ_INT;
	GW_adsr* ug = (GW_adsr*)UGEN(o)->module.gen.data;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
	sp_data* sp;
	sp_allpass* osc;
	m_bool is_init;
} GW_allpass;

static TICK(allpass_tick) {
	const GW_allpass* ug = (GW_allpass*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_allpass_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(allpass_ctor) {
	GW_allpass* ug = (GW_allpass*)xcalloc(1, sizeof(GW_allpass));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), allpass_tick, ug, 0);
}

DTOR(allpass_dtor) {
	GW_allpass* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_allpass_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(allpass_init) {
	m_uint gw_offset = SZ_INT;
	GW_allpass* ug = (GW_allpass*)UGEN(o)->module.gen.data;
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

MFUN(allpass_get_revtime) {
	GW_allpass* ug = (GW_allpass*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->revtime;
}

MFUN(allpass_set_revtime) {
	m_uint gw_offset = SZ_INT;
	GW_allpass* ug = (GW_allpass*)UGEN(o)->module.gen.data;
	m_float revtime = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->revtime = revtime);
}

typedef struct {
	sp_data* sp;
	sp_atone* osc;
} GW_atone;

static TICK(atone_tick) {
	const GW_atone* ug = (GW_atone*)u->module.gen.data;
	sp_atone_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(atone_ctor) {
	GW_atone* ug = (GW_atone*)xcalloc(1, sizeof(GW_atone));
	ug->sp = shred->vm_ref->sp;
	sp_atone_create(&ug->osc);
	sp_atone_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), atone_tick, ug, 0);
}

DTOR(atone_dtor) {
	GW_atone* ug = UGEN(o)->module.gen.data;
	sp_atone_destroy(&ug->osc);
	free(ug);
}

MFUN(atone_get_hp) {
	GW_atone* ug = (GW_atone*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->hp;
}

MFUN(atone_set_hp) {
	m_uint gw_offset = SZ_INT;
	GW_atone* ug = (GW_atone*)UGEN(o)->module.gen.data;
	m_float hp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->hp = hp);
}

typedef struct {
	sp_data* sp;
	sp_autowah* osc;
} GW_autowah;

static TICK(autowah_tick) {
	const GW_autowah* ug = (GW_autowah*)u->module.gen.data;
	sp_autowah_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(autowah_ctor) {
	GW_autowah* ug = (GW_autowah*)xcalloc(1, sizeof(GW_autowah));
	ug->sp = shred->vm_ref->sp;
	sp_autowah_create(&ug->osc);
	sp_autowah_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), autowah_tick, ug, 0);
}

DTOR(autowah_dtor) {
	GW_autowah* ug = UGEN(o)->module.gen.data;
	sp_autowah_destroy(&ug->osc);
	free(ug);
}

MFUN(autowah_get_level) {
	GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->level;
}

MFUN(autowah_set_level) {
	m_uint gw_offset = SZ_INT;
	GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
	m_float level = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->level = level);
}

MFUN(autowah_get_wah) {
	GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->wah;
}

MFUN(autowah_set_wah) {
	m_uint gw_offset = SZ_INT;
	GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
	m_float wah = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->wah = wah);
}

MFUN(autowah_get_mix) {
	GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->mix;
}

MFUN(autowah_set_mix) {
	m_uint gw_offset = SZ_INT;
	GW_autowah* ug = (GW_autowah*)UGEN(o)->module.gen.data;
	m_float mix = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->mix = mix);
}

typedef struct {
	sp_data* sp;
	sp_bal* osc;
} GW_bal;

static TICK(bal_tick) {
	const GW_bal* ug = (GW_bal*)u->module.gen.data;
	sp_bal_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);

}

CTOR(bal_ctor) {
	GW_bal* ug = (GW_bal*)xcalloc(1, sizeof(GW_bal));
	ug->sp = shred->vm_ref->sp;
	sp_bal_create(&ug->osc);
	sp_bal_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 1);
	ugen_gen(UGEN(o), bal_tick, ug, 0);
}

DTOR(bal_dtor) {
	GW_bal* ug = UGEN(o)->module.gen.data;
	sp_bal_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_bar* osc;
	m_bool is_init;
} GW_bar;

static TICK(bar_tick) {
	const GW_bar* ug = (GW_bar*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_bar_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(bar_ctor) {
	GW_bar* ug = (GW_bar*)xcalloc(1, sizeof(GW_bar));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), bar_tick, ug, 1);
}

DTOR(bar_dtor) {
	GW_bar* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_bar_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(bar_init) {
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
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

MFUN(bar_get_bcL) {
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bcL;
}

MFUN(bar_set_bcL) {
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	m_float bcL = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bcL = bcL);
}

MFUN(bar_get_bcR) {
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bcR;
}

MFUN(bar_set_bcR) {
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	m_float bcR = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bcR = bcR);
}

MFUN(bar_get_T30) {
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->T30;
}

MFUN(bar_set_T30) {
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	m_float T30 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->T30 = T30);
}

MFUN(bar_get_scan) {
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->scan;
}

MFUN(bar_set_scan) {
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	m_float scan = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->scan = scan);
}

MFUN(bar_get_pos) {
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->pos;
}

MFUN(bar_set_pos) {
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	m_float pos = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->pos = pos);
}

MFUN(bar_get_vel) {
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->vel;
}

MFUN(bar_set_vel) {
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	m_float vel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->vel = vel);
}

MFUN(bar_get_wid) {
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->wid;
}

MFUN(bar_set_wid) {
	m_uint gw_offset = SZ_INT;
	GW_bar* ug = (GW_bar*)UGEN(o)->module.gen.data;
	m_float wid = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->wid = wid);
}

typedef struct {
	sp_data* sp;
	sp_biquad* osc;
} GW_biquad;

static TICK(biquad_tick) {
	const GW_biquad* ug = (GW_biquad*)u->module.gen.data;
	sp_biquad_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(biquad_ctor) {
	GW_biquad* ug = (GW_biquad*)xcalloc(1, sizeof(GW_biquad));
	ug->sp = shred->vm_ref->sp;
	sp_biquad_create(&ug->osc);
	sp_biquad_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), biquad_tick, ug, 0);
}

DTOR(biquad_dtor) {
	GW_biquad* ug = UGEN(o)->module.gen.data;
	sp_biquad_destroy(&ug->osc);
	free(ug);
}

MFUN(biquad_get_b0) {
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->b0;
}

MFUN(biquad_set_b0) {
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	m_float b0 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->b0 = b0);
}

MFUN(biquad_get_b1) {
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->b1;
}

MFUN(biquad_set_b1) {
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	m_float b1 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->b1 = b1);
}

MFUN(biquad_get_b2) {
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->b2;
}

MFUN(biquad_set_b2) {
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	m_float b2 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->b2 = b2);
}

MFUN(biquad_get_a0) {
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->a0;
}

MFUN(biquad_set_a0) {
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	m_float a0 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->a0 = a0);
}

MFUN(biquad_get_a1) {
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->a1;
}

MFUN(biquad_set_a1) {
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	m_float a1 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->a1 = a1);
}

MFUN(biquad_get_a2) {
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->a2;
}

MFUN(biquad_set_a2) {
	m_uint gw_offset = SZ_INT;
	GW_biquad* ug = (GW_biquad*)UGEN(o)->module.gen.data;
	m_float a2 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->a2 = a2);
}

typedef struct {
	sp_data* sp;
	sp_biscale* osc;
} GW_biscale;

static TICK(biscale_tick) {
	const GW_biscale* ug = (GW_biscale*)u->module.gen.data;
	sp_biscale_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(biscale_ctor) {
	GW_biscale* ug = (GW_biscale*)xcalloc(1, sizeof(GW_biscale));
	ug->sp = shred->vm_ref->sp;
	sp_biscale_create(&ug->osc);
	sp_biscale_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), biscale_tick, ug, 0);
}

DTOR(biscale_dtor) {
	GW_biscale* ug = UGEN(o)->module.gen.data;
	sp_biscale_destroy(&ug->osc);
	free(ug);
}

MFUN(biscale_get_min) {
	GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->min;
}

MFUN(biscale_set_min) {
	m_uint gw_offset = SZ_INT;
	GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->min = min);
}

MFUN(biscale_get_max) {
	GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->max;
}

MFUN(biscale_set_max) {
	m_uint gw_offset = SZ_INT;
	GW_biscale* ug = (GW_biscale*)UGEN(o)->module.gen.data;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
	sp_data* sp;
	sp_bitcrush* osc;
} GW_bitcrush;

static TICK(bitcrush_tick) {
	const GW_bitcrush* ug = (GW_bitcrush*)u->module.gen.data;
	sp_bitcrush_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(bitcrush_ctor) {
	GW_bitcrush* ug = (GW_bitcrush*)xcalloc(1, sizeof(GW_bitcrush));
	ug->sp = shred->vm_ref->sp;
	sp_bitcrush_create(&ug->osc);
	sp_bitcrush_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), bitcrush_tick, ug, 0);
}

DTOR(bitcrush_dtor) {
	GW_bitcrush* ug = UGEN(o)->module.gen.data;
	sp_bitcrush_destroy(&ug->osc);
	free(ug);
}

MFUN(bitcrush_get_bitdepth) {
	GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bitdepth;
}

MFUN(bitcrush_set_bitdepth) {
	m_uint gw_offset = SZ_INT;
	GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
	m_float bitdepth = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bitdepth = bitdepth);
}

MFUN(bitcrush_get_srate) {
	GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->srate;
}

MFUN(bitcrush_set_srate) {
	m_uint gw_offset = SZ_INT;
	GW_bitcrush* ug = (GW_bitcrush*)UGEN(o)->module.gen.data;
	m_float srate = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->srate = srate);
}

typedef struct {
	sp_data* sp;
	sp_blsaw* osc;
} GW_blsaw;

static TICK(blsaw_tick) {
	const GW_blsaw* ug = (GW_blsaw*)u->module.gen.data;
	sp_blsaw_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(blsaw_ctor) {
	GW_blsaw* ug = (GW_blsaw*)xcalloc(1, sizeof(GW_blsaw));
	ug->sp = shred->vm_ref->sp;
	sp_blsaw_create(&ug->osc);
	sp_blsaw_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), blsaw_tick, ug, 0);
}

DTOR(blsaw_dtor) {
	GW_blsaw* ug = UGEN(o)->module.gen.data;
	sp_blsaw_destroy(&ug->osc);
	free(ug);
}

MFUN(blsaw_get_freq) {
	GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->freq;
}

MFUN(blsaw_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->freq = freq);
}

MFUN(blsaw_get_amp) {
	GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->amp;
}

MFUN(blsaw_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_blsaw* ug = (GW_blsaw*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->amp = amp);
}

typedef struct {
	sp_data* sp;
	sp_blsquare* osc;
} GW_blsquare;

static TICK(blsquare_tick) {
	const GW_blsquare* ug = (GW_blsquare*)u->module.gen.data;
	sp_blsquare_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(blsquare_ctor) {
	GW_blsquare* ug = (GW_blsquare*)xcalloc(1, sizeof(GW_blsquare));
	ug->sp = shred->vm_ref->sp;
	sp_blsquare_create(&ug->osc);
	sp_blsquare_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), blsquare_tick, ug, 0);
}

DTOR(blsquare_dtor) {
	GW_blsquare* ug = UGEN(o)->module.gen.data;
	sp_blsquare_destroy(&ug->osc);
	free(ug);
}

MFUN(blsquare_get_freq) {
	GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->freq;
}

MFUN(blsquare_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->freq = freq);
}

MFUN(blsquare_get_amp) {
	GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->amp;
}

MFUN(blsquare_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->amp = amp);
}

MFUN(blsquare_get_width) {
	GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->width;
}

MFUN(blsquare_set_width) {
	m_uint gw_offset = SZ_INT;
	GW_blsquare* ug = (GW_blsquare*)UGEN(o)->module.gen.data;
	m_float width = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->width = width);
}

typedef struct {
	sp_data* sp;
	sp_bltriangle* osc;
} GW_bltriangle;

static TICK(bltriangle_tick) {
	const GW_bltriangle* ug = (GW_bltriangle*)u->module.gen.data;
	sp_bltriangle_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(bltriangle_ctor) {
	GW_bltriangle* ug = (GW_bltriangle*)xcalloc(1, sizeof(GW_bltriangle));
	ug->sp = shred->vm_ref->sp;
	sp_bltriangle_create(&ug->osc);
	sp_bltriangle_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), bltriangle_tick, ug, 0);
}

DTOR(bltriangle_dtor) {
	GW_bltriangle* ug = UGEN(o)->module.gen.data;
	sp_bltriangle_destroy(&ug->osc);
	free(ug);
}

MFUN(bltriangle_get_freq) {
	GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->freq;
}

MFUN(bltriangle_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->freq = freq);
}

MFUN(bltriangle_get_amp) {
	GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->amp;
}

MFUN(bltriangle_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_bltriangle* ug = (GW_bltriangle*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->amp = amp);
}

typedef struct {
	sp_data* sp;
	sp_brown* osc;
} GW_brown;

static TICK(brown_tick) {
	const GW_brown* ug = (GW_brown*)u->module.gen.data;
	sp_brown_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(brown_ctor) {
	GW_brown* ug = (GW_brown*)xcalloc(1, sizeof(GW_brown));
	ug->sp = shred->vm_ref->sp;
	sp_brown_create(&ug->osc);
	sp_brown_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), brown_tick, ug, 0);
}

DTOR(brown_dtor) {
	GW_brown* ug = UGEN(o)->module.gen.data;
	sp_brown_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_butbp* osc;
} GW_butbp;

static TICK(butbp_tick) {
	const GW_butbp* ug = (GW_butbp*)u->module.gen.data;
	sp_butbp_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(butbp_ctor) {
	GW_butbp* ug = (GW_butbp*)xcalloc(1, sizeof(GW_butbp));
	ug->sp = shred->vm_ref->sp;
	sp_butbp_create(&ug->osc);
	sp_butbp_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), butbp_tick, ug, 0);
}

DTOR(butbp_dtor) {
	GW_butbp* ug = UGEN(o)->module.gen.data;
	sp_butbp_destroy(&ug->osc);
	free(ug);
}

MFUN(butbp_get_freq) {
	GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(butbp_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(butbp_get_bw) {
	GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bw;
}

MFUN(butbp_set_bw) {
	m_uint gw_offset = SZ_INT;
	GW_butbp* ug = (GW_butbp*)UGEN(o)->module.gen.data;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bw = bw);
}

typedef struct {
	sp_data* sp;
	sp_butbr* osc;
} GW_butbr;

static TICK(butbr_tick) {
	const GW_butbr* ug = (GW_butbr*)u->module.gen.data;
	sp_butbr_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(butbr_ctor) {
	GW_butbr* ug = (GW_butbr*)xcalloc(1, sizeof(GW_butbr));
	ug->sp = shred->vm_ref->sp;
	sp_butbr_create(&ug->osc);
	sp_butbr_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), butbr_tick, ug, 0);
}

DTOR(butbr_dtor) {
	GW_butbr* ug = UGEN(o)->module.gen.data;
	sp_butbr_destroy(&ug->osc);
	free(ug);
}

MFUN(butbr_get_freq) {
	GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(butbr_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(butbr_get_bw) {
	GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bw;
}

MFUN(butbr_set_bw) {
	m_uint gw_offset = SZ_INT;
	GW_butbr* ug = (GW_butbr*)UGEN(o)->module.gen.data;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bw = bw);
}

typedef struct {
	sp_data* sp;
	sp_buthp* osc;
} GW_buthp;

static TICK(buthp_tick) {
	const GW_buthp* ug = (GW_buthp*)u->module.gen.data;
	sp_buthp_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(buthp_ctor) {
	GW_buthp* ug = (GW_buthp*)xcalloc(1, sizeof(GW_buthp));
	ug->sp = shred->vm_ref->sp;
	sp_buthp_create(&ug->osc);
	sp_buthp_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), buthp_tick, ug, 0);
}

DTOR(buthp_dtor) {
	GW_buthp* ug = UGEN(o)->module.gen.data;
	sp_buthp_destroy(&ug->osc);
	free(ug);
}

MFUN(buthp_get_freq) {
	GW_buthp* ug = (GW_buthp*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(buthp_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_buthp* ug = (GW_buthp*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
	sp_data* sp;
	sp_butlp* osc;
} GW_butlp;

static TICK(butlp_tick) {
	const GW_butlp* ug = (GW_butlp*)u->module.gen.data;
	sp_butlp_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(butlp_ctor) {
	GW_butlp* ug = (GW_butlp*)xcalloc(1, sizeof(GW_butlp));
	ug->sp = shred->vm_ref->sp;
	sp_butlp_create(&ug->osc);
	sp_butlp_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), butlp_tick, ug, 0);
}

DTOR(butlp_dtor) {
	GW_butlp* ug = UGEN(o)->module.gen.data;
	sp_butlp_destroy(&ug->osc);
	free(ug);
}

MFUN(butlp_get_freq) {
	GW_butlp* ug = (GW_butlp*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(butlp_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_butlp* ug = (GW_butlp*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
	sp_data* sp;
	sp_clip* osc;
} GW_clip;

static TICK(clip_tick) {
	const GW_clip* ug = (GW_clip*)u->module.gen.data;
	sp_clip_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(clip_ctor) {
	GW_clip* ug = (GW_clip*)xcalloc(1, sizeof(GW_clip));
	ug->sp = shred->vm_ref->sp;
	sp_clip_create(&ug->osc);
	sp_clip_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), clip_tick, ug, 0);
}

DTOR(clip_dtor) {
	GW_clip* ug = UGEN(o)->module.gen.data;
	sp_clip_destroy(&ug->osc);
	free(ug);
}

MFUN(clip_get_lim) {
	GW_clip* ug = (GW_clip*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->lim;
}

MFUN(clip_set_lim) {
	m_uint gw_offset = SZ_INT;
	GW_clip* ug = (GW_clip*)UGEN(o)->module.gen.data;
	m_float lim = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->lim = lim);
}

typedef struct {
	sp_data* sp;
	sp_clock* osc;
} GW_clock;

static TICK(clock_tick) {
	const GW_clock* ug = (GW_clock*)u->module.gen.data;
	sp_clock_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(clock_ctor) {
	GW_clock* ug = (GW_clock*)xcalloc(1, sizeof(GW_clock));
	ug->sp = shred->vm_ref->sp;
	sp_clock_create(&ug->osc);
	sp_clock_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), clock_tick, ug, 1);
}

DTOR(clock_dtor) {
	GW_clock* ug = UGEN(o)->module.gen.data;
	sp_clock_destroy(&ug->osc);
	free(ug);
}

MFUN(clock_get_bpm) {
	GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bpm;
}

MFUN(clock_set_bpm) {
	m_uint gw_offset = SZ_INT;
	GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
	m_float bpm = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bpm = bpm);
}

MFUN(clock_get_subdiv) {
	GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->subdiv;
}

MFUN(clock_set_subdiv) {
	m_uint gw_offset = SZ_INT;
	GW_clock* ug = (GW_clock*)UGEN(o)->module.gen.data;
	m_float subdiv = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->subdiv = subdiv);
}

typedef struct {
	sp_data* sp;
	sp_comb* osc;
	m_bool is_init;
} GW_comb;

static TICK(comb_tick) {
	const GW_comb* ug = (GW_comb*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_comb_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(comb_ctor) {
	GW_comb* ug = (GW_comb*)xcalloc(1, sizeof(GW_comb));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), comb_tick, ug, 0);
}

DTOR(comb_dtor) {
	GW_comb* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_comb_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(comb_init) {
	m_uint gw_offset = SZ_INT;
	GW_comb* ug = (GW_comb*)UGEN(o)->module.gen.data;
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

MFUN(comb_get_revtime) {
	GW_comb* ug = (GW_comb*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->revtime;
}

MFUN(comb_set_revtime) {
	m_uint gw_offset = SZ_INT;
	GW_comb* ug = (GW_comb*)UGEN(o)->module.gen.data;
	m_float revtime = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->revtime = revtime);
}

typedef struct {
	sp_data* sp;
	sp_compressor* osc;
} GW_compressor;

static TICK(compressor_tick) {
	const GW_compressor* ug = (GW_compressor*)u->module.gen.data;
	sp_compressor_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(compressor_ctor) {
	GW_compressor* ug = (GW_compressor*)xcalloc(1, sizeof(GW_compressor));
	ug->sp = shred->vm_ref->sp;
	sp_compressor_create(&ug->osc);
	sp_compressor_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), compressor_tick, ug, 0);
}

DTOR(compressor_dtor) {
	GW_compressor* ug = UGEN(o)->module.gen.data;
	sp_compressor_destroy(&ug->osc);
	free(ug);
}

MFUN(compressor_get_ratio) {
	GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->ratio;
}

MFUN(compressor_set_ratio) {
	m_uint gw_offset = SZ_INT;
	GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
	m_float ratio = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->ratio = ratio);
}

MFUN(compressor_get_thresh) {
	GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->thresh;
}

MFUN(compressor_set_thresh) {
	m_uint gw_offset = SZ_INT;
	GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
	m_float thresh = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->thresh = thresh);
}

MFUN(compressor_get_atk) {
	GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->atk;
}

MFUN(compressor_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->atk = atk);
}

MFUN(compressor_get_rel) {
	GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->rel;
}

MFUN(compressor_set_rel) {
	m_uint gw_offset = SZ_INT;
	GW_compressor* ug = (GW_compressor*)UGEN(o)->module.gen.data;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->rel = rel);
}

typedef struct {
	sp_data* sp;
	sp_conv* osc;
	m_bool is_init;
} GW_conv;

static TICK(conv_tick) {
	const GW_conv* ug = (GW_conv*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_conv_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(conv_ctor) {
	GW_conv* ug = (GW_conv*)xcalloc(1, sizeof(GW_conv));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), conv_tick, ug, 0);
}

DTOR(conv_dtor) {
	GW_conv* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_conv_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(conv_init) {
	m_uint gw_offset = SZ_INT;
	GW_conv* ug = (GW_conv*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_count* osc;
} GW_count;

static TICK(count_tick) {
	const GW_count* ug = (GW_count*)u->module.gen.data;
	sp_count_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(count_ctor) {
	GW_count* ug = (GW_count*)xcalloc(1, sizeof(GW_count));
	ug->sp = shred->vm_ref->sp;
	sp_count_create(&ug->osc);
	sp_count_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), count_tick, ug, 1);
}

DTOR(count_dtor) {
	GW_count* ug = UGEN(o)->module.gen.data;
	sp_count_destroy(&ug->osc);
	free(ug);
}

MFUN(count_get_count) {
	GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->count;
}

MFUN(count_set_count) {
	m_uint gw_offset = SZ_INT;
	GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
	m_float count = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->count = count);
}

MFUN(count_get_mode) {
	GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->mode;
}

MFUN(count_set_mode) {
	m_uint gw_offset = SZ_INT;
	GW_count* ug = (GW_count*)UGEN(o)->module.gen.data;
	m_float mode = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
	sp_data* sp;
	sp_crossfade* osc;
} GW_crossfade;

static TICK(crossfade_tick) {
	const GW_crossfade* ug = (GW_crossfade*)u->module.gen.data;
	sp_crossfade_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);

}

CTOR(crossfade_ctor) {
	GW_crossfade* ug = (GW_crossfade*)xcalloc(1, sizeof(GW_crossfade));
	ug->sp = shred->vm_ref->sp;
	sp_crossfade_create(&ug->osc);
	sp_crossfade_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 1);
	ugen_gen(UGEN(o), crossfade_tick, ug, 0);
}

DTOR(crossfade_dtor) {
	GW_crossfade* ug = UGEN(o)->module.gen.data;
	sp_crossfade_destroy(&ug->osc);
	free(ug);
}

MFUN(crossfade_get_pos) {
	GW_crossfade* ug = (GW_crossfade*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->pos;
}

MFUN(crossfade_set_pos) {
	m_uint gw_offset = SZ_INT;
	GW_crossfade* ug = (GW_crossfade*)UGEN(o)->module.gen.data;
	m_float pos = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->pos = pos);
}

typedef struct {
	sp_data* sp;
	sp_dcblock* osc;
} GW_dcblock;

static TICK(dcblock_tick) {
	const GW_dcblock* ug = (GW_dcblock*)u->module.gen.data;
	sp_dcblock_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(dcblock_ctor) {
	GW_dcblock* ug = (GW_dcblock*)xcalloc(1, sizeof(GW_dcblock));
	ug->sp = shred->vm_ref->sp;
	sp_dcblock_create(&ug->osc);
	sp_dcblock_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), dcblock_tick, ug, 0);
}

DTOR(dcblock_dtor) {
	GW_dcblock* ug = UGEN(o)->module.gen.data;
	sp_dcblock_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_delay* osc;
	m_bool is_init;
} GW_delay;

static TICK(delay_tick) {
	const GW_delay* ug = (GW_delay*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_delay_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(delay_ctor) {
	GW_delay* ug = (GW_delay*)xcalloc(1, sizeof(GW_delay));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), delay_tick, ug, 0);
}

DTOR(delay_dtor) {
	GW_delay* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_delay_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(delay_init) {
	m_uint gw_offset = SZ_INT;
	GW_delay* ug = (GW_delay*)UGEN(o)->module.gen.data;
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

MFUN(delay_get_feedback) {
	GW_delay* ug = (GW_delay*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->feedback;
}

MFUN(delay_set_feedback) {
	m_uint gw_offset = SZ_INT;
	GW_delay* ug = (GW_delay*)UGEN(o)->module.gen.data;
	m_float feedback = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->feedback = feedback);
}

typedef struct {
	sp_data* sp;
	sp_diode* osc;
} GW_diode;

static TICK(diode_tick) {
	const GW_diode* ug = (GW_diode*)u->module.gen.data;
	sp_diode_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(diode_ctor) {
	GW_diode* ug = (GW_diode*)xcalloc(1, sizeof(GW_diode));
	ug->sp = shred->vm_ref->sp;
	sp_diode_create(&ug->osc);
	sp_diode_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), diode_tick, ug, 0);
}

DTOR(diode_dtor) {
	GW_diode* ug = UGEN(o)->module.gen.data;
	sp_diode_destroy(&ug->osc);
	free(ug);
}

MFUN(diode_get_freq) {
	GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(diode_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(diode_get_res) {
	GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->res;
}

MFUN(diode_set_res) {
	m_uint gw_offset = SZ_INT;
	GW_diode* ug = (GW_diode*)UGEN(o)->module.gen.data;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->res = res);
}

typedef struct {
	sp_data* sp;
	sp_diskin* osc;
	m_bool is_init;
} GW_diskin;

static TICK(diskin_tick) {
	const GW_diskin* ug = (GW_diskin*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_diskin_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(diskin_ctor) {
	GW_diskin* ug = (GW_diskin*)xcalloc(1, sizeof(GW_diskin));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), diskin_tick, ug, 0);
}

DTOR(diskin_dtor) {
	GW_diskin* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_diskin_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(diskin_init) {
	m_uint gw_offset = SZ_INT;
	GW_diskin* ug = (GW_diskin*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_dist* osc;
} GW_dist;

static TICK(dist_tick) {
	const GW_dist* ug = (GW_dist*)u->module.gen.data;
	sp_dist_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(dist_ctor) {
	GW_dist* ug = (GW_dist*)xcalloc(1, sizeof(GW_dist));
	ug->sp = shred->vm_ref->sp;
	sp_dist_create(&ug->osc);
	sp_dist_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), dist_tick, ug, 0);
}

DTOR(dist_dtor) {
	GW_dist* ug = UGEN(o)->module.gen.data;
	sp_dist_destroy(&ug->osc);
	free(ug);
}

MFUN(dist_get_pregain) {
	GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->pregain;
}

MFUN(dist_set_pregain) {
	m_uint gw_offset = SZ_INT;
	GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
	m_float pregain = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->pregain = pregain);
}

MFUN(dist_get_postgain) {
	GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->postgain;
}

MFUN(dist_set_postgain) {
	m_uint gw_offset = SZ_INT;
	GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
	m_float postgain = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->postgain = postgain);
}

MFUN(dist_get_shape1) {
	GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->shape1;
}

MFUN(dist_set_shape1) {
	m_uint gw_offset = SZ_INT;
	GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
	m_float shape1 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->shape1 = shape1);
}

MFUN(dist_get_shape2) {
	GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->shape2;
}

MFUN(dist_set_shape2) {
	m_uint gw_offset = SZ_INT;
	GW_dist* ug = (GW_dist*)UGEN(o)->module.gen.data;
	m_float shape2 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->shape2 = shape2);
}

typedef struct {
	sp_data* sp;
	sp_dmetro* osc;
} GW_dmetro;

static TICK(dmetro_tick) {
	const GW_dmetro* ug = (GW_dmetro*)u->module.gen.data;
	sp_dmetro_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(dmetro_ctor) {
	GW_dmetro* ug = (GW_dmetro*)xcalloc(1, sizeof(GW_dmetro));
	ug->sp = shred->vm_ref->sp;
	sp_dmetro_create(&ug->osc);
	sp_dmetro_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), dmetro_tick, ug, 0);
}

DTOR(dmetro_dtor) {
	GW_dmetro* ug = UGEN(o)->module.gen.data;
	sp_dmetro_destroy(&ug->osc);
	free(ug);
}

MFUN(dmetro_get_time) {
	GW_dmetro* ug = (GW_dmetro*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->time;
}

MFUN(dmetro_set_time) {
	m_uint gw_offset = SZ_INT;
	GW_dmetro* ug = (GW_dmetro*)UGEN(o)->module.gen.data;
	m_float time = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->time = time);
}

typedef struct {
	sp_data* sp;
	sp_drip* osc;
	m_bool is_init;
} GW_drip;

static TICK(drip_tick) {
	const GW_drip* ug = (GW_drip*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_drip_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(drip_ctor) {
	GW_drip* ug = (GW_drip*)xcalloc(1, sizeof(GW_drip));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), drip_tick, ug, 1);
}

DTOR(drip_dtor) {
	GW_drip* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_drip_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(drip_init) {
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
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

MFUN(drip_get_num_tubes) {
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->num_tubes;
}

MFUN(drip_set_num_tubes) {
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	m_float num_tubes = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->num_tubes = num_tubes);
}

MFUN(drip_get_amp) {
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(drip_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(drip_get_damp) {
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->damp;
}

MFUN(drip_set_damp) {
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	m_float damp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->damp = damp);
}

MFUN(drip_get_shake_max) {
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->shake_max;
}

MFUN(drip_set_shake_max) {
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	m_float shake_max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->shake_max = shake_max);
}

MFUN(drip_get_freq) {
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(drip_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(drip_get_freq1) {
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq1;
}

MFUN(drip_set_freq1) {
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	m_float freq1 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq1 = freq1);
}

MFUN(drip_get_freq2) {
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq2;
}

MFUN(drip_set_freq2) {
	m_uint gw_offset = SZ_INT;
	GW_drip* ug = (GW_drip*)UGEN(o)->module.gen.data;
	m_float freq2 = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq2 = freq2);
}

typedef struct {
	sp_data* sp;
	sp_dtrig* osc;
	m_bool is_init;
} GW_dtrig;

static TICK(dtrig_tick) {
	const GW_dtrig* ug = (GW_dtrig*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_dtrig_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(dtrig_ctor) {
	GW_dtrig* ug = (GW_dtrig*)xcalloc(1, sizeof(GW_dtrig));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), dtrig_tick, ug, 1);
}

DTOR(dtrig_dtor) {
	GW_dtrig* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_dtrig_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(dtrig_init) {
	m_uint gw_offset = SZ_INT;
	GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
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

MFUN(dtrig_get_loop) {
	GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->loop;
}

MFUN(dtrig_set_loop) {
	m_uint gw_offset = SZ_INT;
	GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
	m_int loop = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->loop = loop);
}

MFUN(dtrig_get_delay) {
	GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->delay;
}

MFUN(dtrig_set_delay) {
	m_uint gw_offset = SZ_INT;
	GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
	m_float delay = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->delay = delay);
}

MFUN(dtrig_get_scale) {
	GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->scale;
}

MFUN(dtrig_set_scale) {
	m_uint gw_offset = SZ_INT;
	GW_dtrig* ug = (GW_dtrig*)UGEN(o)->module.gen.data;
	m_float scale = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->scale = scale);
}

typedef struct {
	sp_data* sp;
	sp_dust* osc;
} GW_dust;

static TICK(dust_tick) {
	const GW_dust* ug = (GW_dust*)u->module.gen.data;
	sp_dust_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(dust_ctor) {
	GW_dust* ug = (GW_dust*)xcalloc(1, sizeof(GW_dust));
	ug->sp = shred->vm_ref->sp;
	sp_dust_create(&ug->osc);
	sp_dust_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), dust_tick, ug, 0);
}

DTOR(dust_dtor) {
	GW_dust* ug = UGEN(o)->module.gen.data;
	sp_dust_destroy(&ug->osc);
	free(ug);
}

MFUN(dust_get_amp) {
	GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(dust_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(dust_get_density) {
	GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->density;
}

MFUN(dust_set_density) {
	m_uint gw_offset = SZ_INT;
	GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
	m_float density = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->density = density);
}

MFUN(dust_get_bipolar) {
	GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->bipolar;
}

MFUN(dust_set_bipolar) {
	m_uint gw_offset = SZ_INT;
	GW_dust* ug = (GW_dust*)UGEN(o)->module.gen.data;
	m_int bipolar = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->bipolar = bipolar);
}

typedef struct {
	sp_data* sp;
	sp_eqfil* osc;
} GW_eqfil;

static TICK(eqfil_tick) {
	const GW_eqfil* ug = (GW_eqfil*)u->module.gen.data;
	sp_eqfil_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(eqfil_ctor) {
	GW_eqfil* ug = (GW_eqfil*)xcalloc(1, sizeof(GW_eqfil));
	ug->sp = shred->vm_ref->sp;
	sp_eqfil_create(&ug->osc);
	sp_eqfil_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), eqfil_tick, ug, 0);
}

DTOR(eqfil_dtor) {
	GW_eqfil* ug = UGEN(o)->module.gen.data;
	sp_eqfil_destroy(&ug->osc);
	free(ug);
}

MFUN(eqfil_get_freq) {
	GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(eqfil_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(eqfil_get_bw) {
	GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bw;
}

MFUN(eqfil_set_bw) {
	m_uint gw_offset = SZ_INT;
	GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bw = bw);
}

MFUN(eqfil_get_gain) {
	GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->gain;
}

MFUN(eqfil_set_gain) {
	m_uint gw_offset = SZ_INT;
	GW_eqfil* ug = (GW_eqfil*)UGEN(o)->module.gen.data;
	m_float gain = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->gain = gain);
}

typedef struct {
	sp_data* sp;
	sp_expon* osc;
} GW_expon;

static TICK(expon_tick) {
	const GW_expon* ug = (GW_expon*)u->module.gen.data;
	sp_expon_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(expon_ctor) {
	GW_expon* ug = (GW_expon*)xcalloc(1, sizeof(GW_expon));
	ug->sp = shred->vm_ref->sp;
	sp_expon_create(&ug->osc);
	sp_expon_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), expon_tick, ug, 1);
}

DTOR(expon_dtor) {
	GW_expon* ug = UGEN(o)->module.gen.data;
	sp_expon_destroy(&ug->osc);
	free(ug);
}

MFUN(expon_get_a) {
	GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->a;
}

MFUN(expon_set_a) {
	m_uint gw_offset = SZ_INT;
	GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
	m_float a = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->a = a);
}

MFUN(expon_get_dur) {
	GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dur;
}

MFUN(expon_set_dur) {
	m_uint gw_offset = SZ_INT;
	GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dur = dur);
}

MFUN(expon_get_b) {
	GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->b;
}

MFUN(expon_set_b) {
	m_uint gw_offset = SZ_INT;
	GW_expon* ug = (GW_expon*)UGEN(o)->module.gen.data;
	m_float b = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->b = b);
}

typedef struct {
	sp_data* sp;
	sp_fof* osc;
	m_bool is_init;
} GW_fof;

static TICK(fof_tick) {
	const GW_fof* ug = (GW_fof*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_fof_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(fof_ctor) {
	GW_fof* ug = (GW_fof*)xcalloc(1, sizeof(GW_fof));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), fof_tick, ug, 0);
}

DTOR(fof_dtor) {
	GW_fof* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_fof_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(fof_init) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
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

MFUN(fof_get_amp) {
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(fof_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(fof_get_fund) {
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->fund;
}

MFUN(fof_set_fund) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	m_float fund = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->fund = fund);
}

MFUN(fof_get_form) {
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->form;
}

MFUN(fof_set_form) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	m_float form = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->form = form);
}

MFUN(fof_get_oct) {
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->oct;
}

MFUN(fof_set_oct) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	m_float oct = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->oct = oct);
}

MFUN(fof_get_band) {
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->band;
}

MFUN(fof_set_band) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	m_float band = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->band = band);
}

MFUN(fof_get_ris) {
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->ris;
}

MFUN(fof_set_ris) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	m_float ris = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->ris = ris);
}

MFUN(fof_get_dec) {
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dec;
}

MFUN(fof_set_dec) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dec = dec);
}

MFUN(fof_get_dur) {
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dur;
}

MFUN(fof_set_dur) {
	m_uint gw_offset = SZ_INT;
	GW_fof* ug = (GW_fof*)UGEN(o)->module.gen.data;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dur = dur);
}

typedef struct {
	sp_data* sp;
	sp_fofilt* osc;
} GW_fofilt;

static TICK(fofilt_tick) {
	const GW_fofilt* ug = (GW_fofilt*)u->module.gen.data;
	sp_fofilt_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(fofilt_ctor) {
	GW_fofilt* ug = (GW_fofilt*)xcalloc(1, sizeof(GW_fofilt));
	ug->sp = shred->vm_ref->sp;
	sp_fofilt_create(&ug->osc);
	sp_fofilt_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), fofilt_tick, ug, 0);
}

DTOR(fofilt_dtor) {
	GW_fofilt* ug = UGEN(o)->module.gen.data;
	sp_fofilt_destroy(&ug->osc);
	free(ug);
}

MFUN(fofilt_get_freq) {
	GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(fofilt_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(fofilt_get_atk) {
	GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->atk;
}

MFUN(fofilt_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->atk = atk);
}

MFUN(fofilt_get_dec) {
	GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dec;
}

MFUN(fofilt_set_dec) {
	m_uint gw_offset = SZ_INT;
	GW_fofilt* ug = (GW_fofilt*)UGEN(o)->module.gen.data;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dec = dec);
}

typedef struct {
	sp_data* sp;
	sp_fog* osc;
	m_bool is_init;
} GW_fog;

static TICK(fog_tick) {
	const GW_fog* ug = (GW_fog*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_fog_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(fog_ctor) {
	GW_fog* ug = (GW_fog*)xcalloc(1, sizeof(GW_fog));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), fog_tick, ug, 0);
}

DTOR(fog_dtor) {
	GW_fog* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_fog_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(fog_init) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
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

MFUN(fog_get_amp) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(fog_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(fog_get_dens) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dens;
}

MFUN(fog_set_dens) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float dens = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dens = dens);
}

MFUN(fog_get_trans) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->trans;
}

MFUN(fog_set_trans) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float trans = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->trans = trans);
}

MFUN(fog_get_spd) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->spd;
}

MFUN(fog_set_spd) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float spd = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->spd = spd);
}

MFUN(fog_get_oct) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->oct;
}

MFUN(fog_set_oct) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float oct = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->oct = oct);
}

MFUN(fog_get_band) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->band;
}

MFUN(fog_set_band) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float band = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->band = band);
}

MFUN(fog_get_ris) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->ris;
}

MFUN(fog_set_ris) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float ris = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->ris = ris);
}

MFUN(fog_get_dec) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dec;
}

MFUN(fog_set_dec) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dec = dec);
}

MFUN(fog_get_dur) {
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dur;
}

MFUN(fog_set_dur) {
	m_uint gw_offset = SZ_INT;
	GW_fog* ug = (GW_fog*)UGEN(o)->module.gen.data;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dur = dur);
}

typedef struct {
	sp_data* sp;
	sp_fold* osc;
} GW_fold;

static TICK(fold_tick) {
	const GW_fold* ug = (GW_fold*)u->module.gen.data;
	sp_fold_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(fold_ctor) {
	GW_fold* ug = (GW_fold*)xcalloc(1, sizeof(GW_fold));
	ug->sp = shred->vm_ref->sp;
	sp_fold_create(&ug->osc);
	sp_fold_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), fold_tick, ug, 0);
}

DTOR(fold_dtor) {
	GW_fold* ug = UGEN(o)->module.gen.data;
	sp_fold_destroy(&ug->osc);
	free(ug);
}

MFUN(fold_get_incr) {
	GW_fold* ug = (GW_fold*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->incr;
}

MFUN(fold_set_incr) {
	m_uint gw_offset = SZ_INT;
	GW_fold* ug = (GW_fold*)UGEN(o)->module.gen.data;
	m_float incr = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->incr = incr);
}

typedef struct {
	sp_data* sp;
	sp_fosc* osc;
	m_bool is_init;
} GW_fosc;

static TICK(fosc_tick) {
	const GW_fosc* ug = (GW_fosc*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_fosc_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(fosc_ctor) {
	GW_fosc* ug = (GW_fosc*)xcalloc(1, sizeof(GW_fosc));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), fosc_tick, ug, 0);
}

DTOR(fosc_dtor) {
	GW_fosc* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_fosc_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(fosc_init) {
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
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

MFUN(fosc_get_freq) {
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(fosc_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(fosc_get_amp) {
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(fosc_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(fosc_get_car) {
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->car;
}

MFUN(fosc_set_car) {
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	m_float car = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->car = car);
}

MFUN(fosc_get_mod) {
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->mod;
}

MFUN(fosc_set_mod) {
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	m_float mod = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->mod = mod);
}

MFUN(fosc_get_indx) {
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->indx;
}

MFUN(fosc_set_indx) {
	m_uint gw_offset = SZ_INT;
	GW_fosc* ug = (GW_fosc*)UGEN(o)->module.gen.data;
	m_float indx = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->indx = indx);
}

typedef struct {
	sp_data* sp;
	sp_gbuzz* osc;
	m_bool is_init;
} GW_gbuzz;

static TICK(gbuzz_tick) {
	const GW_gbuzz* ug = (GW_gbuzz*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_gbuzz_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(gbuzz_ctor) {
	GW_gbuzz* ug = (GW_gbuzz*)xcalloc(1, sizeof(GW_gbuzz));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), gbuzz_tick, ug, 0);
}

DTOR(gbuzz_dtor) {
	GW_gbuzz* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_gbuzz_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(gbuzz_init) {
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
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

MFUN(gbuzz_get_freq) {
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(gbuzz_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(gbuzz_get_amp) {
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(gbuzz_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(gbuzz_get_nharm) {
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->nharm;
}

MFUN(gbuzz_set_nharm) {
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	m_float nharm = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->nharm = nharm);
}

MFUN(gbuzz_get_lharm) {
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->lharm;
}

MFUN(gbuzz_set_lharm) {
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	m_float lharm = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->lharm = lharm);
}

MFUN(gbuzz_get_mul) {
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->mul;
}

MFUN(gbuzz_set_mul) {
	m_uint gw_offset = SZ_INT;
	GW_gbuzz* ug = (GW_gbuzz*)UGEN(o)->module.gen.data;
	m_float mul = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->mul = mul);
}

MFUN(ftbl_gen_composite) {
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_composite(shred->vm_ref->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_file) {
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object filename_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str filename = STRING(filename_obj);
	release(filename_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_file(shred->vm_ref->sp, ftbl, filename);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_gauss) {
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	m_float scale = *(m_float*)(shred->mem + gw_offset);
	gw_offset +=SZ_FLOAT;
	m_int seed = *(m_int*)(shred->mem + gw_offset);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_gauss(shred->vm_ref->sp, ftbl, scale, seed);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_line) {
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_line(shred->vm_ref->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_padsynth) {
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
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_padsynth(shred->vm_ref->sp, ftbl, amps, f, bw);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_rand) {
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_rand(shred->vm_ref->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_scrambler) {
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object dest_obj = *(M_Object*)(shred->mem + gw_offset);
	sp_ftbl** dest = &FTBL(dest_obj);
	release(dest_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_scrambler(shred->vm_ref->sp, ftbl, dest);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_sine) {
	sp_ftbl* ftbl = FTBL(o);
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_sine(shred->vm_ref->sp, ftbl);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_sinesum) {
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_sinesum(shred->vm_ref->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

MFUN(ftbl_gen_xline) {
	sp_ftbl* ftbl = FTBL(o);
	m_uint gw_offset = SZ_INT*2;
	if(FTBL(o))
    sp_ftbl_destroy(&ftbl);
	m_int size = *(m_int*)(shred->mem + SZ_INT);
	M_Object argstring_obj = *(M_Object*)(shred->mem + gw_offset);
	m_str argstring = STRING(argstring_obj);
	release(argstring_obj, shred);
	CHECK_SIZE(size);
	sp_ftbl_create(shred->vm_ref->sp, &ftbl, size);
	sp_gen_xline(shred->vm_ref->sp, ftbl, argstring);
	FTBL(o) = ftbl;
}

typedef struct {
	sp_data* sp;
	sp_hilbert* osc;
} GW_hilbert;

static TICK(hilbert_tick) {
	const GW_hilbert* ug = (GW_hilbert*)u->module.gen.data;
	sp_hilbert_compute(ug->sp, ug->osc, &u->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

CTOR(hilbert_ctor) {
	GW_hilbert* ug = (GW_hilbert*)xcalloc(1, sizeof(GW_hilbert));
	ug->sp = shred->vm_ref->sp;
	sp_hilbert_create(&ug->osc);
	sp_hilbert_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 2);
	ugen_gen(UGEN(o), hilbert_tick, ug, 0);
}

DTOR(hilbert_dtor) {
	GW_hilbert* ug = UGEN(o)->module.gen.data;
	sp_hilbert_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_in* osc;
} GW_in;

static TICK(in_tick) {
	const GW_in* ug = (GW_in*)u->module.gen.data;
	sp_in_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(in_ctor) {
	GW_in* ug = (GW_in*)xcalloc(1, sizeof(GW_in));
	ug->sp = shred->vm_ref->sp;
	sp_in_create(&ug->osc);
	sp_in_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), in_tick, ug, 0);
}

DTOR(in_dtor) {
	GW_in* ug = UGEN(o)->module.gen.data;
	sp_in_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_incr* osc;
	m_bool is_init;
} GW_incr;

static TICK(incr_tick) {
	const GW_incr* ug = (GW_incr*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_incr_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(incr_ctor) {
	GW_incr* ug = (GW_incr*)xcalloc(1, sizeof(GW_incr));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), incr_tick, ug, 1);
}

DTOR(incr_dtor) {
	GW_incr* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_incr_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(incr_init) {
	m_uint gw_offset = SZ_INT;
	GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
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

MFUN(incr_get_step) {
	GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->step;
}

MFUN(incr_set_step) {
	m_uint gw_offset = SZ_INT;
	GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
	m_float step = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->step = step);
}

MFUN(incr_get_min) {
	GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->min;
}

MFUN(incr_set_min) {
	m_uint gw_offset = SZ_INT;
	GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->min = min);
}

MFUN(incr_get_max) {
	GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->max;
}

MFUN(incr_set_max) {
	m_uint gw_offset = SZ_INT;
	GW_incr* ug = (GW_incr*)UGEN(o)->module.gen.data;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
	sp_data* sp;
	sp_jcrev* osc;
} GW_jcrev;

static TICK(jcrev_tick) {
	const GW_jcrev* ug = (GW_jcrev*)u->module.gen.data;
	sp_jcrev_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(jcrev_ctor) {
	GW_jcrev* ug = (GW_jcrev*)xcalloc(1, sizeof(GW_jcrev));
	ug->sp = shred->vm_ref->sp;
	sp_jcrev_create(&ug->osc);
	sp_jcrev_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), jcrev_tick, ug, 0);
}

DTOR(jcrev_dtor) {
	GW_jcrev* ug = UGEN(o)->module.gen.data;
	sp_jcrev_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_jitter* osc;
} GW_jitter;

static TICK(jitter_tick) {
	const GW_jitter* ug = (GW_jitter*)u->module.gen.data;
	sp_jitter_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(jitter_ctor) {
	GW_jitter* ug = (GW_jitter*)xcalloc(1, sizeof(GW_jitter));
	ug->sp = shred->vm_ref->sp;
	sp_jitter_create(&ug->osc);
	sp_jitter_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), jitter_tick, ug, 0);
}

DTOR(jitter_dtor) {
	GW_jitter* ug = UGEN(o)->module.gen.data;
	sp_jitter_destroy(&ug->osc);
	free(ug);
}

MFUN(jitter_get_amp) {
	GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(jitter_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(jitter_get_cpsMin) {
	GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->cpsMin;
}

MFUN(jitter_set_cpsMin) {
	m_uint gw_offset = SZ_INT;
	GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
	m_float cpsMin = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->cpsMin = cpsMin);
}

MFUN(jitter_get_cpsMax) {
	GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->cpsMax;
}

MFUN(jitter_set_cpsMax) {
	m_uint gw_offset = SZ_INT;
	GW_jitter* ug = (GW_jitter*)UGEN(o)->module.gen.data;
	m_float cpsMax = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->cpsMax = cpsMax);
}

typedef struct {
	sp_data* sp;
	sp_line* osc;
} GW_line;

static TICK(line_tick) {
	const GW_line* ug = (GW_line*)u->module.gen.data;
	sp_line_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(line_ctor) {
	GW_line* ug = (GW_line*)xcalloc(1, sizeof(GW_line));
	ug->sp = shred->vm_ref->sp;
	sp_line_create(&ug->osc);
	sp_line_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), line_tick, ug, 1);
}

DTOR(line_dtor) {
	GW_line* ug = UGEN(o)->module.gen.data;
	sp_line_destroy(&ug->osc);
	free(ug);
}

MFUN(line_get_a) {
	GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->a;
}

MFUN(line_set_a) {
	m_uint gw_offset = SZ_INT;
	GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
	m_float a = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->a = a);
}

MFUN(line_get_dur) {
	GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dur;
}

MFUN(line_set_dur) {
	m_uint gw_offset = SZ_INT;
	GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dur = dur);
}

MFUN(line_get_b) {
	GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->b;
}

MFUN(line_set_b) {
	m_uint gw_offset = SZ_INT;
	GW_line* ug = (GW_line*)UGEN(o)->module.gen.data;
	m_float b = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->b = b);
}

typedef struct {
	sp_data* sp;
	sp_lpc* osc;
	m_bool is_init;
} GW_lpc;

static TICK(lpc_tick) {
	const GW_lpc* ug = (GW_lpc*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_lpc_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(lpc_ctor) {
	GW_lpc* ug = (GW_lpc*)xcalloc(1, sizeof(GW_lpc));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), lpc_tick, ug, 0);
}

DTOR(lpc_dtor) {
	GW_lpc* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_lpc_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(lpc_init) {
	m_uint gw_offset = SZ_INT;
	GW_lpc* ug = (GW_lpc*)UGEN(o)->module.gen.data;
	if(ug->osc) {
		sp_lpc_destroy(&ug->osc);
		ug->osc = NULL;
	}
	m_int framesize = *(m_int*)(shred->mem + gw_offset);
	if(sp_lpc_create(&ug->osc) == SP_NOT_OK || sp_lpc_init(ug->sp, ug->osc, framesize) == SP_NOT_OK) {
		Except(shred, "UgenCreateException") // LCOV_EXCL_LINE
	}
	ug->is_init = 1;
}

typedef struct {
	sp_data* sp;
	sp_lpf18* osc;
} GW_lpf18;

static TICK(lpf18_tick) {
	const GW_lpf18* ug = (GW_lpf18*)u->module.gen.data;
	sp_lpf18_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(lpf18_ctor) {
	GW_lpf18* ug = (GW_lpf18*)xcalloc(1, sizeof(GW_lpf18));
	ug->sp = shred->vm_ref->sp;
	sp_lpf18_create(&ug->osc);
	sp_lpf18_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), lpf18_tick, ug, 0);
}

DTOR(lpf18_dtor) {
	GW_lpf18* ug = UGEN(o)->module.gen.data;
	sp_lpf18_destroy(&ug->osc);
	free(ug);
}

MFUN(lpf18_get_cutoff) {
	GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->cutoff;
}

MFUN(lpf18_set_cutoff) {
	m_uint gw_offset = SZ_INT;
	GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
	m_float cutoff = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->cutoff = cutoff);
}

MFUN(lpf18_get_res) {
	GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->res;
}

MFUN(lpf18_set_res) {
	m_uint gw_offset = SZ_INT;
	GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->res = res);
}

MFUN(lpf18_get_dist) {
	GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dist;
}

MFUN(lpf18_set_dist) {
	m_uint gw_offset = SZ_INT;
	GW_lpf18* ug = (GW_lpf18*)UGEN(o)->module.gen.data;
	m_float dist = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dist = dist);
}

typedef struct {
	sp_data* sp;
	sp_maygate* osc;
} GW_maygate;

static TICK(maygate_tick) {
	const GW_maygate* ug = (GW_maygate*)u->module.gen.data;
	sp_maygate_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(maygate_ctor) {
	GW_maygate* ug = (GW_maygate*)xcalloc(1, sizeof(GW_maygate));
	ug->sp = shred->vm_ref->sp;
	sp_maygate_create(&ug->osc);
	sp_maygate_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), maygate_tick, ug, 1);
}

DTOR(maygate_dtor) {
	GW_maygate* ug = UGEN(o)->module.gen.data;
	sp_maygate_destroy(&ug->osc);
	free(ug);
}

MFUN(maygate_get_prob) {
	GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->prob;
}

MFUN(maygate_set_prob) {
	m_uint gw_offset = SZ_INT;
	GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
	m_float prob = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->prob = prob);
}

MFUN(maygate_get_mode) {
	GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->mode;
}

MFUN(maygate_set_mode) {
	m_uint gw_offset = SZ_INT;
	GW_maygate* ug = (GW_maygate*)UGEN(o)->module.gen.data;
	m_int mode = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
	sp_data* sp;
	sp_metro* osc;
} GW_metro;

static TICK(metro_tick) {
	const GW_metro* ug = (GW_metro*)u->module.gen.data;
	sp_metro_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(metro_ctor) {
	GW_metro* ug = (GW_metro*)xcalloc(1, sizeof(GW_metro));
	ug->sp = shred->vm_ref->sp;
	sp_metro_create(&ug->osc);
	sp_metro_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), metro_tick, ug, 0);
}

DTOR(metro_dtor) {
	GW_metro* ug = UGEN(o)->module.gen.data;
	sp_metro_destroy(&ug->osc);
	free(ug);
}

MFUN(metro_get_freq) {
	GW_metro* ug = (GW_metro*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(metro_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_metro* ug = (GW_metro*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
	sp_data* sp;
	sp_mincer* osc;
	m_bool is_init;
} GW_mincer;

static TICK(mincer_tick) {
	const GW_mincer* ug = (GW_mincer*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_mincer_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(mincer_ctor) {
	GW_mincer* ug = (GW_mincer*)xcalloc(1, sizeof(GW_mincer));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), mincer_tick, ug, 0);
}

DTOR(mincer_dtor) {
	GW_mincer* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_mincer_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(mincer_init) {
	m_uint gw_offset = SZ_INT;
	GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
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

MFUN(mincer_get_time) {
	GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->time;
}

MFUN(mincer_set_time) {
	m_uint gw_offset = SZ_INT;
	GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
	m_float time = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->time = time);
}

MFUN(mincer_get_amp) {
	GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(mincer_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(mincer_get_pitch) {
	GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->pitch;
}

MFUN(mincer_set_pitch) {
	m_uint gw_offset = SZ_INT;
	GW_mincer* ug = (GW_mincer*)UGEN(o)->module.gen.data;
	m_float pitch = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->pitch = pitch);
}

typedef struct {
	sp_data* sp;
	sp_mode* osc;
} GW_mode;

static TICK(mode_tick) {
	const GW_mode* ug = (GW_mode*)u->module.gen.data;
	sp_mode_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(mode_ctor) {
	GW_mode* ug = (GW_mode*)xcalloc(1, sizeof(GW_mode));
	ug->sp = shred->vm_ref->sp;
	sp_mode_create(&ug->osc);
	sp_mode_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), mode_tick, ug, 0);
}

DTOR(mode_dtor) {
	GW_mode* ug = UGEN(o)->module.gen.data;
	sp_mode_destroy(&ug->osc);
	free(ug);
}

MFUN(mode_get_freq) {
	GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(mode_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(mode_get_q) {
	GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->q;
}

MFUN(mode_set_q) {
	m_uint gw_offset = SZ_INT;
	GW_mode* ug = (GW_mode*)UGEN(o)->module.gen.data;
	m_float q = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->q = q);
}

typedef struct {
	sp_data* sp;
	sp_moogladder* osc;
} GW_moogladder;

static TICK(moogladder_tick) {
	const GW_moogladder* ug = (GW_moogladder*)u->module.gen.data;
	sp_moogladder_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(moogladder_ctor) {
	GW_moogladder* ug = (GW_moogladder*)xcalloc(1, sizeof(GW_moogladder));
	ug->sp = shred->vm_ref->sp;
	sp_moogladder_create(&ug->osc);
	sp_moogladder_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), moogladder_tick, ug, 0);
}

DTOR(moogladder_dtor) {
	GW_moogladder* ug = UGEN(o)->module.gen.data;
	sp_moogladder_destroy(&ug->osc);
	free(ug);
}

MFUN(moogladder_get_freq) {
	GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(moogladder_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(moogladder_get_res) {
	GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->res;
}

MFUN(moogladder_set_res) {
	m_uint gw_offset = SZ_INT;
	GW_moogladder* ug = (GW_moogladder*)UGEN(o)->module.gen.data;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->res = res);
}

typedef struct {
	sp_data* sp;
	sp_noise* osc;
} GW_noise;

static TICK(noise_tick) {
	const GW_noise* ug = (GW_noise*)u->module.gen.data;
	sp_noise_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(noise_ctor) {
	GW_noise* ug = (GW_noise*)xcalloc(1, sizeof(GW_noise));
	ug->sp = shred->vm_ref->sp;
	sp_noise_create(&ug->osc);
	sp_noise_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), noise_tick, ug, 0);
}

DTOR(noise_dtor) {
	GW_noise* ug = UGEN(o)->module.gen.data;
	sp_noise_destroy(&ug->osc);
	free(ug);
}

MFUN(noise_get_amp) {
	GW_noise* ug = (GW_noise*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(noise_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_noise* ug = (GW_noise*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
	sp_data* sp;
	sp_nsmp* osc;
	m_bool is_init;
} GW_nsmp;

static TICK(nsmp_tick) {
	const GW_nsmp* ug = (GW_nsmp*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_nsmp_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(nsmp_ctor) {
	GW_nsmp* ug = (GW_nsmp*)xcalloc(1, sizeof(GW_nsmp));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), nsmp_tick, ug, 1);
}

DTOR(nsmp_dtor) {
	GW_nsmp* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_nsmp_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(nsmp_init) {
	m_uint gw_offset = SZ_INT;
	GW_nsmp* ug = (GW_nsmp*)UGEN(o)->module.gen.data;
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

MFUN(nsmp_get_index) {
	GW_nsmp* ug = (GW_nsmp*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->index;
}

MFUN(nsmp_set_index) {
	m_uint gw_offset = SZ_INT;
	GW_nsmp* ug = (GW_nsmp*)UGEN(o)->module.gen.data;
	m_int index = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->index = index);
}

typedef struct {
	sp_data* sp;
	sp_osc* osc;
	m_bool is_init;
} GW_osc;

static TICK(osc_tick) {
	const GW_osc* ug = (GW_osc*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_osc_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(osc_ctor) {
	GW_osc* ug = (GW_osc*)xcalloc(1, sizeof(GW_osc));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), osc_tick, ug, 0);
}

DTOR(osc_dtor) {
	GW_osc* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_osc_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(osc_init) {
	m_uint gw_offset = SZ_INT;
	GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
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

MFUN(osc_get_freq) {
	GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(osc_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(osc_get_amp) {
	GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(osc_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_osc* ug = (GW_osc*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
	sp_data* sp;
	sp_oscmorph* osc;
	m_bool is_init;
	sp_ftbl** tbl;

} GW_oscmorph;

static TICK(oscmorph_tick) {
	const GW_oscmorph* ug = (GW_oscmorph*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_oscmorph_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(oscmorph_ctor) {
	GW_oscmorph* ug = (GW_oscmorph*)xcalloc(1, sizeof(GW_oscmorph));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), oscmorph_tick, ug, 0);
}

DTOR(oscmorph_dtor) {
	GW_oscmorph* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		free(ug->osc->tbl);

		sp_oscmorph_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(oscmorph_init) {
	m_uint gw_offset = SZ_INT;
	GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
	if(ug->osc) {
		sp_oscmorph_destroy(&ug->osc);
		free(ug->tbl);
		ug->osc = NULL;
	}
	M_Object tbl_ptr = *(M_Object*)(shred->mem + gw_offset);
	m_uint tbl_iter;
	sp_ftbl** tbl = (sp_ftbl**)xmalloc(m_vector_size(ARRAY(tbl_ptr)) * SZ_INT);
	for(tbl_iter = 0; tbl_iter < m_vector_size(ARRAY(tbl_ptr)); tbl_iter++) {
			M_Object tbl_ftl_obj;
			m_vector_get(ARRAY(tbl_ptr), tbl_iter, &tbl_ftl_obj);
			tbl[tbl_iter] = FTBL(tbl_ftl_obj);
	}
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

MFUN(oscmorph_get_freq) {
	GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(oscmorph_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(oscmorph_get_amp) {
	GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(oscmorph_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

MFUN(oscmorph_get_wtpos) {
	GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->wtpos;
}

MFUN(oscmorph_set_wtpos) {
	m_uint gw_offset = SZ_INT;
	GW_oscmorph* ug = (GW_oscmorph*)UGEN(o)->module.gen.data;
	m_float wtpos = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->wtpos = wtpos);
}

typedef struct {
	sp_data* sp;
	sp_pan2* osc;
} GW_pan2;

static TICK(pan2_tick) {
	const GW_pan2* ug = (GW_pan2*)u->module.gen.data;
	sp_pan2_compute(ug->sp, ug->osc, &u->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

CTOR(pan2_ctor) {
	GW_pan2* ug = (GW_pan2*)xcalloc(1, sizeof(GW_pan2));
	ug->sp = shred->vm_ref->sp;
	sp_pan2_create(&ug->osc);
	sp_pan2_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 2);
	ugen_gen(UGEN(o), pan2_tick, ug, 0);
}

DTOR(pan2_dtor) {
	GW_pan2* ug = UGEN(o)->module.gen.data;
	sp_pan2_destroy(&ug->osc);
	free(ug);
}

MFUN(pan2_get_type) {
	GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->type;
}

MFUN(pan2_set_type) {
	m_uint gw_offset = SZ_INT;
	GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
	m_int type = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->type = type);
}

MFUN(pan2_get_pan) {
	GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->pan;
}

MFUN(pan2_set_pan) {
	m_uint gw_offset = SZ_INT;
	GW_pan2* ug = (GW_pan2*)UGEN(o)->module.gen.data;
	m_float pan = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->pan = pan);
}

typedef struct {
	sp_data* sp;
	sp_panst* osc;
} GW_panst;

static TICK(panst_tick) {
	const GW_panst* ug = (GW_panst*)u->module.gen.data;
	sp_panst_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

CTOR(panst_ctor) {
	GW_panst* ug = (GW_panst*)xcalloc(1, sizeof(GW_panst));
	ug->sp = shred->vm_ref->sp;
	sp_panst_create(&ug->osc);
	sp_panst_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 2);
	ugen_gen(UGEN(o), panst_tick, ug, 0);
}

DTOR(panst_dtor) {
	GW_panst* ug = UGEN(o)->module.gen.data;
	sp_panst_destroy(&ug->osc);
	free(ug);
}

MFUN(panst_get_type) {
	GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->type;
}

MFUN(panst_set_type) {
	m_uint gw_offset = SZ_INT;
	GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
	m_int type = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->type = type);
}

MFUN(panst_get_pan) {
	GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->pan;
}

MFUN(panst_set_pan) {
	m_uint gw_offset = SZ_INT;
	GW_panst* ug = (GW_panst*)UGEN(o)->module.gen.data;
	m_float pan = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->pan = pan);
}

typedef struct {
	sp_data* sp;
	sp_pareq* osc;
} GW_pareq;

static TICK(pareq_tick) {
	const GW_pareq* ug = (GW_pareq*)u->module.gen.data;
	sp_pareq_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(pareq_ctor) {
	GW_pareq* ug = (GW_pareq*)xcalloc(1, sizeof(GW_pareq));
	ug->sp = shred->vm_ref->sp;
	sp_pareq_create(&ug->osc);
	sp_pareq_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), pareq_tick, ug, 0);
}

DTOR(pareq_dtor) {
	GW_pareq* ug = UGEN(o)->module.gen.data;
	sp_pareq_destroy(&ug->osc);
	free(ug);
}

MFUN(pareq_get_fc) {
	GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->fc;
}

MFUN(pareq_set_fc) {
	m_uint gw_offset = SZ_INT;
	GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
	m_float fc = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->fc = fc);
}

MFUN(pareq_get_v) {
	GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->v;
}

MFUN(pareq_set_v) {
	m_uint gw_offset = SZ_INT;
	GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
	m_float v = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->v = v);
}

MFUN(pareq_get_q) {
	GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->q;
}

MFUN(pareq_set_q) {
	m_uint gw_offset = SZ_INT;
	GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
	m_float q = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->q = q);
}

MFUN(pareq_get_mode) {
	GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->mode;
}

MFUN(pareq_set_mode) {
	m_uint gw_offset = SZ_INT;
	GW_pareq* ug = (GW_pareq*)UGEN(o)->module.gen.data;
	m_float mode = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
	sp_data* sp;
	sp_paulstretch* osc;
	m_bool is_init;
} GW_paulstretch;

static TICK(paulstretch_tick) {
	const GW_paulstretch* ug = (GW_paulstretch*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_paulstretch_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(paulstretch_ctor) {
	GW_paulstretch* ug = (GW_paulstretch*)xcalloc(1, sizeof(GW_paulstretch));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), paulstretch_tick, ug, 0);
}

DTOR(paulstretch_dtor) {
	GW_paulstretch* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_paulstretch_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(paulstretch_init) {
	m_uint gw_offset = SZ_INT;
	GW_paulstretch* ug = (GW_paulstretch*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_pdhalf* osc;
} GW_pdhalf;

static TICK(pdhalf_tick) {
	const GW_pdhalf* ug = (GW_pdhalf*)u->module.gen.data;
	sp_pdhalf_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(pdhalf_ctor) {
	GW_pdhalf* ug = (GW_pdhalf*)xcalloc(1, sizeof(GW_pdhalf));
	ug->sp = shred->vm_ref->sp;
	sp_pdhalf_create(&ug->osc);
	sp_pdhalf_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), pdhalf_tick, ug, 0);
}

DTOR(pdhalf_dtor) {
	GW_pdhalf* ug = UGEN(o)->module.gen.data;
	sp_pdhalf_destroy(&ug->osc);
	free(ug);
}

MFUN(pdhalf_get_amount) {
	GW_pdhalf* ug = (GW_pdhalf*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amount;
}

MFUN(pdhalf_set_amount) {
	m_uint gw_offset = SZ_INT;
	GW_pdhalf* ug = (GW_pdhalf*)UGEN(o)->module.gen.data;
	m_float amount = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amount = amount);
}

typedef struct {
	sp_data* sp;
	sp_peaklim* osc;
} GW_peaklim;

static TICK(peaklim_tick) {
	const GW_peaklim* ug = (GW_peaklim*)u->module.gen.data;
	sp_peaklim_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(peaklim_ctor) {
	GW_peaklim* ug = (GW_peaklim*)xcalloc(1, sizeof(GW_peaklim));
	ug->sp = shred->vm_ref->sp;
	sp_peaklim_create(&ug->osc);
	sp_peaklim_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), peaklim_tick, ug, 0);
}

DTOR(peaklim_dtor) {
	GW_peaklim* ug = UGEN(o)->module.gen.data;
	sp_peaklim_destroy(&ug->osc);
	free(ug);
}

MFUN(peaklim_get_atk) {
	GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->atk;
}

MFUN(peaklim_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->atk = atk);
}

MFUN(peaklim_get_rel) {
	GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->rel;
}

MFUN(peaklim_set_rel) {
	m_uint gw_offset = SZ_INT;
	GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->rel = rel);
}

MFUN(peaklim_get_thresh) {
	GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->thresh;
}

MFUN(peaklim_set_thresh) {
	m_uint gw_offset = SZ_INT;
	GW_peaklim* ug = (GW_peaklim*)UGEN(o)->module.gen.data;
	m_float thresh = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->thresh = thresh);
}

typedef struct {
	sp_data* sp;
	sp_phaser* osc;
} GW_phaser;

static TICK(phaser_tick) {
	const GW_phaser* ug = (GW_phaser*)u->module.gen.data;
	sp_phaser_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

CTOR(phaser_ctor) {
	GW_phaser* ug = (GW_phaser*)xcalloc(1, sizeof(GW_phaser));
	ug->sp = shred->vm_ref->sp;
	sp_phaser_create(&ug->osc);
	sp_phaser_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 2);
	ugen_gen(UGEN(o), phaser_tick, ug, 0);
}

DTOR(phaser_dtor) {
	GW_phaser* ug = UGEN(o)->module.gen.data;
	sp_phaser_destroy(&ug->osc);
	free(ug);
}

MFUN(phaser_get_MaxNotch1Freq) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->MaxNotch1Freq;
}

MFUN(phaser_set_MaxNotch1Freq) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float MaxNotch1Freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->MaxNotch1Freq = MaxNotch1Freq);
}

MFUN(phaser_get_MinNotch1Freq) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->MinNotch1Freq;
}

MFUN(phaser_set_MinNotch1Freq) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float MinNotch1Freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->MinNotch1Freq = MinNotch1Freq);
}

MFUN(phaser_get_Notch_width) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->Notch_width;
}

MFUN(phaser_set_Notch_width) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float Notch_width = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->Notch_width = Notch_width);
}

MFUN(phaser_get_NotchFreq) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->NotchFreq;
}

MFUN(phaser_set_NotchFreq) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float NotchFreq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->NotchFreq = NotchFreq);
}

MFUN(phaser_get_VibratoMode) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->VibratoMode;
}

MFUN(phaser_set_VibratoMode) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float VibratoMode = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->VibratoMode = VibratoMode);
}

MFUN(phaser_get_depth) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->depth;
}

MFUN(phaser_set_depth) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float depth = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->depth = depth);
}

MFUN(phaser_get_feedback_gain) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->feedback_gain;
}

MFUN(phaser_set_feedback_gain) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float feedback_gain = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->feedback_gain = feedback_gain);
}

MFUN(phaser_get_invert) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->invert;
}

MFUN(phaser_set_invert) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float invert = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->invert = invert);
}

MFUN(phaser_get_level) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->level;
}

MFUN(phaser_set_level) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float level = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->level = level);
}

MFUN(phaser_get_lfobpm) {
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->lfobpm;
}

MFUN(phaser_set_lfobpm) {
	m_uint gw_offset = SZ_INT;
	GW_phaser* ug = (GW_phaser*)UGEN(o)->module.gen.data;
	m_float lfobpm = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->lfobpm = lfobpm);
}

typedef struct {
	sp_data* sp;
	sp_phasor* osc;
	m_bool is_init;
} GW_phasor;

static TICK(phasor_tick) {
	const GW_phasor* ug = (GW_phasor*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_phasor_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(phasor_ctor) {
	GW_phasor* ug = (GW_phasor*)xcalloc(1, sizeof(GW_phasor));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), phasor_tick, ug, 0);
}

DTOR(phasor_dtor) {
	GW_phasor* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_phasor_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(phasor_init) {
	m_uint gw_offset = SZ_INT;
	GW_phasor* ug = (GW_phasor*)UGEN(o)->module.gen.data;
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

MFUN(phasor_get_freq) {
	GW_phasor* ug = (GW_phasor*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(phasor_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_phasor* ug = (GW_phasor*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
	sp_data* sp;
	sp_pinknoise* osc;
} GW_pinknoise;

static TICK(pinknoise_tick) {
	const GW_pinknoise* ug = (GW_pinknoise*)u->module.gen.data;
	sp_pinknoise_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(pinknoise_ctor) {
	GW_pinknoise* ug = (GW_pinknoise*)xcalloc(1, sizeof(GW_pinknoise));
	ug->sp = shred->vm_ref->sp;
	sp_pinknoise_create(&ug->osc);
	sp_pinknoise_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), pinknoise_tick, ug, 0);
}

DTOR(pinknoise_dtor) {
	GW_pinknoise* ug = UGEN(o)->module.gen.data;
	sp_pinknoise_destroy(&ug->osc);
	free(ug);
}

MFUN(pinknoise_get_amp) {
	GW_pinknoise* ug = (GW_pinknoise*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(pinknoise_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_pinknoise* ug = (GW_pinknoise*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
	sp_data* sp;
	sp_pitchamdf* osc;
	m_bool is_init;
} GW_pitchamdf;

static TICK(pitchamdf_tick) {
	const GW_pitchamdf* ug = (GW_pitchamdf*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_pitchamdf_compute(ug->sp, ug->osc, &u->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

CTOR(pitchamdf_ctor) {
	GW_pitchamdf* ug = (GW_pitchamdf*)xcalloc(1, sizeof(GW_pitchamdf));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 2);
	ugen_gen(UGEN(o), pitchamdf_tick, ug, 0);
}

DTOR(pitchamdf_dtor) {
	GW_pitchamdf* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_pitchamdf_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(pitchamdf_init) {
	m_uint gw_offset = SZ_INT;
	GW_pitchamdf* ug = (GW_pitchamdf*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_pluck* osc;
	m_bool is_init;
} GW_pluck;

static TICK(pluck_tick) {
	const GW_pluck* ug = (GW_pluck*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_pluck_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(pluck_ctor) {
	GW_pluck* ug = (GW_pluck*)xcalloc(1, sizeof(GW_pluck));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), pluck_tick, ug, 1);
}

DTOR(pluck_dtor) {
	GW_pluck* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_pluck_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(pluck_init) {
	m_uint gw_offset = SZ_INT;
	GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
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

MFUN(pluck_get_freq) {
	GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(pluck_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(pluck_get_amp) {
	GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(pluck_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_pluck* ug = (GW_pluck*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
	sp_data* sp;
	sp_port* osc;
	m_bool is_init;
} GW_port;

static TICK(port_tick) {
	const GW_port* ug = (GW_port*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_port_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(port_ctor) {
	GW_port* ug = (GW_port*)xcalloc(1, sizeof(GW_port));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), port_tick, ug, 0);
}

DTOR(port_dtor) {
	GW_port* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_port_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(port_init) {
	m_uint gw_offset = SZ_INT;
	GW_port* ug = (GW_port*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_posc3* osc;
	m_bool is_init;
} GW_posc3;

static TICK(posc3_tick) {
	const GW_posc3* ug = (GW_posc3*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_posc3_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(posc3_ctor) {
	GW_posc3* ug = (GW_posc3*)xcalloc(1, sizeof(GW_posc3));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), posc3_tick, ug, 0);
}

DTOR(posc3_dtor) {
	GW_posc3* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_posc3_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(posc3_init) {
	m_uint gw_offset = SZ_INT;
	GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
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

MFUN(posc3_get_freq) {
	GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(posc3_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(posc3_get_amp) {
	GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->amp;
}

MFUN(posc3_set_amp) {
	m_uint gw_offset = SZ_INT;
	GW_posc3* ug = (GW_posc3*)UGEN(o)->module.gen.data;
	m_float amp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->amp = amp);
}

typedef struct {
	sp_data* sp;
	sp_progress* osc;
} GW_progress;

static TICK(progress_tick) {
	const GW_progress* ug = (GW_progress*)u->module.gen.data;
	sp_progress_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(progress_ctor) {
	GW_progress* ug = (GW_progress*)xcalloc(1, sizeof(GW_progress));
	ug->sp = shred->vm_ref->sp;
	sp_progress_create(&ug->osc);
	sp_progress_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), progress_tick, ug, 0);
}

DTOR(progress_dtor) {
	GW_progress* ug = UGEN(o)->module.gen.data;
	sp_progress_destroy(&ug->osc);
	free(ug);
}

MFUN(progress_get_nbars) {
	GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->nbars;
}

MFUN(progress_set_nbars) {
	m_uint gw_offset = SZ_INT;
	GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
	m_int nbars = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->nbars = nbars);
}

MFUN(progress_get_skip) {
	GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->skip;
}

MFUN(progress_set_skip) {
	m_uint gw_offset = SZ_INT;
	GW_progress* ug = (GW_progress*)UGEN(o)->module.gen.data;
	m_int skip = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->skip = skip);
}

typedef struct {
	sp_data* sp;
	sp_prop* osc;
	m_bool is_init;
} GW_prop;

static TICK(prop_tick) {
	const GW_prop* ug = (GW_prop*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_prop_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(prop_ctor) {
	GW_prop* ug = (GW_prop*)xcalloc(1, sizeof(GW_prop));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), prop_tick, ug, 0);
}

DTOR(prop_dtor) {
	GW_prop* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_prop_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(prop_init) {
	m_uint gw_offset = SZ_INT;
	GW_prop* ug = (GW_prop*)UGEN(o)->module.gen.data;
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

MFUN(prop_get_bpm) {
	GW_prop* ug = (GW_prop*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bpm;
}

MFUN(prop_set_bpm) {
	m_uint gw_offset = SZ_INT;
	GW_prop* ug = (GW_prop*)UGEN(o)->module.gen.data;
	m_float bpm = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bpm = bpm);
}

typedef struct {
	sp_data* sp;
	sp_pshift* osc;
} GW_pshift;

static TICK(pshift_tick) {
	const GW_pshift* ug = (GW_pshift*)u->module.gen.data;
	sp_pshift_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(pshift_ctor) {
	GW_pshift* ug = (GW_pshift*)xcalloc(1, sizeof(GW_pshift));
	ug->sp = shred->vm_ref->sp;
	sp_pshift_create(&ug->osc);
	sp_pshift_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), pshift_tick, ug, 0);
}

DTOR(pshift_dtor) {
	GW_pshift* ug = UGEN(o)->module.gen.data;
	sp_pshift_destroy(&ug->osc);
	free(ug);
}

MFUN(pshift_get_shift) {
	GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->shift;
}

MFUN(pshift_set_shift) {
	m_uint gw_offset = SZ_INT;
	GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
	m_float shift = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->shift = shift);
}

MFUN(pshift_get_window) {
	GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->window;
}

MFUN(pshift_set_window) {
	m_uint gw_offset = SZ_INT;
	GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
	m_float window = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->window = window);
}

MFUN(pshift_get_xfade) {
	GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->xfade;
}

MFUN(pshift_set_xfade) {
	m_uint gw_offset = SZ_INT;
	GW_pshift* ug = (GW_pshift*)UGEN(o)->module.gen.data;
	m_float xfade = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->xfade = xfade);
}

typedef struct {
	sp_data* sp;
	sp_ptrack* osc;
	m_bool is_init;
} GW_ptrack;

static TICK(ptrack_tick) {
	const GW_ptrack* ug = (GW_ptrack*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_ptrack_compute(ug->sp, ug->osc, &u->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

CTOR(ptrack_ctor) {
	GW_ptrack* ug = (GW_ptrack*)xcalloc(1, sizeof(GW_ptrack));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 2);
	ugen_gen(UGEN(o), ptrack_tick, ug, 0);
}

DTOR(ptrack_dtor) {
	GW_ptrack* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_ptrack_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(ptrack_init) {
	m_uint gw_offset = SZ_INT;
	GW_ptrack* ug = (GW_ptrack*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_randh* osc;
} GW_randh;

static TICK(randh_tick) {
	const GW_randh* ug = (GW_randh*)u->module.gen.data;
	sp_randh_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(randh_ctor) {
	GW_randh* ug = (GW_randh*)xcalloc(1, sizeof(GW_randh));
	ug->sp = shred->vm_ref->sp;
	sp_randh_create(&ug->osc);
	sp_randh_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), randh_tick, ug, 0);
}

DTOR(randh_dtor) {
	GW_randh* ug = UGEN(o)->module.gen.data;
	sp_randh_destroy(&ug->osc);
	free(ug);
}

MFUN(randh_get_min) {
	GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->min;
}

MFUN(randh_set_min) {
	m_uint gw_offset = SZ_INT;
	GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->min = min);
}

MFUN(randh_get_max) {
	GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->max;
}

MFUN(randh_set_max) {
	m_uint gw_offset = SZ_INT;
	GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->max = max);
}

MFUN(randh_get_freq) {
	GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(randh_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_randh* ug = (GW_randh*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

typedef struct {
	sp_data* sp;
	sp_randi* osc;
} GW_randi;

static TICK(randi_tick) {
	const GW_randi* ug = (GW_randi*)u->module.gen.data;
	sp_randi_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(randi_ctor) {
	GW_randi* ug = (GW_randi*)xcalloc(1, sizeof(GW_randi));
	ug->sp = shred->vm_ref->sp;
	sp_randi_create(&ug->osc);
	sp_randi_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), randi_tick, ug, 0);
}

DTOR(randi_dtor) {
	GW_randi* ug = UGEN(o)->module.gen.data;
	sp_randi_destroy(&ug->osc);
	free(ug);
}

MFUN(randi_get_min) {
	GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->min;
}

MFUN(randi_set_min) {
	m_uint gw_offset = SZ_INT;
	GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->min = min);
}

MFUN(randi_get_max) {
	GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->max;
}

MFUN(randi_set_max) {
	m_uint gw_offset = SZ_INT;
	GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->max = max);
}

MFUN(randi_get_cps) {
	GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->cps;
}

MFUN(randi_set_cps) {
	m_uint gw_offset = SZ_INT;
	GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
	m_float cps = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->cps = cps);
}

MFUN(randi_get_mode) {
	GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->mode;
}

MFUN(randi_set_mode) {
	m_uint gw_offset = SZ_INT;
	GW_randi* ug = (GW_randi*)UGEN(o)->module.gen.data;
	m_float mode = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
	sp_data* sp;
	sp_random* osc;
} GW_random;

static TICK(random_tick) {
	const GW_random* ug = (GW_random*)u->module.gen.data;
	sp_random_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(random_ctor) {
	GW_random* ug = (GW_random*)xcalloc(1, sizeof(GW_random));
	ug->sp = shred->vm_ref->sp;
	sp_random_create(&ug->osc);
	sp_random_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), random_tick, ug, 0);
}

DTOR(random_dtor) {
	GW_random* ug = UGEN(o)->module.gen.data;
	sp_random_destroy(&ug->osc);
	free(ug);
}

MFUN(random_get_min) {
	GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->min;
}

MFUN(random_set_min) {
	m_uint gw_offset = SZ_INT;
	GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->min = min);
}

MFUN(random_get_max) {
	GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->max;
}

MFUN(random_set_max) {
	m_uint gw_offset = SZ_INT;
	GW_random* ug = (GW_random*)UGEN(o)->module.gen.data;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
	sp_data* sp;
	sp_reson* osc;
} GW_reson;

static TICK(reson_tick) {
	const GW_reson* ug = (GW_reson*)u->module.gen.data;
	sp_reson_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(reson_ctor) {
	GW_reson* ug = (GW_reson*)xcalloc(1, sizeof(GW_reson));
	ug->sp = shred->vm_ref->sp;
	sp_reson_create(&ug->osc);
	sp_reson_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), reson_tick, ug, 0);
}

DTOR(reson_dtor) {
	GW_reson* ug = UGEN(o)->module.gen.data;
	sp_reson_destroy(&ug->osc);
	free(ug);
}

MFUN(reson_get_freq) {
	GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(reson_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(reson_get_bw) {
	GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->bw;
}

MFUN(reson_set_bw) {
	m_uint gw_offset = SZ_INT;
	GW_reson* ug = (GW_reson*)UGEN(o)->module.gen.data;
	m_float bw = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->bw = bw);
}

typedef struct {
	sp_data* sp;
	sp_reverse* osc;
	m_bool is_init;
} GW_reverse;

static TICK(reverse_tick) {
	const GW_reverse* ug = (GW_reverse*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_reverse_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(reverse_ctor) {
	GW_reverse* ug = (GW_reverse*)xcalloc(1, sizeof(GW_reverse));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), reverse_tick, ug, 0);
}

DTOR(reverse_dtor) {
	GW_reverse* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_reverse_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(reverse_init) {
	m_uint gw_offset = SZ_INT;
	GW_reverse* ug = (GW_reverse*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_revsc* osc;
} GW_revsc;

static TICK(revsc_tick) {
	const GW_revsc* ug = (GW_revsc*)u->module.gen.data;
	sp_revsc_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

CTOR(revsc_ctor) {
	GW_revsc* ug = (GW_revsc*)xcalloc(1, sizeof(GW_revsc));
	ug->sp = shred->vm_ref->sp;
	sp_revsc_create(&ug->osc);
	sp_revsc_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 2);
	ugen_gen(UGEN(o), revsc_tick, ug, 0);
}

DTOR(revsc_dtor) {
	GW_revsc* ug = UGEN(o)->module.gen.data;
	sp_revsc_destroy(&ug->osc);
	free(ug);
}

MFUN(revsc_get_feedback) {
	GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->feedback;
}

MFUN(revsc_set_feedback) {
	m_uint gw_offset = SZ_INT;
	GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
	m_float feedback = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->feedback = feedback);
}

MFUN(revsc_get_lpfreq) {
	GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->lpfreq;
}

MFUN(revsc_set_lpfreq) {
	m_uint gw_offset = SZ_INT;
	GW_revsc* ug = (GW_revsc*)UGEN(o)->module.gen.data;
	m_float lpfreq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->lpfreq = lpfreq);
}

typedef struct {
	sp_data* sp;
	sp_rms* osc;
} GW_rms;

static TICK(rms_tick) {
	const GW_rms* ug = (GW_rms*)u->module.gen.data;
	sp_rms_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(rms_ctor) {
	GW_rms* ug = (GW_rms*)xcalloc(1, sizeof(GW_rms));
	ug->sp = shred->vm_ref->sp;
	sp_rms_create(&ug->osc);
	sp_rms_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), rms_tick, ug, 0);
}

DTOR(rms_dtor) {
	GW_rms* ug = UGEN(o)->module.gen.data;
	sp_rms_destroy(&ug->osc);
	free(ug);
}

MFUN(rms_get_ihp) {
	GW_rms* ug = (GW_rms*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->ihp;
}

MFUN(rms_set_ihp) {
	m_uint gw_offset = SZ_INT;
	GW_rms* ug = (GW_rms*)UGEN(o)->module.gen.data;
	m_float ihp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->ihp = ihp);
}

typedef struct {
	sp_data* sp;
	sp_rpt* osc;
	m_bool is_init;
} GW_rpt;

static TICK(rpt_tick) {
	const GW_rpt* ug = (GW_rpt*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_rpt_compute(ug->sp, ug->osc, &u->in, &u->module.gen.trig->in, &u->out);

}

CTOR(rpt_ctor) {
	GW_rpt* ug = (GW_rpt*)xcalloc(1, sizeof(GW_rpt));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 2, 1);
	ugen_gen(UGEN(o), rpt_tick, ug, 1);
}

DTOR(rpt_dtor) {
	GW_rpt* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_rpt_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(rpt_init) {
	m_uint gw_offset = SZ_INT;
	GW_rpt* ug = (GW_rpt*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_rspline* osc;
} GW_rspline;

static TICK(rspline_tick) {
	const GW_rspline* ug = (GW_rspline*)u->module.gen.data;
	sp_rspline_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(rspline_ctor) {
	GW_rspline* ug = (GW_rspline*)xcalloc(1, sizeof(GW_rspline));
	ug->sp = shred->vm_ref->sp;
	sp_rspline_create(&ug->osc);
	sp_rspline_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), rspline_tick, ug, 0);
}

DTOR(rspline_dtor) {
	GW_rspline* ug = UGEN(o)->module.gen.data;
	sp_rspline_destroy(&ug->osc);
	free(ug);
}

MFUN(rspline_get_min) {
	GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->min;
}

MFUN(rspline_set_min) {
	m_uint gw_offset = SZ_INT;
	GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->min = min);
}

MFUN(rspline_get_max) {
	GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->max;
}

MFUN(rspline_set_max) {
	m_uint gw_offset = SZ_INT;
	GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->max = max);
}

MFUN(rspline_get_cps_min) {
	GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->cps_min;
}

MFUN(rspline_set_cps_min) {
	m_uint gw_offset = SZ_INT;
	GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
	m_float cps_min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->cps_min = cps_min);
}

MFUN(rspline_get_cps_max) {
	GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->cps_max;
}

MFUN(rspline_set_cps_max) {
	m_uint gw_offset = SZ_INT;
	GW_rspline* ug = (GW_rspline*)UGEN(o)->module.gen.data;
	m_float cps_max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->cps_max = cps_max);
}

typedef struct {
	sp_data* sp;
	sp_samphold* osc;
} GW_samphold;

static TICK(samphold_tick) {
	const GW_samphold* ug = (GW_samphold*)u->module.gen.data;
	sp_samphold_compute(ug->sp, ug->osc, &u->in, &u->module.gen.trig->in, &u->out);

}

CTOR(samphold_ctor) {
	GW_samphold* ug = (GW_samphold*)xcalloc(1, sizeof(GW_samphold));
	ug->sp = shred->vm_ref->sp;
	sp_samphold_create(&ug->osc);
	sp_samphold_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 1);
	ugen_gen(UGEN(o), samphold_tick, ug, 1);
}

DTOR(samphold_dtor) {
	GW_samphold* ug = UGEN(o)->module.gen.data;
	sp_samphold_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_saturator* osc;
} GW_saturator;

static TICK(saturator_tick) {
	const GW_saturator* ug = (GW_saturator*)u->module.gen.data;
	sp_saturator_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(saturator_ctor) {
	GW_saturator* ug = (GW_saturator*)xcalloc(1, sizeof(GW_saturator));
	ug->sp = shred->vm_ref->sp;
	sp_saturator_create(&ug->osc);
	sp_saturator_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), saturator_tick, ug, 0);
}

DTOR(saturator_dtor) {
	GW_saturator* ug = UGEN(o)->module.gen.data;
	sp_saturator_destroy(&ug->osc);
	free(ug);
}

MFUN(saturator_get_drive) {
	GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->drive;
}

MFUN(saturator_set_drive) {
	m_uint gw_offset = SZ_INT;
	GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
	m_float drive = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->drive = drive);
}

MFUN(saturator_get_dcoffset) {
	GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dcoffset;
}

MFUN(saturator_set_dcoffset) {
	m_uint gw_offset = SZ_INT;
	GW_saturator* ug = (GW_saturator*)UGEN(o)->module.gen.data;
	m_float dcoffset = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dcoffset = dcoffset);
}

typedef struct {
	sp_data* sp;
	sp_scale* osc;
} GW_scale;

static TICK(scale_tick) {
	const GW_scale* ug = (GW_scale*)u->module.gen.data;
	sp_scale_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(scale_ctor) {
	GW_scale* ug = (GW_scale*)xcalloc(1, sizeof(GW_scale));
	ug->sp = shred->vm_ref->sp;
	sp_scale_create(&ug->osc);
	sp_scale_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), scale_tick, ug, 0);
}

DTOR(scale_dtor) {
	GW_scale* ug = UGEN(o)->module.gen.data;
	sp_scale_destroy(&ug->osc);
	free(ug);
}

MFUN(scale_get_min) {
	GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->min;
}

MFUN(scale_set_min) {
	m_uint gw_offset = SZ_INT;
	GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->min = min);
}

MFUN(scale_get_max) {
	GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->max;
}

MFUN(scale_set_max) {
	m_uint gw_offset = SZ_INT;
	GW_scale* ug = (GW_scale*)UGEN(o)->module.gen.data;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
	sp_data* sp;
	sp_sdelay* osc;
	m_bool is_init;
} GW_sdelay;

static TICK(sdelay_tick) {
	const GW_sdelay* ug = (GW_sdelay*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_sdelay_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(sdelay_ctor) {
	GW_sdelay* ug = (GW_sdelay*)xcalloc(1, sizeof(GW_sdelay));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), sdelay_tick, ug, 0);
}

DTOR(sdelay_dtor) {
	GW_sdelay* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_sdelay_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(sdelay_init) {
	m_uint gw_offset = SZ_INT;
	GW_sdelay* ug = (GW_sdelay*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_slice* osc;
	m_bool is_init;
} GW_slice;

static TICK(slice_tick) {
	const GW_slice* ug = (GW_slice*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_slice_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(slice_ctor) {
	GW_slice* ug = (GW_slice*)xcalloc(1, sizeof(GW_slice));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), slice_tick, ug, 1);
}

DTOR(slice_dtor) {
	GW_slice* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_slice_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(slice_init) {
	m_uint gw_offset = SZ_INT;
	GW_slice* ug = (GW_slice*)UGEN(o)->module.gen.data;
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

MFUN(slice_get_id) {
	GW_slice* ug = (GW_slice*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->id;
}

MFUN(slice_set_id) {
	m_uint gw_offset = SZ_INT;
	GW_slice* ug = (GW_slice*)UGEN(o)->module.gen.data;
	m_float id = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->id = id);
}

typedef struct {
	sp_data* sp;
	sp_smoothdelay* osc;
	m_bool is_init;
} GW_smoothdelay;

static TICK(smoothdelay_tick) {
	const GW_smoothdelay* ug = (GW_smoothdelay*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_smoothdelay_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(smoothdelay_ctor) {
	GW_smoothdelay* ug = (GW_smoothdelay*)xcalloc(1, sizeof(GW_smoothdelay));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), smoothdelay_tick, ug, 0);
}

DTOR(smoothdelay_dtor) {
	GW_smoothdelay* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_smoothdelay_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(smoothdelay_init) {
	m_uint gw_offset = SZ_INT;
	GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
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

MFUN(smoothdelay_get_feedback) {
	GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->feedback;
}

MFUN(smoothdelay_set_feedback) {
	m_uint gw_offset = SZ_INT;
	GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
	m_float feedback = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->feedback = feedback);
}

MFUN(smoothdelay_get_del) {
	GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->del;
}

MFUN(smoothdelay_set_del) {
	m_uint gw_offset = SZ_INT;
	GW_smoothdelay* ug = (GW_smoothdelay*)UGEN(o)->module.gen.data;
	m_float del = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->del = del);
}

typedef struct {
	sp_data* sp;
	sp_spa* osc;
	m_bool is_init;
} GW_spa;

static TICK(spa_tick) {
	const GW_spa* ug = (GW_spa*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_spa_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(spa_ctor) {
	GW_spa* ug = (GW_spa*)xcalloc(1, sizeof(GW_spa));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), spa_tick, ug, 0);
}

DTOR(spa_dtor) {
	GW_spa* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_spa_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(spa_init) {
	m_uint gw_offset = SZ_INT;
	GW_spa* ug = (GW_spa*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_sparec* osc;
	m_bool is_init;
} GW_sparec;

static TICK(sparec_tick) {
	const GW_sparec* ug = (GW_sparec*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_sparec_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(sparec_ctor) {
	GW_sparec* ug = (GW_sparec*)xcalloc(1, sizeof(GW_sparec));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), sparec_tick, ug, 0);
}

DTOR(sparec_dtor) {
	GW_sparec* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_sparec_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(sparec_init) {
	m_uint gw_offset = SZ_INT;
	GW_sparec* ug = (GW_sparec*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_streson* osc;
} GW_streson;

static TICK(streson_tick) {
	const GW_streson* ug = (GW_streson*)u->module.gen.data;
	sp_streson_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(streson_ctor) {
	GW_streson* ug = (GW_streson*)xcalloc(1, sizeof(GW_streson));
	ug->sp = shred->vm_ref->sp;
	sp_streson_create(&ug->osc);
	sp_streson_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), streson_tick, ug, 0);
}

DTOR(streson_dtor) {
	GW_streson* ug = UGEN(o)->module.gen.data;
	sp_streson_destroy(&ug->osc);
	free(ug);
}

MFUN(streson_get_freq) {
	GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->freq;
}

MFUN(streson_set_freq) {
	m_uint gw_offset = SZ_INT;
	GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
	m_float freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->freq = freq);
}

MFUN(streson_get_fdbgain) {
	GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->fdbgain;
}

MFUN(streson_set_fdbgain) {
	m_uint gw_offset = SZ_INT;
	GW_streson* ug = (GW_streson*)UGEN(o)->module.gen.data;
	m_float fdbgain = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->fdbgain = fdbgain);
}

typedef struct {
	sp_data* sp;
	sp_switch* osc;
} GW_switch;

static TICK(switch_tick) {
	const GW_switch* ug = (GW_switch*)u->module.gen.data;
	sp_switch_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->module.gen.trig->in, &u->out);

}

CTOR(switch_ctor) {
	GW_switch* ug = (GW_switch*)xcalloc(1, sizeof(GW_switch));
	ug->sp = shred->vm_ref->sp;
	sp_switch_create(&ug->osc);
	sp_switch_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 3, 1);
	ugen_gen(UGEN(o), switch_tick, ug, 1);
}

DTOR(switch_dtor) {
	GW_switch* ug = UGEN(o)->module.gen.data;
	sp_switch_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_tabread* osc;
	m_bool is_init;
} GW_tabread;

static TICK(tabread_tick) {
	const GW_tabread* ug = (GW_tabread*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_tabread_compute(ug->sp, ug->osc, NULL, &u->out);

}

CTOR(tabread_ctor) {
	GW_tabread* ug = (GW_tabread*)xcalloc(1, sizeof(GW_tabread));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), tabread_tick, ug, 0);
}

DTOR(tabread_dtor) {
	GW_tabread* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_tabread_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(tabread_init) {
	m_uint gw_offset = SZ_INT;
	GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
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

MFUN(tabread_get_index) {
	GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->index;
}

MFUN(tabread_set_index) {
	m_uint gw_offset = SZ_INT;
	GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
	m_float index = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->index = index);
}

MFUN(tabread_get_offset) {
	GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->offset;
}

MFUN(tabread_set_offset) {
	m_uint gw_offset = SZ_INT;
	GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
	m_float offset = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->offset = offset);
}

MFUN(tabread_get_wrap) {
	GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->wrap;
}

MFUN(tabread_set_wrap) {
	m_uint gw_offset = SZ_INT;
	GW_tabread* ug = (GW_tabread*)UGEN(o)->module.gen.data;
	m_float wrap = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->wrap = wrap);
}

typedef struct {
	sp_data* sp;
	sp_tadsr* osc;
} GW_tadsr;

static TICK(tadsr_tick) {
	const GW_tadsr* ug = (GW_tadsr*)u->module.gen.data;
	sp_tadsr_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tadsr_ctor) {
	GW_tadsr* ug = (GW_tadsr*)xcalloc(1, sizeof(GW_tadsr));
	ug->sp = shred->vm_ref->sp;
	sp_tadsr_create(&ug->osc);
	sp_tadsr_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tadsr_tick, ug, 1);
}

DTOR(tadsr_dtor) {
	GW_tadsr* ug = UGEN(o)->module.gen.data;
	sp_tadsr_destroy(&ug->osc);
	free(ug);
}

MFUN(tadsr_get_atk) {
	GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->atk;
}

MFUN(tadsr_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->atk = atk);
}

MFUN(tadsr_get_dec) {
	GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dec;
}

MFUN(tadsr_set_dec) {
	m_uint gw_offset = SZ_INT;
	GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
	m_float dec = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dec = dec);
}

MFUN(tadsr_get_sus) {
	GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->sus;
}

MFUN(tadsr_set_sus) {
	m_uint gw_offset = SZ_INT;
	GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
	m_float sus = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->sus = sus);
}

MFUN(tadsr_get_rel) {
	GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->rel;
}

MFUN(tadsr_set_rel) {
	m_uint gw_offset = SZ_INT;
	GW_tadsr* ug = (GW_tadsr*)UGEN(o)->module.gen.data;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
	sp_data* sp;
	sp_talkbox* osc;
} GW_talkbox;

static TICK(talkbox_tick) {
	const GW_talkbox* ug = (GW_talkbox*)u->module.gen.data;
	sp_talkbox_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);

}

CTOR(talkbox_ctor) {
	GW_talkbox* ug = (GW_talkbox*)xcalloc(1, sizeof(GW_talkbox));
	ug->sp = shred->vm_ref->sp;
	sp_talkbox_create(&ug->osc);
	sp_talkbox_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 1);
	ugen_gen(UGEN(o), talkbox_tick, ug, 0);
}

DTOR(talkbox_dtor) {
	GW_talkbox* ug = UGEN(o)->module.gen.data;
	sp_talkbox_destroy(&ug->osc);
	free(ug);
}

MFUN(talkbox_get_quality) {
	GW_talkbox* ug = (GW_talkbox*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->quality;
}

MFUN(talkbox_set_quality) {
	m_uint gw_offset = SZ_INT;
	GW_talkbox* ug = (GW_talkbox*)UGEN(o)->module.gen.data;
	m_float quality = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->quality = quality);
}

typedef struct {
	sp_data* sp;
	sp_tblrec* osc;
	m_bool is_init;
} GW_tblrec;

static TICK(tblrec_tick) {
	const GW_tblrec* ug = (GW_tblrec*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_tblrec_compute(ug->sp, ug->osc, &u->in, &u->module.gen.trig->in, &u->out);

}

CTOR(tblrec_ctor) {
	GW_tblrec* ug = (GW_tblrec*)xcalloc(1, sizeof(GW_tblrec));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 2, 1);
	ugen_gen(UGEN(o), tblrec_tick, ug, 1);
}

DTOR(tblrec_dtor) {
	GW_tblrec* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_tblrec_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(tblrec_init) {
	m_uint gw_offset = SZ_INT;
	GW_tblrec* ug = (GW_tblrec*)UGEN(o)->module.gen.data;
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

typedef struct {
	sp_data* sp;
	sp_tbvcf* osc;
} GW_tbvcf;

static TICK(tbvcf_tick) {
	const GW_tbvcf* ug = (GW_tbvcf*)u->module.gen.data;
	sp_tbvcf_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(tbvcf_ctor) {
	GW_tbvcf* ug = (GW_tbvcf*)xcalloc(1, sizeof(GW_tbvcf));
	ug->sp = shred->vm_ref->sp;
	sp_tbvcf_create(&ug->osc);
	sp_tbvcf_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tbvcf_tick, ug, 0);
}

DTOR(tbvcf_dtor) {
	GW_tbvcf* ug = UGEN(o)->module.gen.data;
	sp_tbvcf_destroy(&ug->osc);
	free(ug);
}

MFUN(tbvcf_get_fco) {
	GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->fco;
}

MFUN(tbvcf_set_fco) {
	m_uint gw_offset = SZ_INT;
	GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
	m_float fco = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->fco = fco);
}

MFUN(tbvcf_get_res) {
	GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->res;
}

MFUN(tbvcf_set_res) {
	m_uint gw_offset = SZ_INT;
	GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->res = res);
}

MFUN(tbvcf_get_dist) {
	GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dist;
}

MFUN(tbvcf_set_dist) {
	m_uint gw_offset = SZ_INT;
	GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
	m_float dist = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dist = dist);
}

MFUN(tbvcf_get_asym) {
	GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->asym;
}

MFUN(tbvcf_set_asym) {
	m_uint gw_offset = SZ_INT;
	GW_tbvcf* ug = (GW_tbvcf*)UGEN(o)->module.gen.data;
	m_float asym = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->asym = asym);
}

typedef struct {
	sp_data* sp;
	sp_tdiv* osc;
} GW_tdiv;

static TICK(tdiv_tick) {
	const GW_tdiv* ug = (GW_tdiv*)u->module.gen.data;
	sp_tdiv_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tdiv_ctor) {
	GW_tdiv* ug = (GW_tdiv*)xcalloc(1, sizeof(GW_tdiv));
	ug->sp = shred->vm_ref->sp;
	sp_tdiv_create(&ug->osc);
	sp_tdiv_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tdiv_tick, ug, 1);
}

DTOR(tdiv_dtor) {
	GW_tdiv* ug = UGEN(o)->module.gen.data;
	sp_tdiv_destroy(&ug->osc);
	free(ug);
}

MFUN(tdiv_get_num) {
	GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->num;
}

MFUN(tdiv_set_num) {
	m_uint gw_offset = SZ_INT;
	GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
	m_float num = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->num = num);
}

MFUN(tdiv_get_offset) {
	GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->offset;
}

MFUN(tdiv_set_offset) {
	m_uint gw_offset = SZ_INT;
	GW_tdiv* ug = (GW_tdiv*)UGEN(o)->module.gen.data;
	m_float offset = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->offset = offset);
}

typedef struct {
	sp_data* sp;
	sp_tenv* osc;
} GW_tenv;

static TICK(tenv_tick) {
	const GW_tenv* ug = (GW_tenv*)u->module.gen.data;
	sp_tenv_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tenv_ctor) {
	GW_tenv* ug = (GW_tenv*)xcalloc(1, sizeof(GW_tenv));
	ug->sp = shred->vm_ref->sp;
	sp_tenv_create(&ug->osc);
	sp_tenv_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tenv_tick, ug, 1);
}

DTOR(tenv_dtor) {
	GW_tenv* ug = UGEN(o)->module.gen.data;
	sp_tenv_destroy(&ug->osc);
	free(ug);
}

MFUN(tenv_get_atk) {
	GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->atk;
}

MFUN(tenv_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->atk = atk);
}

MFUN(tenv_get_hold) {
	GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->hold;
}

MFUN(tenv_set_hold) {
	m_uint gw_offset = SZ_INT;
	GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
	m_float hold = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->hold = hold);
}

MFUN(tenv_get_rel) {
	GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->rel;
}

MFUN(tenv_set_rel) {
	m_uint gw_offset = SZ_INT;
	GW_tenv* ug = (GW_tenv*)UGEN(o)->module.gen.data;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
	sp_data* sp;
	sp_tenv2* osc;
} GW_tenv2;

static TICK(tenv2_tick) {
	const GW_tenv2* ug = (GW_tenv2*)u->module.gen.data;
	sp_tenv2_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tenv2_ctor) {
	GW_tenv2* ug = (GW_tenv2*)xcalloc(1, sizeof(GW_tenv2));
	ug->sp = shred->vm_ref->sp;
	sp_tenv2_create(&ug->osc);
	sp_tenv2_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tenv2_tick, ug, 1);
}

DTOR(tenv2_dtor) {
	GW_tenv2* ug = UGEN(o)->module.gen.data;
	sp_tenv2_destroy(&ug->osc);
	free(ug);
}

MFUN(tenv2_get_atk) {
	GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->atk;
}

MFUN(tenv2_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->atk = atk);
}

MFUN(tenv2_get_rel) {
	GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->rel;
}

MFUN(tenv2_set_rel) {
	m_uint gw_offset = SZ_INT;
	GW_tenv2* ug = (GW_tenv2*)UGEN(o)->module.gen.data;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
	sp_data* sp;
	sp_tenvx* osc;
} GW_tenvx;

static TICK(tenvx_tick) {
	const GW_tenvx* ug = (GW_tenvx*)u->module.gen.data;
	sp_tenvx_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tenvx_ctor) {
	GW_tenvx* ug = (GW_tenvx*)xcalloc(1, sizeof(GW_tenvx));
	ug->sp = shred->vm_ref->sp;
	sp_tenvx_create(&ug->osc);
	sp_tenvx_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tenvx_tick, ug, 1);
}

DTOR(tenvx_dtor) {
	GW_tenvx* ug = UGEN(o)->module.gen.data;
	sp_tenvx_destroy(&ug->osc);
	free(ug);
}

MFUN(tenvx_get_atk) {
	GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->atk;
}

MFUN(tenvx_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->atk = atk);
}

MFUN(tenvx_get_hold) {
	GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->hold;
}

MFUN(tenvx_set_hold) {
	m_uint gw_offset = SZ_INT;
	GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
	m_float hold = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->hold = hold);
}

MFUN(tenvx_get_rel) {
	GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->rel;
}

MFUN(tenvx_set_rel) {
	m_uint gw_offset = SZ_INT;
	GW_tenvx* ug = (GW_tenvx*)UGEN(o)->module.gen.data;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->rel = rel);
}

typedef struct {
	sp_data* sp;
	sp_tgate* osc;
} GW_tgate;

static TICK(tgate_tick) {
	const GW_tgate* ug = (GW_tgate*)u->module.gen.data;
	sp_tgate_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tgate_ctor) {
	GW_tgate* ug = (GW_tgate*)xcalloc(1, sizeof(GW_tgate));
	ug->sp = shred->vm_ref->sp;
	sp_tgate_create(&ug->osc);
	sp_tgate_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tgate_tick, ug, 1);
}

DTOR(tgate_dtor) {
	GW_tgate* ug = UGEN(o)->module.gen.data;
	sp_tgate_destroy(&ug->osc);
	free(ug);
}

MFUN(tgate_get_time) {
	GW_tgate* ug = (GW_tgate*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->time;
}

MFUN(tgate_set_time) {
	m_uint gw_offset = SZ_INT;
	GW_tgate* ug = (GW_tgate*)UGEN(o)->module.gen.data;
	m_float time = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->time = time);
}

typedef struct {
	sp_data* sp;
	sp_thresh* osc;
} GW_thresh;

static TICK(thresh_tick) {
	const GW_thresh* ug = (GW_thresh*)u->module.gen.data;
	sp_thresh_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(thresh_ctor) {
	GW_thresh* ug = (GW_thresh*)xcalloc(1, sizeof(GW_thresh));
	ug->sp = shred->vm_ref->sp;
	sp_thresh_create(&ug->osc);
	sp_thresh_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), thresh_tick, ug, 0);
}

DTOR(thresh_dtor) {
	GW_thresh* ug = UGEN(o)->module.gen.data;
	sp_thresh_destroy(&ug->osc);
	free(ug);
}

MFUN(thresh_get_thresh) {
	GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->thresh;
}

MFUN(thresh_set_thresh) {
	m_uint gw_offset = SZ_INT;
	GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
	m_float thresh = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->thresh = thresh);
}

MFUN(thresh_get_mode) {
	GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->mode;
}

MFUN(thresh_set_mode) {
	m_uint gw_offset = SZ_INT;
	GW_thresh* ug = (GW_thresh*)UGEN(o)->module.gen.data;
	m_int mode = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->mode = mode);
}

typedef struct {
	sp_data* sp;
	sp_timer* osc;
} GW_timer;

static TICK(timer_tick) {
	const GW_timer* ug = (GW_timer*)u->module.gen.data;
	sp_timer_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(timer_ctor) {
	GW_timer* ug = (GW_timer*)xcalloc(1, sizeof(GW_timer));
	ug->sp = shred->vm_ref->sp;
	sp_timer_create(&ug->osc);
	sp_timer_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), timer_tick, ug, 0);
}

DTOR(timer_dtor) {
	GW_timer* ug = UGEN(o)->module.gen.data;
	sp_timer_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_tin* osc;
} GW_tin;

static TICK(tin_tick) {
	const GW_tin* ug = (GW_tin*)u->module.gen.data;
	sp_tin_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tin_ctor) {
	GW_tin* ug = (GW_tin*)xcalloc(1, sizeof(GW_tin));
	ug->sp = shred->vm_ref->sp;
	sp_tin_create(&ug->osc);
	sp_tin_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tin_tick, ug, 1);
}

DTOR(tin_dtor) {
	GW_tin* ug = UGEN(o)->module.gen.data;
	sp_tin_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_tone* osc;
} GW_tone;

static TICK(tone_tick) {
	const GW_tone* ug = (GW_tone*)u->module.gen.data;
	sp_tone_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(tone_ctor) {
	GW_tone* ug = (GW_tone*)xcalloc(1, sizeof(GW_tone));
	ug->sp = shred->vm_ref->sp;
	sp_tone_create(&ug->osc);
	sp_tone_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tone_tick, ug, 0);
}

DTOR(tone_dtor) {
	GW_tone* ug = UGEN(o)->module.gen.data;
	sp_tone_destroy(&ug->osc);
	free(ug);
}

MFUN(tone_get_hp) {
	GW_tone* ug = (GW_tone*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->hp;
}

MFUN(tone_set_hp) {
	m_uint gw_offset = SZ_INT;
	GW_tone* ug = (GW_tone*)UGEN(o)->module.gen.data;
	m_float hp = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->hp = hp);
}

typedef struct {
	sp_data* sp;
	sp_trand* osc;
} GW_trand;

static TICK(trand_tick) {
	const GW_trand* ug = (GW_trand*)u->module.gen.data;
	sp_trand_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(trand_ctor) {
	GW_trand* ug = (GW_trand*)xcalloc(1, sizeof(GW_trand));
	ug->sp = shred->vm_ref->sp;
	sp_trand_create(&ug->osc);
	sp_trand_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), trand_tick, ug, 1);
}

DTOR(trand_dtor) {
	GW_trand* ug = UGEN(o)->module.gen.data;
	sp_trand_destroy(&ug->osc);
	free(ug);
}

MFUN(trand_get_min) {
	GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->min;
}

MFUN(trand_set_min) {
	m_uint gw_offset = SZ_INT;
	GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
	m_float min = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->min = min);
}

MFUN(trand_get_max) {
	GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->max;
}

MFUN(trand_set_max) {
	m_uint gw_offset = SZ_INT;
	GW_trand* ug = (GW_trand*)UGEN(o)->module.gen.data;
	m_float max = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->max = max);
}

typedef struct {
	sp_data* sp;
	sp_tseg* osc;
	m_bool is_init;
} GW_tseg;

static TICK(tseg_tick) {
	const GW_tseg* ug = (GW_tseg*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_tseg_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tseg_ctor) {
	GW_tseg* ug = (GW_tseg*)xcalloc(1, sizeof(GW_tseg));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tseg_tick, ug, 1);
}

DTOR(tseg_dtor) {
	GW_tseg* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_tseg_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(tseg_init) {
	m_uint gw_offset = SZ_INT;
	GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
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

MFUN(tseg_get_end) {
	GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->end;
}

MFUN(tseg_set_end) {
	m_uint gw_offset = SZ_INT;
	GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
	m_float end = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->end = end);
}

MFUN(tseg_get_dur) {
	GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->dur;
}

MFUN(tseg_set_dur) {
	m_uint gw_offset = SZ_INT;
	GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
	m_float dur = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->dur = dur);
}

MFUN(tseg_get_type) {
	GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->type;
}

MFUN(tseg_set_type) {
	m_uint gw_offset = SZ_INT;
	GW_tseg* ug = (GW_tseg*)UGEN(o)->module.gen.data;
	m_float type = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->type = type);
}

typedef struct {
	sp_data* sp;
	sp_tseq* osc;
	m_bool is_init;
} GW_tseq;

static TICK(tseq_tick) {
	const GW_tseq* ug = (GW_tseq*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_tseq_compute(ug->sp, ug->osc, &u->module.gen.trig->in, &u->out);

}

CTOR(tseq_ctor) {
	GW_tseq* ug = (GW_tseq*)xcalloc(1, sizeof(GW_tseq));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), tseq_tick, ug, 1);
}

DTOR(tseq_dtor) {
	GW_tseq* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_tseq_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(tseq_init) {
	m_uint gw_offset = SZ_INT;
	GW_tseq* ug = (GW_tseq*)UGEN(o)->module.gen.data;
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

MFUN(tseq_get_shuf) {
	GW_tseq* ug = (GW_tseq*)UGEN(o)->module.gen.data;
	*(m_uint*)RETURN = ug->osc->shuf;
}

MFUN(tseq_set_shuf) {
	m_uint gw_offset = SZ_INT;
	GW_tseq* ug = (GW_tseq*)UGEN(o)->module.gen.data;
	m_int shuf = *(m_int*)(shred->mem + gw_offset);
	*(m_uint*)RETURN = (ug->osc->shuf = shuf);
}

typedef struct {
	sp_data* sp;
	sp_vdelay* osc;
	m_bool is_init;
} GW_vdelay;

static TICK(vdelay_tick) {
	const GW_vdelay* ug = (GW_vdelay*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_vdelay_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(vdelay_ctor) {
	GW_vdelay* ug = (GW_vdelay*)xcalloc(1, sizeof(GW_vdelay));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), vdelay_tick, ug, 0);
}

DTOR(vdelay_dtor) {
	GW_vdelay* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_vdelay_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(vdelay_init) {
	m_uint gw_offset = SZ_INT;
	GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
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

MFUN(vdelay_get_del) {
	GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->del;
}

MFUN(vdelay_set_del) {
	m_uint gw_offset = SZ_INT;
	GW_vdelay* ug = (GW_vdelay*)UGEN(o)->module.gen.data;
	m_float del = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->del = del);
}

typedef struct {
	sp_data* sp;
	sp_voc* osc;
} GW_voc;

static TICK(voc_tick) {
	const GW_voc* ug = (GW_voc*)u->module.gen.data;
	sp_voc_compute(ug->sp, ug->osc, &u->out);

}

CTOR(voc_ctor) {
	GW_voc* ug = (GW_voc*)xcalloc(1, sizeof(GW_voc));
	ug->sp = shred->vm_ref->sp;
	sp_voc_create(&ug->osc);
	sp_voc_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 0, 1);
	ugen_gen(UGEN(o), voc_tick, ug, 0);
}

DTOR(voc_dtor) {
	GW_voc* ug = UGEN(o)->module.gen.data;
	sp_voc_destroy(&ug->osc);
	free(ug);
}

typedef struct {
	sp_data* sp;
	sp_vocoder* osc;
} GW_vocoder;

static TICK(vocoder_tick) {
	const GW_vocoder* ug = (GW_vocoder*)u->module.gen.data;
	sp_vocoder_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &u->out);

}

CTOR(vocoder_ctor) {
	GW_vocoder* ug = (GW_vocoder*)xcalloc(1, sizeof(GW_vocoder));
	ug->sp = shred->vm_ref->sp;
	sp_vocoder_create(&ug->osc);
	sp_vocoder_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 1);
	ugen_gen(UGEN(o), vocoder_tick, ug, 0);
}

DTOR(vocoder_dtor) {
	GW_vocoder* ug = UGEN(o)->module.gen.data;
	sp_vocoder_destroy(&ug->osc);
	free(ug);
}

MFUN(vocoder_get_atk) {
	GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->atk;
}

MFUN(vocoder_set_atk) {
	m_uint gw_offset = SZ_INT;
	GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
	m_float atk = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->atk = atk);
}

MFUN(vocoder_get_rel) {
	GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->rel;
}

MFUN(vocoder_set_rel) {
	m_uint gw_offset = SZ_INT;
	GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
	m_float rel = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->rel = rel);
}

MFUN(vocoder_get_bwratio) {
	GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->bwratio;
}

MFUN(vocoder_set_bwratio) {
	m_uint gw_offset = SZ_INT;
	GW_vocoder* ug = (GW_vocoder*)UGEN(o)->module.gen.data;
	m_float bwratio = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->bwratio = bwratio);
}

typedef struct {
	sp_data* sp;
	sp_waveset* osc;
	m_bool is_init;
} GW_waveset;

static TICK(waveset_tick) {
	const GW_waveset* ug = (GW_waveset*)u->module.gen.data;
	if(!ug->is_init) { // LCOV_EXCL_START
		u->out = 0;
		return;
	} // LCOV_EXCL_STOP
	sp_waveset_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(waveset_ctor) {
	GW_waveset* ug = (GW_waveset*)xcalloc(1, sizeof(GW_waveset));
	ug->sp = shred->vm_ref->sp;
	ug->is_init = 0;
	ug->osc = NULL;
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), waveset_tick, ug, 0);
}

DTOR(waveset_dtor) {
	GW_waveset* ug = UGEN(o)->module.gen.data;
	if(ug->is_init) {

		sp_waveset_destroy(&ug->osc);
	}
	free(ug);
}

MFUN(waveset_init) {
	m_uint gw_offset = SZ_INT;
	GW_waveset* ug = (GW_waveset*)UGEN(o)->module.gen.data;
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

MFUN(waveset_get_rep) {
	GW_waveset* ug = (GW_waveset*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->rep;
}

MFUN(waveset_set_rep) {
	m_uint gw_offset = SZ_INT;
	GW_waveset* ug = (GW_waveset*)UGEN(o)->module.gen.data;
	m_float rep = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->rep = rep);
}

typedef struct {
	sp_data* sp;
	sp_wpkorg35* osc;
} GW_wpkorg35;

static TICK(wpkorg35_tick) {
	const GW_wpkorg35* ug = (GW_wpkorg35*)u->module.gen.data;
	sp_wpkorg35_compute(ug->sp, ug->osc, &u->in, &u->out);

}

CTOR(wpkorg35_ctor) {
	GW_wpkorg35* ug = (GW_wpkorg35*)xcalloc(1, sizeof(GW_wpkorg35));
	ug->sp = shred->vm_ref->sp;
	sp_wpkorg35_create(&ug->osc);
	sp_wpkorg35_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 1, 1);
	ugen_gen(UGEN(o), wpkorg35_tick, ug, 0);
}

DTOR(wpkorg35_dtor) {
	GW_wpkorg35* ug = UGEN(o)->module.gen.data;
	sp_wpkorg35_destroy(&ug->osc);
	free(ug);
}

MFUN(wpkorg35_get_cutoff) {
	GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->cutoff;
}

MFUN(wpkorg35_set_cutoff) {
	m_uint gw_offset = SZ_INT;
	GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
	m_float cutoff = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->cutoff = cutoff);
}

MFUN(wpkorg35_get_res) {
	GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->res;
}

MFUN(wpkorg35_set_res) {
	m_uint gw_offset = SZ_INT;
	GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
	m_float res = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->res = res);
}

MFUN(wpkorg35_get_saturation) {
	GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = ug->osc->saturation;
}

MFUN(wpkorg35_set_saturation) {
	m_uint gw_offset = SZ_INT;
	GW_wpkorg35* ug = (GW_wpkorg35*)UGEN(o)->module.gen.data;
	m_float saturation = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (ug->osc->saturation = saturation);
}

typedef struct {
	sp_data* sp;
	sp_zitarev* osc;
} GW_zitarev;

static TICK(zitarev_tick) {
	const GW_zitarev* ug = (GW_zitarev*)u->module.gen.data;
	sp_zitarev_compute(ug->sp, ug->osc, &UGEN(u->connect.multi->channel[0])->in, &UGEN(u->connect.multi->channel[1])->in, &UGEN(u->connect.multi->channel[0])->out, &UGEN(u->connect.multi->channel[1])->out);

}

CTOR(zitarev_ctor) {
	GW_zitarev* ug = (GW_zitarev*)xcalloc(1, sizeof(GW_zitarev));
	ug->sp = shred->vm_ref->sp;
	sp_zitarev_create(&ug->osc);
	sp_zitarev_init(ug->sp, ug->osc);
	ugen_ini(UGEN(o), 2, 2);
	ugen_gen(UGEN(o), zitarev_tick, ug, 0);
}

DTOR(zitarev_dtor) {
	GW_zitarev* ug = UGEN(o)->module.gen.data;
	sp_zitarev_destroy(&ug->osc);
	free(ug);
}

MFUN(zitarev_get_in_delay) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->in_delay;
}

MFUN(zitarev_set_in_delay) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float in_delay = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->in_delay = in_delay);
}

MFUN(zitarev_get_lf_x) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->lf_x;
}

MFUN(zitarev_set_lf_x) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float lf_x = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->lf_x = lf_x);
}

MFUN(zitarev_get_rt60_low) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->rt60_low;
}

MFUN(zitarev_set_rt60_low) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float rt60_low = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->rt60_low = rt60_low);
}

MFUN(zitarev_get_rt60_mid) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->rt60_mid;
}

MFUN(zitarev_set_rt60_mid) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float rt60_mid = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->rt60_mid = rt60_mid);
}

MFUN(zitarev_get_hf_damping) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->hf_damping;
}

MFUN(zitarev_set_hf_damping) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float hf_damping = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->hf_damping = hf_damping);
}

MFUN(zitarev_get_eq1_freq) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->eq1_freq;
}

MFUN(zitarev_set_eq1_freq) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float eq1_freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->eq1_freq = eq1_freq);
}

MFUN(zitarev_get_eq1_level) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->eq1_level;
}

MFUN(zitarev_set_eq1_level) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float eq1_level = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->eq1_level = eq1_level);
}

MFUN(zitarev_get_eq2_freq) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->eq2_freq;
}

MFUN(zitarev_set_eq2_freq) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float eq2_freq = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->eq2_freq = eq2_freq);
}

MFUN(zitarev_get_eq2_level) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->eq2_level;
}

MFUN(zitarev_set_eq2_level) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float eq2_level = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->eq2_level = eq2_level);
}

MFUN(zitarev_get_mix) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->mix;
}

MFUN(zitarev_set_mix) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float mix = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->mix = mix);
}

MFUN(zitarev_get_level) {
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	*(m_float*)RETURN = *ug->osc->level;
}

MFUN(zitarev_set_level) {
	m_uint gw_offset = SZ_INT;
	GW_zitarev* ug = (GW_zitarev*)UGEN(o)->module.gen.data;
	m_float level = *(m_float*)(shred->mem + gw_offset);
	*(m_float*)RETURN = (*ug->osc->level = level);
}


m_bool import_soundpipe(Gwi gwi) {

	Type t_ftbl = gwi_mk_type(gwi, "ftbl", SZ_INT, t_object);
	CHECK_BB(gwi_class_ini(gwi, t_ftbl, NULL, ftbl_dtor))
	CHECK_BB(gwi_item_ini(gwi, "int", "@ftbl"))
	o_ftbl_data = gwi_item_end(gwi, 0, NULL);
	gwi_func_ini(gwi, "void", "gen_composite", ftbl_gen_composite);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "string", "argstring");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_file", ftbl_gen_file);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "string", "filename");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_gauss", ftbl_gen_gauss);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "float", "scale");
		 gwi_func_arg(gwi, "int", "seed");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_line", ftbl_gen_line);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "string", "argstring");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_padsynth", ftbl_gen_padsynth);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "ftbl", "amps");
		 gwi_func_arg(gwi, "float", "f");
		 gwi_func_arg(gwi, "float", "bw");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_rand", ftbl_gen_rand);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "string", "argstring");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_scrambler", ftbl_gen_scrambler);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "ftbl", "dest");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_sine", ftbl_gen_sine);
	gwi_func_arg(gwi, "int", "size");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_sinesum", ftbl_gen_sinesum);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "string", "argstring");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "void", "gen_xline", ftbl_gen_xline);
	gwi_func_arg(gwi, "int", "size");
		 gwi_func_arg(gwi, "string", "argstring");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_adsr = gwi_mk_type(gwi, "Adsr", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_adsr, adsr_ctor, adsr_dtor))
	gwi_func_ini(gwi, "float", "atk", adsr_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", adsr_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", adsr_get_dec);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", adsr_set_dec);
		 gwi_func_arg(gwi, "float", "dec");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "sus", adsr_get_sus);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "sus", adsr_set_sus);
		 gwi_func_arg(gwi, "float", "sus");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", adsr_get_rel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", adsr_set_rel);
		 gwi_func_arg(gwi, "float", "rel");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_allpass = gwi_mk_type(gwi, "Allpass", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_allpass, allpass_ctor, allpass_dtor))
	gwi_func_ini(gwi, "void", "init", allpass_init);
		 gwi_func_arg(gwi, "float", "looptime");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "revtime", allpass_get_revtime);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "revtime", allpass_set_revtime);
		 gwi_func_arg(gwi, "float", "revtime");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_atone = gwi_mk_type(gwi, "Atone", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_atone, atone_ctor, atone_dtor))
	gwi_func_ini(gwi, "float", "hp", atone_get_hp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "hp", atone_set_hp);
		 gwi_func_arg(gwi, "float", "hp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_autowah = gwi_mk_type(gwi, "Autowah", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_autowah, autowah_ctor, autowah_dtor))
	gwi_func_ini(gwi, "float", "level", autowah_get_level);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "level", autowah_set_level);
		 gwi_func_arg(gwi, "float", "level");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "wah", autowah_get_wah);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "wah", autowah_set_wah);
		 gwi_func_arg(gwi, "float", "wah");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mix", autowah_get_mix);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mix", autowah_set_mix);
		 gwi_func_arg(gwi, "float", "mix");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_bal = gwi_mk_type(gwi, "Bal", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_bal, bal_ctor, bal_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_bar = gwi_mk_type(gwi, "Bar", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_bar, bar_ctor, bar_dtor))
	gwi_func_ini(gwi, "void", "init", bar_init);
		 gwi_func_arg(gwi, "float", "iK");
		 gwi_func_arg(gwi, "float", "ib");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bcL", bar_get_bcL);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bcL", bar_set_bcL);
		 gwi_func_arg(gwi, "float", "bcL");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bcR", bar_get_bcR);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bcR", bar_set_bcR);
		 gwi_func_arg(gwi, "float", "bcR");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "T30", bar_get_T30);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "T30", bar_set_T30);
		 gwi_func_arg(gwi, "float", "T30");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "scan", bar_get_scan);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "scan", bar_set_scan);
		 gwi_func_arg(gwi, "float", "scan");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pos", bar_get_pos);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pos", bar_set_pos);
		 gwi_func_arg(gwi, "float", "pos");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "vel", bar_get_vel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "vel", bar_set_vel);
		 gwi_func_arg(gwi, "float", "vel");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "wid", bar_get_wid);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "wid", bar_set_wid);
		 gwi_func_arg(gwi, "float", "wid");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_biquad = gwi_mk_type(gwi, "Biquad", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_biquad, biquad_ctor, biquad_dtor))
	gwi_func_ini(gwi, "float", "b0", biquad_get_b0);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b0", biquad_set_b0);
		 gwi_func_arg(gwi, "float", "b0");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b1", biquad_get_b1);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b1", biquad_set_b1);
		 gwi_func_arg(gwi, "float", "b1");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b2", biquad_get_b2);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b2", biquad_set_b2);
		 gwi_func_arg(gwi, "float", "b2");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "a0", biquad_get_a0);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "a0", biquad_set_a0);
		 gwi_func_arg(gwi, "float", "a0");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "a1", biquad_get_a1);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "a1", biquad_set_a1);
		 gwi_func_arg(gwi, "float", "a1");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "a2", biquad_get_a2);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "a2", biquad_set_a2);
		 gwi_func_arg(gwi, "float", "a2");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_biscale = gwi_mk_type(gwi, "Biscale", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_biscale, biscale_ctor, biscale_dtor))
	gwi_func_ini(gwi, "float", "min", biscale_get_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", biscale_set_min);
		 gwi_func_arg(gwi, "float", "min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", biscale_get_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", biscale_set_max);
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_bitcrush = gwi_mk_type(gwi, "Bitcrush", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_bitcrush, bitcrush_ctor, bitcrush_dtor))
	gwi_func_ini(gwi, "float", "bitdepth", bitcrush_get_bitdepth);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bitdepth", bitcrush_set_bitdepth);
		 gwi_func_arg(gwi, "float", "bitdepth");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "srate", bitcrush_get_srate);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "srate", bitcrush_set_srate);
		 gwi_func_arg(gwi, "float", "srate");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_blsaw = gwi_mk_type(gwi, "Blsaw", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_blsaw, blsaw_ctor, blsaw_dtor))
	gwi_func_ini(gwi, "float", "freq", blsaw_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", blsaw_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", blsaw_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", blsaw_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_blsquare = gwi_mk_type(gwi, "Blsquare", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_blsquare, blsquare_ctor, blsquare_dtor))
	gwi_func_ini(gwi, "float", "freq", blsquare_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", blsquare_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", blsquare_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", blsquare_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "width", blsquare_get_width);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "width", blsquare_set_width);
		 gwi_func_arg(gwi, "float", "width");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_bltriangle = gwi_mk_type(gwi, "Bltriangle", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_bltriangle, bltriangle_ctor, bltriangle_dtor))
	gwi_func_ini(gwi, "float", "freq", bltriangle_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", bltriangle_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", bltriangle_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", bltriangle_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_brown = gwi_mk_type(gwi, "Brown", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_brown, brown_ctor, brown_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_butbp = gwi_mk_type(gwi, "Butbp", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_butbp, butbp_ctor, butbp_dtor))
	gwi_func_ini(gwi, "float", "freq", butbp_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", butbp_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bw", butbp_get_bw);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bw", butbp_set_bw);
		 gwi_func_arg(gwi, "float", "bw");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_butbr = gwi_mk_type(gwi, "Butbr", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_butbr, butbr_ctor, butbr_dtor))
	gwi_func_ini(gwi, "float", "freq", butbr_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", butbr_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bw", butbr_get_bw);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bw", butbr_set_bw);
		 gwi_func_arg(gwi, "float", "bw");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_buthp = gwi_mk_type(gwi, "Buthp", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_buthp, buthp_ctor, buthp_dtor))
	gwi_func_ini(gwi, "float", "freq", buthp_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", buthp_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_butlp = gwi_mk_type(gwi, "Butlp", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_butlp, butlp_ctor, butlp_dtor))
	gwi_func_ini(gwi, "float", "freq", butlp_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", butlp_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_clip = gwi_mk_type(gwi, "Clip", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_clip, clip_ctor, clip_dtor))
	gwi_func_ini(gwi, "float", "lim", clip_get_lim);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lim", clip_set_lim);
		 gwi_func_arg(gwi, "float", "lim");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_clock = gwi_mk_type(gwi, "Clock", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_clock, clock_ctor, clock_dtor))
	gwi_func_ini(gwi, "float", "bpm", clock_get_bpm);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bpm", clock_set_bpm);
		 gwi_func_arg(gwi, "float", "bpm");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "subdiv", clock_get_subdiv);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "subdiv", clock_set_subdiv);
		 gwi_func_arg(gwi, "float", "subdiv");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_comb = gwi_mk_type(gwi, "Comb", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_comb, comb_ctor, comb_dtor))
	gwi_func_ini(gwi, "void", "init", comb_init);
		 gwi_func_arg(gwi, "float", "looptime");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "revtime", comb_get_revtime);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "revtime", comb_set_revtime);
		 gwi_func_arg(gwi, "float", "revtime");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_compressor = gwi_mk_type(gwi, "Compressor", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_compressor, compressor_ctor, compressor_dtor))
	gwi_func_ini(gwi, "float", "ratio", compressor_get_ratio);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "ratio", compressor_set_ratio);
		 gwi_func_arg(gwi, "float", "ratio");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "thresh", compressor_get_thresh);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "thresh", compressor_set_thresh);
		 gwi_func_arg(gwi, "float", "thresh");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", compressor_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", compressor_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", compressor_get_rel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", compressor_set_rel);
		 gwi_func_arg(gwi, "float", "rel");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_conv = gwi_mk_type(gwi, "Conv", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_conv, conv_ctor, conv_dtor))
	gwi_func_ini(gwi, "void", "init", conv_init);
		 gwi_func_arg(gwi, "ftbl", "ft");
		 gwi_func_arg(gwi, "float", "iPartLen");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_count = gwi_mk_type(gwi, "Count", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_count, count_ctor, count_dtor))
	gwi_func_ini(gwi, "float", "count", count_get_count);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "count", count_set_count);
		 gwi_func_arg(gwi, "float", "count");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mode", count_get_mode);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mode", count_set_mode);
		 gwi_func_arg(gwi, "float", "mode");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_crossfade = gwi_mk_type(gwi, "Crossfade", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_crossfade, crossfade_ctor, crossfade_dtor))
	gwi_func_ini(gwi, "float", "pos", crossfade_get_pos);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pos", crossfade_set_pos);
		 gwi_func_arg(gwi, "float", "pos");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_dcblock = gwi_mk_type(gwi, "Dcblock", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_dcblock, dcblock_ctor, dcblock_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_delay = gwi_mk_type(gwi, "Delay", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_delay, delay_ctor, delay_dtor))
	gwi_func_ini(gwi, "void", "init", delay_init);
		 gwi_func_arg(gwi, "float", "time");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "feedback", delay_get_feedback);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "feedback", delay_set_feedback);
		 gwi_func_arg(gwi, "float", "feedback");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_diode = gwi_mk_type(gwi, "Diode", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_diode, diode_ctor, diode_dtor))
	gwi_func_ini(gwi, "float", "freq", diode_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", diode_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", diode_get_res);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", diode_set_res);
		 gwi_func_arg(gwi, "float", "res");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_diskin = gwi_mk_type(gwi, "Diskin", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_diskin, diskin_ctor, diskin_dtor))
	gwi_func_ini(gwi, "void", "init", diskin_init);
		 gwi_func_arg(gwi, "string", "filename");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_dist = gwi_mk_type(gwi, "Dist", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_dist, dist_ctor, dist_dtor))
	gwi_func_ini(gwi, "float", "pregain", dist_get_pregain);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pregain", dist_set_pregain);
		 gwi_func_arg(gwi, "float", "pregain");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "postgain", dist_get_postgain);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "postgain", dist_set_postgain);
		 gwi_func_arg(gwi, "float", "postgain");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "shape1", dist_get_shape1);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "shape1", dist_set_shape1);
		 gwi_func_arg(gwi, "float", "shape1");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "shape2", dist_get_shape2);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "shape2", dist_set_shape2);
		 gwi_func_arg(gwi, "float", "shape2");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_dmetro = gwi_mk_type(gwi, "Dmetro", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_dmetro, dmetro_ctor, dmetro_dtor))
	gwi_func_ini(gwi, "float", "time", dmetro_get_time);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "time", dmetro_set_time);
		 gwi_func_arg(gwi, "float", "time");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_drip = gwi_mk_type(gwi, "Drip", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_drip, drip_ctor, drip_dtor))
	gwi_func_ini(gwi, "void", "init", drip_init);
		 gwi_func_arg(gwi, "float", "dettack");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "num_tubes", drip_get_num_tubes);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "num_tubes", drip_set_num_tubes);
		 gwi_func_arg(gwi, "float", "num_tubes");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", drip_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", drip_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "damp", drip_get_damp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "damp", drip_set_damp);
		 gwi_func_arg(gwi, "float", "damp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "shake_max", drip_get_shake_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "shake_max", drip_set_shake_max);
		 gwi_func_arg(gwi, "float", "shake_max");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", drip_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", drip_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq1", drip_get_freq1);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq1", drip_set_freq1);
		 gwi_func_arg(gwi, "float", "freq1");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq2", drip_get_freq2);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq2", drip_set_freq2);
		 gwi_func_arg(gwi, "float", "freq2");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_dtrig = gwi_mk_type(gwi, "Dtrig", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_dtrig, dtrig_ctor, dtrig_dtor))
	gwi_func_ini(gwi, "void", "init", dtrig_init);
		 gwi_func_arg(gwi, "ftbl", "ft");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "loop", dtrig_get_loop);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "loop", dtrig_set_loop);
		 gwi_func_arg(gwi, "int", "loop");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "delay", dtrig_get_delay);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "delay", dtrig_set_delay);
		 gwi_func_arg(gwi, "float", "delay");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "scale", dtrig_get_scale);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "scale", dtrig_set_scale);
		 gwi_func_arg(gwi, "float", "scale");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_dust = gwi_mk_type(gwi, "Dust", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_dust, dust_ctor, dust_dtor))
	gwi_func_ini(gwi, "float", "amp", dust_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", dust_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "density", dust_get_density);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "density", dust_set_density);
		 gwi_func_arg(gwi, "float", "density");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "bipolar", dust_get_bipolar);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "bipolar", dust_set_bipolar);
		 gwi_func_arg(gwi, "int", "bipolar");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_eqfil = gwi_mk_type(gwi, "Eqfil", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_eqfil, eqfil_ctor, eqfil_dtor))
	gwi_func_ini(gwi, "float", "freq", eqfil_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", eqfil_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bw", eqfil_get_bw);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bw", eqfil_set_bw);
		 gwi_func_arg(gwi, "float", "bw");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "gain", eqfil_get_gain);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "gain", eqfil_set_gain);
		 gwi_func_arg(gwi, "float", "gain");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_expon = gwi_mk_type(gwi, "Expon", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_expon, expon_ctor, expon_dtor))
	gwi_func_ini(gwi, "float", "a", expon_get_a);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "a", expon_set_a);
		 gwi_func_arg(gwi, "float", "a");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", expon_get_dur);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", expon_set_dur);
		 gwi_func_arg(gwi, "float", "dur");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b", expon_get_b);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b", expon_set_b);
		 gwi_func_arg(gwi, "float", "b");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_fof = gwi_mk_type(gwi, "Fof", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_fof, fof_ctor, fof_dtor))
	gwi_func_ini(gwi, "void", "init", fof_init);
		 gwi_func_arg(gwi, "ftbl", "sine");
		 gwi_func_arg(gwi, "ftbl", "win");
		 gwi_func_arg(gwi, "int", "iolaps");
		 gwi_func_arg(gwi, "float", "iphs");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", fof_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", fof_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "fund", fof_get_fund);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "fund", fof_set_fund);
		 gwi_func_arg(gwi, "float", "fund");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "form", fof_get_form);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "form", fof_set_form);
		 gwi_func_arg(gwi, "float", "form");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "oct", fof_get_oct);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "oct", fof_set_oct);
		 gwi_func_arg(gwi, "float", "oct");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "band", fof_get_band);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "band", fof_set_band);
		 gwi_func_arg(gwi, "float", "band");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "ris", fof_get_ris);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "ris", fof_set_ris);
		 gwi_func_arg(gwi, "float", "ris");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", fof_get_dec);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", fof_set_dec);
		 gwi_func_arg(gwi, "float", "dec");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", fof_get_dur);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", fof_set_dur);
		 gwi_func_arg(gwi, "float", "dur");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_fofilt = gwi_mk_type(gwi, "Fofilt", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_fofilt, fofilt_ctor, fofilt_dtor))
	gwi_func_ini(gwi, "float", "freq", fofilt_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", fofilt_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", fofilt_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", fofilt_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", fofilt_get_dec);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", fofilt_set_dec);
		 gwi_func_arg(gwi, "float", "dec");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_fog = gwi_mk_type(gwi, "Fog", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_fog, fog_ctor, fog_dtor))
	gwi_func_ini(gwi, "void", "init", fog_init);
		 gwi_func_arg(gwi, "ftbl", "wav");
		 gwi_func_arg(gwi, "ftbl", "win");
		 gwi_func_arg(gwi, "int", "iolaps");
		 gwi_func_arg(gwi, "float", "iphs");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", fog_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", fog_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dens", fog_get_dens);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dens", fog_set_dens);
		 gwi_func_arg(gwi, "float", "dens");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "trans", fog_get_trans);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "trans", fog_set_trans);
		 gwi_func_arg(gwi, "float", "trans");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "spd", fog_get_spd);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "spd", fog_set_spd);
		 gwi_func_arg(gwi, "float", "spd");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "oct", fog_get_oct);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "oct", fog_set_oct);
		 gwi_func_arg(gwi, "float", "oct");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "band", fog_get_band);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "band", fog_set_band);
		 gwi_func_arg(gwi, "float", "band");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "ris", fog_get_ris);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "ris", fog_set_ris);
		 gwi_func_arg(gwi, "float", "ris");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", fog_get_dec);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", fog_set_dec);
		 gwi_func_arg(gwi, "float", "dec");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", fog_get_dur);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", fog_set_dur);
		 gwi_func_arg(gwi, "float", "dur");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_fold = gwi_mk_type(gwi, "Fold", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_fold, fold_ctor, fold_dtor))
	gwi_func_ini(gwi, "float", "incr", fold_get_incr);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "incr", fold_set_incr);
		 gwi_func_arg(gwi, "float", "incr");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_fosc = gwi_mk_type(gwi, "Fosc", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_fosc, fosc_ctor, fosc_dtor))
	gwi_func_ini(gwi, "void", "init", fosc_init);
		 gwi_func_arg(gwi, "ftbl", "tbl");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", fosc_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", fosc_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", fosc_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", fosc_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "car", fosc_get_car);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "car", fosc_set_car);
		 gwi_func_arg(gwi, "float", "car");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mod", fosc_get_mod);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mod", fosc_set_mod);
		 gwi_func_arg(gwi, "float", "mod");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "indx", fosc_get_indx);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "indx", fosc_set_indx);
		 gwi_func_arg(gwi, "float", "indx");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_gbuzz = gwi_mk_type(gwi, "Gbuzz", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_gbuzz, gbuzz_ctor, gbuzz_dtor))
	gwi_func_ini(gwi, "void", "init", gbuzz_init);
		 gwi_func_arg(gwi, "ftbl", "ft");
		 gwi_func_arg(gwi, "float", "iphs");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", gbuzz_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", gbuzz_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", gbuzz_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", gbuzz_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "nharm", gbuzz_get_nharm);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "nharm", gbuzz_set_nharm);
		 gwi_func_arg(gwi, "float", "nharm");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lharm", gbuzz_get_lharm);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lharm", gbuzz_set_lharm);
		 gwi_func_arg(gwi, "float", "lharm");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mul", gbuzz_get_mul);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mul", gbuzz_set_mul);
		 gwi_func_arg(gwi, "float", "mul");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_hilbert = gwi_mk_type(gwi, "Hilbert", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_hilbert, hilbert_ctor, hilbert_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_in = gwi_mk_type(gwi, "In", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_in, in_ctor, in_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_incr = gwi_mk_type(gwi, "Incr", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_incr, incr_ctor, incr_dtor))
	gwi_func_ini(gwi, "void", "init", incr_init);
		 gwi_func_arg(gwi, "float", "val");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "step", incr_get_step);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "step", incr_set_step);
		 gwi_func_arg(gwi, "float", "step");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", incr_get_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", incr_set_min);
		 gwi_func_arg(gwi, "float", "min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", incr_get_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", incr_set_max);
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_jcrev = gwi_mk_type(gwi, "Jcrev", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_jcrev, jcrev_ctor, jcrev_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_jitter = gwi_mk_type(gwi, "Jitter", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_jitter, jitter_ctor, jitter_dtor))
	gwi_func_ini(gwi, "float", "amp", jitter_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", jitter_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cpsMin", jitter_get_cpsMin);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cpsMin", jitter_set_cpsMin);
		 gwi_func_arg(gwi, "float", "cpsMin");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cpsMax", jitter_get_cpsMax);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cpsMax", jitter_set_cpsMax);
		 gwi_func_arg(gwi, "float", "cpsMax");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_line = gwi_mk_type(gwi, "Line", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_line, line_ctor, line_dtor))
	gwi_func_ini(gwi, "float", "a", line_get_a);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "a", line_set_a);
		 gwi_func_arg(gwi, "float", "a");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", line_get_dur);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", line_set_dur);
		 gwi_func_arg(gwi, "float", "dur");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b", line_get_b);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "b", line_set_b);
		 gwi_func_arg(gwi, "float", "b");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_lpc = gwi_mk_type(gwi, "Lpc", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_lpc, lpc_ctor, lpc_dtor))
	gwi_func_ini(gwi, "void", "init", lpc_init);
		 gwi_func_arg(gwi, "int", "framesize");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_lpf18 = gwi_mk_type(gwi, "Lpf18", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_lpf18, lpf18_ctor, lpf18_dtor))
	gwi_func_ini(gwi, "float", "cutoff", lpf18_get_cutoff);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cutoff", lpf18_set_cutoff);
		 gwi_func_arg(gwi, "float", "cutoff");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", lpf18_get_res);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", lpf18_set_res);
		 gwi_func_arg(gwi, "float", "res");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dist", lpf18_get_dist);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dist", lpf18_set_dist);
		 gwi_func_arg(gwi, "float", "dist");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_maygate = gwi_mk_type(gwi, "Maygate", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_maygate, maygate_ctor, maygate_dtor))
	gwi_func_ini(gwi, "float", "prob", maygate_get_prob);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "prob", maygate_set_prob);
		 gwi_func_arg(gwi, "float", "prob");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "mode", maygate_get_mode);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "mode", maygate_set_mode);
		 gwi_func_arg(gwi, "int", "mode");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_metro = gwi_mk_type(gwi, "Metro", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_metro, metro_ctor, metro_dtor))
	gwi_func_ini(gwi, "float", "freq", metro_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", metro_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_mincer = gwi_mk_type(gwi, "Mincer", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_mincer, mincer_ctor, mincer_dtor))
	gwi_func_ini(gwi, "void", "init", mincer_init);
		 gwi_func_arg(gwi, "ftbl", "ft");
		 gwi_func_arg(gwi, "int", "winsize");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "time", mincer_get_time);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "time", mincer_set_time);
		 gwi_func_arg(gwi, "float", "time");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", mincer_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", mincer_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pitch", mincer_get_pitch);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pitch", mincer_set_pitch);
		 gwi_func_arg(gwi, "float", "pitch");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_mode = gwi_mk_type(gwi, "Mode", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_mode, mode_ctor, mode_dtor))
	gwi_func_ini(gwi, "float", "freq", mode_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", mode_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "q", mode_get_q);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "q", mode_set_q);
		 gwi_func_arg(gwi, "float", "q");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_moogladder = gwi_mk_type(gwi, "Moogladder", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_moogladder, moogladder_ctor, moogladder_dtor))
	gwi_func_ini(gwi, "float", "freq", moogladder_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", moogladder_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", moogladder_get_res);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", moogladder_set_res);
		 gwi_func_arg(gwi, "float", "res");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_noise = gwi_mk_type(gwi, "Noise", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_noise, noise_ctor, noise_dtor))
	gwi_func_ini(gwi, "float", "amp", noise_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", noise_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_nsmp = gwi_mk_type(gwi, "Nsmp", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_nsmp, nsmp_ctor, nsmp_dtor))
	gwi_func_ini(gwi, "void", "init", nsmp_init);
		 gwi_func_arg(gwi, "ftbl", "ft");
		 gwi_func_arg(gwi, "int", "sr");
		 gwi_func_arg(gwi, "string", "init");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "index", nsmp_get_index);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "index", nsmp_set_index);
		 gwi_func_arg(gwi, "int", "index");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_osc = gwi_mk_type(gwi, "Osc", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_osc, osc_ctor, osc_dtor))
	gwi_func_ini(gwi, "void", "init", osc_init);
		 gwi_func_arg(gwi, "ftbl", "tbl");
		 gwi_func_arg(gwi, "float", "phase");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", osc_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", osc_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", osc_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", osc_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_oscmorph = gwi_mk_type(gwi, "Oscmorph", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_oscmorph, oscmorph_ctor, oscmorph_dtor))
	gwi_func_ini(gwi, "void", "init", oscmorph_init);
		 gwi_func_arg(gwi, "ftbl[]", "tbl");
		 gwi_func_arg(gwi, "int", "nft");
		 gwi_func_arg(gwi, "float", "phase");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", oscmorph_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", oscmorph_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", oscmorph_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", oscmorph_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "wtpos", oscmorph_get_wtpos);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "wtpos", oscmorph_set_wtpos);
		 gwi_func_arg(gwi, "float", "wtpos");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_pan2 = gwi_mk_type(gwi, "Pan2", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_pan2, pan2_ctor, pan2_dtor))
	gwi_func_ini(gwi, "int", "type", pan2_get_type);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "type", pan2_set_type);
		 gwi_func_arg(gwi, "int", "type");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pan", pan2_get_pan);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pan", pan2_set_pan);
		 gwi_func_arg(gwi, "float", "pan");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_panst = gwi_mk_type(gwi, "Panst", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_panst, panst_ctor, panst_dtor))
	gwi_func_ini(gwi, "int", "type", panst_get_type);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "type", panst_set_type);
		 gwi_func_arg(gwi, "int", "type");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pan", panst_get_pan);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "pan", panst_set_pan);
		 gwi_func_arg(gwi, "float", "pan");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_pareq = gwi_mk_type(gwi, "Pareq", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_pareq, pareq_ctor, pareq_dtor))
	gwi_func_ini(gwi, "float", "fc", pareq_get_fc);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "fc", pareq_set_fc);
		 gwi_func_arg(gwi, "float", "fc");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "v", pareq_get_v);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "v", pareq_set_v);
		 gwi_func_arg(gwi, "float", "v");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "q", pareq_get_q);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "q", pareq_set_q);
		 gwi_func_arg(gwi, "float", "q");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mode", pareq_get_mode);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mode", pareq_set_mode);
		 gwi_func_arg(gwi, "float", "mode");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_paulstretch = gwi_mk_type(gwi, "Paulstretch", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_paulstretch, paulstretch_ctor, paulstretch_dtor))
	gwi_func_ini(gwi, "void", "init", paulstretch_init);
		 gwi_func_arg(gwi, "ftbl", "ft");
		 gwi_func_arg(gwi, "float", "windowsize");
		 gwi_func_arg(gwi, "float", "stretch");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_pdhalf = gwi_mk_type(gwi, "Pdhalf", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_pdhalf, pdhalf_ctor, pdhalf_dtor))
	gwi_func_ini(gwi, "float", "amount", pdhalf_get_amount);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amount", pdhalf_set_amount);
		 gwi_func_arg(gwi, "float", "amount");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_peaklim = gwi_mk_type(gwi, "Peaklim", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_peaklim, peaklim_ctor, peaklim_dtor))
	gwi_func_ini(gwi, "float", "atk", peaklim_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", peaklim_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", peaklim_get_rel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", peaklim_set_rel);
		 gwi_func_arg(gwi, "float", "rel");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "thresh", peaklim_get_thresh);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "thresh", peaklim_set_thresh);
		 gwi_func_arg(gwi, "float", "thresh");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_phaser = gwi_mk_type(gwi, "Phaser", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_phaser, phaser_ctor, phaser_dtor))
	gwi_func_ini(gwi, "float", "MaxNotch1Freq", phaser_get_MaxNotch1Freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "MaxNotch1Freq", phaser_set_MaxNotch1Freq);
		 gwi_func_arg(gwi, "float", "MaxNotch1Freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "MinNotch1Freq", phaser_get_MinNotch1Freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "MinNotch1Freq", phaser_set_MinNotch1Freq);
		 gwi_func_arg(gwi, "float", "MinNotch1Freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "Notch_width", phaser_get_Notch_width);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "Notch_width", phaser_set_Notch_width);
		 gwi_func_arg(gwi, "float", "Notch_width");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "NotchFreq", phaser_get_NotchFreq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "NotchFreq", phaser_set_NotchFreq);
		 gwi_func_arg(gwi, "float", "NotchFreq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "VibratoMode", phaser_get_VibratoMode);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "VibratoMode", phaser_set_VibratoMode);
		 gwi_func_arg(gwi, "float", "VibratoMode");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "depth", phaser_get_depth);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "depth", phaser_set_depth);
		 gwi_func_arg(gwi, "float", "depth");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "feedback_gain", phaser_get_feedback_gain);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "feedback_gain", phaser_set_feedback_gain);
		 gwi_func_arg(gwi, "float", "feedback_gain");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "invert", phaser_get_invert);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "invert", phaser_set_invert);
		 gwi_func_arg(gwi, "float", "invert");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "level", phaser_get_level);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "level", phaser_set_level);
		 gwi_func_arg(gwi, "float", "level");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lfobpm", phaser_get_lfobpm);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lfobpm", phaser_set_lfobpm);
		 gwi_func_arg(gwi, "float", "lfobpm");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_phasor = gwi_mk_type(gwi, "Phasor", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_phasor, phasor_ctor, phasor_dtor))
	gwi_func_ini(gwi, "void", "init", phasor_init);
		 gwi_func_arg(gwi, "float", "iphs");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", phasor_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", phasor_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_pinknoise = gwi_mk_type(gwi, "Pinknoise", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_pinknoise, pinknoise_ctor, pinknoise_dtor))
	gwi_func_ini(gwi, "float", "amp", pinknoise_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", pinknoise_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_pitchamdf = gwi_mk_type(gwi, "Pitchamdf", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_pitchamdf, pitchamdf_ctor, pitchamdf_dtor))
	gwi_func_ini(gwi, "void", "init", pitchamdf_init);
		 gwi_func_arg(gwi, "float", "min");
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_pluck = gwi_mk_type(gwi, "Pluck", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_pluck, pluck_ctor, pluck_dtor))
	gwi_func_ini(gwi, "void", "init", pluck_init);
		 gwi_func_arg(gwi, "float", "ifreq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", pluck_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", pluck_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", pluck_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", pluck_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_port = gwi_mk_type(gwi, "Port", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_port, port_ctor, port_dtor))
	gwi_func_ini(gwi, "void", "init", port_init);
		 gwi_func_arg(gwi, "float", "htime");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_posc3 = gwi_mk_type(gwi, "Posc3", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_posc3, posc3_ctor, posc3_dtor))
	gwi_func_ini(gwi, "void", "init", posc3_init);
		 gwi_func_arg(gwi, "ftbl", "tbl");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", posc3_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", posc3_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", posc3_get_amp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "amp", posc3_set_amp);
		 gwi_func_arg(gwi, "float", "amp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_progress = gwi_mk_type(gwi, "Progress", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_progress, progress_ctor, progress_dtor))
	gwi_func_ini(gwi, "int", "nbars", progress_get_nbars);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "nbars", progress_set_nbars);
		 gwi_func_arg(gwi, "int", "nbars");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "skip", progress_get_skip);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "skip", progress_set_skip);
		 gwi_func_arg(gwi, "int", "skip");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_prop = gwi_mk_type(gwi, "Prop", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_prop, prop_ctor, prop_dtor))
	gwi_func_ini(gwi, "void", "init", prop_init);
		 gwi_func_arg(gwi, "string", "str");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bpm", prop_get_bpm);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bpm", prop_set_bpm);
		 gwi_func_arg(gwi, "float", "bpm");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_pshift = gwi_mk_type(gwi, "Pshift", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_pshift, pshift_ctor, pshift_dtor))
	gwi_func_ini(gwi, "float", "shift", pshift_get_shift);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "shift", pshift_set_shift);
		 gwi_func_arg(gwi, "float", "shift");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "window", pshift_get_window);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "window", pshift_set_window);
		 gwi_func_arg(gwi, "float", "window");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "xfade", pshift_get_xfade);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "xfade", pshift_set_xfade);
		 gwi_func_arg(gwi, "float", "xfade");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_ptrack = gwi_mk_type(gwi, "Ptrack", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_ptrack, ptrack_ctor, ptrack_dtor))
	gwi_func_ini(gwi, "void", "init", ptrack_init);
		 gwi_func_arg(gwi, "int", "ihopsize");
		 gwi_func_arg(gwi, "int", "ipeaks");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_randh = gwi_mk_type(gwi, "Randh", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_randh, randh_ctor, randh_dtor))
	gwi_func_ini(gwi, "float", "min", randh_get_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", randh_set_min);
		 gwi_func_arg(gwi, "float", "min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", randh_get_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", randh_set_max);
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", randh_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", randh_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_randi = gwi_mk_type(gwi, "Randi", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_randi, randi_ctor, randi_dtor))
	gwi_func_ini(gwi, "float", "min", randi_get_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", randi_set_min);
		 gwi_func_arg(gwi, "float", "min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", randi_get_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", randi_set_max);
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cps", randi_get_cps);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cps", randi_set_cps);
		 gwi_func_arg(gwi, "float", "cps");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mode", randi_get_mode);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mode", randi_set_mode);
		 gwi_func_arg(gwi, "float", "mode");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_random = gwi_mk_type(gwi, "Random", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_random, random_ctor, random_dtor))
	gwi_func_ini(gwi, "float", "min", random_get_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", random_set_min);
		 gwi_func_arg(gwi, "float", "min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", random_get_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", random_set_max);
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_reson = gwi_mk_type(gwi, "Reson", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_reson, reson_ctor, reson_dtor))
	gwi_func_ini(gwi, "float", "freq", reson_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", reson_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bw", reson_get_bw);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bw", reson_set_bw);
		 gwi_func_arg(gwi, "float", "bw");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_reverse = gwi_mk_type(gwi, "Reverse", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_reverse, reverse_ctor, reverse_dtor))
	gwi_func_ini(gwi, "void", "init", reverse_init);
		 gwi_func_arg(gwi, "float", "delay");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_revsc = gwi_mk_type(gwi, "Revsc", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_revsc, revsc_ctor, revsc_dtor))
	gwi_func_ini(gwi, "float", "feedback", revsc_get_feedback);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "feedback", revsc_set_feedback);
		 gwi_func_arg(gwi, "float", "feedback");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lpfreq", revsc_get_lpfreq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lpfreq", revsc_set_lpfreq);
		 gwi_func_arg(gwi, "float", "lpfreq");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_rms = gwi_mk_type(gwi, "Rms", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_rms, rms_ctor, rms_dtor))
	gwi_func_ini(gwi, "float", "ihp", rms_get_ihp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "ihp", rms_set_ihp);
		 gwi_func_arg(gwi, "float", "ihp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_rpt = gwi_mk_type(gwi, "Rpt", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_rpt, rpt_ctor, rpt_dtor))
	gwi_func_ini(gwi, "void", "init", rpt_init);
		 gwi_func_arg(gwi, "float", "maxdur");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_rspline = gwi_mk_type(gwi, "Rspline", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_rspline, rspline_ctor, rspline_dtor))
	gwi_func_ini(gwi, "float", "min", rspline_get_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", rspline_set_min);
		 gwi_func_arg(gwi, "float", "min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", rspline_get_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", rspline_set_max);
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cps_min", rspline_get_cps_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cps_min", rspline_set_cps_min);
		 gwi_func_arg(gwi, "float", "cps_min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cps_max", rspline_get_cps_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cps_max", rspline_set_cps_max);
		 gwi_func_arg(gwi, "float", "cps_max");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_samphold = gwi_mk_type(gwi, "Samphold", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_samphold, samphold_ctor, samphold_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_saturator = gwi_mk_type(gwi, "Saturator", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_saturator, saturator_ctor, saturator_dtor))
	gwi_func_ini(gwi, "float", "drive", saturator_get_drive);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "drive", saturator_set_drive);
		 gwi_func_arg(gwi, "float", "drive");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dcoffset", saturator_get_dcoffset);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dcoffset", saturator_set_dcoffset);
		 gwi_func_arg(gwi, "float", "dcoffset");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_scale = gwi_mk_type(gwi, "Scale", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_scale, scale_ctor, scale_dtor))
	gwi_func_ini(gwi, "float", "min", scale_get_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", scale_set_min);
		 gwi_func_arg(gwi, "float", "min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", scale_get_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", scale_set_max);
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_sdelay = gwi_mk_type(gwi, "Sdelay", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_sdelay, sdelay_ctor, sdelay_dtor))
	gwi_func_ini(gwi, "void", "init", sdelay_init);
		 gwi_func_arg(gwi, "float", "size");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_slice = gwi_mk_type(gwi, "Slice", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_slice, slice_ctor, slice_dtor))
	gwi_func_ini(gwi, "void", "init", slice_init);
		 gwi_func_arg(gwi, "ftbl", "vals");
		 gwi_func_arg(gwi, "ftbl", "buf");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "id", slice_get_id);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "id", slice_set_id);
		 gwi_func_arg(gwi, "float", "id");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_smoothdelay = gwi_mk_type(gwi, "Smoothdelay", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_smoothdelay, smoothdelay_ctor, smoothdelay_dtor))
	gwi_func_ini(gwi, "void", "init", smoothdelay_init);
		 gwi_func_arg(gwi, "float", "maxdel");
		 gwi_func_arg(gwi, "int", "interp");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "feedback", smoothdelay_get_feedback);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "feedback", smoothdelay_set_feedback);
		 gwi_func_arg(gwi, "float", "feedback");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "del", smoothdelay_get_del);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "del", smoothdelay_set_del);
		 gwi_func_arg(gwi, "float", "del");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_spa = gwi_mk_type(gwi, "Spa", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_spa, spa_ctor, spa_dtor))
	gwi_func_ini(gwi, "void", "init", spa_init);
		 gwi_func_arg(gwi, "string", "filename");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_sparec = gwi_mk_type(gwi, "Sparec", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_sparec, sparec_ctor, sparec_dtor))
	gwi_func_ini(gwi, "void", "init", sparec_init);
		 gwi_func_arg(gwi, "string", "filename");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_streson = gwi_mk_type(gwi, "Streson", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_streson, streson_ctor, streson_dtor))
	gwi_func_ini(gwi, "float", "freq", streson_get_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "freq", streson_set_freq);
		 gwi_func_arg(gwi, "float", "freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "fdbgain", streson_get_fdbgain);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "fdbgain", streson_set_fdbgain);
		 gwi_func_arg(gwi, "float", "fdbgain");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_switch = gwi_mk_type(gwi, "Switch", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_switch, switch_ctor, switch_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tabread = gwi_mk_type(gwi, "Tabread", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tabread, tabread_ctor, tabread_dtor))
	gwi_func_ini(gwi, "void", "init", tabread_init);
		 gwi_func_arg(gwi, "ftbl", "ft");
		 gwi_func_arg(gwi, "float", "mode");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "index", tabread_get_index);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "index", tabread_set_index);
		 gwi_func_arg(gwi, "float", "index");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "offset", tabread_get_offset);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "offset", tabread_set_offset);
		 gwi_func_arg(gwi, "float", "offset");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "wrap", tabread_get_wrap);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "wrap", tabread_set_wrap);
		 gwi_func_arg(gwi, "float", "wrap");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tadsr = gwi_mk_type(gwi, "Tadsr", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tadsr, tadsr_ctor, tadsr_dtor))
	gwi_func_ini(gwi, "float", "atk", tadsr_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", tadsr_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", tadsr_get_dec);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dec", tadsr_set_dec);
		 gwi_func_arg(gwi, "float", "dec");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "sus", tadsr_get_sus);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "sus", tadsr_set_sus);
		 gwi_func_arg(gwi, "float", "sus");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", tadsr_get_rel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", tadsr_set_rel);
		 gwi_func_arg(gwi, "float", "rel");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_talkbox = gwi_mk_type(gwi, "Talkbox", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_talkbox, talkbox_ctor, talkbox_dtor))
	gwi_func_ini(gwi, "float", "quality", talkbox_get_quality);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "quality", talkbox_set_quality);
		 gwi_func_arg(gwi, "float", "quality");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tblrec = gwi_mk_type(gwi, "Tblrec", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tblrec, tblrec_ctor, tblrec_dtor))
	gwi_func_ini(gwi, "void", "init", tblrec_init);
		 gwi_func_arg(gwi, "ftbl", "bar");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tbvcf = gwi_mk_type(gwi, "Tbvcf", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tbvcf, tbvcf_ctor, tbvcf_dtor))
	gwi_func_ini(gwi, "float", "fco", tbvcf_get_fco);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "fco", tbvcf_set_fco);
		 gwi_func_arg(gwi, "float", "fco");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", tbvcf_get_res);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", tbvcf_set_res);
		 gwi_func_arg(gwi, "float", "res");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dist", tbvcf_get_dist);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dist", tbvcf_set_dist);
		 gwi_func_arg(gwi, "float", "dist");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "asym", tbvcf_get_asym);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "asym", tbvcf_set_asym);
		 gwi_func_arg(gwi, "float", "asym");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tdiv = gwi_mk_type(gwi, "Tdiv", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tdiv, tdiv_ctor, tdiv_dtor))
	gwi_func_ini(gwi, "float", "num", tdiv_get_num);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "num", tdiv_set_num);
		 gwi_func_arg(gwi, "float", "num");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "offset", tdiv_get_offset);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "offset", tdiv_set_offset);
		 gwi_func_arg(gwi, "float", "offset");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tenv = gwi_mk_type(gwi, "Tenv", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tenv, tenv_ctor, tenv_dtor))
	gwi_func_ini(gwi, "float", "atk", tenv_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", tenv_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "hold", tenv_get_hold);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "hold", tenv_set_hold);
		 gwi_func_arg(gwi, "float", "hold");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", tenv_get_rel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", tenv_set_rel);
		 gwi_func_arg(gwi, "float", "rel");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tenv2 = gwi_mk_type(gwi, "Tenv2", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tenv2, tenv2_ctor, tenv2_dtor))
	gwi_func_ini(gwi, "float", "atk", tenv2_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", tenv2_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", tenv2_get_rel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", tenv2_set_rel);
		 gwi_func_arg(gwi, "float", "rel");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tenvx = gwi_mk_type(gwi, "Tenvx", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tenvx, tenvx_ctor, tenvx_dtor))
	gwi_func_ini(gwi, "float", "atk", tenvx_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", tenvx_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "hold", tenvx_get_hold);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "hold", tenvx_set_hold);
		 gwi_func_arg(gwi, "float", "hold");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", tenvx_get_rel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", tenvx_set_rel);
		 gwi_func_arg(gwi, "float", "rel");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tgate = gwi_mk_type(gwi, "Tgate", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tgate, tgate_ctor, tgate_dtor))
	gwi_func_ini(gwi, "float", "time", tgate_get_time);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "time", tgate_set_time);
		 gwi_func_arg(gwi, "float", "time");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_thresh = gwi_mk_type(gwi, "Thresh", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_thresh, thresh_ctor, thresh_dtor))
	gwi_func_ini(gwi, "float", "thresh", thresh_get_thresh);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "thresh", thresh_set_thresh);
		 gwi_func_arg(gwi, "float", "thresh");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "mode", thresh_get_mode);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "mode", thresh_set_mode);
		 gwi_func_arg(gwi, "int", "mode");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_timer = gwi_mk_type(gwi, "Timer", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_timer, timer_ctor, timer_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tin = gwi_mk_type(gwi, "Tin", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tin, tin_ctor, tin_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tone = gwi_mk_type(gwi, "Tone", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tone, tone_ctor, tone_dtor))
	gwi_func_ini(gwi, "float", "hp", tone_get_hp);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "hp", tone_set_hp);
		 gwi_func_arg(gwi, "float", "hp");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_trand = gwi_mk_type(gwi, "Trand", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_trand, trand_ctor, trand_dtor))
	gwi_func_ini(gwi, "float", "min", trand_get_min);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "min", trand_set_min);
		 gwi_func_arg(gwi, "float", "min");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", trand_get_max);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "max", trand_set_max);
		 gwi_func_arg(gwi, "float", "max");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tseg = gwi_mk_type(gwi, "Tseg", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tseg, tseg_ctor, tseg_dtor))
	gwi_func_ini(gwi, "void", "init", tseg_init);
		 gwi_func_arg(gwi, "float", "ibeg");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "end", tseg_get_end);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "end", tseg_set_end);
		 gwi_func_arg(gwi, "float", "end");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", tseg_get_dur);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "dur", tseg_set_dur);
		 gwi_func_arg(gwi, "float", "dur");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "type", tseg_get_type);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "type", tseg_set_type);
		 gwi_func_arg(gwi, "float", "type");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_tseq = gwi_mk_type(gwi, "Tseq", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_tseq, tseq_ctor, tseq_dtor))
	gwi_func_ini(gwi, "void", "init", tseq_init);
		 gwi_func_arg(gwi, "ftbl", "ft");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "shuf", tseq_get_shuf);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "int", "shuf", tseq_set_shuf);
		 gwi_func_arg(gwi, "int", "shuf");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_vdelay = gwi_mk_type(gwi, "Vdelay", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_vdelay, vdelay_ctor, vdelay_dtor))
	gwi_func_ini(gwi, "void", "init", vdelay_init);
		 gwi_func_arg(gwi, "float", "maxdel");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "del", vdelay_get_del);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "del", vdelay_set_del);
		 gwi_func_arg(gwi, "float", "del");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_voc = gwi_mk_type(gwi, "Voc", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_voc, voc_ctor, voc_dtor))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_vocoder = gwi_mk_type(gwi, "Vocoder", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_vocoder, vocoder_ctor, vocoder_dtor))
	gwi_func_ini(gwi, "float", "atk", vocoder_get_atk);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "atk", vocoder_set_atk);
		 gwi_func_arg(gwi, "float", "atk");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", vocoder_get_rel);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rel", vocoder_set_rel);
		 gwi_func_arg(gwi, "float", "rel");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bwratio", vocoder_get_bwratio);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "bwratio", vocoder_set_bwratio);
		 gwi_func_arg(gwi, "float", "bwratio");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_waveset = gwi_mk_type(gwi, "Waveset", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_waveset, waveset_ctor, waveset_dtor))
	gwi_func_ini(gwi, "void", "init", waveset_init);
		 gwi_func_arg(gwi, "float", "ilen");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rep", waveset_get_rep);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rep", waveset_set_rep);
		 gwi_func_arg(gwi, "float", "rep");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_wpkorg35 = gwi_mk_type(gwi, "Wpkorg35", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_wpkorg35, wpkorg35_ctor, wpkorg35_dtor))
	gwi_func_ini(gwi, "float", "cutoff", wpkorg35_get_cutoff);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "cutoff", wpkorg35_set_cutoff);
		 gwi_func_arg(gwi, "float", "cutoff");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", wpkorg35_get_res);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "res", wpkorg35_set_res);
		 gwi_func_arg(gwi, "float", "res");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "saturation", wpkorg35_get_saturation);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "saturation", wpkorg35_set_saturation);
		 gwi_func_arg(gwi, "float", "saturation");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	const Type t_zitarev = gwi_mk_type(gwi, "Zitarev", SZ_INT, t_ugen);
	CHECK_BB(gwi_class_ini(gwi, t_zitarev, zitarev_ctor, zitarev_dtor))
	gwi_func_ini(gwi, "float", "in_delay", zitarev_get_in_delay);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "in_delay", zitarev_set_in_delay);
		 gwi_func_arg(gwi, "float", "in_delay");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lf_x", zitarev_get_lf_x);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "lf_x", zitarev_set_lf_x);
		 gwi_func_arg(gwi, "float", "lf_x");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rt60_low", zitarev_get_rt60_low);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rt60_low", zitarev_set_rt60_low);
		 gwi_func_arg(gwi, "float", "rt60_low");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rt60_mid", zitarev_get_rt60_mid);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "rt60_mid", zitarev_set_rt60_mid);
		 gwi_func_arg(gwi, "float", "rt60_mid");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "hf_damping", zitarev_get_hf_damping);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "hf_damping", zitarev_set_hf_damping);
		 gwi_func_arg(gwi, "float", "hf_damping");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "eq1_freq", zitarev_get_eq1_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "eq1_freq", zitarev_set_eq1_freq);
		 gwi_func_arg(gwi, "float", "eq1_freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "eq1_level", zitarev_get_eq1_level);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "eq1_level", zitarev_set_eq1_level);
		 gwi_func_arg(gwi, "float", "eq1_level");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "eq2_freq", zitarev_get_eq2_freq);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "eq2_freq", zitarev_set_eq2_freq);
		 gwi_func_arg(gwi, "float", "eq2_freq");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "eq2_level", zitarev_get_eq2_level);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "eq2_level", zitarev_set_eq2_level);
		 gwi_func_arg(gwi, "float", "eq2_level");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mix", zitarev_get_mix);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "mix", zitarev_set_mix);
		 gwi_func_arg(gwi, "float", "mix");
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "level", zitarev_get_level);
	CHECK_BB(gwi_func_end(gwi, 0))
	gwi_func_ini(gwi, "float", "level", zitarev_set_level);
		 gwi_func_arg(gwi, "float", "level");
	CHECK_BB(gwi_func_end(gwi, 0))
	CHECK_BB(gwi_class_end(gwi))

	return 1;
}
