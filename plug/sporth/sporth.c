#include <string.h>
#include "defs.h"
#include "type.h"
#include "lang.h"
#include "import.h"
#include "bbq.h"
#include "soundpipe.h"
#include "ugen.h"
#include "sporth.h"

typedef struct sporthData {
    m_float var;
    int parsed;
    sp_data *sp;
    plumber_data pd;
    SPFLOAT in;
} sporthData;

struct Type_ t_gworth = { "Sporth", SZ_INT, &t_ugen };

static int sporth_chuck_in(sporth_stack *stack, void *ud)
{
	plumber_data *pd = (plumber_data *) ud;
	sporthData * data = (sporthData *) pd->ud;
	switch(pd->mode) {
		case PLUMBER_CREATE:
#ifdef DEBUG_MODE
			fprintf(stderr, "CHUCK IN: creating\n");
#endif
			plumber_add_ugen(pd, SPORTH_IN, NULL);
			sporth_stack_push_float(stack, 0);
      break;

		case PLUMBER_INIT:
#ifdef DEBUG_MODE
			fprintf(stderr, "CHUCK IN: initialising.\n");
#endif
			sporth_stack_push_float(stack, 0);
			break;

		case PLUMBER_COMPUTE:
			sporth_stack_push_float(stack, data->in);
			break;

		case PLUMBER_DESTROY:
#ifdef DEBUG_MODE
			fprintf(stderr, "CHUCK IN: destroying.\n");
#endif
			break;

		default:
			fprintf(stderr, "Unknown mode!\n");
			break;
	}
	return PLUMBER_OK;
}

TICK(sporth_tick)
{
	sporthData * data = u->ug;
	data->in= u->in;
	plumber_compute(&data->pd, PLUMBER_COMPUTE);
	u->out = sporth_stack_pop_float(&data->pd.sporth.stack);
	return 1;
}
CTOR(sporth_ctor)
{
#ifdef DEBUG_MODE
	fprintf(stderr, "Creating Sporth\n");
#endif
	sporthData * data = malloc(sizeof(sporthData));
	data->parsed = 0;
	data->in = 0;
	data->sp = shred->vm_ref->bbq->sp;

	plumber_register(&data->pd);
	data->pd.sporth.flist[SPORTH_IN - SPORTH_FOFFSET].func = sporth_chuck_in;
	plumber_init(&data->pd);
	data->pd.sp = data->sp;
	data->pd.ud = data;
	o->ugen->tick = sporth_tick;
	assign_ugen(o->ugen, 1, 1, 0, data);
}

DTOR(sporth_dtor)
{
	sporthData * data = (sporthData*)o->ugen->ug;
	if(data)
	{
#ifdef DEBUG_MODE
		fprintf(stderr, "Destroying Sporth\n");
#endif
		plumber_clean(&data->pd);
		sp_destroy(&data->sp);
    free(data);
   }
}

MFUN(sporth_setp)
{
	sporthData * data = (sporthData*)o->ugen->ug;
	int i = *(m_uint*)(shred->mem + SZ_INT);
	m_float val = *(m_float*)(shred->mem + SZ_INT*2);
	data->pd.p[i] = val;
	RETURN->v_float = val;
}

MFUN(sporth_set_table)
{
	sporthData * data = (sporthData*)o->ugen->ug;
	int i = *(m_uint*)(shred->mem + SZ_INT);
	m_float val = *(m_float*)(shred->mem + SZ_INT*2);
	const char * cstr = STRING(*(M_Object*)(shred->mem + SZ_INT*2 + SZ_FLOAT));
	char* ftname = strndup(cstr, (strlen(cstr) + 1));

  int err = 0;
	sp_ftbl *ft;

	if(plumber_ftmap_search(&data->pd, ftname, &ft) == PLUMBER_NOTOK) {
		fprintf(stderr, "Could not find ftable %s", ftname);
		err++;
	}
	if(i > ft->size) {
		fprintf(stderr, "Stack overflow! %d exceeds table size %d\n", i, ft->size);
		err++;
	}
	if(!err) {
		ft->tbl[i] = val;
	}
	free(ftname);
	RETURN->v_float = val;
}

MFUN(sporth_get_table)
{
	sporthData * data = (sporthData*)o->ugen->ug;
	int i = *(m_uint*)(shred->mem + SZ_INT);
  sp_ftbl *ft;
	const char * cstr = STRING(*(M_Object*)(shred->mem + SZ_INT));
	char* ftname = strndup(cstr, (strlen(cstr) + 1));
  int err = 0;

	if(plumber_ftmap_search(&data->pd, ftname, &ft) == PLUMBER_NOTOK) {
		fprintf(stderr, "Could not find ftable %s", ftname);
		err++;
	}
	if(i > ft->size) {
		fprintf(stderr, "Stack overflow! %d exceeds table size %d\n", i, ft->size);
		err++;
	}
	free(ftname);
	if(!err) {
  	RETURN->v_float = ft->tbl[i];
  } else {
		RETURN->v_float = 0;
	}
}

MFUN(sporth_getp)
{
	sporthData * data = (sporthData*)o->ugen->ug;
	int i = *(m_uint*)(shred->mem + SZ_INT);
  RETURN->v_float = data->pd.p[i];
}

MFUN(sporth_parse_string)
{
	sporthData * data = (sporthData*)o->ugen->ug;
	const char * cstr = STRING(*(M_Object*)(shred->mem + SZ_INT));
	char* str = strndup(cstr, (strlen(cstr) + 1));
	if(!data->parsed) {
  	data->parsed = 1;
    plumber_parse_string(&data->pd, str);
    plumber_compute(&data->pd, PLUMBER_INIT);
	} else {
		plumber_recompile_string(&data->pd, str);
  }
  free(str);
  RETURN->v_float = data->var;
}

MFUN(sporth_parse_file)
{
	sporthData * data = (sporthData*)o->ugen->ug;
	const char * cstr = STRING(*(M_Object*)(shred->mem + SZ_INT));
	char* str = strndup(cstr, (strlen(cstr) + 1));

	if(plumber_open_file(&data->pd, str) == PLUMBER_OK) {
		if(!data->parsed) {
			data->parsed = 1;
			plumber_parse(&data->pd);
			plumber_compute(&data->pd, PLUMBER_INIT);
		} else {
    	plumber_recompile(&data->pd);
   }
   plumber_close_file(&data->pd);
   free(str);
   RETURN->v_float = data->var;
	}
}

IMPORT
{
	DL_Func* fun;
  CHECK_BB(add_global_type(env, &t_gworth))
  CHECK_BB(import_class_begin(env, &t_gworth, env->global_nspc, sporth_ctor, sporth_dtor))

	fun = new_DL_Func("float", "p", (m_uint)sporth_setp);
		dl_func_add_arg(fun, "int", "index");
		dl_func_add_arg(fun, "float", "val");
	CHECK_OB(import_mfun(env, fun))

	fun = new_DL_Func("float", "p", (m_uint)sporth_getp);
		dl_func_add_arg(fun, "int", "index");
	CHECK_OB(import_mfun(env, fun))

	fun = new_DL_Func("float", "t", (m_uint)sporth_set_table);
		dl_func_add_arg(fun, "int", "index");
		dl_func_add_arg(fun, "float", "val");
		dl_func_add_arg(fun, "string", "table");
	CHECK_OB(import_mfun(env, fun))

	fun = new_DL_Func("float", "t", (m_uint)sporth_get_table);
		dl_func_add_arg(fun, "int", "index");
		dl_func_add_arg(fun, "string", "table");
	CHECK_OB(import_mfun(env, fun))

	fun = new_DL_Func("string", "parse", (m_uint)sporth_parse_string);
		dl_func_add_arg(fun, "string", "arg");
	CHECK_OB(import_mfun(env, fun))

	fun = new_DL_Func("string", "parsefile", (m_uint)sporth_parse_file);
		dl_func_add_arg(fun, "string", "arg");
	CHECK_OB(import_mfun(env, fun))

	CHECK_BB(import_class_end(env))
	return 1;
}
