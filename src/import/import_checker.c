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

ANN m_bool str2var(const Gwion gwion, Var_Decl *vd, const m_str path, const loc_t pos) {
  struct td_checker tdc = {.str = path, .pos = pos};
  DECL_OB(const Symbol, sym, = __str2sym(gwion, &tdc));
  struct AC ac = {.str = tdc.str, .pos = pos};
  CHECK_BB(ac_run(gwion, &ac));
  vd->xid = sym;
  vd->value = NULL;
  vd->pos = pos;
  return GW_OK;
}

#define SPEC_ERROR (Specialized_List) GW_ERROR
ANN static bool _tmpl_list(const Gwion        gwion,
                                       struct td_checker *tdc, Specialized_List *sl) {
  DECL_OO(const Symbol, sym, = __str2sym(gwion, tdc));
  // TODO: handle traits?
  Specialized spec = {
    .xid = sym,
    .pos = tdc->pos
  };
  mp_vector_add(gwion->mp, sl, Specialized, spec);
  if (*tdc->str == ',') {
    ++tdc->str;
    if (!_tmpl_list(gwion, tdc, sl))
      return false;
  }
  return true;
}

ANN static Specialized_List __tmpl_list(const Gwion        gwion,
                                        struct td_checker *tdc) {
  if (tdc->str[0] != ':') return NULL;
  if (tdc->str[1] != '[') return SPEC_ERROR;
  tdc->str += 2;
  Specialized_List sl = new_mp_vector(gwion->mp, Specialized, 0);
  if(!_tmpl_list(gwion, tdc, &sl) || tdc->str[0] != ']') {
    free_specialized_list(gwion->mp, sl);
  }
  ++tdc->str;
  return sl;
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
ANN bool str2tl(const Gwion gwion, struct td_checker *tdc, Type_List *tl) {
  Type_Decl *td = _str2td(gwion, tdc);
  if (!td) GWION_ERR_B(tdc->pos, "invalid types");
  mp_vector_add(gwion->mp, tl, Type_Decl*, td);
  if (*tdc->str == ',') {
    ++tdc->str;
    if (!str2tl(gwion, tdc, tl))
      return false;
  }
  return true;
}

ANN static Type_List td_tmpl(const Gwion gwion, struct td_checker *tdc) {
  if (*tdc->str != ':') return NULL; // GW_PASS
  ++tdc->str;
  if (*tdc->str != '[') {
    GWION_ERR(tdc->pos, "invalid character");
    return (Type_List)GW_ERROR;
  }
  ++tdc->str;
  Type_List tl = new_mp_vector(gwion->mp, Type_Decl*, 0);
  if (!str2tl(gwion, tdc, &tl)) {
    free_type_list(gwion->mp, tl);
    return (Type_List)GW_ERROR;
  }
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

ANN static Arg_List fptr_args(const Gwion gwion, struct td_checker *tdc) {
  if(tdc->str[1] == ')')
    return NULL;
  Arg_List args = new_mp_vector(gwion->mp, Arg, 0);
  do {
    Type_Decl *td = _str2td(gwion, tdc);
    if(!td) {
      free_arg_list(gwion->mp, args);
      return (Arg_List)GW_ERROR;
    }
    mp_vector_add(gwion->mp, &args, Arg, (Arg){ .td = td });
  } while(*tdc->str == ',' && tdc->str++);
  return args;
}

ANN static Type_Decl *str2td_fptr(const Gwion gwion, struct td_checker *tdc) {
  const m_str base = tdc->str;
  tdc->str++;
  Type_Decl *const ret_td = _str2td(gwion, tdc);
  const Type_List tl = td_tmpl(gwion, tdc);
  if (tl == (Type_List)GW_ERROR) {
    free_type_decl(gwion->mp, ret_td);
    return NULL;
  }
  const uint option = get_n(tdc, '?');
  tdc->str++;
  Arg_List args = fptr_args(gwion, tdc);
  if (args == (Arg_List)GW_ERROR) {
    if(tl) free_type_list(gwion->mp, tl);
    free_type_decl(gwion->mp, ret_td);
    return NULL;
  }
  if(tdc->str[0] != ')' || tdc->str[1] != ')') {
    if(tl) free_type_list(gwion->mp, tl);
    if(args) free_arg_list(gwion->mp, args);
    return NULL;
  }
  tdc->str += 2;
  struct AC ac = {.str = tdc->str, .pos = tdc->pos};
  if(ac_run(gwion, &ac) < 0 ) {
    if(tl) free_type_list(gwion->mp, tl);
    if(args) free_arg_list(gwion->mp, args);
    return NULL;
  }
  Func_Base *fbase = new_func_base(gwion->mp, ret_td, insert_symbol(gwion->st, base), args, ae_flag_none, tdc->pos);
  const Fptr_Def fptr = new_fptr_def(gwion->mp, fbase);
  Type_Decl *td = new_type_decl(gwion->mp, insert_symbol(gwion->st, base), tdc->pos);
  td->fptr = fptr;
  td->types = tl;
  if (ac.depth) td->array = mk_array(gwion->mp, &ac);
  tdc->str = ac.str;
  td->option = option;
  return td;
}

ANN static Type_Decl *_str2td(const Gwion gwion, struct td_checker *tdc) {
  const uint ref = get_n(tdc, '&');
  if(*tdc->str == '(') {
    Type_Decl *const td = str2td_fptr(gwion, tdc);
    td->ref = ref;
    return td;
  }
  DECL_OO(const Symbol, sym, = __str2sym(gwion, tdc));
  Type_List tl = td_tmpl(gwion, tdc);
  struct AC ac = {.str = tdc->str, .pos = tdc->pos};
  CHECK_BO(ac_run(gwion, &ac));
  tdc->str     = ac.str;
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
  if(*tdc.str) {
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
  for(uint32_t i = 0; i < tl->len; i++) {
    if (i) text_add(&info->text, ",");
    DECL_OB(Type_Decl *, td, = *mp_vector_at(tl, Type_Decl*, i));
    DECL_OB(const Type, t, = known_type(env, td));
    td_fullname(env, &info->text, t);
  }
  return GW_OK;
}

ANEW ANN m_str type2str(const Gwion gwion, const Type t,
                        const loc_t pos NUSED) {
  GwText     text  = {.mp = gwion->mp};
  td_fullname(gwion->env, &text, t);
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
