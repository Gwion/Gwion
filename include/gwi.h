#ifndef __GWI
#define __GWI
typedef struct {
  Type_Decl *td;
  struct Var_Decl_List_ list;
  struct Var_Decl_ var;
  struct Exp_ exp;
  size_t array_depth;
} DL_Var;

typedef struct {
  m_str name;
  m_str type;
} DL_Value;

typedef struct {
  m_str    name;
  m_str    type;
  f_xfun   addr;
  DL_Value args[DLARG_MAX];
  uint narg;
} DL_Func;

typedef struct {
  Symbol op;
  m_str ret, lhs, rhs;
  Type   (*ck)(Env, void*, m_bool*);
  Instr  (*em)(Emitter, void*);
} DL_Oper;

typedef struct {
  m_str* list;
  size_t n;
} Templater;

typedef struct {
  m_str t;
  ID_List base, curr;
  struct Vector_ addr;
} DL_Enum;

typedef struct {
  m_str type_name;
  m_str name;
  Decl_List list;
} DL_Union;

struct Gwi_ {
  struct Gwion_* gwion;
  Class_Body body;
  DL_Value val;
  DL_Var var;
  DL_Func func;
  DL_Enum enum_data;
  DL_Union union_data;
  DL_Oper oper;
  void* addr;
  Templater templater;
  loc_t loc;
};

#include "import/internals.h"

#endif
