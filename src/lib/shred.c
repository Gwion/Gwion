#include <string.h>
#include <libgen.h>
#include "absyn.h"
#include "type.h"
#include "instr.h"
#include "import.h"

m_int o_shred_me;

M_Object new_shred(VM* vm, VM_Shred shred) {
  M_Object obj = new_M_Object(NULL);
  initialize_object(obj, t_shred);
  ME(obj) = shred;
  return obj;
}

static MFUN(gw_shred_exit) {
  VM_Shred  s = ME(o);
  shred->next_pc = vector_size(s->code->instr) -2;
}

static MFUN(vm_shred_id) {
  VM_Shred  s = ME(o);
  *(m_uint*)RETURN = s ? s->xid : -1;
}

static MFUN(vm_shred_is_running) {
  *(m_uint*)RETURN = ME(o) ? 1 : 0;
}

static MFUN(vm_shred_is_done) {
  *(m_uint*)RETURN = ME(o) ? 0 : 1;
}

static MFUN(shred_yield) {
  VM_Shred  s = ME(o);
  Shreduler sh = shred->vm_ref->shreduler;
  shreduler_remove(sh, s, 0);
  shredule(sh, s, .5);
}

static SFUN(vm_shred_from_id) {
  VM_Shred s = (VM_Shred)vector_at(&shred->vm_ref->shred, *(m_uint*)MEM(SZ_INT) - 1);
  if(!s)
    *(m_uint*)RETURN = 0;
  else {
    *(m_uint*)RETURN = (m_uint)s->me;
    s->me->ref++;
    vector_add(&shred->gc, (vtype) s->me);
  }
}

static MFUN(shred_args) {
  VM_Shred  s = ME(o);
  *(m_uint*)RETURN = s->args ? vector_size(s->args) : 0;
}

static MFUN(shred_arg) {
  m_str str;
  VM_Shred  s = ME(o);
  if(!s->args) {
    *(m_uint*)RETURN = 0;
    return;
  }
  str = (m_str)vector_at(s->args, *(m_uint*)MEM(SZ_INT));
  *(m_uint*)RETURN = str ? (m_uint)new_String(shred, str) : 0;
}

static MFUN(shred_path) {
  VM_Shred  s = ME(o);
  m_str str = code_name(s->code->name, 1);
  *(m_uint*)RETURN = (m_uint)new_String(shred, str);
}

static MFUN(shred_dir) {
  VM_Shred  s = ME(o);
  m_str str = code_name(s->code->name, 1);
  size_t len = strlen(str);
  char c[len + 1];
  memset(c, 0, len + 1);
  strncpy(c, str, len);
  *(m_uint*)RETURN = (m_uint)new_String(shred, dirname(c));
}

static DTOR(shred_dtor) {
  release(o, shred);
}

m_bool import_shred(Gwi gwi) {
  CHECK_OB((t_shred = gwi_mk_type(gwi, "Shred", SZ_INT, t_object)))
  SET_FLAG((t_shred), ae_flag_abstract);
  CHECK_BB(gwi_class_ini(gwi,  t_shred, NULL, shred_dtor))

	gwi_item_ini(gwi, "int", "@me");
  o_shred_me = gwi_item_end(gwi, ae_flag_member, NULL);
  CHECK_BB(o_shred_me)

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
