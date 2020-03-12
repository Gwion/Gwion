/** @file: checker.c                                   *
 *  \brief: functions to check names in import module  *
 *                                                     */
#include <ctype.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "traverse.h"
#include "instr.h"
#include "object.h"
#include "emit.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

ANN m_bool array_check(const Gwi gwi, struct array_checker *ck);

__attribute__((returns_nonnull))
ANN static Symbol gwisym(const Gwi gwi, const m_str str) {
  return insert_symbol(gwi->gwion->st, str);
}

//! check that there is no illegal character in the string
// TODO: get rid of second argument, make it useless
ANN static m_bool check_illegal(const char c, const m_uint i) {
   return isalnum(c) || c == '_' || (!i && c == '@');
}

/** convert a string to a symbol, with error checking **/
//ANN Symbol str2sym(const Env env, const m_str path, const loc_t pos) {
ANN Symbol str2sym(const Gwi gwi, const m_str path) {
  const size_t sz = strlen(path);
  m_uint i;
  char curr[sz + 1];
  for(i = 0; i < sz; ++i) {
    const char c = path[i];
    if(c != '.') {
      if(!check_illegal(c, i))
        GWI_ERR_O(_("illegal character '%c' in path '%s'."), c, path)
      curr[i] = c;
    } else
      break;
  }
  curr[i++] = '\0';
  return gwisym(gwi, curr);
}

ANN ID_List str2symlist(const Gwi gwi, const m_str path) {
  DECL_OO(const Symbol, sym, = str2sym(gwi, path))
  return new_id_list(gwi->gwion->mp, sym, loc(gwi));
}

ANN ID_List path_valid(const Gwi gwi, const m_str path) {
  const size_t sz = strlen(path);
  if(path[0] == '.' || path[sz] == '.')
    GWI_ERR_O(_("path '%s' must not ini or end with '.'."), path)
//  DECL_OO(const Symbol, sym, = str2sym(gwi, path))
//  const ID_List list = new_id_list(gwi->gwion->mp, sym, loc(gwi));
  const ID_List list = str2symlist(gwi, path);
  if(strlen(s_name(list->xid)) < sz)
    list->next = path_valid(gwi, path + strlen(s_name(list->xid)));
  return list;
}


//
// similar to import array_sub ?
ANN Array_Sub ck_array(MemPool mp, const m_uint depth) {
  if(!depth)
    return NULL;
  const Array_Sub array = new_array_sub(mp, NULL);
  array->depth = depth;
  return array;
}

ANN Var_Decl str2var(const Gwi gwi, const m_str path) {
  struct array_checker ck = { .str=path };
  CHECK_BO(array_check(gwi, &ck))
  const m_uint sz = strlen(path);
  const m_uint len = sz - ck.sz;
  char curr[len + 1];
  memcpy(curr, path, len);
  curr[len] = '\0';
  DECL_OO(const Symbol, sym, = str2sym(gwi, curr))
  const Array_Sub array = ck_array(gwi->gwion->mp, ck.depth);
  return new_var_decl(gwi->gwion->mp, sym, array, loc(gwi));
}

ANN Var_Decl_List str2varlist(const Gwi gwi, const m_str path) {
  DECL_OO(const Var_Decl, var, = str2var(gwi, path))
  return new_var_decl_list(gwi->gwion->mp, var, NULL);
}

struct tmpl_checker {
  const m_str str;
  ID_List list;
//  const loc_t pos;
};

ANN static m_bool tmpl_list(const Gwi gwi, struct tmpl_checker *ck) {
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
      ck->list = new_id_list(gwi->gwion->mp, gwisym(gwi, c), loc(gwi));
      return GW_OK;
    }
    if(s[i] == ',') {
      if(!i)break;
      c[i] = '\0';
      ck->list = new_id_list(gwi->gwion->mp, gwisym(gwi, c), loc(gwi));
      struct tmpl_checker _ck = { .str=ck->str + i + 1 };
      CHECK_BB(tmpl_list(gwi, &_ck))
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
  struct tmpl_checker ck = { .str=str+2 };
  if(tmpl_list(gwi, &ck) == GW_ERROR)
    return (ID_List)GW_ERROR;
  return ck.list;
}

ANN ID_List tmpl_valid(const Gwi gwi, const m_str str) {
  const ID_List ret = _tmpl_valid(gwi, str);
  if(ret == (ID_List)GW_ERROR)
    env_err(gwi->gwion->env, gwi->loc, _("invalid templating definition"));
  return ret;
}

ANN ID_List ck2list(const Gwi gwi, struct array_checker *ck) {
  const m_str base = ck->str;
  CHECK_BO(array_check(gwi, ck))
  const m_uint sz = strlen(base);
  const m_uint len = sz - ck->sz;
  char curr[len + 1];
  memcpy(curr, base, len);
  curr[len] = '\0';
  return path_valid(gwi, curr);
}

ANN static Type_List str2tl(const Gwi gwi, const m_str s);
ANN static Type_List _str2tl(const Gwi gwi, const m_str s) {
  DECL_OO(Type_Decl*, td, = str2decl(gwi, s))
  return new_type_list(gwi->gwion->mp, td, NULL);
}

ANN Type_List tlnext(const Gwi gwi, const m_str s, size_t split) {
  char curr[split+1];
  memcpy(curr, s, split);
  curr[split] = '\0';
  const Type_List tl = _str2tl(gwi, curr);
  tl->next = str2tl(gwi, s + split + 1);
  return tl;
}

struct GetTl {
  const m_str str;
  m_uint i;
  m_uint lvl;
  const size_t sz;
};

//! a funtion factory to open/close the template
#define tl_xxx(name, tgt, op)                             \
ANN m_bool tl_##name(struct GetTl *gtl, const m_uint i) { \
  if(!(i < gtl->sz && gtl->str[i] == tgt))                \
    return GW_ERROR;                                      \
  op gtl->lvl;                                            \
  return GW_OK;                                           \
}
tl_xxx(open,  '~', ++)
tl_xxx(close, '>', --)

ANN static Type_List str2tl(const Gwi gwi, const m_str s) {
  struct GetTl gtl = { .str=s, .sz = strlen(s) };
  for(m_uint i = 0; i < gtl.sz; ++i) {
    if(s[i] == '<')
      CHECK_BO(tl_open(&gtl, ++i))
    else if(s[i] == '~')
      CHECK_BO(tl_close(&gtl, ++i))
    else if(s[i] == ',' && !gtl.lvl)
      return tlnext(gwi, s, i);
  }
  return _str2tl(gwi, s);
}

//! convert a string to a Type_Decl
ANN Type_Decl* str2decl(const Gwi gwi, const m_str str) {
  const ae_flag flag = strncmp(str, "nonnull ", 8) ? ae_flag_none : ae_flag_nonnull;
  const m_str s = strncmp(str, "nonnull ", 8) ? str : str + 8;
// we can do better
  DECL_OO(const m_str, type_name, = get_type_name(gwi->gwion->env, s, 0))
  struct array_checker ck = { .str=type_name };
  const ID_List id = ck2list(gwi, &ck);
  if(id == (ID_List)GW_ERROR)
    return NULL;
  Type_Decl* td = new_type_decl(gwi->gwion->mp, id);
  const m_str tl_name = get_type_name(gwi->gwion->env, s, 1);
  if(tl_name) {
    if(!(td->types = str2tl(gwi, tl_name))) {
      free_type_decl(gwi->gwion->mp, td);
      return NULL;
    }
  }
  if(ck.depth) {
    td->array = new_array_sub(gwi->gwion->mp, ck.exp);
    td->array->depth = ck.depth;
  }
  td->flag |= flag;
  return td;
}

ANN static void array_add_exp(struct array_checker *ck, const Exp exp) {
  if(ck->exp)
    ck->exp = (ck->exp->next = exp);
  else
    ck->base = ck->exp = exp;
  ++ck->is_exp;
}

ANN m_bool _array_check(const Gwi gwi, struct array_checker *ck) {
  const m_str base = ck->str;
  const size_t sz = strlen(ck->str);
  char tmp[sz + 1];
  for(m_uint i = 0; i < sz; ++i) {
    const char c = ck->str[i];
    if(c == ']') {
      const m_bool is_end = ck->str[i + 1] == '\0';
      if(!is_end && ck->str[i + 1] != '[')
        break;
      ck->str += i + 2;
      ck->sz += i + 2;
      if(i) {
        if(!ck->is_exp && ck->depth)
          break;
        tmp[i] = '\0';
        const m_uint num = strtol(tmp, NULL, 10);// migth use &endptr and check errno
        const Exp exp = new_prim_int(gwi->gwion->mp, num, loc(gwi));
        array_add_exp(ck, exp);
      } else {
        if(ck->is_exp > 0)
          break;
      }
      ++ck->depth;
      return is_end ? GW_OK : _array_check(gwi, ck);
    }
    if(isdigit(c))
      tmp[i] = c;
    else
      GWI_ERR_B(_("invalid subscript '%c' in '%s'"), c, base)
  }
  GWI_ERR_B(_("incoherent subscript '%s'"), base)
}

ANN m_bool array_check(const Gwi gwi, struct array_checker *ck) {
  ck->str = ck->str ? strchr(ck->str, '[') : NULL;
  if(!ck->str)
    return GW_OK;
  ++ck->str;
  return _array_check(gwi, ck);
}

ANN m_bool check_typename_def(const Gwi gwi, ImportCK *ck) {
  const m_str base = ck->name;
  char str[strlen(base) + 1];
  const m_str c = strchr(ck->name, '<');
  memcpy(str, base, strlen(base) - (c ? strlen(c) : 0));
  str[strlen(base) - (c ? strlen(c) : 0)] = '\0';
  ck->name = str;
  CHECK_OB((ck->sym = str2sym(gwi, str)))
  if(c && (ck->tmpl = tmpl_valid(gwi, c)) == (ID_List)GW_ERROR)
    return GW_ERROR;
  ck->name = base;
  return GW_OK;
}

ANN m_bool ck_ini(const Gwi gwi, const enum importck_type t) {
  if(gwi->ck) // TODO: improve error message
    GWI_ERR_B(_("already importing"))
  gwi->ck = mp_calloc2(gwi->gwion->mp, sizeof(ImportCK));
  gwi->ck->type = t;
  return GW_OK;
}

ANN m_bool ck_ok(const Gwi gwi, const enum importck_type t) {
  if(!gwi->ck)
    GWI_ERR_B(_("import not started"))
  if(gwi->ck->type == t)
    return GW_OK;
  // TODO: improve error message
  GWI_ERR_B(_("already importing"))
}

ANN void ck_end(const Gwi gwi) {
  mp_free2(gwi->gwion->mp, sizeof(ImportCK), gwi->ck);
  gwi->ck = NULL;
}

typedef void (*cleaner) (MemPool, ImportCK*);
static cleaner cleaners[] =
{
  ck_clean_edef,
  ck_clean_udef,
  ck_clean_tdef,
NULL,//  ck_clean_oper,
  ck_clean_item,
  ck_clean_fdef
};

ANN void ck_clean(const Gwi gwi) {
  cleaners[gwi->ck->type](gwi->gwion->mp, gwi->ck);
}

