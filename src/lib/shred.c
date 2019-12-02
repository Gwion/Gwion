#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "gwion_thread.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "shreduler_private.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "specialid.h"
#include "gwi.h"

static m_int o_fork_thread, o_shred_cancel, o_fork_done, o_fork_ev, o_fork_retsize, o_fork_retval, 
  o_fork_orig;

#define FORK_THREAD(o) *(THREAD_TYPE*)(o->data + o_fork_thread)
#define FORK_RETSIZE(o) *(m_int*)(o->data + o_fork_retsize)
#define FORK_RETVAL(o) (o->data + o_fork_retval)
#define FORK_ORIG(o) (*(VM**)((o)->data + o_fork_orig))

VM_Shred new_shred_base(const VM_Shred shred, const VM_Code code) {
  const VM_Shred sh = new_vm_shred(shred->info->mp, code);
  ADD_REF(code)
  sh->base = shred->base;
  return sh;
}

M_Object new_shred(const VM_Shred shred, m_bool is_spork) {
  const M_Object obj = new_object(shred->info->mp, NULL,
    shred->info->vm->gwion->type[is_spork ? et_shred :et_fork]);
  ME(obj) = shred;
  if(!is_spork) {
    *(M_Object*)(obj->data + o_fork_ev) = new_object(shred->info->mp, NULL, shred->info->vm->gwion->type[et_event]);
    EV_SHREDS(*(M_Object*)(obj->data + o_fork_ev)) = new_vector(shred->info->mp);
  }
  return obj;
}

static MFUN(gw_shred_exit) {
  const VM_Shred s = ME(o);
  s->mem -= SZ_INT;
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
  const Shreduler sh = s->tick->shreduler;
  shredule(sh, s, GWION_EPSILON);
}

static SFUN(vm_shred_from_id) {
  const m_int index =  *(m_int*)MEM(0);
  if(index > 0) {
    for(m_uint i = 0; i < vector_size(&shred->tick->shreduler->shreds); ++i) {
      const VM_Shred s = (VM_Shred)vector_at(&shred->tick->shreduler->shreds, i);
      if(s->tick->xid == (m_uint)index) {
        *(M_Object*)RETURN = s->info->me;
        return;
      }
    }
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

#ifndef BUILD_ON_WINDOWS
#define PATH_CHR '/'
#else
#define PATH_CHR '\\'
#endif

#define describe_name(name, src) \
static MFUN(shred##name##_name) { \
  const VM_Shred s = ME(o); \
  const m_str str = code_name((src), 0); \
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->mp, shred, str); \
}
describe_name(, s->info->name)
describe_name(_code, s->code->name)

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
  size_t sz = len;\
  while(sz) {\
    if(c[sz] == PATH_CHR) {\
      c[sz] = 0;\
      break;\
    }\
    --sz;\
  }\
  *(m_uint*)RETURN = (m_uint)new_string(shred->info->mp, shred, c); \
}
describe_path_and_dir(, s->info->name)
describe_path_and_dir(_code, s->code->name)

static DTOR(shred_dtor) {
  if(ME(o))
    free_vm_shred(ME(o));
}

static MFUN(shred_lock) {
  MUTEX_LOCK(ME(o)->tick->shreduler->mutex);
}

static MFUN(shred_unlock) {
  MUTEX_UNLOCK(ME(o)->tick->shreduler->mutex);
}

static DTOR(fork_dtor) {
  THREAD_JOIN(FORK_THREAD(o));
  const VM *vm = FORK_ORIG(o);
  if(*(m_int*)(o->data + o_fork_done)) {
    vector_rem2(&vm->gwion->data->child, (vtype)o);
    if(!vm->gwion->data->child2.ptr)
      vector_init(&vm->gwion->data->child2);
    vector_add(&vm->gwion->data->child2, (vtype)ME(o)->info->vm->gwion);
  }
}

static MFUN(fork_join) {
  MUTEX_LOCK(ME(o)->tick->shreduler->mutex);
  MUTEX_LOCK(shred->tick->shreduler->mutex);
  release(o, shred);
  if(*(m_int*)(o->data + o_fork_done)) {
    MUTEX_UNLOCK(shred->tick->shreduler->mutex);
    MUTEX_UNLOCK(ME(o)->tick->shreduler->mutex);
    return;
  }
  MUTEX_UNLOCK(ME(o)->tick->shreduler->mutex);
  MUTEX_UNLOCK(shred->tick->shreduler->mutex);
  shreduler_remove(shred->tick->shreduler, shred, 0);
  vector_add(EV_SHREDS(*(M_Object*)(o->data + o_fork_ev)), (vtype)shred);
}

static MFUN(shred_cancel) {
  MUTEX_LOCK(ME(o)->tick->shreduler->mutex);
  *(m_int*)(o->data + o_shred_cancel) = *(m_int*)MEM(SZ_INT);
  MUTEX_UNLOCK(ME(o)->tick->shreduler->mutex);
}

static MFUN(shred_test_cancel) {
  MUTEX_LOCK(ME(o)->tick->shreduler->mutex);
  if(*(m_int*)(o->data + o_shred_cancel)) {
    const m_bool is_me = ME(o) == shred;
    if(is_me)
      MUTEX_UNLOCK(ME(o)->tick->shreduler->mutex);
    vm_shred_exit(ME(o));
    if(is_me)
      return;
  }
  MUTEX_UNLOCK(ME(o)->tick->shreduler->mutex);
}

void fork_retval(const M_Object o) {
  const m_uint sz = FORK_RETSIZE(o);
  memcpy(FORK_RETVAL(o), ME(o)->reg - sz, sz);
}

static ANN void* fork_run(void* data) {
  const M_Object me = (M_Object)data;
  VM *vm = ME(me)->info->vm;
  do {
    vm_run(vm);
    ++vm->bbq->pos;
  } while(vm->bbq->is_running);
  fork_retval(me);
  MUTEX_LOCK(vm->shreduler->mutex);
  *(m_int*)(me->data + o_fork_done) = 1;
  broadcast(*(M_Object*)(me->data + o_fork_ev));
  MUTEX_UNLOCK(vm->shreduler->mutex);
  THREAD_RETURN(NULL);
}

ANN void fork_launch(const VM* vm, const M_Object o, const m_uint sz) {
  o->ref += 1;
  if(!vm->gwion->data->child.ptr)
    vector_init(&vm->gwion->data->child);
  vector_add(&vm->gwion->data->child, (vtype)o);
  FORK_ORIG(o) = (VM*)vm;
  FORK_RETSIZE(o) = sz;
  THREAD_CREATE(FORK_THREAD(o), fork_run, o);
}

ANN void fork_clean(const VM_Shred shred, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); ++i) {
    const M_Object o = (M_Object)vector_at(v, i);
    THREAD_JOIN(FORK_THREAD(o));
    release(o, shred);
  }
  vector_release(v);
}

GWION_IMPORT(shred) {
  const Type t_shred = gwi_class_ini(gwi,  "Shred", NULL);
  gwi_class_xtor(gwi, NULL, shred_dtor);
  gwi->gwion->type[et_shred] = t_shred;

  gwi_item_ini(gwi, "@internal", "@me");
  GWI_BB(gwi_item_end(gwi, ae_flag_const, NULL))

  gwi_item_ini(gwi, "int", "cancel");
  GWI_BB((o_shred_cancel = gwi_item_end(gwi, ae_flag_const, NULL)))

  gwi_func_ini(gwi, "void", "exit");
  GWI_BB(gwi_func_end(gwi, gw_shred_exit, ae_flag_none))

  gwi_func_ini(gwi, "int", "running");
  GWI_BB(gwi_func_end(gwi, vm_shred_is_running, ae_flag_none))

  gwi_func_ini(gwi, "int", "done");
  GWI_BB(gwi_func_end(gwi, vm_shred_is_done, ae_flag_none))

  gwi_func_ini(gwi, "int", "id");
  GWI_BB(gwi_func_end(gwi, vm_shred_id, ae_flag_none))

  gwi_func_ini(gwi, "Shred", "fromId");
  gwi_func_arg(gwi, "int", "id");
  GWI_BB(gwi_func_end(gwi, vm_shred_from_id, ae_flag_static))

  gwi_func_ini(gwi, "void", "yield");
  GWI_BB(gwi_func_end(gwi, shred_yield, ae_flag_none))

  gwi_func_ini(gwi, "int", "args");
  GWI_BB(gwi_func_end(gwi, shred_args, ae_flag_none))

  gwi_func_ini(gwi, "string", "arg");
  gwi_func_arg(gwi, "int", "n");
  GWI_BB(gwi_func_end(gwi, shred_arg, ae_flag_none))

  gwi_func_ini(gwi, "string", "name");
  GWI_BB(gwi_func_end(gwi, shred_name, ae_flag_none))

  gwi_func_ini(gwi, "string", "path");
  GWI_BB(gwi_func_end(gwi, shred_path, ae_flag_none))

  gwi_func_ini(gwi, "string", "dir");
  GWI_BB(gwi_func_end(gwi, shred_dir, ae_flag_none))

  gwi_func_ini(gwi, "string", "code_name");
  GWI_BB(gwi_func_end(gwi, shred_code_name, ae_flag_none))

  gwi_func_ini(gwi, "string", "code_path");
  GWI_BB(gwi_func_end(gwi, shred_code_path, ae_flag_none))

  gwi_func_ini(gwi, "string", "code_dir");
  GWI_BB(gwi_func_end(gwi, shred_code_dir, ae_flag_none))

  gwi_func_ini(gwi, "void", "set_cancel");
  gwi_func_arg(gwi, "int", "n");
  GWI_BB(gwi_func_end(gwi, shred_cancel, ae_flag_none))
  gwi_func_ini(gwi, "void", "test_cancel");
  GWI_BB(gwi_func_end(gwi, shred_test_cancel, ae_flag_none))
  gwi_func_ini(gwi, "void", "lock");
  GWI_BB(gwi_func_end(gwi, shred_lock, ae_flag_none))
  gwi_func_ini(gwi, "void", "unlock");
  GWI_BB(gwi_func_end(gwi, shred_unlock, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))

  struct SpecialId_ spid = { .type=t_shred, .exec=RegPushMe, .is_const=1 };
  gwi_specialid(gwi, "me", &spid);

  SET_FLAG((t_shred), abstract);

  const Type t_fork= gwi_class_ini(gwi,  "Fork", "Shred");
  gwi_class_xtor(gwi, NULL, fork_dtor);
  gwi->gwion->type[et_fork] = t_fork;

  gwi_item_ini(gwi, "@internal", "@thread");
  GWI_BB((o_fork_thread = gwi_item_end(gwi, ae_flag_const, NULL)))
  gwi_item_ini(gwi, "int", "is_done");
  GWI_BB((o_fork_done = gwi_item_end(gwi, ae_flag_const, NULL)))
  gwi_item_ini(gwi, "Event", "ev");
  GWI_BB((o_fork_ev = gwi_item_end(gwi, ae_flag_const, NULL)))
  gwi_item_ini(gwi, "int", "retsize");
  GWI_BB((o_fork_retsize = gwi_item_end(gwi, ae_flag_const, NULL)))
  gwi_item_ini(gwi, "@internal", "@orig");
  GWI_BB((o_fork_orig = gwi_item_end(gwi, ae_flag_const, NULL)))
  o_fork_retval = t_fork->nspc->info->offset;
  GWI_BB(gwi_union_ini(gwi, NULL, NULL))
  GWI_BB(gwi_union_add(gwi, "int", "i"))
  GWI_BB(gwi_union_add(gwi, "float", "f"))
  GWI_BB(gwi_union_add(gwi, "Vec3", "v"))
  GWI_BB(gwi_union_add(gwi, "Vec4", "w"))
  GWI_BB(gwi_union_add(gwi, "VarObject", "o"))
  GWI_OB(gwi_union_end(gwi, ae_flag_const))
  gwi_func_ini(gwi, "void", "join");
  GWI_BB(gwi_func_end(gwi, fork_join, ae_flag_none))
  GWI_BB(gwi_class_end(gwi))
  SET_FLAG((t_fork), abstract);
  return GW_OK;
}
