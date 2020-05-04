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
  ae_flag flag; // ????
  enum importck_type type;
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
ANN ID_List tmpl_valid(const Gwi gwi, const m_str str);
ANN Symbol str2sym(const Gwi gwi, const m_str path);
ANN ID_List str2symlist(const Gwi gwi, const m_str path);

ANN m_bool ck_ini(const Gwi, const enum importck_type);
ANN m_bool ck_ok(const Gwi, const enum importck_type);
ANN void ck_end(const Gwi gwi);
ANN void ck_clean(const Gwi gwi);
#endif
