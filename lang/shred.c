#include <libgen.h>
#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "instr.h"
#include "lang.h"
#include "import.h"

struct Type_ t_shred      = { "Shred",      sizeof(m_uint), &t_object, te_shred};
m_int o_shred_me;

M_Object new_Shred(VM* vm, VM_Shred shred)
{
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_shred);
  ME(obj) = shred;
  return obj;
}

static MFUN(vm_shred_exit)
{
  VM_Shred  s = ME(o);
  s->is_running = 0;
  s->is_done = 1;
}

static MFUN(vm_shred_id)
{
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = s ? s->xid : -1;
}

static MFUN(vm_shred_is_running)
{
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = s ? s->is_running : 0;
}

static MFUN(vm_shred_is_done)
{
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = s ? s->is_done : 0;
}

static MFUN(shred_yield)
{
  VM_Shred  s = ME(o);
  Shreduler sh = shred->vm_ref->shreduler;
  shreduler_remove(sh, s, 0);
  shredule(sh, s, get_now(sh) + .5);
  RETURN->d.v_uint = 1;
}

static SFUN(vm_shred_from_id)
{
  VM_Shred s = (VM_Shred)vector_at(shred->vm_ref->shred, *(m_uint*)(shred->mem + SZ_INT) - 1);
  if(!s)
    RETURN->d.v_uint = 0;
  else {
    RETURN->d.v_uint = (m_uint)s->me;
    s->me->ref++;
  }
}

static MFUN(shred_args)
{
  VM_Shred  s = ME(o);
  RETURN->d.v_uint = s->args ? vector_size(s->args) : 0;
}

static MFUN(shred_arg)
{
  VM_Shred  s = ME(o);
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  STRING(obj) = (m_str)vector_at(s->args, *(m_uint*)(shred->mem + SZ_INT));
  RETURN->d.v_uint = (m_uint)obj;
}

static MFUN(shred_path)
{
  VM_Shred  s = ME(o);
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  STRING(obj) = s->code->filename;
  RETURN->d.v_uint = (m_uint)obj;
}

static MFUN(shred_dir)
{
  VM_Shred  s = ME(o);
  M_Object obj = new_M_Object();
  char c[strlen(s->code->filename) + 1];
  memset(c, 0, strlen(s->code->filename) + 1);
  strncpy(c, s->code->filename, strlen(s->code->filename));
  initialize_object(obj, &t_string);
  STRING(obj) = S_name(insert_symbol(dirname(c)));
  RETURN->d.v_uint = (m_uint)obj;
}

static DTOR(shred_dtor)
{
  release(o, shred);
}

m_bool import_shred(Env env)
{
  DL_Func* fun;
  DL_Value* arg;
  Func f;

  CHECK_BB(add_global_type(env, &t_shred))
  CHECK_OB(import_class_begin(env, &t_shred, env->global_nspc, NULL, shred_dtor))
  env->class_def->doc = "Shred is the type for processes, allowing to handle concurrency";

  o_shred_me = import_mvar(env, "int", "@me",   0, 0, "shred placeholder");
  CHECK_BB(o_shred_me)

  fun = new_DL_Func("void", "exit", (m_uint)vm_shred_exit);
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "make the shred exit";

  fun = new_DL_Func("int", "running", (m_uint)vm_shred_is_running);
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "return 1 if shred is done, 0 otherwise";

  fun = new_DL_Func("int", "done", (m_uint)vm_shred_is_done);
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "return 1 if shred is done, 0 otherwise";

  fun = new_DL_Func("int", "id", (m_uint)vm_shred_id);
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "return shred id";

  fun = new_DL_Func("Shred", "fromId", (m_uint)vm_shred_from_id);
  arg = dl_func_add_arg(fun, "int", "arg1");
  arg->doc = "id of the shred";
  CHECK_OB((f = import_sfun(env, fun)))
  f->doc = "get a shred from id";

  fun = new_DL_Func("int", "yield", (m_uint)shred_yield);
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "let other shreds a chance to compute";

  fun = new_DL_Func("int", "args", (m_uint)shred_args);
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "return shred arguments number";

  fun = new_DL_Func("string", "arg", (m_uint)shred_arg);
  dl_func_add_arg(fun, "int", "n");
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "return shred nth argument";

  fun = new_DL_Func("string", "path", (m_uint)shred_path);
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "return shred basename";

  fun = new_DL_Func("string", "dir", (m_uint)shred_dir);
  CHECK_OB((f = import_mfun(env, fun)))
  f->doc = "return shred dirname";

  CHECK_BB(import_class_end(env))
  return 1;
}
