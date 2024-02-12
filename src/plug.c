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

struct PlugHandle {
  MemPool mp;
  Map     map;
  size_t  len;
};

ANN static struct Plug_ *new_dl_plug(MemPool p, void *dl, const char *name) {
  Plug plug = new_plug(p);
  plug->dl     = dl;
  char s[256] = { [0] = 'g', [1] = 'w', [8] = '_' };
  strcpy(s + 9, name);
  memcpy(s + 2, "import", 6);
  plug->plugin = DLSYM(plug->dl, gwplugin_t, s);
  memcpy(s + 2, "modini", 6);
  plug->modini = DLSYM(plug->dl, gwmodini_t, s);
  memcpy(s + 2, "modend", 6);
  plug->modend = DLSYM(plug->dl, gwmodend_t, s);
  memcpy(s + 2, "driver", 6);
  plug->driver = DLSYM(plug->dl, gwdriver_t, s);
  memcpy(s + 2, "depend", 6);
  plug->depend = DLSYM(plug->dl, gwdepend_t, s);
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
    Plug plug = new_dl_plug(h->mp, dl, name);
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

ANN bool plug_ini(const struct Gwion_ *gwion, const Vector list) {
  gwion->data->plugs = mp_calloc2(gwion->mp, sizeof(Plugs));
  const Map map = &gwion->data->plugs->map;
  map_init(map);
  vector_init(&gwion->data->plugs->vec);
  struct PlugHandle h = {.mp = gwion->mp, .map = map};
  for (m_uint i = vector_size(list) + 1 ; --i;) {
    const m_str dir = (m_str)vector_at(list, i - 1);
    h.len           = strlen(dir);
    char name[PATH_MAX];
    sprintf(name, "%s/" "*.so", dir);
    plug_get_all(&h, name);
  }
  return true;
}

void free_plug(const Gwion gwion) {
  const Vector vec = &gwion->data->plugs->vec;
  for (m_uint i = vector_size(vec) + 1; --i;) {
    const Nspc nspc = (Nspc)vector_at(vec, i-1);
    nspc_remref(nspc, gwion);
  }
  vector_release(&gwion->data->plugs->vec);
  const Map map = &gwion->data->plugs->map;
  for (m_uint i = 0; i < map_size(map); ++i) {
    const Plug   plug = (Plug)VVAL(map, i);
    const gwmodend_t end  = plug->modend;
    if (end && plug->self) end(gwion, plug->self);
    free((m_str)VKEY(map, i));
    if(plug->dl) DLCLOSE(plug->dl);
  }
  map_release(map);
  mp_free2(gwion->mp, sizeof(Plugs), gwion->data->plugs);
}

ANN static void plug_free_arg(MemPool p, const Vector v) {
  for (m_uint i = 0; i < vector_size(v); ++i)
    free_mstr(p, (m_str)vector_at(v, i));
  free_vector(p, v);
}

ANN bool set_module(const struct Gwion_ *gwion, const m_str name,
                    void *const ptr) {
  const Map map = &gwion->data->plugs->map;
  for (m_uint j = 0; j < map_size(map); ++j) {
    if (!strcmp(name, (m_str)VKEY(map, j))) {
      Plug plug  = (Plug)VVAL(map, j);
      plug->self = ptr;
      return true;
    }
  }
  gw_err("%s: no such module\n", name);
  return false;
}

ANN bool plug_run(const struct Gwion_ *gwion, const Map mod) {
  const Map map = &gwion->data->plugs->map;
  for (m_uint i = 0; i < map_size(mod); ++i) {
    const m_str name = (m_str)VKEY(mod, i);
    const m_str opt  = (m_str)VVAL(mod, i);
    m_uint j;
    for (j = 0; j < map_size(map); ++j) {
      if (!strcmp(name, (m_str)VKEY(map, j))) {
        Plug         plug = (Plug)VVAL(map, j);
        const Vector arg  = opt ? split_args(gwion->mp, opt) : NULL;
        if(!plug->modini) continue;
        plug->self        = plug->modini(gwion, arg);
        if (arg) plug_free_arg(gwion->mp, arg);
        break;
      }
    }
    if(j == map_size(map)) {
      gw_err("%s: no such module\n", name);
      return false;
    }
  }
  return true;
}

ANN static bool dependencies(struct Gwion_ *gwion, const Plug plug, const loc_t loc) {
  const gwdepend_t dep = plug->depend;
  bool ret = true;
  if (dep) {
    m_str *const base = dep();
    m_str *      deps = base;
    while (*deps) {
      if(!plugin_ini(gwion, *deps, loc)) {
        gw_err("%s: no such plugin (dependency)\n", *deps);
        ret = false;
      }
      ++deps;
    }
  }
  return ret;
}

ANN static void plug_name(m_str name, const m_str iname, const m_uint size) {
  strcpy(name, iname);
  for (size_t j = 0; j < size; j++)
    if (name[j] == ':' || name[j] == '[' || name[j] == ']') name[j] = '_';
}

ANN static void set_parent(const Nspc nspc, const Gwion gwion ) {
  const Nspc user = (Nspc)vector_at(&gwion->env->scope->nspc_stack, 1);
  nspc->parent = user->parent;
  user->parent = nspc;
}

ANN static bool start(const Plug plug, const Gwion gwion, const m_str iname, const loc_t loc) {
  if(!plug->plugin) return false;
  const bool cdoc = gwion->data->cdoc;
  gwion->data->cdoc = 0; // check cdoc
  CHECK_B(dependencies(gwion, plug, loc));
  gwion->data->cdoc = cdoc;
  plug->nspc = new_nspc(gwion->mp, iname);
  vector_add(&gwion->data->plugs->vec, (m_uint)plug->nspc);
  set_parent(plug->nspc, gwion);
  const m_uint scope = env_push(gwion->env, NULL, plug->nspc);
  const m_str  name  = gwion->env->name;
  gwion->env->name   = iname;
  const bool ret   = gwi_run(gwion, plug->plugin);
  gwion->env->name   = name;
  env_pop(gwion->env, scope);
  return ret;
}

ANN static bool started(const Plug plug, const Gwion gwion, const m_str iname) {
  Nspc nspc = gwion->env->global_nspc->parent;
  do if(!strcmp(nspc->name, iname))
    return true;
  while((nspc = nspc->parent));
  set_parent(plug->nspc, gwion);
  return true;
}

ANN static bool _plugin_ini(struct Gwion_ *gwion, const m_str iname, const loc_t loc) {
  const Map map = &gwion->data->plugs->map;
  for (m_uint i = 0; i < map_size(map); ++i) {
    const Plug   plug = (Plug)VVAL(map, i);
    const m_str  base = (m_str)VKEY(map, i);
    const size_t size = strlen(iname);
    char         name[size + 1];
    plug_name(name, iname, size);
    if (!strcmp(name, base)) {
      if (!plug->nspc) return start(plug, gwion, iname, loc);
      else return started(plug, gwion, iname);
    }
  }
  return false;
}

ANN bool plugin_ini(struct Gwion_ *gwion, const m_str iname, const loc_t loc) {
  const Env env = gwion->env;
  if(!_plugin_ini(gwion, iname, loc)) {
    if(gwion->env->context && !gwion->env->context->error)
      env_err(env, loc, "%s: no such plugin\n", iname);
    return false;
  }
  return GW_OK;
}

ANN gwdriver_t driver_ini(const struct Gwion_ *gwion, struct SoundInfo_ *si) {
  const Map  map   = &gwion->data->plugs->map;
  m_str      dname = strdup(si->arg);
  m_str      opt   = strchr(dname, '=');
  if (opt) *opt = '\0';
  for (m_uint i = 0; i < map_size(map); ++i) {
    const m_str name = (m_str)VKEY(map, i);
    if (!strcmp(name, dname)) {
      const Plug     plug = (Plug)VVAL(map, i);
      const gwdriver_t drv  = plug->driver;
      free(dname);
      return drv;
    }
  }
  gw_err("%s: no such driver\n", dname);
  free(dname);
  return NULL;
}

ANN void *get_module(const struct Gwion_ *gwion, const m_str name) {
  const Map map = &gwion->data->plugs->map;
  for (m_uint i = 0; i < map_size(map); ++i) {
    if (!strcmp(name, (m_str)VKEY(map, i))) {
      const Plug plug = (Plug)VVAL(map, i);
      return plug->self;
    }
  }
  return NULL;
}
