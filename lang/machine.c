#include <string.h>
#include <dirent.h>
#ifdef USE_DOUBLE
#undef USE_DOUBLE
#endif
#include <soundpipe.h> // for sp_rand
#include "defs.h"
#include "absyn.h"
#include "import.h"
#include "shreduler.h"
#include "compile.h"
#include "lang.h"

#include "bbq.h"  // for sp_rand also
#include <err_msg.h>
struct Type_ t_machine   = { "Machine",      0, NULL, te_machine};

static m_str randstring(VM* vm, int length)
{
  char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
  size_t stringLen = 26 * 2 + 10 + 2;
  char *randomString;

  randomString = malloc(sizeof(char) * (length + 1));

  if (!randomString) {
    return (char*)0;
  }

  for (int n = 0; n < length; n++) {
    unsigned int key = sp_rand(vm->bbq->sp) % stringLen;
    randomString[n] = string[key];
  }

  randomString[length] = '\0';

  return randomString;
}

static SFUN(machine_add)
{
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  if(!obj)
	return;
  m_str str = STRING(obj);
  release(obj, shred);
  if(!str)
    return;
  RETURN->d.v_uint = compile(shred->vm_ref, str);
}

static SFUN(machine_check)
{
  char c[104];
  m_str prefix, filename;
  M_Object prefix_obj = *(M_Object*)(shred->mem + SZ_INT);
  M_Object code_obj = *(M_Object*)(shred->mem + SZ_INT * 2);
  if(!prefix_obj)
    prefix = ".";
  else {
    prefix = STRING(prefix_obj);
    release(prefix_obj, shred);
  }
  if(!code_obj) {
    RETURN->d.v_uint = 0;
    return;
  }
  filename = randstring(shred->vm_ref, 12);
  sprintf(c, "%s/%s", prefix, filename);
  FILE* file = fopen(c, "w");
  fprintf(file, "%s\n", STRING(code_obj));
  release(code_obj, shred);
  fclose(file);
  free(filename);
  Ast ast = parse(c);
  if(!ast) {
    RETURN->d.v_uint = 0;
    return;
  }
  m_str s = strdup(c);
  if(type_engine_check_prog(shred->vm_ref->env, ast, s) < 0) {
    RETURN->d.v_uint = 0;
    return;
  }
  free(s);
  free_Ast(ast); // it could be in 'type_engine_check_prog'
//  RETURN->d.v_uint = (m_uint)new_String(shred,c);
  RETURN->d.v_uint = 1;
}

static SFUN(machine_compile)
{

  char c[104];
  m_str prefix, filename;
  M_Object prefix_obj = *(M_Object*)(shred->mem + SZ_INT);
  M_Object code_obj = *(M_Object*)(shred->mem + SZ_INT * 2);
  if(!prefix_obj)
    prefix = ".";
  else {
    prefix = STRING(prefix_obj);
    release(prefix_obj, shred);
  }
  if(!code_obj) {
    RETURN->d.v_uint = 0;
    return;
  }
  filename = randstring(shred->vm_ref, 12);
  sprintf(c, "%s/%s.gw", prefix, filename);
  FILE* file = fopen(c, "w");
  fprintf(file, "%s\n", STRING(code_obj));
  release(code_obj, shred);
  fclose(file);
  free(filename);
  compile(shred->vm_ref, c);
}

static SFUN(machine_shreds)
{
  int i;
  VM* vm = shred->vm_ref;
  VM_Shred sh;
  M_Object obj = new_M_Array(SZ_INT, vector_size(vm->shred), 1);
  for(i = 0; i < vector_size(vm->shred); i++) {
    sh = (VM_Shred)vector_at(vm->shred, i);
    i_vector_set(obj->d.array, i, sh->xid);
  }
  vector_append(shred->gc, (vtype)obj);
  RETURN->d.v_object = obj;
}

m_bool import_machine(Env env)
{
  DL_Func* fun;

  CHECK_BB(add_global_type(env, &t_machine))
  CHECK_OB(import_class_begin(env, &t_machine, env->global_nspc, NULL, NULL))
  env->class_def->doc = "access the virtual machine, including docs";

  fun = new_DL_Func("void",  "add",     (m_uint)machine_add);
  dl_func_add_arg(fun,       "string",  "filename");
  CHECK_OB(import_sfun(env,  fun))

  fun = new_DL_Func("int[]", "shreds", (m_uint)machine_shreds);
  CHECK_OB(import_sfun(env,  fun))

  fun = new_DL_Func("int",  "check",     (m_uint)machine_check);
  dl_func_add_arg(fun,       "string",  "prefix");
  dl_func_add_arg(fun,       "string",  "code");
  CHECK_OB(import_sfun(env,  fun))

  fun = new_DL_Func("void",  "compile",     (m_uint)machine_compile);
  dl_func_add_arg(fun,       "string",  "prefix");
  dl_func_add_arg(fun,       "string",  "filename");
  CHECK_OB(import_sfun(env,  fun))

  CHECK_BB(import_class_end(env))
  return 1;
}
