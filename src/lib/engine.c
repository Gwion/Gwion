#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "value.h"
#include "import.h"
#include "gwi.h"
#include "lang_private.h"
#include "emit.h"

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
  Func  f = isa(v->m_type, t_func_ptr) > 0 ?
            v->m_type->d.func :
            nspc_lookup_func1(env->curr, insert_symbol(v->name));
  CHECK_BO(compat_func(t->d.func->def, f->def))
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

static m_bool import_core_libs(Gwi gwi) {
  CHECK_OB((t_class = gwi_mk_type(gwi, "@Class", SZ_INT, NULL)))
  CHECK_OB((t_void = gwi_mk_type(gwi, "void", 0, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_void))
  CHECK_OB((t_null    = gwi_mk_type(gwi, "@null",  SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_null))
  CHECK_OB((t_function = gwi_mk_type(gwi, "@function", SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_function))
  CHECK_OB((t_func_ptr = gwi_mk_type(gwi, "@func_ptr", SZ_INT, t_function)))
  CHECK_BB(gwi_add_type(gwi, t_func_ptr))
  CHECK_OB((t_gack = gwi_mk_type(gwi, "@Gack", SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_gack))
  CHECK_OB((t_int = gwi_mk_type(gwi, "int", SZ_INT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_int))
  CHECK_OB((t_float = gwi_mk_type(gwi, "float", SZ_FLOAT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_float))
  CHECK_OB((t_dur = gwi_mk_type(gwi, "dur", SZ_FLOAT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_dur))
  CHECK_OB((t_time = gwi_mk_type(gwi, "time", SZ_FLOAT, NULL)))
  CHECK_BB(gwi_add_type(gwi, t_time))
  CHECK_OB((t_now = gwi_mk_type(gwi, "@now", SZ_FLOAT, t_time)))
  CHECK_BB(gwi_add_type(gwi, t_now))
  CHECK_OB((t_union = gwi_mk_type(gwi, "@Union", SZ_INT, t_object)))
  CHECK_BB(import_object(gwi))
  CHECK_BB(import_array(gwi))
  CHECK_BB(gwi_add_type(gwi, t_union))
  CHECK_BB(import_event(gwi))
  CHECK_BB(import_ugen(gwi))
  CHECK_BB(import_ptr(gwi))
  CHECK_BB(gwi_oper_ini(gwi, (m_str)OP_ANY_TYPE, "@function", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_func_call))
  CHECK_BB(gwi_oper_end(gwi, op_chuck, NULL))
  CHECK_BB(gwi_oper_ini(gwi, "@function", "@func_ptr", NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_fptr_at))
  CHECK_BB(gwi_oper_emi(gwi, opem_fptr_at))
  CHECK_BB(gwi_oper_end(gwi, op_at_chuck, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_fptr_cast))
  CHECK_BB(gwi_oper_emi(gwi, opem_basic_cast))
  CHECK_BB(gwi_oper_end(gwi, op_dollar, NULL))
  CHECK_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_spork))
  CHECK_BB(gwi_oper_emi(gwi, opem_spork))
  CHECK_BB(gwi_oper_end(gwi, op_spork, NULL))
  CHECK_BB(gwi_oper_ini(gwi, NULL, (m_str)OP_ANY_TYPE, NULL))
  CHECK_BB(gwi_oper_add(gwi, opck_new))
  CHECK_BB(gwi_oper_emi(gwi, opem_new))
  CHECK_BB(gwi_oper_end(gwi, op_new, NULL))
  CHECK_BB(import_int(gwi))
  CHECK_BB(import_float(gwi))
  CHECK_BB(import_complex(gwi))
  CHECK_BB(import_vec3(gwi))
  CHECK_BB(import_vec4(gwi))
  CHECK_BB(import_vararg(gwi))
  CHECK_BB(import_string(gwi))
  CHECK_BB(import_shred(gwi))
  return 1;
}

static m_bool import_other_libs(Gwi gwi) {
  CHECK_BB(import_pair(gwi))
  CHECK_BB(import_map(gwi))
  CHECK_BB(import_fileio(gwi))
  CHECK_BB(import_std(gwi))
  CHECK_BB(import_math(gwi))
  CHECK_BB(import_machine(gwi))
  CHECK_BB(import_soundpipe(gwi))
  CHECK_BB(import_modules(gwi))
  return 1;
}

static int so_filter(const struct dirent* dir) {
  return strstr(dir->d_name, ".so") ? 1 : 0;
}

static void handle_plug(Gwi gwi, m_str c) {
  void* handler;
  if((handler = dlopen(c, RTLD_LAZY))) {
    m_bool(*import)(Gwi) = (m_bool(*)(Gwi))(intptr_t)dlsym(handler, "import");
    if(import) {
      if(import(gwi) > 0) {
        vector_add(&gwi->vm->plug, (vtype)handler);
        nspc_commit(gwi->env->curr);
      } else { // maybe we should rollback
        env_reset(gwi->env);
        vector_add(&gwi->vm->plug, (vtype)handler);
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

static void add_plugs(Gwi gwi, Vector plug_dirs) {
  m_uint i;
   for(i = 0; i < vector_size(plug_dirs); i++) {
    m_str dirname = (m_str)vector_at(plug_dirs, i);
    struct dirent **namelist;
    int n = scandir(dirname, &namelist, so_filter, alphasort);
    if(n > 0) {
      while(n--) {
        char c[strlen(dirname) + strlen(namelist[n]->d_name) + 2];
        sprintf(c, "%s/%s", dirname, namelist[n]->d_name);
        handle_plug(gwi, c);
        free(namelist[n]);
       }
      free(namelist);
     }
   }
}

Env type_engine_init(VM* vm, const Vector plug_dirs) {
  Env env = new_env();
  CHECK_OO((vm->emit = new_emitter(env)))
  vm->emit->filename = "[builtin]";
  struct Gwi_ gwi = { vm, vm->emit, env };
   if(import_core_libs(&gwi) < 0 ||
      import_other_libs(&gwi) < 0 ) {
    free_env(env);
    return NULL;
  }
  nspc_commit(env->global_nspc);
  // user nspc
  /*  env->curr = env->user_nspc = new_nspc("[user]");*/
  /*  env->user_nspc->parent = env->global_nspc;*/
  add_plugs(&gwi, plug_dirs);
  return env;
}
