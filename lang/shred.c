#include <libgen.h>
#include "defs.h"
#include "err_msg.h"
//#include "func.h"
#include "import.h"
#include "shreduler.h"

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
  s->is_running = 0;
  s->is_done = 1;
  for(i = 0; i < vector_size(&shred->gc1); i++)
    release((M_Object)vector_at(&shred->gc1, i), shred);
}

static MFUN(vm_shred_id) {
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = s ? s->xid : -1;
}

static MFUN(vm_shred_is_running) {
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = s ? s->is_running : 0;
}

static MFUN(vm_shred_is_done) {
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = s ? s->is_done : 0;
}

static MFUN(shred_yield) {
  VM_Shred  s = ME(o);
  Shreduler sh = shred->vm_ref->shreduler;
  shreduler_remove(sh, s, 0);
  shredule(sh, s, get_now(sh) + .5);
}

static SFUN(vm_shred_from_id) {
  VM_Shred s = (VM_Shred)vector_at(&shred->vm_ref->shred, *(m_uint*)MEM(SZ_INT) - 1);
  if(!s)
    RETURN->d.v_uint = 0;
  else {
    RETURN->d.v_uint = (m_uint)s->me;
    s->me->ref++;
    vector_add(&shred->gc, (vtype) s->me);
  }
}

static MFUN(shred_args) {
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = s->args ? vector_size(s->args) : 0;
}

static MFUN(shred_arg) {
  m_str str;
  VM_Shred  s = ME(o);
  if(!s->args) {
    RETURN->d.v_uint = 0;
    return;
  }
  str = (m_str)vector_at(s->args, *(m_uint*)MEM(SZ_INT));
  RETURN->d.v_uint = str ? (m_uint)new_String(shred, str) : 0;
}

static MFUN(shred_path) {
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = (m_uint)new_String(shred, s->code->filename);
}

static MFUN(shred_dir) {
  VM_Shred  s = ME(o);
  char c[strlen(s->code->filename) + 1];
  memset(c, 0, strlen(s->code->filename) + 1);
  strncpy(c, s->code->filename, strlen(s->code->filename));
  RETURN->d.v_uint = (m_uint)new_String(shred, dirname(c));
}

static DTOR(shred_dtor) {
  release(o, shred);
}

m_bool import_shred(Env env) {
  DL_Func fun;

  CHECK_BB(import_class_begin(env, &t_shred, env->global_nspc, NULL, shred_dtor))

  o_shred_me = import_var(env, "int", "@me", ae_flag_member, NULL);
  CHECK_BB(o_shred_me)

  dl_func_init(&fun, "void", "exit", (m_uint)vm_shred_exit);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "running", (m_uint)vm_shred_is_running);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "done", (m_uint)vm_shred_is_done);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "id", (m_uint)vm_shred_id);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "Shred", "fromId", (m_uint)vm_shred_from_id);
  dl_func_add_arg(&fun, "int", "arg1");
  CHECK_BB(import_fun(env, &fun, ae_flag_static))

  dl_func_init(&fun, "void", "yield", (m_uint)shred_yield);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "int", "args", (m_uint)shred_args);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "arg", (m_uint)shred_arg);
  dl_func_add_arg(&fun, "int", "n");
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "path", (m_uint)shred_path);
  CHECK_BB(import_fun(env, &fun, 0))

  dl_func_init(&fun, "string", "dir", (m_uint)shred_dir);
  CHECK_BB(import_fun(env, &fun, 0))

  CHECK_BB(import_class_end(env))
  return 1;
}
