#ifndef _IMPORT_CHECKER
#define _IMPORT_CHECKER

enum importck_type { ck_edef, ck_udef, ck_tdef, ck_oper, ck_item, ck_fdef };

typedef struct ImportCK { // name_checker ?
  m_str  name;
  Symbol sym;
  union {
    ID_List          tmpl;
    Specialized_List sl;
    Exp*              exp;
  };
  union {
    Symbol xid; // union name
    f_xfun addr;
  };
  union {
    Variable_List     list; // union
    struct Vector_ v;
    MP_Vector *mpv;
    //    ID_List curr;// enum
  };
  union {
    Type_Decl *td;   // typedef
    ID_List    curr; // enum
  };
  enum importck_type type;
  ae_flag            flag; // ????
} ImportCK;

typedef struct OperCK { // name_checker ?
  m_str  ret;
  Symbol sym;
  Type (*ck)(Env, void *);       // oper
  //bool (*em)(Emitter, void *); // oper should be opem
  opem em;
  m_str          lhs;            // oper
  m_str          rhs;            // oper
  struct Vector_ effect;
} OperCK;

ANN void func_checker_clean(const Gwi gwi, struct ImportCK *ck);
ANN bool check_typename_def(const Gwi gwi, struct ImportCK *ck);

ANN Symbol     str2sym(const Gwion, const m_str, const loc_t);
ANN ID_List    str2symlist(const Gwion, const m_str, const loc_t);
ANN bool       str2var(const Gwion, Var_Decl*, const m_str, const loc_t);
ANN Type_Decl *str2td(const Gwion, const m_str, const loc_t);
ANN Type       str2type(const Gwion, const m_str, const loc_t);

// those functions return a mp_alloced string
ANEW ANN m_str tl2str(const Gwion, const TmplArg_List, const loc_t);
ANEW ANN m_str type2str(const Gwion, const Type, const loc_t);

ANN static inline Type_Decl *type2td(const Gwion gwion, const Type t,
                                     const loc_t loc) {
  const m_str str = type2str(gwion, t, loc);
  Type_Decl * td  = str2td(gwion, str, loc);
  free_mstr(gwion->mp, str);
  return td;
}

#define gwi_str2sym(gwi, path)     str2sym(gwi->gwion, path, gwi->loc)
#define gwi_str2symlist(gwi, path) str2symlist(gwi->gwion, path, gwi->loc)
#define gwi_str2var(gwi, decl, path)     str2var(gwi->gwion, decl, path, gwi->loc)
#define gwi_str2td(gwi, path)      str2td(gwi->gwion, path, gwi->loc)
#define gwi_str2type(gwi, path)    str2type(gwi->gwion, path, gwi->loc)

ANN bool ck_ini(const Gwi, const enum importck_type);
ANN bool ck_ok(const Gwi, const enum importck_type);
ANN void   ck_end(const Gwi gwi);
ANN void   ck_clean(const Gwi gwi);
#endif
