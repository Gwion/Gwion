#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <dirent.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "operator.h"
#include "instr.h"
#include "object.h"
#include "import.h"
#include "gwion.h"

static inline int so_filter(const struct dirent* dir) {
  return strstr(dir->d_name, ".so") ? 1 : 0;
}

struct Plug_ {
  m_str name;
  f_gwmodini ini;
  f_gwmodend end;
  void* self;
};

ANN static void handle_plug(PlugInfo v, const m_str c) {
  void* handler = dlopen(c, RTLD_LAZY);
  if(handler) {
    vector_add(&v[GWPLUG_DL], (vtype)handler);
    m_bool (*import)(Gwi) = (m_bool(*)(Gwi))(intptr_t)dlsym(handler, "import");
    if(import)
      vector_add(&v[GWPLUG_IMPORT], (vtype)import);
    const f_gwmodini ini = (f_gwmodini)(intptr_t)dlsym(handler, GWMODINI_NAME);
    if(ini) {
      struct Plug_ *plug = mp_alloc(Plug);
      plug->ini  = ini;
      m_str (*name)() = (m_str(*)())(intptr_t)dlsym(handler, GWMODNAME_NAME);
      plug->name = name();
      plug->end  = (f_gwmodend)(intptr_t)dlsym(handler, GWMODEND_NAME);
      vector_add(&v[GWPLUG_MODULE], (vtype)plug);
    }
  } else
    err_msg(0, "error in %s.", dlerror());
}

void plug_ini(PlugInfo v, Vector list) {
  for(m_uint i = 0; i < GWPLUG_LAST; ++i)
    vector_init(&v[i]);
  for(m_uint i = 0; i < vector_size(list); i++) {
   const m_str dirname = (m_str)vector_at(list, i);
   struct dirent **namelist;
   int n = scandir(dirname, &namelist, so_filter, alphasort);
   if(n > 0) {
     while(n--) {
       char c[strlen(dirname) + strlen(namelist[n]->d_name) + 2];
       sprintf(c, "%s/%s", dirname, namelist[n]->d_name);
       handle_plug(v, c);
       free(namelist[n]);
      }
     free(namelist);
    }
  }
}

void plug_end(const Gwion gwion) {
  struct Vector_ *v = gwion->plug;
  for(m_uint i = 0; i < vector_size(&v[GWPLUG_MODULE]); ++i) {
    struct Plug_ *plug = (struct Plug_*)vector_at(&v[GWPLUG_MODULE], i);
    if(plug->end)
      plug->end(gwion, plug->self);
    mp_free(Plug, plug);
  }
  for(m_uint i = 0; i < vector_size(&v[GWPLUG_DL]); ++i)
    dlclose((void*)vector_at(&v[GWPLUG_DL], i));
  for(m_uint i = 0; i < GWPLUG_LAST; ++i)
    vector_release(&v[i]);
}

ANN static Vector get_arg(const m_str name, const Vector v) {
  const size_t len = strlen(name);
  for(m_uint i = vector_size(v) + 1; --i;) {
    const m_str str = (m_str)vector_at(v, i - 1);
    if(!strncmp(name, str, len)) {
      const m_str arg = strchr(str, '=');
      m_str c, d = strdup(arg+1);
      c = d;
      const Vector args = new_vector();
      while(d)
        vector_add(args, (vtype)strdup(strsep(&d, ",")));
      free(d);
      free(c);
      vector_rem(v, i-1);
      return args;
    }
  }
  return NULL;
}

void module_ini(const Gwion gwion, Vector args) {
  Vector v = &gwion->plug[GWPLUG_MODULE];
  for(m_uint i = 0; i < vector_size(v); ++i) {
    struct Plug_ *plug = (struct Plug_*)vector_at(v, i);
    const Vector arg = get_arg(plug->name, args);
    plug->self = plug->ini(gwion, arg);
    if(arg) {
      for(m_uint i = 0; i < vector_size(arg); ++i)
        xfree((m_str)vector_at(arg, i));
      free_vector(arg);
    }
  }
}
