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

struct PlugHandle {
  MemPool mp;
  PlugInfo* pi;
  void *dl;
  m_str file;
  m_str name;
};

ANN static struct Plug_* new_plug(MemPool p, const modini ini) {
  struct Plug_ *plug = mp_calloc(p, Plug);
  plug->ini  = ini;
  return plug;
}

ANN static void plug_import(struct PlugHandle *h) {
  const import imp = DLSYM(h->dl, import, GWIMPORT_NAME);
  if(imp)
    vector_add(&h->pi->vec[GWPLUG_IMPORT], (vtype)imp);
}

ANN static void plug_module(struct PlugHandle *h) {
  const modini ini = DLSYM(h->dl, modini, GWMODINI_NAME);
  if(ini) {
    struct Plug_ *plug = new_plug(h->mp, ini);
    plug->name = h->name;
    plug->end  = DLSYM(h->dl, modend, GWMODEND_NAME);
    vector_add(&h->pi->vec[GWPLUG_MODULE], (vtype)plug);
  }
}

ANN static void plug_driver(struct PlugHandle *h) {
  const f_bbqset drv = DLSYM(h->dl, f_bbqset, GWDRIVER_NAME);
  if(drv)
    map_set(&h->pi->drv, (vtype)h->name, (vtype)drv);
}

ANN static inline m_str plug_name(struct PlugHandle *h) {
  const modstr str = DLSYM(h->dl, modstr, GWMODSTR_NAME);
  return str ? str() : NULL;
}

ANN static void plug_get(struct PlugHandle *h, const m_str c) {
  void* dl = DLOPEN(c, RTLD_LAZY | RTLD_GLOBAL);
  if(dl) {
    vector_add(&h->pi->vec[GWPLUG_DL], (vtype)dl);
    h->name = plug_name(h);
    plug_import(h);
    plug_module(h);
    plug_driver(h);
  } else
    gw_err(_("error in %s."), DLERROR());
}

ANN static void plug_get_all(struct PlugHandle *h, const m_str name) {
#ifndef BUILD_ON_WINDOWS
  glob_t results;
  if(glob(name, 0, NULL, &results))
    return;
  for(m_uint i = 0; i < results.gl_pathc; i++)
    plug_get(h, results.gl_pathv[i]);
  globfree(&results);
#else
  WIN32_FIND_DATA filedata;
  HANDLE file = FindFirstFileA(name,&filedata);
  if(file == INVALID_HANDLE_VALUE)
    return;
  do plug_get(&h, filedata.cFileName);
  while(FindNextFile(file,&filedata) == 0);
  FindClose(file);
#endif
}

ANN2(1) static void* pp_ini(const struct Gwion_ *gwion, const Vector v) {
  pparg_run(gwion->ppa, v);
  return NULL;
}

ANN static void register_pp(const struct PlugHandle* h) {
  struct Plug_ *plug = new_plug(h->mp, pp_ini);
  plug->name = "pp";
  vector_add(&h->pi->vec[GWPLUG_MODULE], (vtype)plug);
}

ANN PlugInfo* new_pluginfo(MemPool p, const Vector list) {
  PlugInfo *pi = (PlugInfo*)mp_calloc(p, PlugInfo);
  for(m_uint i = 0; i < GWPLUG_LAST; ++i)
    vector_init(&pi->vec[i]);
  map_init(&pi->drv);
  struct PlugHandle h = { .mp=p, .pi=pi };
  register_pp(&h);
  for(m_uint i = 0; i < vector_size(list); i++) {
    const m_str dir = (m_str)vector_at(list, i);
    char name[strlen(dir) + 6];
    sprintf(name, "%s/*.so", dir);
    plug_get_all(&h, name);
  }
  return pi;
}

ANN static void plug_free_module(const struct Gwion_* gwion, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); ++i) {
    struct Plug_ *plug = (struct Plug_*)vector_at(v, i);
    if(plug->end)
      plug->end(gwion, plug->self);
    mp_free(gwion->mp, Plug, plug);
  }
}

ANN static inline void plug_free_dls(const Vector v) {
  for(m_uint i = 0; i < vector_size(v); ++i)
    DLCLOSE((void*)vector_at(v, i));
}

void free_plug(const struct Gwion_ *gwion) {
  PlugInfo *p = gwion->data->plug;
  struct Vector_ * const v = p->vec;
  plug_free_module(gwion, &v[GWPLUG_MODULE]);
  plug_free_dls(&v[GWPLUG_DL]);
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
      return arg ? split_args(p, arg+1) : NULL;
    }
  }
  return NULL;
}

ANN static void plug_free_arg(MemPool p, const Vector v) {
  for(m_uint i = 0; i < vector_size(v); ++i)
    free_mstr(p, (m_str)vector_at(v, i));
  free_vector(p, v);
}

ANN void plug_run(const struct Gwion_ *gwion, const Vector args) {
  const Vector v = &gwion->data->plug->vec[GWPLUG_MODULE];
  for(m_uint i = 0; i < vector_size(v); ++i) {
    struct Plug_ *plug = (struct Plug_*)vector_at(v, i);
    const Vector arg = get_arg(gwion->mp, plug->name, args);
    plug->self = plug->ini(gwion, arg);
    if(arg)
      plug_free_arg(gwion->mp, arg);
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
