#ifndef BUILD_ON_WINDOWS
#include <glob.h>
#include <dlfcn.h>
#else
#include <windows.h>
#endif
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "gwion.h"
#include "operator.h"
#include "instr.h"
#include "object.h"
#include "import.h"

typedef m_bool (*import)(Gwi);
typedef m_str  (*modstr)(void);
typedef void*  (*modini)(const struct Gwion_*, const Vector);
typedef void*  (*modrun)(const struct Gwion_*, void*);
typedef void*  (*modend)(const struct Gwion_*, void*);

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
#define DLERROR() dlerror()
#else
#include "windows.h"
#define DLOPEN(dl, b) LoadLibrary(dl)
#define DLSYM(dl, t, a) (t)(intptr_t)GetProcAddress(dl, STR(a));
#define DLCLOSE(dl) FreeLibrary(dl);
#define DLERROR() "plugin"
#endif

ANN static void plug_get(MemPool p, PlugInfo* pi, const m_str c) {
  void* dl = DLOPEN(c, RTLD_LAZY | RTLD_GLOBAL);
  if(dl) {
    vector_add(&pi->vec[GWPLUG_DL], (vtype)dl);
    const import imp = DLSYM(dl, import, GWIMPORT_NAME);
    if(imp)
      vector_add(&pi->vec[GWPLUG_IMPORT], (vtype)imp);
    const modini ini = DLSYM(dl, modini, GWMODINI_NAME);
    if(ini) {
      struct Plug_ *plug = mp_calloc(p, Plug);
      plug->ini  = ini;
      const modstr str = DLSYM(dl, modstr, GWMODSTR_NAME);
      plug->name = str();
      plug->end  = DLSYM(dl, modend, GWMODEND_NAME);
      vector_add(&pi->vec[GWPLUG_MODULE], (vtype)plug);
    }
    const f_bbqset drv = DLSYM(dl, f_bbqset, GWDRIVER_NAME);
    if(drv) {
      const modstr str = DLSYM(dl, modstr, GWMODSTR_NAME);
      map_set(&pi->drv, (vtype)str(), (vtype)drv);
    }
  } else
    gw_err(_("error in %s."), DLERROR());
}

ANN PlugInfo* new_plug(MemPool p, const Vector list) {
  PlugInfo *pi = (PlugInfo*)mp_calloc(p, PlugInfo);
  for(m_uint i = 0; i < GWPLUG_LAST; ++i)
    vector_init(&pi->vec[i]);
  map_init(&pi->drv);
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
       plug_get(p, pi, results.gl_pathv[i]);
    globfree(& results);
#else
  WIN32_FIND_DATA filedata;
  HANDLE file = FindFirstFileA(gname,&filedata);
  if(file == INVALID_HANDLE_VALUE)
    continue;
  do plug_get(p, pi, filedata.cFileName);
  while(FindNextFile(file,&filedata) == 0);
  FindClose(file);
#endif
  }
  return pi;
}

void free_plug(const struct Gwion_ *gwion) {
  PlugInfo *p = gwion->data->plug;
  struct Vector_ * const v = p->vec;
  for(m_uint i = 0; i < vector_size(&v[GWPLUG_MODULE]); ++i) {
    struct Plug_ *plug = (struct Plug_*)vector_at(&v[GWPLUG_MODULE], i);
    if(plug->end)
      plug->end(gwion, plug->self);
    mp_free(gwion->mp, Plug, plug);
  }
  for(m_uint i = 0; i < vector_size(&v[GWPLUG_DL]); ++i)
    DLCLOSE((void*)vector_at(&v[GWPLUG_DL], i));
  for(m_uint i = 0; i < GWPLUG_LAST; ++i)
    vector_release(&v[i]);
  map_release(&p->drv);
  mp_free(gwion->mp, PlugInfo, p);
}

ANN Vector split_args(MemPool p, const m_str str) {
  const Vector args = new_vector(p);
  m_str d = strdup(str), e = d;
  while(e)
    vector_add(args, (vtype)mstrdup(p, strsep(&e, ",")));
  xfree(d);
  return args;
}

ANN static Vector get_arg(MemPool p, const m_str name, const Vector v) {
  const size_t len = strlen(name);
  for(m_uint i = vector_size(v) + 1; --i;) {
    const m_str str = (m_str)vector_at(v, i - 1);
    if(!strncmp(name, str, len)) {
      vector_rem(v, i-1);
      const m_str arg = strchr(str, '=');
      return arg ? split_args(p, str) : NULL;
    }
  }
  return NULL;
}

void plug_run(const struct Gwion_ *gwion, const Vector args) {
  const Vector v = &gwion->data->plug->vec[GWPLUG_MODULE];
  for(m_uint i = 0; i < vector_size(v); ++i) {
    struct Plug_ *plug = (struct Plug_*)vector_at(v, i);
    const Vector arg = get_arg(gwion->mp, plug->name, args);
    plug->self = plug->ini(gwion, arg);
    if(arg) {
      for(m_uint i = 0; i < vector_size(arg); ++i)
        free_mstr(gwion->mp, (m_str)vector_at(arg, i));
      free_vector(gwion->mp, arg);
    }
  }
}

ANN void* get_module(const struct Gwion_ *gwion, const m_str name) {
  const Vector v = &gwion->data->plug->vec[GWPLUG_MODULE];
  for(m_uint i = 0; i < vector_size(v); ++i) {
    struct Plug_ *plug = (struct Plug_*)vector_at(v, i);
    if(!strcmp(name, plug->name))
      return plug->self;
  }
  return NULL;
}
