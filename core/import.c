#include <string.h>
#include "err_msg.h"
#include "env.h"
#include "func.h"
#include "import.h"
//#include "type.h"

#define CHECK_EB(a) if(!env->class_def) { err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end"); return -1; }
#define CHECK_EO(a) if(!env->class_def) { err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end"); return NULL; }

void free_expression(Exp exp);

m_bool scan1_exp_decl(Env env, Exp_Decl* decl);
m_bool scan2_exp_decl(Env env, Exp_Decl* decl);
Type   check_exp_decl(Env env, Exp_Decl* decl);

m_bool scan1_func_def(Env env, Func_Def f);
m_bool scan2_func_def(Env env, Func_Def f);
m_bool check_func_def(Env env, Func_Def f);


// should be in type_utils
ID_List str2list(m_str path, m_uint* array_depth) {
  m_uint len = 0;
  m_int  i, j;
  ID_List list = NULL;
  char last = '\0';
  m_uint depth = 0;
  while(path[len] != '\0')
    len++;
  char curr[len + 1];
  memset(curr, 0, len + 1);

  while(len > 2 && path[len - 1] == ']' && path[len - 2] == '[') {
    depth++;
    len -= 2;
  }

  for(i = len - 1; i >= 0; i--) {
    char c = path[i];
    if(c != '.') {
      if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
          || (c == '_') || (c >= '0' && c <= '9') || (i == 0 && c == '@'))
        curr[i] = c;
      else {
        err_msg(UTIL_,  0, "illegal character '%c' in path '%s'...", c, path);
        free_id_list(list);
        return NULL;
      }
    }
    if(c == '.' || i == 0) {
      if((i != 0 && last != '.' && last != '\0') ||
          (i == 0 && c != '.')) {
        m_int size = strlen(curr);
        for(j = 0; j < size / 2; j++) {
          char s = curr[j];
          curr[j] = curr[size - j - 1];
          curr[size - j - 1] = s;
        }
        list = prepend_id_list(curr, list, 0);
        memset(curr, 0, sizeof(curr));
      } else {
        err_msg(UTIL_,  0, "path '%s' must not begin or end with '.'", path);
        free_id_list(list);
        return NULL;
      }
    }
    last = c;
  }
  path = strndup(path, len);
  if(!list)
    return NULL;
  list->xid = insert_symbol(path);
  if(array_depth)
    *array_depth = depth;
  free(path);
  return list;
}

static m_bool mk_xtor(Type type, m_uint d, e_native_func e) {
  VM_Code* code;
  m_str name, filename;
  if(e == NATIVE_CTOR) {
    SET_FLAG(type, ae_flag_ctor);
    name = "ctor";
    filename = "[internal ctor definition]";
    code = &type->info->pre_ctor;
  } else {
    SET_FLAG(type, ae_flag_dtor);
    name = type->name;
    filename = "[internal dtor definition]";
    code = &type->info->dtor;
  }
  *code = new_vm_code(NULL, SZ_INT, 1, name, filename);
  (*code)->native_func = (m_uint)d;
  (*code)->native_func_type = e;
  return 1;
}

Type import_class_begin(Env env, Type type, Nspc where, f_xtor pre_ctor, f_xtor dtor) {
  if(type->info) {
    err_msg(TYPE_, 0, "during import: class '%s' already imported...", type->name);
    return NULL;
  }
  CHECK_BO(env_add_type(env, type))
  type->info = new_nspc(type->name, "global_nspc");
  type->info->parent = where;
  if(pre_ctor)
    mk_xtor(type, (m_uint)pre_ctor, NATIVE_CTOR);
  if(dtor)
    mk_xtor(type, (m_uint)dtor,     NATIVE_DTOR);
  if(type->parent) {
    type->info->offset = type->parent->obj_size;
    free_vector(type->info->obj_v_table);
    type->info->obj_v_table = vector_copy(type->parent->info->obj_v_table);
  }

  type->owner = where;
  type->obj_size = 0;

  SET_FLAG(type, ae_flag_checked);
  vector_add(env->nspc_stack, (vtype)env->curr);
  env->curr = type->info;
  vector_add(env->class_stack, (vtype)env->class_def);
  env->class_def = type;
  return type;
}

m_bool import_class_end(Env env) {
  if(!env->class_def) {
    err_msg(TYPE_, 0, "import: too many class_end called...");
    return -1;
  }
  env->class_def->obj_size = env->class_def->info->offset;
  env->class_def = (Type)vector_pop(env->class_stack);
  env->curr = (Nspc)vector_pop(env->nspc_stack);
  return 1;
}

static m_int import_var(Env env, const m_str type, const m_str name,
                        const m_bool is_static, const m_bool is_const, const m_bool is_ref, m_uint* addr) {
  m_int ret = -1;
  m_uint array_depth;
  ID_List path;
  Type_Decl* type_decl;
  Var_Decl var_decl;
  Var_Decl_List var_decl_list;
  Exp exp_decl;
  CHECK_EB(env->class_def)
  if(!(path = str2list(type, &array_depth))) {
    err_msg(TYPE_, 0, "... during %svar import '%s.%s'...", is_static ? "s" : "m", env->class_def->name, name);
    return -1;
  }

  type_decl = new_type_decl(path, is_ref, 0);
  if(array_depth) {
    type_decl->array = new_array_sub(NULL, 0);
    type_decl->array->depth = array_depth;
  }
  var_decl = new_var_decl(name, NULL, 0);
  if(array_depth) {
    var_decl->array = new_array_sub(NULL, 0);
    var_decl->array->depth = array_depth;
  }
  var_decl_list = new_var_decl_list(var_decl, NULL, 0);
  exp_decl = new_exp_decl(type_decl, var_decl_list, is_static, 0);
  var_decl->addr = (void *)addr;
  if(scan1_exp_decl(env, &exp_decl->d.exp_decl) < 0)
    goto error;
  if(scan2_exp_decl(env, &exp_decl->d.exp_decl) < 0)
    goto error;
  if(is_const)
    SET_FLAG(var_decl->value, ae_flag_const);
  if(!check_exp_decl(env, &exp_decl->d.exp_decl))
    goto error;

  SET_FLAG(var_decl->value, ae_flag_builtin);
  ret = var_decl->value->offset;
error:
  free_expression(exp_decl);
  return ret;
}
m_int import_mvar(Env env, const m_str type, const m_str name,
                  const m_bool is_const, const m_bool is_ref) {
  return import_var(env, type, name, 0, is_const, is_ref, NULL);
}
m_int import_svar(Env env, const m_str type, const m_str name,
                  const m_bool is_const, const m_bool is_ref, m_uint* addr) {
  return import_var(env, type, name, 1, is_const, is_ref, addr);
}

static Arg_List make_dll_arg_list(DL_Func * dl_fun) {
  Arg_List arg_list    = NULL;
  Type_Decl* type_decl = NULL;
  Var_Decl var_decl    = NULL;
  ID_List type_path    = NULL;
  ID_List type_path2    = NULL;
  Array_Sub array_sub  = NULL;
  DL_Value* arg        = NULL;
  m_uint array_depth = 0;
  m_uint array_depth2 = 0;
  m_int i = 0, j;

  if(dl_fun->args)
  for(i = vector_size(dl_fun->args) - 1; i >= 0; i--) {
    array_depth = array_depth2 = 0;
    array_sub = NULL;
    arg = (DL_Value*)vector_at(dl_fun->args, i);
    type_path = str2list(arg->type, &array_depth);
    if(!type_path) {
      err_msg(TYPE_,  0, "...at argument '%i'...", i + 1);
      if(arg_list)
        free_arg_list(arg_list);
      return NULL;
    }
    type_decl = new_type_decl(type_path, 0, 0);
    type_path2 = str2list(arg->name, &array_depth2);
    free_id_list(type_path2);
    if(array_depth && array_depth2) {
      err_msg(TYPE_,  0, "array subscript specified incorrectly for built-in module");
      free_type_decl(type_decl);
      if(arg_list)
        free_arg_list(arg_list);
      return NULL;
    }
    if(array_depth2)
      array_depth = array_depth2;
    if(array_depth) {
      array_sub = new_array_sub(NULL, 0);
      for(j = 1; j < array_depth; j++)
        array_sub = prepend_array_sub(array_sub, NULL, 0);
    }
    var_decl = new_var_decl(arg->name, array_sub, 0);
    arg_list = new_arg_list(type_decl, var_decl, arg_list, 0);
    free(arg);
  }
  return arg_list;
}

Func_Def make_dll_as_fun(DL_Func * dl_fun, m_bool is_static) {
  Func_Def func_def = NULL;
  ae_flag func_decl = ae_flag_func;
  ae_flag static_decl = is_static ? ae_flag_static : ae_flag_instance;
  ID_List type_path = NULL;
  Type_Decl* type_decl = NULL;
  m_str name = NULL;
  Arg_List arg_list = NULL;
  m_uint i, array_depth = 0;

  if(!(type_path = str2list(dl_fun->type, &array_depth)) ||
      !(type_decl = new_type_decl(type_path, 0, 0))) {
    err_msg(TYPE_, 0, "...during @ function import '%s' (type)...", dl_fun->name);
    return NULL;
  }

  if(array_depth) {
    Array_Sub array_sub = new_array_sub(NULL, 0);
    for(i = 1; i < array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL, 0);
    type_decl = add_type_decl_array(type_decl, array_sub, 0);
  }

  name = dl_fun->name;
  arg_list = make_dll_arg_list(dl_fun);

  func_def = new_func_def(func_decl | static_decl, type_decl, name, arg_list, NULL, 0);
  SET_FLAG(func_def, ae_flag_builtin);
  func_def->d.dl_func_ptr = (void*)(m_uint)dl_fun->d.mfun;
  free_dl_func(dl_fun);
  return func_def;
}

#define CHECK_FN(a) if(a < 0) { if(func_def->d.func) REM_REF(func_def->d.func); return NULL;}
static Func import_fun(Env env, DL_Func * mfun, m_bool is_static) {
  Func_Def func_def;
  CHECK_OO(mfun) // probably deserve an err msg
  CHECK_BO(name_valid(mfun->name));
  CHECK_EO(env->class_def)
  func_def = make_dll_as_fun(mfun, is_static);
  if(!func_def) {
    free_dl_func(mfun);
    scope_rem(env->global_nspc->type, insert_symbol(env->class_def->name));
    REM_REF(env->class_def);
    return NULL;
  }
  if(scan1_func_def(env, func_def) < 0 ||
        scan2_func_def(env, func_def) < 0 ||
       !check_func_def(env, func_def)) {
    free_func_def(func_def);
    return NULL;
  }
  return func_def->d.func;
}

// those should be defines
Func import_mfun(Env env, DL_Func * fun) {
  return import_fun(env, fun, 0);
}
Func import_sfun(Env env, DL_Func * fun) {
  return import_fun(env, fun, 1);
}

Type get_type(Env env, m_str str) {
    m_uint  depth;
    ID_List list = str2list(str, &depth);
    Type    t = list ? find_type(env, list) : NULL;
    if(list)
      free_id_list(list);
    return t ? (depth ? new_array_type(env, depth, t, env->curr) : t) : NULL;
}

m_bool import_op(Env env, Operator op, m_str l, m_str r, m_str t, f_instr f, m_bool global) {
    Type lhs = l ? get_type(env, l) : NULL;
    Type rhs = r ? get_type(env, r) : NULL;
    Type ret = get_type(env, t);
    return env_add_op(env, op, lhs, rhs, ret, f, global);
}

