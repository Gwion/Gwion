#include <string.h>
#include <libgen.h>
#include "absyn.h"
#include "type.h"
#include "instr.h"
#include "import.h"

M_Object new_shred(const VM_Shred shred) {
  const M_Object obj = new_M_Object(NULL);
  initialize_object(obj, t_shred);
  ME(obj) = shred;
  return obj;
}

static MFUN(gw_shred_exit) {
  vm_shred_exit(shred);
}

static MFUN(vm_shred_id) {
  const VM_Shred  s = ME(o);
  *(m_int*)RETURN = s ? (m_int)s->xid : -1;
}

static MFUN(vm_shred_is_running) {
  *(m_uint*)RETURN = ME(o) ? 1 : 0;
}

static MFUN(vm_shred_is_done) {
  *(m_uint*)RETURN = ME(o) ? 0 : 1;
}

static MFUN(shred_yield) {
  const VM_Shred  s = ME(o);
  const Shreduler sh = shred->vm_ref->shreduler;
  shredule(sh, s, .5);
}

static SFUN(vm_shred_from_id) {
  const m_int index =  *(m_uint*)MEM(SZ_INT);
  const VM_Shred s = (VM_Shred)vector_at(&shred->vm_ref->shred, index);
  if(s) {
    *(m_uint*)RETURN = (m_uint)s->me;
    s->me->ref++;
    vector_add(&shred->gc, (vtype) s->me);
  } else
    *(m_uint*)RETURN = 0;
}

static MFUN(shred_args) {
  const VM_Shred  s = ME(o);
  *(m_uint*)RETURN = s->args ? vector_size(s->args) : 0;
}

static MFUN(shred_arg) {
  const VM_Shred s = ME(o);
  const m_int idx = *(m_int*)MEM(SZ_INT);
  if(s->args && idx >= 0) {
    const m_str str = (m_str)vector_at(s->args, *(m_uint*)MEM(SZ_INT));
    *(m_uint*)RETURN = str ? (m_uint)new_String(shred, str) : 0;
  } else
    *(m_uint*)RETURN = 0;
}

static MFUN(shred_path) {
  const VM_Shred  s = ME(o);
  const m_str str = code_name(s->code->name, 1);
  *(m_uint*)RETURN = (m_uint)new_String(shred, str);
}

static MFUN(shred_dir) {
  const VM_Shred  s = ME(o);
  const m_str str = code_name(s->code->name, 1);
  const size_t len = strlen(str);
  char c[len + 1];
  strcpy(c, str);
  *(m_uint*)RETURN = (m_uint)new_String(shred, dirname(c));
}

ANN m_bool import_shred(const Gwi gwi) {
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
  gwi_func_arg(gwi, "int", "arg1");
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

  CHECK_BB(gwi_class_end(gwi))
  return 1;
}

#ifdef JIT
#include "ctrl/shred.h"
#endif
