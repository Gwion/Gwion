#include <string.h>
#include <libgen.h>
#include "type.h"
#include "instr.h"
#include "import.h"

M_Object new_shred(const VM_Shred shred) {
  const M_Object obj = new_object(NULL, t_shred);
  ME(obj) = shred;
  return obj;
}

static MFUN(gw_shred_exit) {
  const VM_Shred s = ME(o);
  vm_shred_exit(s);
}

static MFUN(vm_shred_id) {
  const VM_Shred s = ME(o);
  *(m_int*)RETURN = s ? (m_int)s->xid : -1;
}

static MFUN(vm_shred_is_running) {
  const VM_Shred s = ME(o);
  *(m_uint*)RETURN = (s->next || s->prev) ? 1 : 0;
}

static MFUN(vm_shred_is_done) {
  *(m_uint*)RETURN = ME(o) ? 0 : 1;
}

static MFUN(shred_yield) {
  const VM_Shred s = ME(o);
  const Shreduler sh = shred->vm->shreduler;
  shredule(sh, s, .5);
}

static SFUN(vm_shred_from_id) {
  const m_int index =  *(m_int*)MEM(SZ_INT);
  const VM_Shred s = (VM_Shred)vector_at(&shred->vm->shred, index);
  if(s) {
    *(M_Object*)RETURN = s->me;
    s->me->ref++;
    vector_add(&shred->gc, (vtype) s->me);
  } else
    *(m_uint*)RETURN = 0;
}

static MFUN(shred_args) {
  const VM_Shred s = ME(o);
  *(m_uint*)RETURN = s->args ? vector_size(s->args) : 0;
}

static MFUN(shred_arg) {
  const VM_Shred s = ME(o);
  const m_int idx = *(m_int*)MEM(SZ_INT);
  if(s->args && idx >= 0) {
    const m_str str = (m_str)vector_at(s->args, *(m_uint*)MEM(SZ_INT));
    *(M_Object*)RETURN = str ? new_string(shred, str) : NULL;
  } else
    *(M_Object*)RETURN = NULL;
}

#define describe_path_and_dir(name, src) \
static MFUN(shred##name##_path) { \
  const VM_Shred s = ME(o); \
  const m_str str = code_name((src), 1); \
  *(m_uint*)RETURN = (m_uint)new_string(shred, str); \
} \
static MFUN(shred##name##_dir) { \
  const VM_Shred  s = ME(o); \
  const m_str str = code_name((src), 1); \
  const size_t len = strlen(str); \
  char c[len + 1]; \
  strcpy(c, str); \
  *(m_uint*)RETURN = (m_uint)new_string(shred, dirname(c)); \
}
describe_path_and_dir(, s->name)
describe_path_and_dir(_code, s->code->name)

GWION_IMPORT(shred) {
  CHECK_OB((t_shred = gwi_mk_type(gwi, "Shred", SZ_INT, t_object)))
  SET_FLAG((t_shred), ae_flag_abstract);
  CHECK_BB(gwi_class_ini(gwi,  t_shred, NULL, NULL))

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
  return 1;
}
