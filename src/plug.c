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

typedef m_bool (*import)(Gwi);
typedef m_str  (*modstr)(void);
typedef void*  (*modini)(const Gwion, const Vector);
typedef void*  (*modend)(const Gwion, void*);
typedef void*  (*modend)(const Gwion, void*);

struct Plug_ {
  m_str name;
  modini ini;
  modend end;
  void* self;
};
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#define DLFUNC(dl, t, a) (t)(intptr_t)dlsym(dl, STR(a));

ANN static void plug_get(PlugInfo* p, const m_str c) {
  void* dl = dlopen(c, RTLD_LAZY);
  if(dl) {
    vector_add(&p->vec[GWPLUG_DL], (vtype)dl);
    const import imp = DLFUNC(dl, import, GWIMPORT_NAME);
    if(imp)
      vector_add(&p->vec[GWPLUG_IMPORT], (vtype)imp);
    const modini ini = DLFUNC(dl, modini, GWMODINI_NAME);
    if(ini) {
      struct Plug_ *plug = mp_alloc(Plug);
      plug->ini  = ini;
      const modstr str = DLFUNC(dl, modstr, GWMODSTR_NAME);
      plug->name = str();
      plug->end  = DLFUNC(dl, modend, GWMODEND_NAME);
      vector_add(&p->vec[GWPLUG_MODULE], (vtype)plug);
    }
    const f_drvset drv = DLFUNC(dl, f_drvset, GWDRIVER_NAME);
    if(drv) {
      const modstr str = DLFUNC(dl, modstr, GWMODSTR_NAME);
      map_set(&p->drv, (vtype)str(), (vtype)drv);
    }
  } else
    err_msg(0, "error in %s.", dlerror());
}

void plug_discover(PlugInfo* p, Vector list) {
  for(m_uint i = 0; i < GWPLUG_LAST; ++i)
    vector_init(&p->vec[i]);
  map_init(&p->drv);
  for(m_uint i = 0; i < vector_size(list); i++) {
   const m_str dir = (m_str)vector_at(list, i);
   struct dirent **file;
   int n = scandir(dir, &file, so_filter, alphasort);
   if(n > 0) {
     while(n--) {
       char c[strlen(dir) + strlen(file[n]->d_name) + 2];
       sprintf(c, "%s/%s", dir, file[n]->d_name);
       plug_get(p, c);
       free(file[n]);
      }
     free(file);
    }
  }
}

void plug_end(const Gwion gwion) {
  struct Vector_ * const v = gwion->plug->vec;
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
  map_release(&gwion->plug->drv);
}

ANN Vector split_args(const m_str str) {
  const m_str arg = strchr(str, '=');
  m_str d = strdup(arg+1), c = d;
  const Vector args = new_vector();
  while(d)
    vector_add(args, (vtype)strdup(strsep(&d, ",")));
  free(d);
  free(c);
  return args;
}

ANN static Vector get_arg(const m_str name, const Vector v) {
  const size_t len = strlen(name);
  for(m_uint i = vector_size(v) + 1; --i;) {
    const m_str str = (m_str)vector_at(v, i - 1);
    if(!strncmp(name, str, len)) {
      vector_rem(v, i-1);
      return split_args(str);
    }
  }
  return NULL;
}

void plug_ini(const Gwion gwion, const Vector args) {
  const Vector v = &gwion->plug->vec[GWPLUG_MODULE];
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
