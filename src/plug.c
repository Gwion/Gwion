#ifndef BUILD_ON_WINDOWS
  #include <glob.h>
  #include <dlfcn.h>
  #include <limits.h>
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
#include "gwi.h"

typedef m_bool (*plugin)(Gwi);
typedef void *(*modini)(const struct Gwion_ *, const Vector);
typedef void *(*modend)(const struct Gwion_ *, void *);
typedef m_str *(*gwdeps)(void);

struct PlugHandle {
  MemPool mp;
  Map     map;
  size_t  len;
};

typedef struct Plug_ {
  void *dl;
  void *self;
  int   imp;
} * Plug;

ANN static struct Plug_ *new_plug(MemPool p, void *dl) {
  struct Plug_ *plug = mp_calloc(p, Plug);
  plug->dl           = dl;
  return plug;
}

ANN static void plug_get(struct PlugHandle *h, const m_str c) {
  void *       dl    = DLOPEN(c, RTLD_LAZY | RTLD_GLOBAL);
  const m_str  pname = c + h->len + 1;
  const size_t sz    = strlen(pname) - 3;
  char         name[PATH_MAX];
  memcpy(name, pname, sz);
  name[sz] = '\0';
  if (dl) {
    Plug plug = new_plug(h->mp, dl);
    map_set(h->map, (vtype)strdup(name), (vtype)plug);
  } else
    gw_err(_("{+R}error{0} in {/+}%s{0}."), DLERROR());
}

ANN static void plug_get_all(struct PlugHandle *h, const m_str name) {
#ifndef BUILD_ON_WINDOWS
  glob_t results;
  if (glob(name, 0, NULL, &results)) return;
  for (m_uint i = 0; i < results.gl_pathc; i++)
    plug_get(h, results.gl_pathv[i]);
  globfree(&results);
#else
  WIN32_FIND_DATA filedata;
  HANDLE          file = FindFirstFile(name, &filedata);
  if (file == INVALID_HANDLE_VALUE) return;
  do {
    char c[PATH_MAX];
    strcpy(c, name);
    strcpy(c + strlen(name) - 4, filedata.cFileName);
    plug_get(h, c);
  } while (FindNextFile(file, &filedata));
  FindClose(file);
#endif
}

ANN m_bool plug_ini(const struct Gwion_ *gwion, const Vector list) {
  const Map map = &gwion->data->plug;
  map_init(map);
  struct PlugHandle h = {.mp = gwion->mp, .map = map};
  for (m_uint i = 0; i < vector_size(list); i++) {
    const m_str dir = (m_str)vector_at(list, i);
    h.len           = strlen(dir);
    char name[PATH_MAX];
    sprintf(name, "%s/*.so", dir);
    plug_get_all(&h, name);
  }
  return GW_OK;
}

void free_plug(const struct Gwion_ *gwion) {
  const Map map = &gwion->data->plug;
  for (m_uint i = 0; i < map_size(map); ++i) {
    const Plug   plug = (Plug)VVAL(map, i);
    const modend end  = DLSYM(plug->dl, modend, GWMODEND_NAME);
    if (end && plug->self) end(gwion, plug->self);
    free((m_str)VKEY(map, i));
    DLCLOSE(plug->dl);
  }
  map_release(map);
}

ANN static void plug_free_arg(MemPool p, const Vector v) {
  for (m_uint i = 0; i < vector_size(v); ++i)
    free_mstr(p, (m_str)vector_at(v, i));
  free_vector(p, v);
}

ANN void set_module(const struct Gwion_ *gwion, const m_str name,
                    void *const ptr) {
  const Map map = &gwion->data->plug;
  for (m_uint j = 0; j < map_size(map); ++j) {
    if (!strcmp(name, (m_str)VKEY(map, j))) {
      Plug plug  = (Plug)VVAL(map, j);
      plug->self = ptr;
      return;
    }
  }
  const Plug plug  = new_plug(gwion->mp, name);
  plug->self = ptr;
  map_set(map, (m_uint)name, (m_uint)plug);
}

ANN void plug_run(const struct Gwion_ *gwion, const Map mod) {
  const Map map = &gwion->data->plug;
  for (m_uint i = 0; i < map_size(mod); ++i) {
    const m_str name = (m_str)VKEY(mod, i);
    const m_str opt  = (m_str)VVAL(mod, i);
    for (m_uint j = 0; j < map_size(map); ++j) {
      if (!strcmp(name, (m_str)VKEY(map, j))) {
        Plug         plug = (Plug)VVAL(map, j);
        const Vector arg  = opt ? split_args(gwion->mp, opt) : NULL;
        const modini ini  = DLSYM(plug->dl, modini, GWMODINI_NAME);
        plug->self        = ini(gwion, arg);
        if (arg) plug_free_arg(gwion->mp, arg);
      }
    }
  }
}

ANN static m_bool dependencies(struct Gwion_ *gwion, const Plug plug) {
  const gwdeps dep = DLSYM(plug->dl, gwdeps, GWDEPEND_NAME);
  if (dep) {
    m_str *const base = dep();
    m_str *      deps = base;
    while (*deps) {
      CHECK_BB(plugin_ini(gwion, *deps));
      ++deps;
    }
  }
  return GW_OK;
}

ANN static m_bool _plugin_ini(struct Gwion_ *gwion, const m_str iname) {
  const Map map = &gwion->data->plug;
  for (m_uint i = 0; i < map_size(map); ++i) {
    const Plug   plug = (Plug)VVAL(map, i);
    const m_str  base = (m_str)VKEY(map, i);
    const size_t size = strlen(iname);
    char         name[size + 1];
    strcpy(name, iname);
    for (size_t i = 0; i < size; i++) {
      if (name[i] == ':' || name[i] == '[' || name[i] == ']') name[i] = '_';
    }
    if (!strcmp(name, base)) {
      if (plug->imp) return GW_OK;
      const plugin imp = DLSYM(plug->dl, plugin, GWIMPORT_NAME);
      if (!imp) break;
      plug->imp = 1;
      CHECK_BB(dependencies(gwion, plug));
      const m_uint scope = env_push_global(gwion->env);
      const m_str  name  = gwion->env->name;
      gwion->env->name   = iname;
      const m_bool ret   = gwi_run(gwion, imp);
      gwion->env->name   = name;
      env_pop(gwion->env, scope);
      return ret;
    }
  }
  gw_err("no such plugin '%s'\n", iname);
  return GW_ERROR;
}

ANN m_bool plugin_ini(struct Gwion_ *gwion, const m_str iname) {
  const Context ctx   = gwion->env->context;
  gwion->env->context = NULL;
  const m_bool ret    = _plugin_ini(gwion, iname);
  gwion->env->context = ctx;
  return ret;
}

ANN m_bool driver_ini(const struct Gwion_ *gwion) {
  const Map  map   = &gwion->data->plug;
  m_str      dname = gwion->vm->bbq->si->arg;
  m_str      opt   = strchr(dname, '=');
  const char c     = opt ? *opt : '\0';
  if (opt) *opt = '\0';
  for (m_uint i = 0; i < map_size(map); ++i) {
    const m_str name = (m_str)VKEY(map, i);
    if (!strcmp(name, dname)) {
      const Plug     plug = (Plug)VVAL(map, i);
      const f_bbqset drv  = DLSYM(plug->dl, f_bbqset, GWDRIVER_NAME);
      if (!drv) break;
      gwion->vm->bbq->func = drv;
      if (opt) *opt = c;
      return GW_OK;
    }
  }
  gw_err("can't find driver '%s'\n", dname);
  return GW_ERROR;
}

ANN void *get_module(const struct Gwion_ *gwion, const m_str name) {
  const Map map = &gwion->data->plug;
  for (m_uint i = 0; i < map_size(map); ++i) {
    if (!strcmp(name, (m_str)VKEY(map, i))) {
      const Plug plug = (Plug)VVAL(map, i);
      return plug->self;
    }
  }
  return NULL;
}
