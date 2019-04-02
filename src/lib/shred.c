#include <string.h>
#include <libgen.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_thread.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "shreduler_private.h"
#include "gwion.h"

static m_int o_fork_thread, o_fork_retsize, o_fork_retval;
#define FORK_THREAD(o) *(THREAD_TYPE*)(o->data + o_fork_thread)
#define FORK_RETSIZE(o) *(m_int*)(o->data + o_fork_retsize)
#define FORK_RETVAL(o) (o->data + o_fork_retval)

M_Object new_shred(const VM_Shred shred, m_bool is_spork) {
  const M_Object obj = new_object(shred->info->mp, NULL, is_spork ? t_shred : t_fork);
  ME(obj) = shred;
  if(!is_spork) {
//    *(M_Object*)(obj->data + o_fork_ev) = new_object(NULL, t_event);
//    EV_SHREDS(*(M_Object*)(obj->data + o_fork_ev)) = new_vector();
  }
  return obj;
}

static MFUN(gw_shred_exit) {
  const VM_Shred s = ME(o);
  vm_shred_exit(s);
}

static MFUN(vm_shred_id) {
  const VM_Shred s = ME(o);
  *(m_int*)RETURN = s ? (m_int)s->tick->xid : -1;
}

static MFUN(vm_shred_is_running) {
  const VM_Shred s = ME(o);
  *(m_uint*)RETURN = (s->tick->next || s->tick->prev) ? 1 : 0;
}

static MFUN(vm_shred_is_done) {
  *(m_uint*)RETURN = ME(o) ? 0 : 1;
}

static MFUN(shred_yield) {
  const VM_Shred s = ME(o);
  const Shreduler sh = shred->tick->shreduler;
  shredule(sh, s, .5);
}

static SFUN(vm_shred_from_id) {
  const m_int index =  *(m_int*)MEM(0);
  const VM_Shred s = (VM_Shred)vector_at(&shred->tick->shreduler->shreds, (vtype)index);
  if(s) {
    *(M_Object*)RETURN = s->info->me;
//    s->info->me->ref++;
//    vector_add(&shred->gc, (vtype) s->info->me);
  } else
    *(m_uint*)RETURN = 0;
}

static MFUN(shred_args) {
  const VM_Shred s = ME(o);
  *(m_uint*)RETURN = s->info->args ? vector_size(s->info->args) : 0;
}

static MFUN(shred_arg) {
  const VM_Shred s = ME(o);
  const m_int idx = *(m_int*)MEM(SZ_INT);
  if(s->info->args && idx >= 0) {
    const m_str str = (m_str)vector_at(s->info->args, *(m_uint*)MEM(SZ_INT));
    *(M_Object*)RETURN = str ? new_string(shred->info->mp, shred, str) : NULL;
  } else
    *(m_uint*)RETURN = 0;
}

#define describe_path_and_dir(name, src) \
static MFUN(shred##name##_path) { \
  const VM_Shred s = ME(o); \
  const m_str str = code_name((src), 1); \
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->mp, shred, str); \
} \
static MFUN(shred##name##_dir) { \
  const VM_Shred  s = ME(o); \
  const m_str str = code_name((src), 1); \
  const size_t len = strlen(str); \
  char c[len + 1]; \
  strcpy(c, str); \
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->mp, shred, dirname(c)); \
}
describe_path_and_dir(, s->info->name)
describe_path_and_dir(_code, s->code->name)

static DTOR(shred_dtor) { free_vm_shred(*(VM_Shred*)o->data); }

static DTOR(fork_dtor) {
  mp_free(shred->info->mp, Gwion, ME(o)->info->vm->gwion);
  free_vm(ME(o)->info->vm);
}

static MFUN(fork_join) {
  /* int pret = */ THREAD_JOIN(FORK_THREAD(o));
  release(o, shred);
}

void fork_retval(const M_Object o) {
  const m_uint sz = FORK_RETSIZE(o);
  memcpy(FORK_RETVAL(o), ME(o)->reg - sz, sz);
}

static ANN void* fork_run(void* data) {
  M_Object me = (M_Object)data;
  VM *vm = ME(me)->info->vm;
  vm_run(vm);
  fork_retval(me);
  THREAD_RETURN(NULL);
}

void fork_launch(const M_Object o, const m_uint sz) {
  ++o->ref;
  FORK_RETSIZE(o) = sz;
  THREAD_CREATE(FORK_THREAD(o), fork_run, o);
}

#include "nspc.h"
GWION_IMPORT(shred) {
  CHECK_OB((t_shred = gwi_mk_type(gwi, "Shred", SZ_INT, t_object)))
  CHECK_BB(gwi_class_ini(gwi,  t_shred, NULL, shred_dtor))

  gwi_item_ini(gwi, "int", "@me");
  CHECK_BB(gwi_item_end(gwi, ae_flag_member, NULL))

  gwi_func_ini(gwi, "void", "exit", gw_shred_exit);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "running", vm_shred_is_running);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "done", vm_shred_is_done);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "id", vm_shred_id);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "Shred", "fromId", vm_shred_from_id);
  gwi_func_arg(gwi, "int", "id");
  CHECK_BB(gwi_func_end(gwi, ae_flag_static))

  gwi_func_ini(gwi, "void", "yield", shred_yield);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "args", shred_args);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "arg", shred_arg);
  gwi_func_arg(gwi, "int", "n");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "path", shred_path);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "dir", shred_dir);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "code_path", shred_code_path);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "code_dir", shred_code_dir);
  CHECK_BB(gwi_func_end(gwi, 0))

  CHECK_BB(gwi_class_end(gwi))

  gwi_item_ini(gwi, "Shred", "me");
  gwi_item_end(gwi, ae_flag_const, NULL);
  SET_FLAG((t_shred), abstract);

  CHECK_OB((t_fork = gwi_mk_type(gwi, "Fork", SZ_INT, t_shred)))
  CHECK_BB(gwi_class_ini(gwi, t_fork, NULL, fork_dtor))
  gwi_item_ini(gwi, "int", "@thread");
  CHECK_BB((o_fork_thread = gwi_item_end(gwi, ae_flag_const, NULL)))
  gwi_item_ini(gwi, "int", "retsize");
  CHECK_BB((o_fork_retsize = gwi_item_end(gwi, ae_flag_const, NULL)))
  o_fork_retval = t_fork->nspc->info->offset;
  CHECK_BB(gwi_union_ini(gwi, NULL))
  CHECK_BB(gwi_union_add(gwi, "int", "i"))
  CHECK_BB(gwi_union_add(gwi, "float", "f"))
  CHECK_BB(gwi_union_add(gwi, "Vec3", "v"))
  CHECK_BB(gwi_union_add(gwi, "Vec4", "w"))
  CHECK_BB(gwi_union_add(gwi, "VarObject", "o"))
  CHECK_BB(gwi_union_end(gwi, ae_flag_const))
  gwi_func_ini(gwi, "int", "join", fork_join);
  CHECK_BB(gwi_func_end(gwi, 0))
  CHECK_BB(gwi_class_end(gwi))
  SET_FLAG((t_fork), abstract);
  return GW_OK;
}
