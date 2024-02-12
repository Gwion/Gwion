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
  const loc_t loc;
};

struct AC {
  m_str  str;
  Exp*    base;
  Exp*    exp;
  loc_t  loc;
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
  if (tmp == buf) GWION_ERR_O(tdc->loc, "empty symbol");
  *tmp = '\0';
  return gwisym(gwion, buf);
}

ANN static inline Symbol _str2sym(const Gwion gwion, struct td_checker *tdc,
                                  const m_str path) {
  const Symbol sym = __str2sym(gwion, tdc);
  if (*tdc->str && *tdc->str != ':')
    GWION_ERR_O(tdc->loc, _("illegal character '%c' in path '%s'."), *tdc->str,
                path)
  return sym;
}

/** convert a string to a symbol, with error checking **/
ANN Symbol str2sym(const Gwion gwion, const m_str path, const loc_t loc) {
  struct td_checker tdc = {.str = path, .loc = loc};
  return _str2sym(gwion, &tdc, path);
}

ANN m_bool str2var(const Gwion gwion, Var_Decl *vd, const m_str path, const loc_t loc) {
  struct td_checker tdc = {.str = path, .loc = loc};
  DECL_OB(const Symbol, sym, = __str2sym(gwion, &tdc));
  struct AC ac = {.str = tdc.str, .loc = loc};
  CHECK_BB(ac_run(gwion, &ac));
  vd->tag = MK_TAG(sym, loc);
  vd->value = NULL;
  return GW_OK;
}

#define SPEC_ERROR (Specialized_List) GW_ERROR
ANN static bool _tmpl_list(const Gwion        gwion,
                                       struct td_checker *tdc, Specialized_List *sl) {
  if(unlikely(!strncmp(tdc->str, "...", 3))) {
    tdc->str += 3;
    Specialized spec = { .tag = MK_TAG(insert_symbol(gwion->st, "..."), tdc->loc) };
    mp_vector_add(gwion->mp, sl, Specialized, spec);
    return true;
  }
  DECL_OO(const Symbol, sym, = __str2sym(gwion, tdc));
  // TODO: handle traits?
  Specialized spec = { .tag = MK_TAG(sym, tdc->loc) };
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
  struct td_checker tdc = {.str = ck->name, .loc = gwi->loc};
  if (!(ck->sym = _str2sym(gwi->gwion, &tdc, tdc.str))) return GW_ERROR;
  Specialized_List sl = __tmpl_list(gwi->gwion, &tdc);
  if (sl == SPEC_ERROR) return GW_ERROR;
  ck->sl   = sl;
  ck->name = s_name(ck->sym);
  return GW_OK;
}

ANN static Type_Decl *_str2td(const Gwion gwion, struct td_checker *tdc);
ANN bool str2tl(const Gwion gwion, struct td_checker *tdc, TmplArg_List *tl) {
  // we probably need smth better
  if(isalpha(*tdc->str)) {
    TmplArg targ = {
      .type = tmplarg_td,
      .d = { .td = _str2td(gwion, tdc) }
    };
    if (!targ.d.td) GWION_ERR_B(tdc->loc, "invalid types");
    mp_vector_add(gwion->mp, tl, TmplArg, targ);
    if (*tdc->str == ',') {
      ++tdc->str;
      if (!str2tl(gwion, tdc, tl))
        return false;
    }
  } else GWION_ERR_B(tdc->loc, "invalid character in template list");
  return true;
}

ANN static TmplArg_List td_tmpl(const Gwion gwion, struct td_checker *tdc) {
  if (*tdc->str != ':') return NULL; // GW_PASS
  ++tdc->str;
  if (*tdc->str != '[') {
    GWION_ERR(tdc->loc, "invalid character");
    return (TmplArg_List)GW_ERROR;
  }
  ++tdc->str;
  TmplArg_List tl = new_mp_vector(gwion->mp, TmplArg, 0);
  if (!str2tl(gwion, tdc, &tl)) {
    free_tmplarg_list(gwion->mp, tl);
    return (TmplArg_List)GW_ERROR;
  }
  if (tdc->str[0] != ']') {
    free_tmplarg_list(gwion->mp, tl);
    GWION_ERR(tdc->loc, "unfinished template");
    return (TmplArg_List)GW_ERROR;
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
    mp_vector_add(gwion->mp, &args, Arg, (Arg){ .var = {.td = td }});
  } while(*tdc->str == ',' && tdc->str++);
  return args;
}

ANN static Type_Decl *str2td_fptr(const Gwion gwion, struct td_checker *tdc) {
  const m_str base = tdc->str;
  tdc->str++;
  Type_Decl *const ret_td = _str2td(gwion, tdc);
  const TmplArg_List tl = td_tmpl(gwion, tdc);
  if (tl == (TmplArg_List)GW_ERROR) {
    free_type_decl(gwion->mp, ret_td);
    return NULL;
  }
  const uint option = get_n(tdc, '?');
  tdc->str++;
  Arg_List args = fptr_args(gwion, tdc);
  if (args == (Arg_List)GW_ERROR) {
    if(tl) free_tmplarg_list(gwion->mp, tl);
    free_type_decl(gwion->mp, ret_td);
    return NULL;
  }
  if(tdc->str[0] != ')' || tdc->str[1] != ')') {
    if(tl) free_tmplarg_list(gwion->mp, tl);
    if(args) free_arg_list(gwion->mp, args);
    return NULL;
  }
  tdc->str += 2;
  struct AC ac = {.str = tdc->str, .loc = tdc->loc};
  if(ac_run(gwion, &ac) < 0 ) {
    if(tl) free_tmplarg_list(gwion->mp, tl);
    if(args) free_arg_list(gwion->mp, args);
    return NULL;
  }
  Func_Base *fbase = new_func_base(gwion->mp, ret_td, insert_symbol(gwion->st, base), args, ae_flag_none, tdc->loc);
  const Fptr_Def fptr = new_fptr_def(gwion->mp, fbase);
  Type_Decl *td = new_type_decl(gwion->mp, insert_symbol(gwion->st, base), tdc->loc);
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
  TmplArg_List tl = td_tmpl(gwion, tdc);
  struct AC ac = {.str = tdc->str, .loc = tdc->loc};
  CHECK_BO(ac_run(gwion, &ac));
  tdc->str     = ac.str;
  if (tl == (TmplArg_List)GW_ERROR) return NULL;
  const uint option = get_n(tdc, '?');
  Type_Decl *next   = NULL;
  if (*tdc->str == '.') {
    ++tdc->str;
    if (!(next = _str2td(gwion, tdc))) {
      if (tl) free_tmplarg_list(gwion->mp, tl);
      if (ac.base) free_exp(gwion->mp, ac.base);
      return NULL;
    }
  }
  Type_Decl *td = new_type_decl(gwion->mp, sym, tdc->loc);
  td->next      = next;
  td->types     = tl;
  td->option    = option;
  td->ref       = ref;
  if (ac.depth) td->array = mk_array(gwion->mp, &ac);
  return td;
}

ANN Type_Decl *str2td(const Gwion gwion, const m_str str, const loc_t loc) {
  struct td_checker tdc = {.str = str, .loc = loc};
  DECL_OO(Type_Decl *, td, = _str2td(gwion, &tdc));
  if(*tdc.str) {
    free_type_decl(gwion->mp, td);
    GWION_ERR_O(loc, "excedental character '%c' in '%s'", *tdc.str, str);
  }
  return td;
}

ANN Type str2type(const Gwion gwion, const m_str str, const loc_t loc) {
  DECL_OO(Type_Decl *, td, = str2td(gwion, str, loc));
  const Type t = known_type(gwion->env, td);
  free_type_decl(gwion->mp, td);
  return t;
}

struct td_info {
  TmplArg_List tl;
  Gwfmt     *fmt;
};

ANN static void td_fullname(const Env env, GwText *text, const Type t) {
  const Type owner = t->info->value->from->owner_class;
  if (owner) {
    td_fullname(env, text, owner);
    text_add(text, ".");
  }
  text_add(text, t->name);
}

ANN Exp* td2exp(const MemPool mp, const Type_Decl *td) {
  Exp* base = new_prim_id(mp, td->tag.sym, td->tag.loc);
  Type_Decl *next = td->next;
  while(next) {
    base = new_exp_dot(mp, base, next->tag.sym, td->tag.loc);
    next = next->next;
  }
  return base;
}

ANN static m_bool td_info_run(const Env env, struct td_info *info) {
  const Gwion gwion = env->gwion;
  TmplArg_List tl = info->tl;
  for(uint32_t i = 0; i < tl->len; i++) {
    if (i) text_add(&info->fmt->ls->text, ",");
    TmplArg *targ = mp_vector_at(tl, TmplArg, i);
    if(targ->type == tmplarg_td) {
      // we may need to stop errors
      if(env->context) env->context->error = true;
      const Type t = known_type(env, targ->d.td);
      if(env->context) env->context->error = false;
      if(t)
        td_fullname(env, &info->fmt->ls->text, t);
      else {
        Exp* exp = td2exp(gwion->mp, targ->d.td);
        if(traverse_exp(env, exp)) {
          if(is_class(gwion, exp->type)) {
            td_fullname(env, &info->fmt->ls->text, exp->type);
            free_exp(gwion->mp, exp);
          } else gwfmt_exp(info->fmt, exp);
        } else GWION_ERR_B(targ->d.td->tag.loc, "invalid template argument");
      }
    } else {
      Exp* exp = targ->d.exp;
      if(check_exp(env, targ->d.exp)) {
        if(!is_class(gwion, exp->type))
          gwfmt_exp(info->fmt, exp);
        else
          td_fullname(env, &info->fmt->ls->text, targ->d.exp->type);
      }
    }
  }
  return GW_OK;
}

ANEW ANN m_str type2str(const Gwion gwion, const Type t,
                        const loc_t loc NUSED) {
  GwText text;
  text_init(&text, gwion->mp);
  td_fullname(gwion->env, &text, t);
  return text.str;
}

ANEW ANN m_str tl2str(const Gwion gwion, const TmplArg_List tl,
                      const loc_t loc NUSED) {
  struct GwfmtState ls = {.minimize=true, .ppa = gwion->ppa};
  text_init(&ls.text, gwion->mp);
  Gwfmt l = {.mp = gwion->mp, .st = gwion->st, .ls = &ls, .line = 1, .last = cht_nl };
  struct td_info info = {.tl = tl, .fmt = &l };
  CHECK_BO(td_info_run(gwion->env, &info));
  return ls.text.str;
}

ANN static inline m_bool ac_finish(const Gwion gwion, const struct AC *ac) {
  if (*ac->str == ']') return GW_OK;
  GWION_ERR_B(ac->loc, "unfinished array");
}

ANN static inline m_bool ac_num(const Gwion gwion, const struct AC *ac,
                                const m_int num) {
  if (num >= 0) return GW_OK;
  GWION_ERR_B(ac->loc, "negative array dimension")
}

ANN static inline m_bool ac_exp(const Gwion gwion, const struct AC *ac) {
  if (!ac->depth || ac->base) return GW_OK;
  GWION_ERR_B(ac->loc, "malformed array [][...]")
}

ANN static void ac_add_exp(struct AC *ac, Exp* exp) {
  if (ac->exp)
    ac->exp = (ac->exp->next = exp);
  else
    ac->base = ac->exp = exp;
}

ANN static inline m_bool ac_noexp(const Gwion gwion, struct AC *ac) {
  if (!ac->exp) return GW_OK;
  GWION_ERR_B(ac->loc, "malformed array [...][]")
}

ANN static m_bool _ac_run(const Gwion gwion, struct AC *const ac) {
  const m_str str = ac->str;
  const m_int num = strtol(str, &ac->str, 10);
  CHECK_BB(ac_finish(gwion, ac));
  if (str != ac->str) {
    CHECK_BB(ac_num(gwion, ac, num));
    CHECK_BB(ac_exp(gwion, ac));
    Exp* exp = new_prim_int(gwion->mp, num, ac->loc);
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
