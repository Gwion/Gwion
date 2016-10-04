#include "defs.h"
#include "import.h"
#include "shreduler.h"
#include "compile.h"
#include "lang.h"
#include "doc.h"
#include <string.h>
#include <dirent.h>
#define prepare() \
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);\
  char* str = STRING(obj);\
	if(strcmp(str, "global_context"))\
	{\
	  str = realpath(str, NULL);\
	  if(!str) return;\
	  Ast ast = parse(str);\
	  if(!ast) return;\
	  if(type_engine_check_prog(shred->vm_ref->env, ast, str) < 0) return;\
	}
static void machine_add(DL_Return * RETURN, VM_Shred shred)
{
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str str = STRING(obj);
  M_Object ret = new_M_Object();
  initialize_object(ret, &t_string);
  RETURN->v_uint= compile(shred->vm_ref, str);
}

static void machine_doc(DL_Return * RETURN, VM_Shred shred)
{
  prepare()
  mkdoc_context(shred->vm_ref->env, str);
}

static int js_filter(const struct dirent* dir)
{
  return strstr(dir->d_name, ".js") > 0 ? 1 : 0;
}

static void machine_doc_update(DL_Return * RETURN, VM_Shred shred)
{
  FILE*f, * all  = fopen("/usr/lib/Gwion/doc/search/all.js", "w");
  if(!all)
    return;
  struct dirent **namelist;
  char* line = NULL;
  int n;
  char c[256];
  ssize_t read;
  size_t len = 0;
  n = scandir("/usr/lib/Gwion/doc/dat", &namelist, js_filter, alphasort);
  fprintf(all , "var searchData = \n[\n");
  if (n > 0)
  {
    while (n--)
    {
      char name[128];
      memset(name, 0, 128);
      strcat(name, "/usr/lib/Gwion/doc/dat/");
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

static void machine_adept(DL_Return * RETURN, VM_Shred shred)
{
  prepare()
  mkadt_context(shred->vm_ref->env, str);
}

static void machine_compile(DL_Return * RETURN, VM_Shred shred)
{
	RETURN->v_uint = 0;
  prepare()
	RETURN->v_uint = 1;
}

static void machine_shreds(DL_Return * RETURN, VM_Shred shred)
{
  int i;
  VM* vm = shred->vm_ref;
  VM_Shred sh;
  M_Object obj = new_M_Array(SZ_INT, vector_size(vm->shred));
  for(i = 0; i < vector_size(vm->shred); i++)
  {
    sh = vector_at(vm->shred, i);
    i_vector_set(obj->array, i, sh->xid);
  }
  RETURN->v_object = obj;
}

m_bool import_machine(Env env)
{
  DL_Func* fun;

  CHECK_BB(add_global_type(env, &t_machine))
  CHECK_BB(import_class_begin(env, &t_machine, env->global_nspc, NULL, NULL))
	env->class_def->doc = "access the virtual machine, including docs";

  fun = new_DL_Func("void",  "add",     (m_uint)machine_add);
  dl_func_add_arg(fun,       "string",  "filename");
  CHECK_BB(import_sfun(env,  fun))

  fun = new_DL_Func("int[]", "shreds", (m_uint)machine_shreds);
  CHECK_BB(import_sfun(env,  fun))

  fun = new_DL_Func("void",  "doc",     (m_uint)machine_doc);
  dl_func_add_arg(fun,       "string",  "context");
  CHECK_BB(import_sfun(env,  fun))

  fun = new_DL_Func("void",  "doc_update",     (m_uint)machine_doc_update);
  CHECK_BB(import_sfun(env,  fun))

  fun = new_DL_Func("void",  "adept",     (m_uint)machine_adept);
  dl_func_add_arg(fun,       "string",  "context");
  CHECK_BB(import_sfun(env,  fun))

  fun = new_DL_Func("void",  "compile",     (m_uint)machine_compile);
  dl_func_add_arg(fun,       "string",  "filename");
  CHECK_BB(import_sfun(env,  fun))

  CHECK_BB(import_class_end(env))
  return 1;
}
