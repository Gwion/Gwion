typedef struct {
  Type_Decl t;
  struct Var_Decl_List_ list;
  struct Var_Decl_ var;
  struct Exp_ exp;
  m_uint array_depth;
} DL_Var;

typedef struct {
  m_str name;
  m_str type;
} DL_Value;

typedef struct {
  m_str    name;
  m_str    type;
  f_xfun   addr;
  m_uint   narg;
  DL_Value args[DLARG_MAX];
} DL_Func;

typedef struct {
  Operator op;
  m_str ret, lhs, rhs;
  Type   (*ck)(Env, void*);
  m_bool (*em)(Emitter, void*);
 } DL_Oper;

typedef struct {
  m_uint n;
  m_str* list;
} Templater;

typedef struct {
  m_str t;
  ID_List base, curr;
  struct Vector_ addr;
} DL_Enum;

typedef struct {
  Symbol xid;
  Decl_List list;
} DL_Union;

struct Gwi_{
  VM* vm;
  Emitter emit;
  Env env;
  Class_Body body;
  DL_Var var;
  DL_Func func;
  DL_Enum enum_data;
  DL_Union union_data;
  DL_Oper oper;
  void* addr;
  Templater templater;
};
