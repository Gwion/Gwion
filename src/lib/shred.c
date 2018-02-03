#include <string.h>
#include <libgen.h>
#include "absyn.h"
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_shred      = { "Shred",      sizeof(m_uint), &t_object, te_shred};
m_int o_shred_me;

M_Object new_shred(VM* vm, VM_Shred shred) {
  M_Object obj = new_M_Object(NULL);
  initialize_object(obj, &t_shred);
  ME(obj) = shred;
  return obj;
}

static MFUN(vm_shred_exit) {
  m_uint i;
  VM_Shred  s = ME(o);
  for(i = 0; i < vector_size(&s->gc1); i++)
    release((M_Object)vector_at(&s->gc1, i), s);
  release(o, s);
  s->me = NULL;
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
  *(m_uint*)RETURN = (m_uint)new_String(shred, s->code->filename);
}

static MFUN(shred_dir) {
  VM_Shred  s = ME(o);
  char c[strlen(s->code->filename) + 1];
  memset(c, 0, strlen(s->code->filename) + 1);
  strncpy(c, s->code->filename, strlen(s->code->filename));
  *(m_uint*)RETURN = (m_uint)new_String(shred, dirname(c));
}

static DTOR(shred_dtor) {
  release(o, shred);
}

m_bool import_shred(Importer importer) {
  SET_FLAG((&t_shred), ae_flag_abstract);
  CHECK_BB(importer_class_ini(importer,  &t_shred, NULL, shred_dtor))

	importer_item_ini(importer, "int", "@me");
  o_shred_me = importer_item_end(importer, ae_flag_member, NULL);
  CHECK_BB(o_shred_me)

  importer_func_ini(importer, "void", "exit", (m_uint)vm_shred_exit);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "int", "running", (m_uint)vm_shred_is_running);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "int", "done", (m_uint)vm_shred_is_done);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "int", "id", (m_uint)vm_shred_id);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "Shred", "fromId", (m_uint)vm_shred_from_id);
  importer_func_arg(importer, "int", "arg1");
  CHECK_BB(importer_func_end(importer, ae_flag_static))

  importer_func_ini(importer, "void", "yield", (m_uint)shred_yield);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "int", "args", (m_uint)shred_args);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "string", "arg", (m_uint)shred_arg);
  importer_func_arg(importer, "int", "n");
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "string", "path", (m_uint)shred_path);
  CHECK_BB(importer_func_end(importer, 0))

  importer_func_ini(importer, "string", "dir", (m_uint)shred_dir);
  CHECK_BB(importer_func_end(importer, 0))

  CHECK_BB(importer_class_end(importer))
  return 1;
}
