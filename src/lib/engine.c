#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "err_msg.h"
#include "type.h"
#include "import.h"
#include "importer.h"
#include "lang_private.h"

static m_bool import_core_libs(Importer importer) {
  CHECK_BB(importer_add_type(importer, &t_void))
  CHECK_BB(importer_add_type(importer, &t_null))
  CHECK_BB(importer_add_type(importer, &t_now))
  CHECK_BB(import_int(importer))
  CHECK_BB(import_float(importer))
  CHECK_BB(import_complex(importer))
  CHECK_BB(import_vec3(importer))
  CHECK_BB(import_vec4(importer))
  CHECK_BB(import_object(importer))
  CHECK_BB(import_vararg(importer))
  CHECK_BB(import_string(importer))
  CHECK_BB(import_shred(importer))
  CHECK_BB(import_event(importer))
  CHECK_BB(import_ugen(importer))
  CHECK_BB(import_array(importer))
  return 1;
}

static m_bool import_other_libs(Importer importer) {
  importer->env->type_xid = te_last;
  CHECK_BB(import_fileio(importer))
  CHECK_BB(import_std(importer))
  CHECK_BB(import_math(importer))
  CHECK_BB(import_machine(importer))
  CHECK_BB(import_soundpipe(importer))
  CHECK_BB(import_modules(importer))
  return 1;
}

static int so_filter(const struct dirent* dir) {
  return strstr(dir->d_name, ".so") ? 1 : 0;
}

static void handle_plug(Importer importer, m_str c) {
  void* handler;
  if((handler = dlopen(c, RTLD_LAZY))) {
    m_bool(*import)(Importer) = (m_bool(*)(Importer))(intptr_t)dlsym(handler, "import");
    if(import) {
      if(import(importer) > 0) {
        vector_add(&vm->plug, (vtype)handler);
        nspc_commit(importer->env->curr);
      } else {
        env_pop_class(importer->env);
        dlclose(handler);
       }
    } else {
      const char* err = dlerror();
      if(err_msg(TYPE_, 0, "%s: no import function.", err) < 0)
        dlclose(handler);
     }
  } else {
    const char* err = dlerror();
    if(err_msg(TYPE_, 0, "error in %s.", err) < 0){}
   }
}

static void add_plugs(Importer importer, Vector plug_dirs) {
  m_uint i;
   for(i = 0; i < vector_size(plug_dirs); i++) {
    m_str dirname = (m_str)vector_at(plug_dirs, i);
    struct dirent **namelist;
    int n = scandir(dirname, &namelist, so_filter, alphasort);
    if(n > 0) {
      while(n--) {
        char c[strlen(dirname) + strlen(namelist[n]->d_name) + 2];
        sprintf(c, "%s/%s", dirname, namelist[n]->d_name);
        handle_plug(importer, c);
        free(namelist[n]);
       }
      free(namelist);
     }
   }
}

 Env type_engine_init(VM* vm, Vector plug_dirs) {
  Env env = new_env();
  struct Importer_ importer = { env };
   if(import_core_libs(&importer) < 0 ||
      import_other_libs(&importer) < 0 ) {
    free_env(env);
    return NULL;
  }
  nspc_commit(env->global_nspc);
  // user nspc
  /*  env->curr = env->user_nspc = new_nspc("[user]", "[user]");*/
  /*  env->user_nspc->parent = env->global_nspc;*/
  add_plugs(&importer, plug_dirs);
  return env;
}

