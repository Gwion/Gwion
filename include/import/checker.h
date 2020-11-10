#ifndef __IMPORT_CHECKER
#define __IMPORT_CHECKER

enum importck_type {
  ck_edef,
  ck_udef,
  ck_tdef,
  ck_oper,
  ck_item,
  ck_fdef
};

typedef struct ImportCK { // name_checker ?
  m_str name;
  Symbol sym;
  union {
    ID_List tmpl;
    Exp exp;
  };
  union {
    Symbol xid;// union name
    f_xfun addr;
  };
  union {
    Decl_List list;// union
    struct Vector_ v;
//    ID_List curr;// enum
  };
  union {
    Type_Decl *td; // typedef
    ID_List curr;// enum
  };
  uint variadic;
  enum importck_type type;
  ae_flag flag; // ????
} ImportCK;

typedef struct OperCK { // name_checker ?
  m_str ret;
  Symbol sym;
  Type   (*ck)(Env, void*, m_bool*); // oper
  Instr  (*em)(Emitter, void*); // oper
  m_str lhs;// oper
  m_str rhs;// oper
} OperCK;

ANN void func_checker_clean(const Gwi gwi, struct ImportCK *ck);
ANN m_bool check_typename_def(const Gwi gwi, struct ImportCK *ck);

ANN Symbol str2sym(const Gwion gwi, const m_str path, const loc_t pos);
ANN ID_List str2symlist(const Gwion gwi, const m_str path, const loc_t);
ANN Var_Decl      str2var(const Gwion, const m_str, const loc_t);
ANN Var_Decl_List str2varlist(const Gwion, const m_str, const loc_t);
ANN Type_Decl*    str2decl(const Gwion, const m_str, const loc_t);
ANN Type str2type(const Gwion, const m_str, const loc_t);

#define gwi_str2sym(gwi, path) str2sym(gwi->gwion, path, gwi->loc)
#define gwi_str2symlist(gwi, path) str2symlist(gwi->gwion, path, gwi->loc)
#define gwi_str2var(gwi, path) str2var(gwi->gwion, path, gwi->loc)
#define gwi_str2varlist(gwi, path) str2varlist(gwi->gwion, path, gwi->loc)
#define gwi_str2decl(gwi, path) str2decl(gwi->gwion, path, gwi->loc)
#define gwi_str2type(gwi, path) str2type(gwi->gwion, path, gwi->loc)

ANN m_bool ck_ini(const Gwi, const enum importck_type);
ANN m_bool ck_ok(const Gwi, const enum importck_type);
ANN void ck_end(const Gwi gwi);
ANN void ck_clean(const Gwi gwi);
#endif
