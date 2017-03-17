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
#include "doc.h"

#include "bbq.h"  // for sp_rand also

#define prepare() \
  Ast ast = NULL; \
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);\
  char* str = STRING(obj);\
  release(obj, shred); \
	if(strcmp(str, "global_context"))\
	{\
	  str = realpath(str, NULL);\
	  if(!str) return;\
	  ast = parse(str);\
	  if(!ast) return;\
	  if(type_engine_check_prog(shred->vm_ref->env, ast, str) < 0) return;\
	}

#define clean() \
  if(ast) \
    free_Ast(ast);

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

static SFUN(machine_doc)
{
//  prepare()
  Ast ast = NULL;
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  char* str = STRING(obj);
  m_bool global = strcmp(str, "global_context") ? 1 : 0;

  if(global) {
    str = realpath(str, NULL);
    if(!str)
      return;
    ast = parse(str);
    if(!ast)
      return;
    if(type_engine_check_prog(shred->vm_ref->env, ast, str) < 0)
      return;
  }

  mkdoc_context(shred->vm_ref->env, str);
  if(ast)
    free_Ast(ast);
  release(obj, shred);
  if(global)
    free(str);
//  clean()
}

static int js_filter(const struct dirent* dir)
{
  return strstr(dir->d_name, ".js") ? 1 : 0;
}

static SFUN(machine_doc_update)
{
  FILE* f, * all;
  char c[strlen(GWION_DOC_DIR) + 15];
  memset(c, 0, strlen(GWION_DOC_DIR) + 15);
  strncpy(c, GWION_DOC_DIR, strlen(GWION_DOC_DIR));
  strncat(c, "/search/all.js", 14);
  all = fopen(c, "w");
  if(!all)
    return;
  struct dirent **namelist;
  char* line = NULL;
  int n;
  ssize_t read;
  size_t len = 0;
  memset(c, 0, strlen(c));
  strncpy(c, GWION_DOC_DIR, strlen(GWION_DOC_DIR));
  strncat(c, "/dat/", 14);
  n = scandir(c, &namelist, js_filter, alphasort);
  fprintf(all, "var searchData = \n[\n");
  if (n > 0) {
    while (n--) {
      char name[strlen(c) + strlen(namelist[n]->d_name) + 1];
      memset(name, 0, strlen(c) + strlen(namelist[n]->d_name) + 1);
      strcat(name, c);
      strcat(name, namelist[n]->d_name);
      f = fopen(name, "r");
      printf("\t'%s', \n", name);
      while((read = getline(&line, &len, f)) != -1)
        fprintf(all, "%s\n", line);
      free(namelist[n]);
      fclose(f);
    }
  }
  free(namelist);
  free(line);
  fprintf(all, "];");
  fclose(all);
}

static SFUN(machine_adept)
{
  prepare()
  mkadt_context(shred->vm_ref->env, str);
}

static m_str randstring(VM* vm, int length)
{
  char *string = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
  size_t stringLen = 26 * 2 + 10 + 2;
  char *randomString;

  randomString = malloc(sizeof(char) * (length + 1));

  if (!randomString) {
    return (char*)0;
  }

  unsigned int key = 0;

  for (int n = 0; n < length; n++) {
    key = sp_rand(vm->bbq->sp) % stringLen;
    randomString[n] = string[key];
  }

  randomString[length] = '\0';

  return randomString;
}

SFUN(machine_check)
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
//  RETURN->d.v_uint = (m_uint)new_String(c);
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

  fun = new_DL_Func("void",  "doc",     (m_uint)machine_doc);
  dl_func_add_arg(fun,       "string",  "context");
  CHECK_OB(import_sfun(env,  fun))

  fun = new_DL_Func("void",  "doc_update",     (m_uint)machine_doc_update);
  CHECK_OB(import_sfun(env,  fun))

  fun = new_DL_Func("void",  "adept",     (m_uint)machine_adept);
  dl_func_add_arg(fun,       "string",  "context");
  CHECK_OB(import_sfun(env,  fun))

//  fun = new_DL_Func("string",  "check",     (m_uint)machine_check);
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
