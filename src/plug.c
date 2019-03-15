#include <string.h>
#include <dlfcn.h>
#ifndef BUILD_ON_WINDOWS
#include <glob.h>
#else
#include <windows.h>
#endif
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

typedef m_bool (*import)(Gwi);
typedef m_str  (*modstr)(void);
typedef void*  (*modini)(const Gwion, const Vector);
typedef void*  (*modrun)(const Gwion, void*);
typedef void*  (*modend)(const Gwion, void*);

struct Plug_ {
  m_str name;
  modini ini;
  modend end;
  void* self;
};
#define STR_EXPAND(tok) #tok
#define STR(tok) STR_EXPAND(tok)
#ifndef BUILD_ON_WINDOWS
#include <dlfcn.h>
#define DLOPEN(dl, b) dlopen(dl, b)
#define DLSYM(dl, t, a) (t)(intptr_t)dlsym(dl, STR(a));
#define DLCLOSE(dl) dlclose(dl);
#else
#include "window.h"
#define DLOPEN(dl, b) LoadLibrary(dl)
#define DLSYM(dl, t, a) (t)(intptr_t)GetProcAddress(dl, STR(a));
#define DLCLOSE(dl) Freelibrary(dl);
#endif
ANN static void plug_get(PlugInfo* p, const m_str c) {
  void* dl = DLOPEN(c, RTLD_LAZY);
  if(dl) {
    vector_add(&p->vec[GWPLUG_DL], (vtype)dl);
    const import imp = DLSYM(dl, import, GWIMPORT_NAME);
    if(imp)
      vector_add(&p->vec[GWPLUG_IMPORT], (vtype)imp);
    const modini ini = DLSYM(dl, modini, GWMODINI_NAME);
    if(ini) {
      struct Plug_ *plug = mp_alloc(Plug);
      plug->ini  = ini;
      const modstr str = DLSYM(dl, modstr, GWMODSTR_NAME);
      plug->name = str();
      plug->end  = DLSYM(dl, modend, GWMODEND_NAME);
      vector_add(&p->vec[GWPLUG_MODULE], (vtype)plug);
    }
    const f_bbqset drv = DLSYM(dl, f_bbqset, GWDRIVER_NAME);
    if(drv) {
      const modstr str = DLSYM(dl, modstr, GWMODSTR_NAME);
      map_set(&p->drv, (vtype)str(), (vtype)drv);
    }
  } else
    err_msg(0, "error in %s.", dlerror());
}

ANN PlugInfo* new_plug(const Vector list) {
  PlugInfo *p = (PlugInfo*)mp_alloc(PlugInfo);
  for(m_uint i = 0; i < GWPLUG_LAST; ++i)
    vector_init(&p->vec[i]);
  map_init(&p->drv);
  for(m_uint i = 0; i < vector_size(list); i++) {
   const m_str dir = (m_str)vector_at(list, i);
   char gname[strlen(dir) + 6];
   strcpy(gname, dir);
   strcpy(gname + strlen(dir), "/*.so");
#ifndef BUILD_ON_WINDOWS
   glob_t results;
   if(glob(gname, 0, NULL, &results))
     continue;
   for(m_uint i = 0; i < results.gl_pathc; i++)
       plug_get(p, results.gl_pathv[i]);
    globfree(& results);
#else
  WIN32_FIND_DATA filedata;
  HANDLE file = FindFirstFileA(gname,&filedata);
  if(file == INVALID_HANDLE_VALUE)
    continue;
  do puts(filedata.cFileName);//plug_get(p, filedata.cFileName);
  while(FindNextFile(file,&filedata) == 0);
  FindClose(file);
#endif
  }
  return p;
}
void free_plug(const Gwion gwion) {
  PlugInfo *p = gwion->plug;
  struct Vector_ * const v = p->vec;
  for(m_uint i = 0; i < vector_size(&v[GWPLUG_MODULE]); ++i) {
    struct Plug_ *plug = (struct Plug_*)vector_at(&v[GWPLUG_MODULE], i);
    if(plug->end)
      plug->end(gwion, plug->self);
    mp_free(Plug, plug);
  }
  for(m_uint i = 0; i < vector_size(&v[GWPLUG_DL]); ++i)
    DLCLOSE((void*)vector_at(&v[GWPLUG_DL], i));
  for(m_uint i = 0; i < GWPLUG_LAST; ++i)
    vector_release(&v[i]);
  map_release(&p->drv);
  mp_free(PlugInfo, p);
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

void plug_run(const Gwion gwion, const Vector args) {
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
