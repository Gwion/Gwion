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

ANN static m_bool check_illegal(const char c, const m_uint i) {
   return isalnum(c) || c == '_' || (!i && c == '@');
}

ANN ID_List path_valid(const Env env, const m_str path, const loc_t pos) {
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

ANN ID_List tmpl_valid(const Gwi gwi, const m_str str) {
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

ANN Type_List _str2tl(const Env env, const m_str s, const loc_t pos) {
  m_uint depth;
  DECL_OO(Type_Decl*, td, = str2decl(env, s, &depth, pos))
  if(depth) {
    td->array = new_array_sub(env->gwion->mp, NULL);
    td->array->depth = depth;
  }
//    td->array = import_array(env->gwion->mp, NULL, &depth, 0);
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

ANN static void array_add_exp(struct array_checker *ck, const Exp exp) {
  if(ck->exp)
    ck->exp = (ck->exp->next = exp);
  else
    ck->base = ck->exp = exp;
  ++ck->depth;
  ++ck->is_exp;
}

ANN m_bool array_check(const Env env, struct array_checker *ck) {
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

ANN void func_checker_clean(const Gwi gwi, struct func_checker *ck) {
  if(ck->tmpl)
    free_id_list(gwi->gwion->mp, ck->tmpl);
}

ANN m_bool check_typename_def(const Gwi gwi, struct func_checker *ck) {
  const m_str base = ck->name;
  const m_str c = strchr(ck->name, '>');
  ck->name = !c ? ck->name : c + 1;
  CHECK_BB(name_valid(gwi, ck->name))
  if((ck->tmpl = tmpl_valid(gwi, base)) == (ID_List)GW_ERROR)
    return GW_ERROR;
  return GW_OK;
}


ANN Array_Sub import_array_sub(const Gwi gwi, const m_str str, const m_bool is_exp) {
  struct array_checker ck = { .str=str + 1, .pos=gwi->loc, .is_exp=is_exp };
  CHECK_BO(array_check(gwi->gwion->env, &ck))
  return new_array_sub(gwi->gwion->mp, ck.exp);
}

ANN Type_Decl* import_td(const Gwi gwi, const m_str name, const m_bool is_exp) {
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
