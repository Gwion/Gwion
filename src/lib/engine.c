#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "import.h"
#include "importer.h"
#include "lang_private.h"
#include "emit.h"

struct Type_ t_void      = { "void",       0,      NULL,        te_void};
struct Type_ t_function  = { "@function",  SZ_INT, NULL,        te_function };
struct Type_ t_func_ptr  = { "@func_ptr",  SZ_INT, &t_function, te_func_ptr};
struct Type_ t_class     = { "@Class",     SZ_INT, NULL,        te_class };
struct Type_ t_gack      = { "@Gack",      SZ_INT, NULL,        te_gack };
struct Type_ t_union     = { "@Union",     SZ_INT, &t_object,   te_union };

Type check_exp_call1(Env env, Exp exp_func, Exp args, Func *m_func);
m_bool emit_exp_binary_ptr(Emitter emit, Exp rhs);
OP_CHECK(opck_fptr_at);

static OP_CHECK(opck_func_call) {
  Exp_Binary* bin = (Exp_Binary*)data;
  return check_exp_call1(env, bin->rhs, bin->lhs, &bin->func);
}

static OP_EMIT(opem_fptr_at) {
  Exp_Binary* bin = (Exp_Binary*)data;
  return emit_exp_binary_ptr(emit, bin->rhs);
}
#include "func.h"
static OP_CHECK(opck_fptr_cast) {
  Exp_Cast* cast = (Exp_Cast*)data;
  Type t = cast->self->type;
  Value v = nspc_lookup_value1(env->curr, cast->exp->d.exp_primary.d.var);
  Func  f = isa(v->m_type, &t_func_ptr) > 0 ?
            v->m_type->d.func :
            nspc_lookup_func1(env->curr, insert_symbol(v->name));
  CHECK_BO(compat_func(t->d.func->def, f->def, f->def->pos))
  cast->func = f;
  return t;
}

OP_CHECK(opck_basic_cast) {
  Exp_Cast* cast = (Exp_Cast*)data;
  return cast->self->type;
}

OP_EMIT(opem_basic_cast) {
  return 1;
}
static m_bool import_core_libs(Importer importer) {
  CHECK_BB(importer_add_type(importer, &t_void))
  CHECK_BB(importer_add_type(importer, &t_null))
  CHECK_BB(importer_add_type(importer, &t_now))
  CHECK_BB(importer_add_type(importer, &t_function))
  CHECK_BB(importer_add_type(importer, &t_func_ptr))
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
  CHECK_BB(import_ptr(importer))
  CHECK_BB(importer_oper_ini(importer, (m_str)OP_ANY_TYPE, "@function", NULL))
  CHECK_BB(importer_oper_add(importer, opck_func_call))
  CHECK_BB(importer_oper_end(importer, op_chuck, NULL))
  CHECK_BB(importer_oper_ini(importer, "@function", "@func_ptr", NULL))
  CHECK_BB(importer_oper_add(importer, opck_fptr_at))
  CHECK_BB(importer_oper_emi(importer, opem_fptr_at))
  CHECK_BB(importer_oper_end(importer, op_at_chuck, NULL))
  CHECK_BB(importer_oper_add(importer, opck_fptr_cast))
  CHECK_BB(importer_oper_emi(importer, opem_basic_cast))
  CHECK_BB(importer_oper_end(importer, op_dollar, NULL))
  return 1;
}

static m_bool import_other_libs(Importer importer) {
  importer->env->type_xid = te_last;
  CHECK_BB(import_pair(importer))
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
exit(2);
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
  CHECK_OO((vm->emit = new_emitter(env)))
  vm->emit->filename = "[builtin]";
  struct Importer_ importer = { vm->emit, env };
   if(import_core_libs(&importer) < 0 ||
      import_other_libs(&importer) < 0 ) {
    free_env(env);
    return NULL;
  }
  nspc_commit(env->global_nspc);
  // user nspc
  /*  env->curr = env->user_nspc = new_nspc("[user]");*/
  /*  env->user_nspc->parent = env->global_nspc;*/
  add_plugs(&importer, plug_dirs);
  return env;
}
