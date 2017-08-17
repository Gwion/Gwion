#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "defs.h"
#include "map.h"
#include "err_msg.h"
#include "vm.h"
#include "env.h"
static int so_filter(const struct dirent* dir) {
  return strstr(dir->d_name, ".so") ? 1 : 0;
}

static void handle_plug(Env env, m_str c) {
  void* handler;
  if((handler = dlopen(c, RTLD_LAZY))) {
    m_bool(*import)(Env) = (m_bool(*)(Env))(intptr_t)dlsym(handler, "import");
    if(import) {
      if(import(env) > 0)
        vector_add(&vm->plug, (vtype)handler);
      else {
        env->class_def = (Type)vector_pop(&env->class_stack);
        env->curr = (Nspc)vector_pop(&env->nspc_stack);
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

void add_plugs(Env env, Vector plug_dirs) {
  m_uint i;
  for(i = 0; i < vector_size(plug_dirs); i++) {
    m_str dirname = (m_str)vector_at(plug_dirs, i);
    struct dirent **namelist;
    int n;
    n = scandir(dirname, &namelist, so_filter, alphasort);
    if(n > 0) {
      while(n--) {
        char c[strlen(dirname) + strlen(namelist[n]->d_name) + 2];
        sprintf(c, "%s/%s", dirname, namelist[n]->d_name);
        handle_plug(env, c);
        free(namelist[n]);
      }
      free(namelist);
    }
  }
}
