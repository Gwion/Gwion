#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <dlfcn.h>
#include <ctype.h>
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"
#include "traverse.h"
#include "lang_private.h"
#define CHECK_EB(a) if(!env->class_def) { CHECK_BB(err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end")) }
#define CHECK_EO(a) if(!env->class_def) { CHECK_BO(err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end")) }

typedef struct {
  m_str name;
  m_str type;
} DL_Value;

typedef struct {
  m_str    name;
  m_str    type;
  m_uint   addr;
  m_uint   narg;
  DL_Value args[DLARG_MAX];
} DL_Func;

typedef struct {
  Operator op;
  m_str ret, lhs, rhs;
} DL_Oper;

struct Importer_{
  Env env;
  DL_Func func;
  DL_Oper oper;
  void* addr;
};

static void dl_func_init(DL_Func* a, const m_str t, const m_str n, m_uint addr) {
  a->name = n;
  a->type = t;
  a->addr = addr;
  a->narg = 0;
}

void importer_func_begin(Importer importer, const m_str t, const m_str n, m_uint addr) {
  dl_func_init(&importer->func, t, n, addr);
}

static void dl_func_add_arg(DL_Func* a, const m_str t, const m_str  n) {
  a->args[a->narg].type = t;
  a->args[a->narg++].name = n;
}

void importer_add_arg(Importer importer, const m_str t, const m_str  n) {
  dl_func_add_arg(&importer->func, t, n);
}

static m_bool check_illegal(char* curr, char c, m_uint i) {
  if(isalnum(c) || c == '_' || (i == 1 && c== '@'))
    curr[i - 1] = c;
  else
    return -1;
  return 1;
}

static void path_valid_inner(m_str curr) {
  m_int j, size = strlen(curr);
  for(j = (size / 2) + 1; --j;) {
    char s = curr[j];
    curr[j] = curr[size - j - 1];
    curr[size - j - 1] = s;
  }
}

static m_bool path_valid(ID_List* list, char* path, char* curr, m_uint len) {
  char last = '\0';
  m_uint i;
  for(i = len; --i;) {
    char c = path[i - 1];
    if(c != '.' && check_illegal(curr, c, i) < 0)
      CHECK_BB(err_msg(UTIL_,  0, "illegal character '%c' in path '%s'...", c, path))
    if(c == '.' || i == 1) {

      if((i != 1 && last != '.' && last != '\0') ||
          (i ==  1 && c != '.')) {
        path_valid_inner(curr);
        *list = prepend_id_list(curr, *list, 0);
        memset(curr, 0, len + 1);
      } else
        CHECK_BB(err_msg(UTIL_,  0, "path '%s' must not begin or end with '.'", path))
    }
    last = c;
  }
  return 1;
}

static ID_List str2list(m_str path, m_uint* array_depth) {
  m_uint len = strlen(path);
  ID_List list = NULL;
  m_uint depth = 0;
  char curr[len + 1];
  memset(curr, 0, len + 1);

  while(len > 2 && path[len - 1] == ']' && path[len - 2] == '[') {
    depth++;
    len -= 2;
  }
  if(path_valid(&list, path, curr, len) < 0) {
	if(list)
      free_id_list(list);
    return NULL;
  }
  CHECK_OO(list)
  strncpy(curr, path, len);
  list->xid = insert_symbol(curr);
  *array_depth = depth;
  return list;
}

static m_bool mk_xtor(Type type, m_uint d, e_native_func e) {
  VM_Code* code = e == NATIVE_CTOR ? &type->info->pre_ctor : &type->info->dtor;
  m_str name = type->name;
  m_str filename = e == NATIVE_CTOR ? "[ctor]" : "[dtor]";

  SET_FLAG(type, e == NATIVE_CTOR ? ae_flag_ctor : ae_flag_dtor);
  *code = new_vm_code(NULL, SZ_INT, 1, name, filename);
  (*code)->native_func = (m_uint)d;
  (*code)->native_func_type = e;
  return 1;
}

m_int importer_add_type(Importer importer, Type type) {
  CHECK_BB(env_add_type(importer->env, type))
  return type->xid;
}

static m_int import_class_begin(Env env, Type type, f_xtor pre_ctor, f_xtor dtor) {
  type->info = new_nspc(type->name, "global_nspc");
  type->info->parent = env->curr;
  if(pre_ctor)
    mk_xtor(type, (m_uint)pre_ctor, NATIVE_CTOR);
  if(dtor)
    mk_xtor(type, (m_uint)dtor,     NATIVE_DTOR);
  if(type->parent) {
    type->info->offset = type->parent->obj_size;
    vector_copy2(&type->info->obj_v_table, &type->parent->info->obj_v_table);
  }
  type->owner = env->curr;

  SET_FLAG(type, ae_flag_checked);
  CHECK_BB(env_push_class(env, type))
  return type->xid;
}

m_int importer_class_begin(Importer importer, Type type, f_xtor pre_ctor, f_xtor dtor) {
  if(type->info)
    CHECK_BB(err_msg(TYPE_, 0, "during import: class '%s' already imported...", type->name))
  CHECK_BB(importer_add_type(importer, type))
  return import_class_begin(importer->env, type, pre_ctor, dtor);
}

static m_int import_class_end(Env env) {
  if(!env->class_def)
    CHECK_BB(err_msg(TYPE_, 0, "import: too many class_end called..."))
  env->class_def->obj_size = env->class_def->info->offset;
  CHECK_BB(env_pop_class(env))
  return 1;
}

m_int importer_class_end(Importer importer) {
  return import_class_end(importer->env);
}

static m_int import_var(Env env, const m_str type, const m_str name, ae_flag flag, m_uint* addr) {
  m_uint array_depth = 0;
  ID_List path;

  CHECK_EB(env->class_def)
  if(!(path = str2list(type, &array_depth)))
    CHECK_BB(err_msg(TYPE_, 0, "... during var import '%s.%s'...", env->class_def->name, name))

  Type_Decl t;
  memset(&t, 0, sizeof(Type_Decl));
  t.xid = path;
  t.flag = flag;
  if(array_depth) {
    t.array = new_array_sub(NULL, 0);
    t.array->depth = array_depth;
  }
  struct Var_Decl_ var;
  memset(&var, 0, sizeof(struct Var_Decl_));
  var.xid = insert_symbol(name);
  if(array_depth) {
    var.array = new_array_sub(NULL, 0);
    var.array->depth = array_depth;
  }
  struct Var_Decl_List_ list;
  memset(&list, 0, sizeof(struct Var_Decl_List_));
  list.self = &var;
  struct Exp_ exp;
  memset(&exp, 0, sizeof(struct Exp_));
  exp.exp_type = ae_exp_decl;
  exp.d.exp_decl.type = &t;
  exp.d.exp_decl.list = &list;
  exp.d.exp_decl.is_static = ((flag & ae_flag_static) == ae_flag_static);
  exp.d.exp_decl.self = &exp;
  var.addr = (void *)addr;
  if(traverse_decl(env, &exp.d.exp_decl) < 0)
    var.value->offset = -1;;
  if(array_depth) {
    free_array_sub(t.array);
    free_array_sub(var.array);
  }
  free(path);
  var.value->flag = flag | ae_flag_builtin;
  return var.value->offset;
}

m_int importer_add_var(Importer importer, const m_str type, const m_str name, ae_flag flag, m_uint* addr) {
  return import_var(importer->env, type, name, flag, addr);
}

static Array_Sub make_dll_arg_list_array(Array_Sub array_sub,
  m_uint* array_depth, m_uint array_depth2) {
  m_uint i;
  if(array_depth2)
    *array_depth = array_depth2;
  if(*array_depth) {
    array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < *array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL, 0);
  }
  return array_sub;
}

static Arg_List make_dll_arg_list(DL_Func * dl_fun) {
  Arg_List arg_list    = NULL;
  m_int i = 0;

  for(i = dl_fun->narg + 1; --i; ) {
    m_uint array_depth = 0, array_depth2 = 0;
    Array_Sub array_sub = NULL;
    Type_Decl* type_decl = NULL;
    Var_Decl var_decl    = NULL;
    DL_Value* arg = &dl_fun->args[i-1];
    ID_List type_path2, type_path = str2list(arg->type, &array_depth);
    if(!type_path) {
      if(arg_list)
        free_arg_list(arg_list);
      CHECK_BO(err_msg(TYPE_,  0, "...at argument '%i'...", i + 1))
    }
    type_decl = new_type_decl(type_path, 0, 0);
    if((type_path2 = str2list(arg->name, &array_depth2)))
      free_id_list(type_path2);
    if(array_depth && array_depth2) {
      free_type_decl(type_decl);
      if(arg_list)
        free_arg_list(arg_list);
      CHECK_BO(err_msg(TYPE_, 0, "array subscript specified incorrectly for built-in module"))
    }
    array_sub = make_dll_arg_list_array(array_sub, &array_depth, array_depth2);
    var_decl = new_var_decl(arg->name, array_sub, 0);
    arg_list = new_arg_list(type_decl, var_decl, arg_list, 0);
  }
  return arg_list;
}

static Func_Def make_dll_as_fun(DL_Func * dl_fun, ae_flag flag) {
  Func_Def func_def = NULL;
  ID_List type_path = NULL;
  Type_Decl* type_decl = NULL;
  m_str name = NULL;
  Arg_List arg_list = NULL;
  m_uint i, array_depth = 0;

  flag |= ae_flag_func | ae_flag_builtin;
  if(!(type_path = str2list(dl_fun->type, &array_depth)) ||
      !(type_decl = new_type_decl(type_path, 0, 0)))
    CHECK_BO(err_msg(TYPE_, 0, "...during @ function import '%s' (type)...", dl_fun->name))
  if(array_depth) {
    Array_Sub array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL, 0);
    type_decl = add_type_decl_array(type_decl, array_sub, 0);
  }
  name = dl_fun->name;
  arg_list = make_dll_arg_list(dl_fun);
  func_def = new_func_def(flag, type_decl, name, arg_list, NULL, 0);
  func_def->d.dl_func_ptr = (void*)(m_uint)dl_fun->addr;
  return func_def;
}

static m_int import_fun(Env env, DL_Func * mfun, ae_flag flag) {
  Func_Def func_def;
  CHECK_OB(mfun) // probably deserve an err msg
  CHECK_BB(name_valid(mfun->name));
  CHECK_EB(env->class_def)
  if(mfun->narg >= DLARG_MAX)
    return -1;
  CHECK_OB((func_def = make_dll_as_fun(mfun, flag)))
  if(traverse_func_def(env, func_def) < 0) {
    free_func_def(func_def);
    return -1;
  }
  return 1;
}

m_int importer_add_fun(Importer importer, ae_flag flag) {
  import_fun(importer->env, &importer->func, flag);
  return 1;
}

static Type get_type(Env env, const m_str str) {
  m_uint  depth = 0;
  ID_List list = str2list(str, &depth);
  Type    t = list ? find_type(env, list) : NULL;
  if(list)
    free_id_list(list);
  return t ? (depth ? new_array_type(env, depth, t, env->curr) : t) : NULL;
}

static m_int import_op(Env env, DL_Oper* op,
                const f_instr f, const m_bool global) {
  Type lhs = op->lhs ? get_type(env, op->lhs) : NULL;
  Type rhs = op->rhs ? get_type(env, op->rhs) : NULL;
  Type ret = get_type(env, op->ret);
  return env_add_op(env, op->op, lhs, rhs, ret, f, global);
}

m_int importer_oper_begin(Importer importer, const m_str l, const m_str r, const m_str t) {
  importer->oper.ret = t;
  importer->oper.rhs = r;
  importer->oper.lhs = l;
  return 1;
}

m_int importer_add_op(Importer importer, Operator op, const f_instr f, const m_bool global) {
  importer->oper.op = op;
  return import_op(importer->env, &importer->oper, f, global);
}

m_int importer_add_value(Importer importer, const m_str name, Type type, const m_bool is_const, void* value) {
  return env_add_value(importer->env, name, type, is_const, value);
}

static m_bool  import_libs(Importer importer) {
  CHECK_BB(importer_add_type(importer, &t_void))
  CHECK_BB(importer_add_type(importer, &t_null))
  CHECK_BB(importer_add_type(importer, &t_now))
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
  importer->env->type_xid = te_last;
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

static void handle_plug(Env env, m_str c) {
  void* handler;
  struct Importer_ importer;
  importer.env = env;
  if((handler = dlopen(c, RTLD_LAZY))) {
    m_bool(*import)(Importer) = (m_bool(*)(Importer))(intptr_t)dlsym(handler, "import");
    if(import) {
      if(import(&importer) > 0)
        vector_add(&vm->plug, (vtype)handler);
      else {
        env_pop_class(env);
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
        handle_plug(importer->env, c);
        free(namelist[n]);
      }
      free(namelist);
    }
  }
}

Env type_engine_init(VM* vm, Vector plug_dirs) {
  Env env = new_env();
  struct Importer_ importer;
  importer.env = env;
  if(import_libs(&importer) < 0) {
    free_env(env);
    return NULL;
  }
  nspc_commit(env->global_nspc);
  // user nspc
  /*  env->curr = env->user_nspc = new_nspc("[user]", "[user]");*/
  /*  env->user_nspc->parent = env->global_nspc;*/
  add_plugs(&importer, plug_dirs);
  nspc_commit(env->curr);
  return env;
}

