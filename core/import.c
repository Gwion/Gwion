#include <string.h>
#include "err_msg.h"
#include "env.h"
#include "func.h"
#include "import.h"
#include "dl.h"

#define CHECK_EB(a) if(!env->class_def) { err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end"); return -1; }
#define CHECK_EO(a) if(!env->class_def) { err_msg(TYPE_, 0, "import error: import_xxx invoked between begin/end"); return NULL; }

void free_expression(Exp exp);

m_bool scan1_exp_decl(Env env, Exp_Decl* decl);
m_bool scan2_exp_decl(Env env, Exp_Decl* decl);
Type   check_exp_decl(Env env, Exp_Decl* decl);

m_bool scan1_func_def(Env env, Func_Def f);
m_bool scan2_func_def(Env env, Func_Def f);
m_bool check_func_def(Env env, Func_Def f);

void dl_return_push(const DL_Return retval, VM_Shred shred, int kind) {
  if(kind == Kindof_Int) {
    *(m_uint*)REG(0) = retval.d.v_uint;
    PUSH_REG(shred, SZ_INT);
  } else if(kind == Kindof_Float) {
    *(m_float*)REG(0) = retval.d.v_float;
    PUSH_REG(shred, SZ_FLOAT);
  } else if(kind == Kindof_Complex) {
    *(m_complex*)REG(0) = retval.d.v_complex;
    PUSH_REG(shred, SZ_COMPLEX);
  } else if(kind == Kindof_Vec3) {
    *(m_vec3*)REG(0) = retval.d.v_vec3;
    PUSH_REG(shred, SZ_VEC3);
  } else if(kind == Kindof_Vec4) {
    *(m_vec4*)REG(0) = retval.d.v_vec4;
    PUSH_REG(shred, SZ_VEC4);
  }
}

void dl_func_init(DL_Func* a, const m_str t, const m_str n, m_uint addr) {
  a->name = n;
  a->type = t;
  a->addr = addr;
  a->narg = 0;
}

void dl_func_add_arg(DL_Func* a, const m_str t, const m_str  n) {
  a->args[a->narg].type = t;
  a->args[a->narg++].name = n;
}

static ID_List str2list(m_str path, m_uint* array_depth) {
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

  for(i = len; --i;) {
    char c = path[i - 1];
    if(c != '.') {
      if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
          || (c == '_') || (c >= '0' && c <= '9') || (i == 1 && c == '@'))
        curr[i - 1] = c;
      else {
        free_id_list(list);
        CHECK_BO(err_msg(UTIL_,  0, "illegal character '%c' in path '%s'...", c, path))
      }
    }
    if(c == '.' || i == 1) {
      if((i != 1 && last != '.' && last != '\0') ||
          (i == 1 && c != '.')) {
        m_int size = strlen(curr);
        for(j = (size / 2) + 1; --j;) {
          char s = curr[j];
          curr[j] = curr[size - j - 1];
          curr[size - j - 1] = s;
        }
        list = prepend_id_list(curr, list, 0);
        memset(curr, 0, sizeof(curr));
      } else {
        free_id_list(list);
        CHECK_BO(err_msg(UTIL_,  0, "path '%s' must not begin or end with '.'", path))
      }
    }
    last = c;
  }
  strncpy(curr, path, len);
  CHECK_OO(list)
  list->xid = insert_symbol(curr);
  *array_depth = depth;
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

m_int import_class_begin(Env env, Type type, Nspc where, f_xtor pre_ctor, f_xtor dtor) {
  if(type->info) {
    err_msg(TYPE_, 0, "during import: class '%s' already imported...", type->name);
    return -1;
  }
  CHECK_BB(env_add_type(env, type))
  type->info = new_nspc(type->name, "global_nspc");
  type->info->parent = where;
  if(pre_ctor)
    mk_xtor(type, (m_uint)pre_ctor, NATIVE_CTOR);
  if(dtor)
    mk_xtor(type, (m_uint)dtor,     NATIVE_DTOR);
  if(type->parent) {
    type->info->offset = type->parent->obj_size;
    vector_copy2(&type->info->obj_v_table, &type->parent->info->obj_v_table);
  }

  type->owner = where;
  type->obj_size = 0;

  SET_FLAG(type, ae_flag_checked);
  CHECK_BB(env_push_class(env, type))
  return type->xid;
}

m_int import_class_end(Env env) {
  if(!env->class_def)
    CHECK_BB(err_msg(TYPE_, 0, "import: too many class_end called..."))
  env->class_def->obj_size = env->class_def->info->offset;
  CHECK_BB(env_pop_class(env))
  return 1;
}

m_int import_var(Env env, const m_str type, const m_str name, ae_flag flag, m_uint* addr) {
  m_int ret = -1;
  m_uint array_depth;
  ID_List path;

  CHECK_EB(env->class_def)
  if(!(path = str2list(type, &array_depth)))
    CHECK_BB(err_msg(TYPE_, 0, "... during var import '%s.%s'...", env->class_def->name, name))

  Type_Decl t;
  memset(&t, 0, sizeof(Type_Decl));
  t.xid = path;
  t.ref = ((flag & ae_flag_ref) == ae_flag_ref);
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
  if(scan1_exp_decl(env, &exp.d.exp_decl) < 0  ||
      scan2_exp_decl(env, &exp.d.exp_decl) < 0 ||
    !check_exp_decl(env, &exp.d.exp_decl))
    goto error;
  var.value->flag = flag | ae_flag_builtin;
  ret = var.value->offset;
error:
  free(path);
  return ret;
}

static Arg_List make_dll_arg_list(DL_Func * dl_fun) {
  Arg_List arg_list    = NULL;
  Type_Decl* type_decl = NULL;
  Var_Decl var_decl    = NULL;
  ID_List type_path    = NULL;
  ID_List type_path2   = NULL;
  Array_Sub array_sub  = NULL;
  DL_Value* arg        = NULL;
  m_uint array_depth = 0;
  m_uint array_depth2 = 0;
  m_int i = 0, j;

    for(i = 0; i < dl_fun->narg; i++) {
      array_depth = array_depth2 = 0;
      array_sub = NULL;
      arg = &dl_fun->args[i];
      type_path = str2list(arg->type, &array_depth);
      if(!type_path) {
        if(arg_list)
          free_arg_list(arg_list);
        CHECK_BO(err_msg(TYPE_,  0, "...at argument '%i'...", i + 1))
      }
      type_decl = new_type_decl(type_path, 0, 0);
      type_path2 = str2list(arg->name, &array_depth2);
      free_id_list(type_path2);
      if(array_depth && array_depth2) {
        free_type_decl(type_decl);
        if(arg_list)
          free_arg_list(arg_list);
        CHECK_BO(err_msg(TYPE_,  0, "array subscript specified incorrectly for built-in module"))
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

#define CHECK_FN(a) if(a < 0) { if(func_def->d.func) REM_REF(func_def->d.func); return NULL;}
m_int import_fun(Env env, DL_Func * mfun, ae_flag flag) {
  Func_Def func_def;
  CHECK_OB(mfun) // probably deserve an err msg
  CHECK_BB(name_valid(mfun->name));
  CHECK_EB(env->class_def)
  CHECK_OB((func_def = make_dll_as_fun(mfun, flag)))
  if(scan1_func_def(env, func_def) < 0 ||
      scan2_func_def(env, func_def) < 0 ||
      !check_func_def(env, func_def)) {
    free_func_def(func_def);
    return -1;
  }
  return 1;
}

static Type get_type(Env env, const m_str str) {
  m_uint  depth;
  ID_List list = str2list(str, &depth);
  Type    t = list ? find_type(env, list) : NULL;
  if(list)
    free_id_list(list);
  return t ? (depth ? new_array_type(env, depth, t, env->curr) : t) : NULL;
}

m_int import_op(Env env, Operator op, const m_str l, const m_str r, const m_str t,
                const f_instr f, const m_bool global) {
  Type lhs = l ? get_type(env, l) : NULL;
  Type rhs = r ? get_type(env, r) : NULL;
  Type ret = get_type(env, t);
  return env_add_op(env, op, lhs, rhs, ret, f, global);
}
