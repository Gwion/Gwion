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

__attribute__((returns_nonnull))
ANN static Symbol gwisym(const Gwion gwion, const m_str str) {
  return insert_symbol(gwion->st, str);
}

struct td_checker {
  m_str str;
  const loc_t pos;
};

struct AC {
  m_str str;
  Exp base;
  Exp exp;
  m_uint depth;
};

ANN static m_bool ac_run(const Gwi gwi, struct AC *ac);
ANN static Array_Sub mk_array(MemPool mp, struct AC *ac) {
  const Array_Sub array = new_array_sub(mp, ac->base);
  array->depth = ac->depth;
  array->exp = ac->base;
  return array;
}


ANN static Symbol __str2sym(const Gwion gwion, struct td_checker *tdc) {
  char buf[strlen(tdc->str) + 1];
  m_str tmp = buf;
  if(*tdc->str == '@')
    *tmp++ = *tdc->str++;
  while(*tdc->str) {
    const char c = *tdc->str;
    if(!isalnum(c) && c != '_')
      break;
    *tmp++ = *tdc->str++;
  }
  if(tmp == buf)
    GWION_ERR_O(tdc->pos, "empty symbol");
  *tmp = '\0';
  return gwisym(gwion, buf);
}

ANN static inline Symbol _str2sym(const Gwion gwion, struct td_checker *tdc, const m_str path) {
  const Symbol sym = __str2sym(gwion, tdc);
  if(*tdc->str && *tdc->str != ':')
    GWION_ERR_O(tdc->pos, _("illegal character '%c' in path '%s'."), *tdc->str, path)
  return sym;
}

/** convert a string to a symbol, with error checking **/
ANN Symbol str2sym(const Gwi gwi, const m_str path) {
  struct td_checker tdc = { .str=path, .pos=gwi->loc };
  return _str2sym(gwi->gwion, &tdc, path);
}

// only in enum.c
ANN ID_List str2symlist(const Gwi gwi, const m_str path) {
  DECL_OO(const Symbol, sym, = str2sym(gwi, path))
  return new_id_list(gwi->gwion->mp, sym, loc(gwi));
}

ANN Var_Decl str2var(const Gwi gwi, const m_str path) {
  struct td_checker tdc = { .str=path, .pos=gwi->loc };
  DECL_OO(const Symbol, sym, = __str2sym(gwi->gwion, &tdc))
  struct AC ac = { .str = tdc.str };
  CHECK_BO(ac_run(gwi, &ac))
  const Array_Sub array = ac.depth ?
    mk_array(gwi->gwion->mp, &ac) : NULL;
  return new_var_decl(gwi->gwion->mp, sym, array, loc(gwi));
}

// only in udef.c
ANN Var_Decl_List str2varlist(const Gwi gwi, const m_str path) {
  DECL_OO(const Var_Decl, var, = str2var(gwi, path))
  return new_var_decl_list(gwi->gwion->mp, var, NULL);
}

ANN static ID_List _tmpl_list(const Gwi gwi, struct td_checker *tdc) {
  DECL_OO(const Symbol, sym, = __str2sym(gwi->gwion, tdc))
  ID_List next = NULL;
  if(*tdc->str == ',') {
    ++tdc->str;
    if(!(next = _tmpl_list(gwi, tdc)) || next == (ID_List)GW_ERROR)
      return (ID_List)GW_ERROR;
  }
  const ID_List list = new_id_list(gwi->gwion->mp, sym, loc(gwi));
  list->next = next;
  return list;
}

ANN static ID_List __tmpl_list(const Gwi gwi, struct td_checker *tdc) {
  if(tdc->str[0] != ':')
    return NULL;
  if(tdc->str[1] != '[')
    return (ID_List)GW_ERROR;
  tdc->str += 2;
  const ID_List list =  _tmpl_list(gwi, tdc);
  if(list == (ID_List)GW_ERROR)
    return (ID_List)GW_ERROR;
  if(tdc->str[0] != ']') {
// unfinished template
    if(list)
      free_id_list(gwi->gwion->mp, list);
    return (ID_List)GW_ERROR;
  }
  ++tdc->str;
  return list;
}

ANN m_bool check_typename_def(const Gwi gwi, ImportCK *ck) {
  struct td_checker tdc = { .str= ck->name, .pos=gwi->loc };
  if(!(ck->sym = _str2sym(gwi->gwion, &tdc, tdc.str)))
    return GW_ERROR;
  ID_List il = __tmpl_list(gwi, &tdc);
  if(il == (ID_List)GW_ERROR)
    return GW_ERROR;
  ck->tmpl = il;
  ck->name = s_name(ck->sym);
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
  memset(gwi->ck, 0, sizeof(ImportCK));
}

ANN static Type_Decl* _str2decl(const Gwi gwi, struct td_checker *tdc);
ANN Type_List __str2tl(const Gwi gwi, struct td_checker *tdc) {
  Type_Decl *td = _str2decl(gwi, tdc);
  if(!td)
    GWI_ERR_O("invalid types");
  Type_List next = NULL;
  if(*tdc->str == ',') {
    ++tdc->str;
    if(!(next = __str2tl(gwi, tdc))) {
      free_type_decl(gwi->gwion->mp, td);
      return NULL;
    }
  }
  return new_type_list(gwi->gwion->mp, td, next);
}

ANN static Type_List td_tmpl(const Gwi gwi, struct td_checker *tdc) {
  if(*tdc->str != ':')
    return NULL; // GW_PASS
  ++tdc->str;
  if(*tdc->str != '[') {
    GWI_ERR("invalid character");
    return (Type_List)GW_ERROR;
  }
  ++tdc->str;
  Type_List tl = __str2tl(gwi, tdc);
  if(!tl)
    return (Type_List)GW_ERROR;
  if(tdc->str[0] != ']') {
    free_type_list(gwi->gwion->mp, tl);
    GWI_ERR("unfinished template");
    return (Type_List)GW_ERROR;
  }
  ++tdc->str;
  return tl;
}

ANN static void ac_add_exp(struct AC *ac, const Exp exp) {
  if(ac->exp)
    ac->exp = (ac->exp->next = exp);
  else
    ac->base = ac->exp = exp;
}


ANN static Type_Decl* _str2decl(const Gwi gwi, struct td_checker *tdc) {
  DECL_OO(const Symbol, sym, = __str2sym(gwi->gwion, tdc))
  struct AC ac = { .str = tdc->str };
  CHECK_BO(ac_run(gwi, &ac))
  tdc->str = ac.str;
  Type_List tl = td_tmpl(gwi, tdc);
  if(tl == (Type_List)GW_ERROR)
    return NULL;
  Type_Decl *next = NULL;
  if(*tdc->str == '.') {
    ++tdc->str;
    if(!(next =  _str2decl(gwi, tdc))) {
      if(tl)
        free_type_list(gwi->gwion->mp, tl);
      if(ac.base)
        free_exp(gwi->gwion->mp, ac.base);
      return NULL;
    }
  }
  Type_Decl *td = new_type_decl(gwi->gwion->mp, sym, loc(gwi));
  td->next = next;
  td->types = tl;
  if(ac.depth)
    td->array = mk_array(gwi->gwion->mp, &ac);
  return td;
}

ANN Type_Decl* str2decl(const Gwi gwi, const m_str str) {
  const ae_flag flag = strncmp(str, "nonnull ", 8) ? ae_flag_none : ae_flag_nonnull;
  struct td_checker tdc = { .str=str, .pos=gwi->loc };
  if(flag == ae_flag_nonnull)
    tdc.str += 8;
  DECL_OO(Type_Decl *, td, = _str2decl(gwi, &tdc))
  if(*tdc.str) {
    free_type_decl(gwi->gwion->mp, td);
    GWI_ERR_O("excedental character '%c'", *tdc.str);
  }
  td->flag |= flag;
  return td;
}

ANN Type str2type(const Gwi gwi, const m_str str) {
  DECL_OO(Type_Decl *, td, = str2decl(gwi, str))
  const Type t = known_type(gwi->gwion->env, td);
  if(t)
    return t;
  free_type_decl(gwi->gwion->mp, td);
  return NULL;
}

ANN static inline m_bool ac_finish(const Gwi gwi, struct AC *ac) {
  if(*ac->str == ']')
    return GW_OK;
  GWI_ERR_B("unfinished array");
}

ANN static inline m_bool ac_num(const Gwi gwi, const m_int num) {
  if(num >= 0)
    return GW_OK;
  GWI_ERR_B("negative array dimension")
}

ANN static inline m_bool ac_exp(const Gwi gwi, struct AC *ac) {
  if(!ac->depth || ac->base)
    return GW_OK;
  GWI_ERR_B("malformed array [][...]")
}

ANN static inline m_bool ac_noexp(const Gwi gwi, struct AC *ac) {
  if(!ac->exp)
    return GW_OK;
  GWI_ERR_B("malformed array [...][]")
}

ANN static m_bool _ac_run(const Gwi gwi, struct AC *ac) {
  const m_str str = ac->str;
  const m_int num = strtol(str, &ac->str, 10);
  CHECK_BB(ac_finish(gwi, ac))
  if(str != ac->str) {
    CHECK_BB(ac_num(gwi, num))
    CHECK_BB(ac_exp(gwi, ac))
    const Exp exp = new_prim_int(gwi->gwion->mp, num, loc(gwi));
    ac_add_exp(ac, exp);
  } else
    CHECK_BB(ac_noexp(gwi, ac))
  ++ac->str;
  return GW_OK;
}

ANN static m_bool ac_run(const Gwi gwi, struct AC *ac) {
  while(*ac->str) {
    if(*ac->str != '[')
      break;
    ++ac->str;
    CHECK_BB(_ac_run(gwi, ac))
    ++ac->depth;
  }
  return GW_OK;
}
