#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "value.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "func.h"
#include "nspc.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"
#include "mpool.h"
#include "specialid.h"

#define GWI_ERR_B(a,...) { env_err(gwi->gwion->env, gwi->loc, (a), ## __VA_ARGS__); return GW_ERROR; }
#define GWI_ERR_O(a,...) { env_err(gwi->gwion->env, gwi->loc, (a), ## __VA_ARGS__); return NULL; }
#define ENV_ERR_B(pos, a,...) { env_err(env, pos, (a), ## __VA_ARGS__); return GW_ERROR; }
#define ENV_ERR_O(pos, a,...) { env_err(env, pos, (a), ## __VA_ARGS__); return NULL; }

#include "parser.h"
struct Path {
  m_str path, curr;
  m_uint len;
  YYLTYPE loc;
};

ANN static ID_List templater_def(SymTable *st, const Gwi gwi) {
  const Templater* templater = &gwi->templater;
  ID_List list[templater->n];
  list[0] = new_id_list(st->p, insert_symbol(st, templater->list[0]), loc_cpy(gwi->gwion->mp, gwi->loc));
  for(m_uint i = 1; i < templater->n; i++) {
    list[i] = new_id_list(st->p, insert_symbol(st, templater->list[i]), loc_cpy(gwi->gwion->mp, gwi->loc));
    list[i - 1]->next = list[i];
  }
  return list[0];
}

ANN VM* gwi_vm(const Gwi gwi) {
  return gwi->gwion->vm;
}

ANN m_int gwi_tmpl_end(const Gwi gwi) {
  gwi->templater.n = 0;
  gwi->templater.list = NULL;
  return GW_OK;
}

ANN m_int gwi_tmpl_ini(const Gwi gwi, const m_uint n, const m_str* list) {
  gwi->templater.n = n;
  gwi->templater.list = (m_str*)list;
  return GW_OK;
}

ANN2(1,2,3) static void dl_func_init(DL_Func* a, const restrict m_str t,
    const restrict m_str n, const f_xfun addr) {
  a->name = n;
  a->type = t;
  a->addr = addr;
  a->narg = 0;
}

ANN m_int gwi_func_ini(const Gwi gwi, const restrict m_str t, const restrict m_str n, const f_xfun addr) {
  dl_func_init(&gwi->func, t, n, addr);
  return GW_OK;
}

ANN static void dl_func_func_arg(DL_Func* a, const restrict m_str t, const restrict m_str n) {
  a->args[a->narg].type = t;
  a->args[a->narg++].name = n;
}

ANN m_int gwi_func_arg(const Gwi gwi, const restrict m_str t, const restrict m_str n) {
  if(gwi->func.narg == DLARG_MAX - 1)
    GWI_ERR_B(_("too many arguments for function '%s'."), gwi->func.name)
  dl_func_func_arg(&gwi->func, t, n);
  return GW_OK;
}

ANN static m_bool _name_valid(const Gwi gwi, const m_str a) {
  const m_uint len = strlen(a);
  m_uint lvl = 0;
  for(m_uint i = 0; i < len; i++) {
    char c = a[i];
    if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
        || (c == '_') || (c >= '0' && c <= '9'))
      continue;
    if(c == '<') {
      if(a[++i] != '~')
        GWI_ERR_B(_("illegal templating in name '%s'."), a)
      ++lvl;
      ++i;
      continue;
    } else if(c == ',') {
      if(!lvl)
        GWI_ERR_B(_("illegal use of ',' outside of templating in name '%s'."), a)
    } else if(c == '~') {
      if(!lvl || a[++i] != '>')
        GWI_ERR_B(_("illegal templating in name '%s'."), a)
      --lvl;
    } else
      GWI_ERR_B(_("illegal character '%c' in name '%s'."), c, a)
  }
  return !lvl ? 1 : -1;
}

ANN static inline m_bool name_valid(const Gwi gwi, const m_str a) {
  return _name_valid(gwi, a[0] != '@' ? a : a + 1);
}

ANN static m_bool check_illegal(const char c, const m_uint i) {
  return isalnum(c) || c == '_' || (!i && c == '@');
}

ANN static ID_List path_valid(const Env env, const m_str path, const loc_t pos) {
  const size_t sz = strlen(path);
  if(path[0] == '.' || path[sz] == '.')
    ENV_ERR_O(pos, _("path '%s' must not ini or end with '.'."), path)
  char curr[sz + 1];
  m_uint i;
  for(i = 0; i < sz; ++i) {
    const char c = path[i];
    if(c != '.') {
      if(!check_illegal(c, i))
        ENV_ERR_O(pos, _("illegal character '%c' in path '%s'."), c, path)
      curr[i] = c;
    } else
      break;
  }
  curr[i++] = '\0';
  const ID_List list = new_id_list(env->gwion->mp,
      insert_symbol(env->gwion->st, curr), loc_cpy(env->gwion->mp, pos));
  if(i < sz)
    list->next = path_valid(env, path + i, pos);
  return list;
}

struct tmpl_checker {
  const m_str str;
  ID_List list;
  const loc_t pos;
};

ANN static m_bool tmpl_list(const Gwion gwion, struct tmpl_checker *ck) {
  m_str s = ck->str;
  const size_t sz = strlen(s);
  char c[sz + 1];
  for(m_uint i = 0; i < sz; ++i) {
    if(isalnum(s[i]) || s[i] == '_') {
      c[i] = s[i];
      continue;
    }
    if(s[i] == '~') {
      if(!i || s[i+1] != '>')
        break;
      c[i] = '\0';
      ck->list = new_id_list(gwion->mp, insert_symbol(gwion->st, c),
          loc_cpy(gwion->mp, ck->pos));
      return GW_OK;
    }
    if(s[i] == ',') {
      if(!i)break;
      c[i] = '\0';
      ck->list = new_id_list(gwion->mp, insert_symbol(gwion->st, c),
          loc_cpy(gwion->mp, ck->pos));
      struct tmpl_checker _ck = { .str=ck->str + i + 1, .pos=ck->pos };
      CHECK_BB(tmpl_list(gwion, &_ck))
      ck->list->next = _ck.list;
      return GW_OK;
    }
    break;
  }
  return GW_ERROR;
}

ANN static m_bool tmpl_check(const m_str str) {
  if(str[0] != '<')
    return 0; // TODO: make it GW_PASS
  if(str[1] != '~')
    return GW_ERROR;
  return GW_OK;
}

ANN static ID_List _tmpl_valid(const Gwi gwi, const m_str str) {
  const m_bool ret = tmpl_check(str);
  if(ret == GW_ERROR)
    return (ID_List)GW_ERROR;
  if(!ret)
    return NULL;
  struct tmpl_checker ck = { .str=str+2, .pos=gwi->loc };
  if(tmpl_list(gwi->gwion, &ck) == GW_ERROR)
    return (ID_List)GW_ERROR;
  return ck.list;
}

ANN static ID_List tmpl_valid(const Gwi gwi, const m_str str) {
  const ID_List ret = _tmpl_valid(gwi, str);
  if(ret == (ID_List)GW_ERROR)
    env_err(gwi->gwion->env, gwi->loc, _("invalid templating definition"));
  return ret;
}

ANN ID_List str2list(const Env env, const m_str path,
      m_uint* array_depth, const loc_t pos) {
  const m_uint sz = strlen(path);
  m_uint len = sz, depth = 0;
  while(len > 2 && path[len - 1] == ']' && path[len - 2] == '[') {
    depth++;
    len -= 2;
  }
  *array_depth = depth;
  char curr[sz + 1];
  memcpy(curr, path, len);
  curr[len] = '\0';
  return path_valid(env, curr, pos);
}

ANN static m_bool mk_gack(MemPool p, const Type type, const f_gack d) {
  const VM_Code code = new_vm_code(p, NULL, SZ_INT, ae_flag_member | ae_flag_builtin, "@gack");
  code->native_func = (m_uint)d;
  type->e->gack = code;
  return GW_OK;
}

ANN m_bool gwi_gack(const Gwi gwi, const Type type, const f_gack d) {
  return mk_gack(gwi->gwion->mp, type, d);
}

ANN static m_bool mk_xtor(MemPool p, const Type type, const m_uint d, const ae_flag e) {
  VM_Code* code = e == ae_flag_ctor ? &type->nspc->pre_ctor : &type->nspc->dtor;
  const m_str name = type->name;
  *code = new_vm_code(p, NULL, SZ_INT, e | ae_flag_member | ae_flag_builtin, name);
  (*code)->native_func = (m_uint)d;
  type->flag |= e;
  return GW_OK;
}

ANN2(1,2) Type gwi_mk_type(const Gwi gwi NUSED, const m_str name, const m_uint size, const m_str parent_name) {
  m_uint depth = 0;
  Type_Decl* td = parent_name ? str2decl(gwi->gwion->env, parent_name, &depth, gwi->loc) : NULL;
  if(depth) {
    free_type_decl(gwi->gwion->mp, td);
    GWI_ERR_O(_("can't create a type with explicit array parent. use typedef for that"))
  }
  const Type parent = td ? known_type(gwi->gwion->env, td) : NULL;
  const Type t = new_type(gwi->gwion->mp, 0, name, parent);
  t->size = size;
  return t;
}

ANN m_int gwi_add_type(const Gwi gwi, const Type type) {
  if(type->name[0] != '@')
    CHECK_BB(name_valid(gwi, type->name));
  env_add_type(gwi->gwion->env, type);
  return (m_int)type->xid;
}

ANN m_int gwi_set_global_type(const Gwi gwi, const Type type, const type_enum te) {
  GWI_BB(gwi_add_type(gwi, type))
  gwi->gwion->type[te] = type;
  return GW_OK;
}

ANN2(1,2) static void import_class_ini(const Env env, const Type type,
    const f_xtor pre_ctor, const f_xtor dtor) {
  type->nspc = new_nspc(env->gwion->mp, type->name);
  type->nspc->parent = env->curr;
  if(pre_ctor)
    mk_xtor(env->gwion->mp, type, (m_uint)pre_ctor, ae_flag_ctor);
  if(dtor)
    mk_xtor(env->gwion->mp, type, (m_uint)dtor, ae_flag_dtor);
  if(type->e->parent) {
    type->nspc->info->offset = type->e->parent->nspc->info->offset;
    if(type->e->parent->nspc->info->vtable.ptr)
      vector_copy2(&type->e->parent->nspc->info->vtable, &type->nspc->info->vtable);
  }
  type->e->owner = env->curr;
  SET_FLAG(type, checked);
  env_push_type(env, type);
}

ANN2(1,2) m_int gwi_class_ini(const Gwi gwi, const Type type, const f_xtor pre_ctor, const f_xtor dtor) {
  if(type->nspc)
    GWI_ERR_B(_("during import: class '%s' already imported."), type->name)
  if(gwi->templater.n) {
    const ID_List types = templater_def(gwi->gwion->st, gwi);
    type->e->def = new_class_def(gwi->gwion->mp, 0, insert_symbol(gwi->gwion->st, type->name), NULL, NULL, loc_cpy(gwi->gwion->mp, gwi->loc));
    type->e->def->base.tmpl = new_tmpl(gwi->gwion->mp, types, -1);
    type->e->def->base.type = type;
    SET_FLAG(type, template);
  } else
    SET_FLAG(type, scan1 | ae_flag_scan2 | ae_flag_check | ae_flag_emit);
  gwi_add_type(gwi, type);
  import_class_ini(gwi->gwion->env, type, pre_ctor, dtor);
  return (m_int)type->xid;
}

ANN m_int gwi_class_ext(const Gwi gwi, Type_Decl* td) {
  if(!gwi->gwion->env->class_def)
    GWI_ERR_B(_("gwi_class_ext invoked before gwi_class_ini"))
  const VM_Code ctor = gwi->gwion->env->class_def->nspc->pre_ctor;
  if(gwi->gwion->env->class_def->e->parent ||
      (gwi->gwion->env->class_def->e->def && gwi->gwion->env->class_def->e->def->base.ext))
    GWI_ERR_B(_("class extend already set"))
  if(td->array && !td->array->exp)
    GWI_ERR_B(_("class extend array can't be empty"))
  if(!gwi->gwion->env->class_def->e->def) {
    DECL_OB(const Type, t, = known_type(gwi->gwion->env, td))
    if(td->array)
      SET_FLAG(gwi->gwion->env->class_def, typedef);
    gwi->gwion->env->class_def->e->parent = t;
    gwi->gwion->env->class_def->nspc->info->offset = t->nspc->info->offset;
    if(t->nspc->info->vtable.ptr)
      vector_copy2(&t->nspc->info->vtable, &gwi->gwion->env->class_def->nspc->info->vtable);
    CHECK_OB((gwi->gwion->emit->code = emit_class_code(gwi->gwion->emit,
          gwi->gwion->env->class_def->name)))
    if(td->array)
      CHECK_BB(emit_array_extend(gwi->gwion->emit, t, td->array->exp))
    if(ctor)
      emit_ext_ctor(gwi->gwion->emit, ctor);
    emit_class_finish(gwi->gwion->emit, gwi->gwion->env->class_def->nspc);
    free_type_decl(gwi->gwion->mp, td);
  } else {
    SET_FLAG(td, typedef);
    gwi->gwion->env->class_def->e->def->base.ext = td;
  }
  return GW_OK;
}

ANN m_int gwi_class_end(const Gwi gwi) {
  if(!gwi->gwion->env->class_def)
    GWI_ERR_B(_("import: too many class_end called."))
  nspc_allocdata(gwi->gwion->mp, gwi->gwion->env->class_def->nspc);
  env_pop(gwi->gwion->env, 0);
  return GW_OK;
}

ANN static void dl_var_new_exp_array(MemPool p, DL_Var* v) {
  v->td->array = new_array_sub(p, NULL);
  v->td->array->depth = v->array_depth;
  v->var.array = new_array_sub(p, NULL);
  v->var.array->depth = v->array_depth;
}

ANN static void dl_var_set(MemPool p, DL_Var* v, const ae_flag flag) {
  v->list.self = &v->var;
  v->td->flag = flag;
  v->exp.exp_type = ae_exp_decl;
  v->exp.d.exp_decl.td   = v->td;
  v->exp.d.exp_decl.list = &v->list;
  if(v->array_depth)
    dl_var_new_exp_array(p, v);
}

ANN static void dl_var_release(MemPool p, const DL_Var* v) {
  if(v->array_depth)
    free_array_sub(p, v->var.array);
  free_type_decl(p, v->td);
}

ANN m_int gwi_item_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  DL_Var* v = &gwi->var;
  memset(v, 0, sizeof(DL_Var));
  if(!(v->td = str2decl(gwi->gwion->env, type, &v->array_depth, gwi->loc)))
    GWI_ERR_B(_("  ...  during var import '%s.%s'."), gwi->gwion->env->name, name)
  CHECK_BB(name_valid(gwi, name))
  v->var.xid = insert_symbol(gwi->gwion->st, name);
  return GW_OK;
}

#undef gwi_item_end

static void gwi_body(const Gwi gwi, const Class_Body body) {
  if(!gwi->gwion->env->class_def->e->def->body)
    gwi->gwion->env->class_def->e->def->body = body;
  else {
    assert(gwi->body);
    gwi->body->next = body;
  }
  gwi->body = body;
}

ANN2(1) m_int gwi_item_end(const Gwi gwi, const ae_flag flag, const m_uint* addr) {
  DL_Var* v = &gwi->var;
  dl_var_set(gwi->gwion->mp, v, flag | ae_flag_builtin);
  v->var.addr = (void*)addr;
  if(gwi->gwion->env->class_def && GET_FLAG(gwi->gwion->env->class_def, template)) {
    Type_Decl *type_decl = new_type_decl(gwi->gwion->mp, v->td->xid);
    type_decl->flag = flag;
    const Var_Decl var_decl = new_var_decl(gwi->gwion->mp, v->var.xid, v->var.array, loc_cpy(gwi->gwion->mp, gwi->loc));
    const Var_Decl_List var_decl_list = new_var_decl_list(gwi->gwion->mp, var_decl, NULL);
    const Exp exp = new_exp_decl(gwi->gwion->mp, type_decl, var_decl_list);
    const Stmt stmt = new_stmt_exp(gwi->gwion->mp, ae_stmt_exp, exp);
    const Stmt_List list = new_stmt_list(gwi->gwion->mp, stmt, NULL);
    Section* section = new_section_stmt_list(gwi->gwion->mp, list);
    const Class_Body body = new_class_body(gwi->gwion->mp, section, NULL);
    type_decl->array = v->td->array;
    gwi_body(gwi, body);
    return GW_OK;
  }
  v->exp.pos = gwi->loc;
  CHECK_BB(traverse_decl(gwi->gwion->env, &v->exp.d.exp_decl))
  SET_FLAG(v->var.value, builtin);
  dl_var_release(gwi->gwion->mp, v);
  return (m_int)v->var.value->from->offset;
}

static Array_Sub make_dll_arg_list_array(MemPool p, Array_Sub array_sub,
  m_uint* array_depth, const m_uint array_depth2) {
  if(array_depth2)
    *array_depth = array_depth2;
  if(*array_depth) { // TODO: clean me
    array_sub = new_array_sub(p, NULL);
    for(m_uint i = 1; i < *array_depth; i++)
      array_sub = prepend_array_sub(array_sub, NULL);
  }
  return array_sub;
}

ANN Type_List _str2tl(const Env env, const m_str s, const loc_t pos) {
  m_uint depth;
  DECL_OO(Type_Decl*, td, = str2decl(env, s, &depth, pos))
  if(depth)
    td->array = make_dll_arg_list_array(env->gwion->mp, NULL, &depth, 0);
  return new_type_list(env->gwion->mp, td, NULL);
}

ANN Type_List tlnext(const Env env, const m_str s, size_t split, const loc_t pos) {
  char curr[split+1];
  strncpy(curr, s, split);
  curr[split] = '\0';
  const Type_List tl = _str2tl(env, curr, pos);
  tl->next = str2tl(env, s + split + 1, pos);
  return tl;
}

struct GetTl {
  const m_str str;
  m_uint i;
  m_uint lvl;
  const size_t sz;
};

#define tl_xxx(name, tgt, op)                             \
ANN m_bool tl_##name(struct GetTl *gtl, const m_uint i) { \
  if(!(i < gtl->sz && gtl->str[i] == tgt))                \
    return GW_ERROR;                                      \
  op gtl->lvl;                                            \
  return GW_OK;                                           \
}
tl_xxx(open,  '~', ++)
tl_xxx(close, '>', --)

ANN Type_List str2tl(const Env env, const m_str s, const loc_t pos) {
  struct GetTl gtl = { .str=s, .sz = strlen(s) };
  for(m_uint i = 0; i < gtl.sz; ++i) {
    if(s[i] == '<')
      CHECK_BO(tl_open(&gtl, ++i))
    else if(s[i] == '~')
      CHECK_BO(tl_close(&gtl, ++i))
    else if(s[i] == ',' && !gtl.lvl)
      return tlnext(env, s, i, pos);
  }
  return _str2tl(env, s, pos);
}

ANN Type_Decl* str2decl(const Env env, const m_str s, m_uint *depth, const loc_t pos) {
  DECL_OO(const m_str, type_name, = get_type_name(env, s, 0))
  DECL_OO(ID_List, id, = str2list(env, type_name, depth, pos))
  Type_Decl* td = new_type_decl(env->gwion->mp, id);
  const m_str tl_name = get_type_name(env, s, 1);
  if(tl_name) {
    if(!(td->types = str2tl(env, tl_name, pos)) || !type_decl_resolve(env, td)) {
      free_type_decl(env->gwion->mp, td);
      return NULL;
    }
  }
  return td;
}

ANN static Arg_List make_dll_arg_list(const Gwi gwi, DL_Func * dl_fun) {
  const Env env = gwi->gwion->env;
  Arg_List arg_list    = NULL;
  for(m_uint i = dl_fun->narg + 1; --i; ) {
    m_uint array_depth = 0, array_depth2 = 0;
    Type_Decl* type_decl = NULL;
    DL_Value* arg = &dl_fun->args[i-1];
    ID_List type_path2;
    if(!(type_decl = str2decl(env, arg->type, &array_depth, gwi->loc))) {
      if(arg_list)
        free_arg_list(env->gwion->mp, arg_list);
      GWI_ERR_O(_("  ...  at argument '%"UINT_F"'"), i + 1)
    }
    if((type_path2 = str2list(env, arg->name, &array_depth2, gwi->loc)))
      free_id_list(env->gwion->mp, type_path2);
    if(array_depth && array_depth2) {
      free_type_decl(env->gwion->mp, type_decl);
      if(arg_list)
        free_arg_list(env->gwion->mp, arg_list);
      GWI_ERR_O(_("array subscript specified incorrectly for built-in module"));
    }
    const Array_Sub array_sub = make_dll_arg_list_array(env->gwion->mp, NULL, &array_depth, array_depth2);
    const Var_Decl var_decl = new_var_decl(env->gwion->mp, insert_symbol(env->gwion->st, arg->name), array_sub, loc_cpy(gwi->gwion->mp, gwi->loc));
    arg_list = new_arg_list(env->gwion->mp, type_decl, var_decl, arg_list);
  }
  return arg_list;
}

struct array_checker {
  m_str str;
  Exp base, exp;
  m_uint depth;
  loc_t pos;
  m_bool is_exp;
};


ANN static void array_add_exp(struct array_checker *ck, const Exp exp) {
  if(ck->exp)
    ck->exp = (ck->exp->next = exp);
  else
    ck->base = ck->exp = exp;
  ++ck->depth;
  ++ck->is_exp;
}

ANN static m_bool array_check(const Env env, struct array_checker *ck) {
  const size_t sz = strlen(ck->str);
  char tmp[sz + 1];
  for(m_uint i = 0; i < sz; ++i) {
    const char c = ck->str[i];
    if(c == ']') {
      const m_bool is_end = ck->str[i + 1] == '\0';
      if(!is_end && ck->str[i + 1] != '[')
        break;
      if(i) {
        if(ck->is_exp == GW_ERROR)
          ENV_ERR_B(ck->pos, _("subscript must be empty"))
        if(!ck->is_exp && ck->depth)
          break;
        tmp[i] = '\0';
        const m_uint num = strtol(tmp, NULL, 10);// migth use &endptr and check errno
        const Exp exp = new_exp_prim_int(env->gwion->mp, num, loc_cpy(env->gwion->mp, ck->pos));
        array_add_exp(ck, exp);
        ck->str += i + 2;
        return is_end ? GW_OK : array_check(env, ck);
      } else {
        if(ck->is_exp)
          break;
        ++ck->depth;
        return array_check(env, ck);
      }
    }
    if(isdigit(c))
      tmp[i] = c;
    else
      ENV_ERR_B(ck->pos, _("invalid subscript '%c' in '%s'"), c, ck->str)
  }
  ENV_ERR_B(ck->pos, _("incoherent subscript '%s'"), ck->str)
}


ANN static Array_Sub import_array_sub(const Gwi gwi, const m_str str, const m_bool is_exp) {
  struct array_checker ck = { .str=str + 1, .pos=gwi->loc, .is_exp=is_exp };
  CHECK_BO(array_check(gwi->gwion->env, &ck))
  return new_array_sub(gwi->gwion->mp, ck.exp);
}

ANN static Type_Decl* import_td(const Gwi gwi, const m_str name, const m_bool is_exp) {
  const m_str subscript = strchr(name, '[');
  const size_t sz = strlen(name), sub_sz = subscript ? strlen(subscript) : 0,
    tmp_sz = sz - sub_sz;
  char str[tmp_sz + 1];
  strncpy(str, name, tmp_sz);
  str[tmp_sz] = '\0';
  DECL_OO(const ID_List, type_path, = path_valid(gwi->gwion->env, str, gwi->loc))
  Type_Decl* td = new_type_decl(gwi->gwion->mp, type_path);
  if(subscript && (td->array = import_array_sub(gwi, subscript, is_exp))) {
    free_type_decl(gwi->gwion->mp, td);
    return NULL;
  }
  return td;
}
struct func_checker { // name_checker ?
  m_str name;
  ID_List tmpl;
  const ae_flag flag;
};

//ANN static Func_Def make_dll_as_fun(const Gwi gwi, DL_Func * dl_fun, ae_flag flag) {
ANN static Func_Def make_dll_as_fun(const Gwi gwi, const struct func_checker *ck) {
//DL_Func * dl_fun, ae_flag flag) {
  const MemPool mp = gwi->gwion->mp;
  DECL_OO(Type_Decl*, type_decl, = import_td(gwi, gwi->func.type, GW_ERROR))
  const m_str name = gwi->func.name;
  const Arg_List arg_list = make_dll_arg_list(gwi, &gwi->func);
  const Func_Def func_def = new_func_def(mp, new_func_base(mp, type_decl, insert_symbol(gwi->gwion->st, name), arg_list),
    NULL, ck->flag | ae_flag_builtin, loc_cpy(mp, gwi->loc));
  func_def->d.dl_func_ptr = (void*)(m_uint)gwi->func.addr;
  return func_def;
}

ANN static m_bool section_fdef(const Gwi gwi, const Func_Def fdef) {
  Section* section = new_section_func_def(gwi->gwion->mp, fdef);
  const Class_Body body = new_class_body(gwi->gwion->mp, section, NULL);
  gwi_body(gwi, body);
  return GW_OK;
}

ANN static m_bool error_fdef(const Gwi gwi, const Func_Def fdef) {
  fdef->d.dl_func_ptr = NULL;
  free_func_def(gwi->gwion->mp, fdef);
  return GW_ERROR;
}

ANN static inline void func_checker_clean(const Gwi gwi, struct func_checker *ck) {
  if(ck->tmpl)
    free_id_list(gwi->gwion->mp, ck->tmpl);
}

ANN static m_bool check_typename_def(const Gwi gwi, struct func_checker *ck) {
  const m_str base = ck->name;
  const m_str c = strchr(ck->name, '>');
  ck->name = !c ? ck->name : c + 1;
  CHECK_BB(name_valid(gwi, ck->name))
  if((ck->tmpl = tmpl_valid(gwi, base)) == (ID_List)GW_ERROR)
    return GW_ERROR;
  return GW_OK;
}

ANN2(1) static Func_Def template_fdef(const Gwi gwi, const struct func_checker *ck) {
  const Arg_List arg_list = make_dll_arg_list(gwi, &gwi->func);
  m_uint depth;
  Type_Decl *td = str2decl(gwi->gwion->env, gwi->func.type, &depth, gwi->loc);
  if(depth)
    td->array = make_dll_arg_list_array(gwi->gwion->mp, NULL, &depth, 0);
  const Func_Def fdef = new_func_def(gwi->gwion->mp, new_func_base(gwi->gwion->mp,
      td, insert_symbol(gwi->gwion->st, ck->name), arg_list), NULL, ae_flag_builtin, loc_cpy(gwi->gwion->mp, gwi->loc));
  fdef->base->tmpl = new_tmpl(gwi->gwion->mp, ck->tmpl, -1);
  fdef->d.dl_func_ptr = (void*)(m_uint)gwi->func.addr;
  SET_FLAG(fdef, template | ae_flag_builtin);
  return fdef;
}

ANN m_int gwi_func_valid(const Gwi gwi, const struct func_checker *ck) {
  gwi->func.name = ck->name;
  DECL_OB(Func_Def, fdef, = (!ck->tmpl ? make_dll_as_fun : template_fdef)(gwi, ck))
  if(gwi->gwion->env->class_def && GET_FLAG(gwi->gwion->env->class_def, template))
    return section_fdef(gwi, fdef);
  if(traverse_func_def(gwi->gwion->env, fdef) < 0)
    return error_fdef(gwi, fdef);
  return GW_OK;
}

ANN m_int gwi_func_end(const Gwi gwi, const ae_flag flag) {
  struct func_checker ck = { .name=gwi->func.name, .flag=flag };
  CHECK_BB(check_typename_def(gwi, &ck))
  if(gwi_func_valid(gwi, &ck) > 0)
    return GW_OK;
  func_checker_clean(gwi, &ck);
  return GW_ERROR;
}

ANN2(1,3) static Type _get_type(const Env env, const m_str str, const loc_t pos) {
  m_uint depth = 0;
  const ID_List list = (str && str != (m_str)OP_ANY_TYPE) ? str2list(env, str, &depth, pos) : NULL;
  const Type  t = (str == (m_str) OP_ANY_TYPE) ? OP_ANY_TYPE : list ? find_type(env, list) : NULL;
  if(list)
    free_id_list(env->gwion->mp, list);
  return t ? (depth ? array_type(env, t, depth) : t) : NULL;
}

ANN2(1,3) static inline Type get_type(const Env env, const m_str str, const loc_t pos) {
  return str ? _get_type(env, str, pos) : NULL;
}

ANN2(1,2) static int import_op(const Gwi gwi, const DL_Oper* op,
    const f_instr f) {
  const Env env = gwi->gwion->env;
  const Type lhs = get_type(env, op->lhs, gwi->loc),
             rhs = get_type(env, op->rhs, gwi->loc),
             ret = _get_type(env, op->ret, gwi->loc);
  const struct Op_Import opi = { lhs, rhs, ret,
    op->ck, op->em, (uintptr_t)f, gwi->loc, op->op };
  return add_op(gwi->gwion, &opi);
}


ANN2(1) m_int gwi_oper_ini(const Gwi gwi, const restrict m_str l,
    const restrict m_str r, const restrict m_str t) {
  gwi->oper.ret = t;
  gwi->oper.rhs = r;
  gwi->oper.lhs = l;
  return GW_OK;
}

ANN m_int gwi_oper_add(const Gwi gwi, Type (*ck)(Env, void*, m_bool*)) {
  gwi->oper.ck = ck;
  return GW_OK;
}

ANN m_int gwi_oper_emi(const Gwi gwi, const opem em) {
  gwi->oper.em = em;
  return GW_OK;
}

ANN m_int gwi_oper_end(const Gwi gwi, const m_str op, const f_instr f) {
  gwi->oper.op = insert_symbol(gwi->gwion->st, op);
  const m_bool ret = import_op(gwi, &gwi->oper, f);
  gwi->oper.ck = NULL;
  gwi->oper.em = NULL;
  return ret;
}

ANN m_int gwi_oper_cond(const Gwi gwi, const m_str type,
  const f_instr f1, const f_instr f2) {
  GWI_BB(gwi_oper_ini(gwi, NULL, type, "int"))
  GWI_BB(gwi_oper_end(gwi, "@conditionnal", f1))
  GWI_BB(gwi_oper_end(gwi, "@unconditionnal", f2))
  return GW_OK;
}

ANN m_int gwi_fptr_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  dl_func_init(&gwi->func, type, name, 0);
  return GW_OK;
}

ANN static Fptr_Def import_fptr(const Gwi gwi, DL_Func* dl_fun, ae_flag flag) {
  const Env env = gwi->gwion->env;
  struct func_checker ck = { .name=gwi->func.name, .flag=flag };
  CHECK_BO(check_typename_def(gwi, &ck))
  m_uint depth;
  const ID_List type_path = str2list(env, dl_fun->type, &depth, gwi->loc);
  if(type_path) {
    Type_Decl *td = new_type_decl(env->gwion->mp, type_path);
    td->array = make_dll_arg_list_array(env->gwion->mp, NULL, &depth, 0);
    const Arg_List args = make_dll_arg_list(gwi, dl_fun);
    Func_Base *base = new_func_base(env->gwion->mp, td, insert_symbol(env->gwion->st, ck.name), args);
    if(ck.tmpl)
      base->tmpl = new_tmpl(gwi->gwion->mp, ck.tmpl, -1);
    return new_fptr_def(env->gwion->mp, base, flag | ae_flag_builtin);
  }
  func_checker_clean(gwi, &ck);
  return NULL;
}

ANN Type gwi_fptr_end(const Gwi gwi, const ae_flag flag) {
  DECL_OO(const Fptr_Def, fptr, = import_fptr(gwi, &gwi->func, flag))
  const m_bool ret = traverse_fptr_def(gwi->gwion->env, fptr);
  if(ret > 0)
    SET_FLAG(fptr->base->func, builtin);
  const Type t = ret > 0 ? fptr->type : NULL;
  free_fptr_def(gwi->gwion->mp, fptr);
  return t;
}

ANN m_int gwi_typedef_ini(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  gwi->val.type = type;
  gwi->val.name = name;
  return GW_OK;
}

ANN Type gwi_typedef_end(const Gwi gwi, const ae_flag flag) {
  struct func_checker ck = { .name=gwi->val.name, .flag=flag };
  CHECK_BO(check_typename_def(gwi, &ck))
// we need to be able to parse e.g int[2][3] here.
// as well as other_type[][]
  Type_Decl* td = import_td(gwi, gwi->val.type, 0); // TODO: make it GW_PASS
  if(td) {
    td->flag |= flag;
    const Symbol sym = insert_symbol(gwi->gwion->st, ck.name);
    const Type_Def tdef = new_type_def(gwi->gwion->mp, td, sym);
    if(ck.tmpl)
      tdef->tmpl = new_tmpl(gwi->gwion->mp, ck.tmpl, -1);
    traverse_type_def(gwi->gwion->env, tdef);
    const Type t = tdef->type;
    free_type_def(gwi->gwion->mp, tdef);
    return t;
  }
  func_checker_clean(gwi, &ck);
  return NULL;
}

ANN static Exp make_exp(const Gwi gwi, const m_str type, const m_str name) {
  const Env env = gwi->gwion->env;
  m_uint array_depth;
  Array_Sub array = NULL;
  DECL_OO(const ID_List, id_list, = str2list(env, type, &array_depth, gwi->loc))
  if(array_depth) {
    array = new_array_sub(env->gwion->mp, NULL);
    array->depth = array_depth;
  }
  Type_Decl *type_decl = new_type_decl(env->gwion->mp, id_list);
  const Var_Decl var_decl = new_var_decl(env->gwion->mp,
      insert_symbol(env->gwion->st, name), array, loc_cpy(env->gwion->mp, gwi->loc));
  const Var_Decl_List var_decl_list = new_var_decl_list(env->gwion->mp, var_decl, NULL);
  return new_exp_decl(env->gwion->mp, type_decl, var_decl_list);
}

ANN2(1) m_int gwi_union_ini(const Gwi gwi, const m_str type, const m_str name) {
  gwi->union_data.type_name = type;
  gwi->union_data.name = name;
  return GW_OK;
}

ANN m_int gwi_union_add(const Gwi gwi, const restrict m_str type, const restrict m_str name) {
  DECL_OB(const Exp, exp, = make_exp(gwi, type, name))
  SET_FLAG(exp->d.exp_decl.td, ref);
  gwi->union_data.list = new_decl_list(gwi->gwion->mp, exp, gwi->union_data.list);
  return GW_OK;
}

ANN static Type union_type(const Gwi gwi, const Union_Def udef) {
  CHECK_BO(scan0_union_def(gwi->gwion->env, udef))
  CHECK_BO(traverse_union_def(gwi->gwion->env, udef))
  if(!udef->tmpl)
    emit_union_offset(udef->l, udef->o);
  if(gwi->gwion->env->class_def && !GET_FLAG(udef, static))
      gwi->gwion->env->class_def->nspc->info->offset =
      udef->o + udef->s;
  if(udef->xid || !udef->type_xid) {
    SET_FLAG(udef->value, builtin);
    const M_Object o = new_object(gwi->gwion->mp, NULL, udef->value->type);
    udef->value->d.ptr = (m_uint*)o;
   return udef->value->type;
  }
  return udef->type;
}

ANN Type gwi_union_end(const Gwi gwi, const ae_flag flag) {
  if(!gwi->union_data.list)
    GWI_ERR_O(_("union is empty"));
  if(gwi->union_data.name)
    CHECK_BO(name_valid(gwi, gwi->union_data.name))
  struct func_checker ck = { .name=gwi->union_data.type_name, .flag=flag };
  if(gwi->union_data.type_name)
    CHECK_BO(check_typename_def(gwi, &ck))
  const Symbol xid = gwi->union_data.name ? insert_symbol(gwi->gwion->st, gwi->union_data.name) : NULL;
  const Symbol type_xid = gwi->union_data.type_name ? insert_symbol(gwi->gwion->st, ck.name) : NULL;
  const Union_Def udef = new_union_def(gwi->gwion->mp, gwi->union_data.list, loc_cpy(gwi->gwion->mp, gwi->loc));
  udef->flag = flag;
  udef->xid = xid;
  udef->type_xid = type_xid;
  if(ck.tmpl) {
    if(udef->xid)
      GWI_ERR_O(_("Template union type can't declare union"));
    udef->tmpl = new_tmpl(gwi->gwion->mp, ck.tmpl, -1);
  }
  const Type t = union_type(gwi, udef);
  if(!SAFE_FLAG(t, template))
    free_union_def(gwi->gwion->mp, udef);
  gwi->union_data.list = NULL;
  gwi->union_data.name  = NULL;
  gwi->union_data.type_name  = NULL;
  return t;
}

ANN2(1) m_int gwi_enum_ini(const Gwi gwi, const m_str type) {
  gwi->enum_data.t = type;
  vector_init(&gwi->enum_data.addr);
  return GW_OK;
}

ANN m_int gwi_enum_add(const Gwi gwi, const m_str name, const m_uint i) {
  CHECK_BB(name_valid(gwi, name))
  const ID_List list = new_id_list(gwi->gwion->mp, insert_symbol(gwi->gwion->st, name), loc_cpy(gwi->gwion->mp, gwi->loc));
  DL_Enum* d = &gwi->enum_data;
  ALLOC_PTR(gwi->gwion->mp, addr, m_int, i);
  vector_add(&gwi->enum_data.addr, (vtype)addr);
  if(!d->base)
    d->base = list;
  else
    d->curr->next = list;
  d->curr = list;
  return list ? 1 : -1;
}

ANN static void import_enum_end(const Gwi gwi, const Vector v) {
  DL_Enum* d = &gwi->enum_data;
  for(m_uint i = 0; i < vector_size(v); i++) {
    Value value = (Value)vector_at(v, i);
    const m_uint addr = vector_at(&d->addr, i);
    SET_FLAG(value, builtin);
    ADD_REF(value->type);
    if(!gwi->gwion->env->class_def)
      value->d.ptr = (m_uint*)(addr ? addr : i);
    else
      value->d.ptr = (m_uint*)(addr ? *(m_uint*)addr : i);
  }
  d->t = NULL;
  d->base = NULL;
  vector_release(&d->addr);
}

ANN Type gwi_enum_end(const Gwi gwi) {
  DL_Enum* d = &gwi->enum_data;
  const Enum_Def edef  = new_enum_def(gwi->gwion->mp, d->base, d->t ? insert_symbol(gwi->gwion->st, d->t) : NULL, 
    loc_cpy(gwi->gwion->mp, gwi->loc));
  const m_bool ret = traverse_enum_def(gwi->gwion->env, edef);
  import_enum_end(gwi, &edef->values);
  const Type t = ret > 0 ? edef->t : NULL;
  free_enum_def(gwi->gwion->mp, edef);
  return t;
}

ANN void register_freearg(const Gwi gwi, const f_instr _exec, const f_freearg _free) {
  map_set(&gwi->gwion->data->freearg, (vtype)_exec, (vtype)_free);
}

ANN void gwi_reserve(const Gwi gwi, const m_str str) {
  vector_add(&gwi->gwion->data->reserved, (vtype)insert_symbol(gwi->gwion->st, str));
}

ANN void gwi_specialid(const Gwi gwi, const m_str id, const SpecialId spid) {
  struct SpecialId_ *a = mp_calloc(gwi->gwion->mp, SpecialId);
  memcpy(a, spid, sizeof(struct SpecialId_));
  map_set(&gwi->gwion->data->id, (vtype)insert_symbol(gwi->gwion->st, id), (vtype)a);
  gwi_reserve(gwi, id);
}

ANN void gwi_set_loc(const Gwi gwi, const m_str file, const uint line) {
  gwi->loc->first_line = gwi->loc->last_line = line;
  gwi->gwion->env->name = file;
}
