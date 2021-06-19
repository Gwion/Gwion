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
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

struct td_checker {
  m_str       str;
  const loc_t pos;
};

struct AC {
  m_str  str;
  Exp    base;
  Exp    exp;
  loc_t  pos;
  m_uint depth;
};

ANN static m_bool    ac_run(const Gwion gwion, struct AC *const ac);
ANN static Array_Sub mk_array(MemPool mp, struct AC *ac) {
  const Array_Sub array = new_array_sub(mp, ac->base);
  array->depth          = ac->depth;
  array->exp            = ac->base;
  return array;
}

__attribute__((returns_nonnull)) ANN static Symbol gwisym(const Gwion gwion,
                                                          const m_str str) {
  return insert_symbol(gwion->st, str);
}

ANN static Symbol __str2sym(const Gwion gwion, struct td_checker *tdc) {
  char  buf[strlen(tdc->str) + 1];
  m_str tmp = buf;
  if (*tdc->str == '@') *tmp++ = *tdc->str++;
  while (*tdc->str) {
    const char c = *tdc->str;
    if (!isalnum(c) && c != '_') break;
    *tmp++ = *tdc->str++;
  }
  if (tmp == buf) GWION_ERR_O(tdc->pos, "empty symbol");
  *tmp = '\0';
  return gwisym(gwion, buf);
}

ANN static inline Symbol _str2sym(const Gwion gwion, struct td_checker *tdc,
                                  const m_str path) {
  const Symbol sym = __str2sym(gwion, tdc);
  if (*tdc->str && *tdc->str != ':')
    GWION_ERR_O(tdc->pos, _("illegal character '%c' in path '%s'."), *tdc->str,
                path)
  return sym;
}

/** convert a string to a symbol, with error checking **/
ANN Symbol str2sym(const Gwion gwion, const m_str path, const loc_t pos) {
  struct td_checker tdc = {.str = path, .pos = pos};
  return _str2sym(gwion, &tdc, path);
}

// only in enum.c
ANN ID_List str2symlist(const Gwion gwion, const m_str path, const loc_t pos) {
  DECL_OO(const Symbol, sym, = str2sym(gwion, path, pos));
  return new_id_list(gwion->mp, sym);
}

ANN Var_Decl str2var(const Gwion gwion, const m_str path, const loc_t pos) {
  struct td_checker tdc = {.str = path, .pos = pos};
  DECL_OO(const Symbol, sym, = __str2sym(gwion, &tdc));
  struct AC ac = {.str = tdc.str, .pos = pos};
  CHECK_BO(ac_run(gwion, &ac));
  const Array_Sub array = ac.depth ? mk_array(gwion->mp, &ac) : NULL;
  return new_var_decl(gwion->mp, sym, array, pos);
}

// only in udef.c
ANN Var_Decl_List str2varlist(const Gwion gwion, const m_str path,
                              const loc_t pos) {
  DECL_OO(const Var_Decl, var, = str2var(gwion, path, pos));
  return new_var_decl_list(gwion->mp, var, NULL);
}

#define SPEC_ERROR (Specialized_List) GW_ERROR
ANN static Specialized_List _tmpl_list(const Gwion        gwion,
                                       struct td_checker *tdc) {
  DECL_OO(const Symbol, sym, = __str2sym(gwion, tdc));
  Specialized_List next = NULL;
  if (*tdc->str == ',') {
    ++tdc->str;
    if (!(next = _tmpl_list(gwion, tdc)) || next == SPEC_ERROR)
      return SPEC_ERROR;
  }
  // TODO: handle traits?
  const Specialized_List list =
      new_specialized_list(gwion->mp, sym, NULL, tdc->pos);
  list->next = next;
  return list;
}

ANN static Specialized_List __tmpl_list(const Gwion        gwion,
                                        struct td_checker *tdc) {
  if (tdc->str[0] != ':') return NULL;
  if (tdc->str[1] != '[') return SPEC_ERROR;
  tdc->str += 2;
  const Specialized_List list = _tmpl_list(gwion, tdc);
  if (list == SPEC_ERROR) return SPEC_ERROR;
  if (tdc->str[0] != ']') { // unfinished template
    if (list) free_specialized_list(gwion->mp, list);
    return SPEC_ERROR;
  }
  ++tdc->str;
  return list;
}

ANN m_bool check_typename_def(const Gwi gwi, ImportCK *ck) {
  struct td_checker tdc = {.str = ck->name, .pos = gwi->loc};
  if (!(ck->sym = _str2sym(gwi->gwion, &tdc, tdc.str))) return GW_ERROR;
  Specialized_List sl = __tmpl_list(gwi->gwion, &tdc);
  if (sl == SPEC_ERROR) return GW_ERROR;
  ck->sl   = sl;
  ck->name = s_name(ck->sym);
  return GW_OK;
}

ANN static Type_Decl *_str2td(const Gwion gwion, struct td_checker *tdc);
ANN Type_List         __str2tl(const Gwion gwion, struct td_checker *tdc) {
  Type_Decl *td = _str2td(gwion, tdc);
  if (!td) GWION_ERR_O(tdc->pos, "invalid types");
  Type_List next = NULL;
  if (*tdc->str == ',') {
    ++tdc->str;
    if (!(next = __str2tl(gwion, tdc))) {
      free_type_decl(gwion->mp, td);
      return NULL;
    }
  }
  return new_type_list(gwion->mp, td, next);
}

ANN static Type_List td_tmpl(const Gwion gwion, struct td_checker *tdc) {
  if (*tdc->str != ':') return NULL; // GW_PASS
  ++tdc->str;
  if (*tdc->str != '[') {
    GWION_ERR(tdc->pos, "invalid character");
    return (Type_List)GW_ERROR;
  }
  ++tdc->str;
  Type_List tl = __str2tl(gwion, tdc);
  if (!tl) return (Type_List)GW_ERROR;
  if (tdc->str[0] != ']') {
    free_type_list(gwion->mp, tl);
    GWION_ERR(tdc->pos, "unfinished template");
    return (Type_List)GW_ERROR;
  }
  ++tdc->str;
  return tl;
}

ANN static inline uint get_n(struct td_checker *tdc, const char c) {
  uint n = 0;
  while (*tdc->str == c) {
    n++;
    ++tdc->str;
  }
  return n;
}

ANN static Type_Decl *_str2td(const Gwion gwion, struct td_checker *tdc) {
  const uint ref = get_n(tdc, '&');
  DECL_OO(const Symbol, sym, = __str2sym(gwion, tdc));
  struct AC ac = {.str = tdc->str, .pos = tdc->pos};
  CHECK_BO(ac_run(gwion, &ac));
  tdc->str     = ac.str;
  Type_List tl = td_tmpl(gwion, tdc);
  if (tl == (Type_List)GW_ERROR) return NULL;
  const uint option = get_n(tdc, '?');
  Type_Decl *next   = NULL;
  if (*tdc->str == '.') {
    ++tdc->str;
    if (!(next = _str2td(gwion, tdc))) {
      if (tl) free_type_list(gwion->mp, tl);
      if (ac.base) free_exp(gwion->mp, ac.base);
      return NULL;
    }
  }
  Type_Decl *td = new_type_decl(gwion->mp, sym, tdc->pos);
  td->next      = next;
  td->types     = tl;
  td->option    = option;
  td->ref       = ref;
  if (ac.depth) td->array = mk_array(gwion->mp, &ac);
  return td;
}

ANN Type_Decl *str2td(const Gwion gwion, const m_str str, const loc_t pos) {
  struct td_checker tdc = {.str = str, .pos = pos};
  DECL_OO(Type_Decl *, td, = _str2td(gwion, &tdc));
  if (*tdc.str) {
    free_type_decl(gwion->mp, td);
    GWION_ERR_O(pos, "excedental character '%c' in '%s'", *tdc.str, str);
  }
  return td;
}

ANN Type str2type(const Gwion gwion, const m_str str, const loc_t pos) {
  DECL_OO(Type_Decl *, td, = str2td(gwion, str, pos));
  const Type t = known_type(gwion->env, td);
  free_type_decl(gwion->mp, td);
  return t;
}

struct td_info {
  Type_List tl;
  GwText    text;
};

ANN static void td_fullname(const Env env, GwText *text, const Type t) {
  const Type owner = t->info->value->from->owner_class;
  if (owner) {
    td_fullname(env, text, owner);
    text_add(text, ".");
  }
  text_add(text, t->name);
}

ANN static m_bool td_info_run(const Env env, struct td_info *info) {
  Type_List tl = info->tl;
  do {
    DECL_OB(const Type, t, = known_type(env, tl->td));
    td_fullname(env, &info->text, t);
    if (tl->next) text_add(&info->text, ",");
  } while ((tl = tl->next));
  return GW_OK;
}

ANEW ANN m_str type2str(const Gwion gwion, const Type t,
                        const loc_t pos NUSED) {
  GwText     text  = {.mp = gwion->mp};
  const Type owner = t->info->value->from->owner_class;
  if (owner) {
    td_fullname(gwion->env, &text, owner);
    text_add(&text, ".");
  }
  text_add(&text, t->name);
  return text.str;
}

ANEW ANN m_str tl2str(const Gwion gwion, const Type_List tl,
                      const loc_t pos NUSED) {
  struct td_info info = {.tl = tl, {.mp = gwion->mp}};
  CHECK_BO(td_info_run(gwion->env, &info));
  return info.text.str;
}

ANN static inline m_bool ac_finish(const Gwion gwion, const struct AC *ac) {
  if (*ac->str == ']') return GW_OK;
  GWION_ERR_B(ac->pos, "unfinished array");
}

ANN static inline m_bool ac_num(const Gwion gwion, const struct AC *ac,
                                const m_int num) {
  if (num >= 0) return GW_OK;
  GWION_ERR_B(ac->pos, "negative array dimension")
}

ANN static inline m_bool ac_exp(const Gwion gwion, const struct AC *ac) {
  if (!ac->depth || ac->base) return GW_OK;
  GWION_ERR_B(ac->pos, "malformed array [][...]")
}

ANN static void ac_add_exp(struct AC *ac, const Exp exp) {
  if (ac->exp)
    ac->exp = (ac->exp->next = exp);
  else
    ac->base = ac->exp = exp;
}

ANN static inline m_bool ac_noexp(const Gwion gwion, struct AC *ac) {
  if (!ac->exp) return GW_OK;
  GWION_ERR_B(ac->pos, "malformed array [...][]")
}

ANN static m_bool _ac_run(const Gwion gwion, struct AC *const ac) {
  const m_str str = ac->str;
  const m_int num = strtol(str, &ac->str, 10);
  CHECK_BB(ac_finish(gwion, ac));
  if (str != ac->str) {
    CHECK_BB(ac_num(gwion, ac, num));
    CHECK_BB(ac_exp(gwion, ac));
    const Exp exp = new_prim_int(gwion->mp, num, ac->pos);
    // set type: otherwise could fail at emit time
    exp->type = gwion->type[et_int];
    ac_add_exp(ac, exp);
  } else
    CHECK_BB(ac_noexp(gwion, ac));
  ++ac->str;
  return GW_OK;
}

ANN static m_bool ac_run(const Gwion gwion, struct AC *const ac) {
  while (*ac->str) {
    if (*ac->str != '[') break;
    ++ac->str;
    CHECK_BB(_ac_run(gwion, ac));
    ++ac->depth;
  }
  return GW_OK;
}
