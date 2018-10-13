#ifndef __ABSYN
#define __ABSYN
#include "vm.h"

typedef struct Ast_           * Ast;
typedef struct Class_Def_     * Class_Def;
typedef struct Func_Def_      * Func_Def;
typedef struct Stmt_List_     * Stmt_List;
typedef struct Exp_           * Exp;
typedef struct Stmt_          * Stmt;
typedef struct Var_Decl_      * Var_Decl;
typedef struct Var_Decl_List_ * Var_Decl_List;
typedef struct Array_Sub_     * Array_Sub;
typedef struct Arg_List_      * Arg_List;

typedef struct {
  Exp base;
  Type t_base;
  Symbol xid;
  Exp self;
} Exp_Dot;

struct Array_Sub_ {
  Exp    exp;
  Type   type;
  m_uint depth;
  int pos;
};
ANEW Array_Sub new_array_sub(const Exp, const int);
ANN2(1) Array_Sub prepend_array_sub(const Array_Sub, const Exp);
ANN void free_array_sub(Array_Sub);

typedef struct {
  Exp       base;
  Array_Sub array;
  Exp       self;
} Exp_Array;

ANEW ANN Exp new_exp_array(const Exp, const Array_Sub, const int);

struct Var_Decl_ {
  Symbol xid;
  Value value;
  Array_Sub array;
  void* addr;
  int pos;
};
ANN2(1) ANEW Var_Decl new_var_decl(const Symbol, const Array_Sub, const int);

struct Var_Decl_List_ {
  Var_Decl self;
  Var_Decl_List next;
};
ANN2(1) ANEW Var_Decl_List new_var_decl_list(const Var_Decl, const Var_Decl_List);

typedef struct Type_Decl_ {
  ID_List xid;
  Array_Sub array;
  Exp_Dot* dot;
  Type_List types;
  int pos;
  ae_flag flag;
} Type_Decl;
ANEW ANN Type_Decl* new_type_decl(const ID_List, const ae_flag, const int);
ANEW ANN Type_Decl* new_type_decl2(const ID_List, const ae_flag, const int);
ANN void free_type_decl(Type_Decl* a);
ANN Type_Decl* add_type_decl_array(Type_Decl*, const Array_Sub);

struct ID_List_    {
  Symbol xid;
  ID_List next;
  ID_List ref;
  int pos;
};
ANEW ANN ID_List new_id_list(const Symbol, const int);
ANN ID_List prepend_id_list(const Symbol, ID_List, const int);
ANN void free_id_list(ID_List a);

struct Type_List_  {
  Type_Decl* td;
  Type_List next;
};
ANN2(1) ANEW Type_List new_type_list(Type_Decl*, const Type_List);
ANN void free_type_list(Type_List a);

typedef struct {
  Exp    exp;
  m_uint dim;
} Vec;

struct Arg_List_ {
  Type_Decl* td;
  Var_Decl var_decl;
  Type type;
  Arg_List  next;
};
ANN2(1,2) ANEW Arg_List new_arg_list(Type_Decl*, const Var_Decl, const Arg_List);
ANN void free_arg_list(Arg_List a);

typedef enum { ae_exp_decl, ae_exp_binary, ae_exp_unary, ae_exp_primary,
               ae_exp_cast, ae_exp_post, ae_exp_call, ae_exp_array,
               ae_exp_if, ae_exp_dot, ae_exp_dur
#ifdef OPTIMIZE
                ,ae_exp_constprop, ae_exp_constprop2
#endif
} ae_exp_t;
typedef enum { ae_meta_var, ae_meta_value, ae_meta_protect } ae_Exp_Meta;
typedef enum { ae_primary_id, ae_primary_num, ae_primary_float,
               ae_primary_str, ae_primary_array,
               ae_primary_hack, ae_primary_complex, ae_primary_polar, ae_primary_vec,
               ae_primary_char, ae_primary_nil
#ifdef OPTIMIZE
  , ae_primary_constprop
#endif
             } ae_prim_t;
typedef struct {
  Type_Decl* td;
  Type type;
  Var_Decl_List list;
  Class_Def base;
  Exp self;
} Exp_Decl;
typedef struct {
  Value value;
  union exp_primary_data {
    Symbol var;
    long num;
    m_float fnum;
    m_str chr;
    m_str str;
    Array_Sub array;
    Exp exp;
    Vec vec;
  } d;
  Exp self;
  ae_prim_t primary_type; // FIXME can't be moved for some reason
} Exp_Primary;

typedef struct Tmpl_Call_ {
  Type_List types;
  ID_List base;
} Tmpl_Call;
ANEW ANN Tmpl_Call* new_tmpl_call(Type_List);
typedef struct {
  Exp func;
  Exp args;
  Func m_func;
  Tmpl_Call* tmpl;
  Exp self;
} Exp_Func;
typedef struct {
  Type_Decl* td;
  Exp exp;
  Exp self;
  Nspc nspc;
  Func func;
} Exp_Cast;
typedef struct {
  Exp lhs, rhs;
  Operator op;
  Nspc nspc;
  Func func;
  Tmpl_Call* tmpl;
  Exp self;
} Exp_Binary;
typedef struct {
  Operator op;
  Nspc nspc;
  Exp exp;
  Exp self;
} Exp_Postfix;
typedef struct {
  Exp cond;
  Exp if_exp;
  Exp else_exp;
  Exp self;
} Exp_If;
typedef struct {
  Exp base;
  Exp unit;
  Exp self;
} Exp_Dur;
typedef struct {
  Operator op;
  Nspc nspc;
  Exp exp;
  Type_Decl* td;
  Stmt code;
  m_uint code_depth;
  Exp self;
} Exp_Unary;
#ifdef OPTIMIZE
typedef struct {
  Exp bin;
  m_uint num;
} Exp_Constprop;
#endif
struct Exp_ {
  Type type;
  Type cast_to;
  Exp next;
  union exp_data {
    Exp_Postfix   exp_post;
    Exp_Primary   exp_primary;
    Exp_Decl      exp_decl;
    Exp_Unary     exp_unary;
    Exp_Binary    exp_binary;
    Exp_Cast      exp_cast;
    Exp_Func      exp_func;
    Exp_If        exp_if;
    Exp_Dot       exp_dot;
    Exp_Array     exp_array;
    Exp_Dur       exp_dur;
#ifdef OPTIMIZE
    Exp_Constprop exp_constprop;
#endif
  } d;
  int pos;
  ae_exp_t exp_type;
  ae_Exp_Meta meta;
  unsigned emit_var : 1;
};

ANEW ANN Exp new_exp_prim_id(const Symbol, const int);
ANEW Exp new_exp_prim_int(const long, const int);
ANEW Exp new_exp_prim_float(const m_float, const int);
ANEW ANN Exp new_exp_prim_string(const m_str, const int);
ANEW ANN Exp new_exp_prim_array(const Array_Sub, const int);
ANEW Exp new_exp_prim_hack(const Exp, const int);
ANEW ANN Exp new_exp_prim_vec(const ae_prim_t t, Exp, const int pos);
ANEW ANN Exp new_exp_prim_char(const m_str, const int);
ANEW Exp new_exp_prim_nil(const int pos);
ANEW ANN Exp new_exp_decl(Type_Decl*, const Var_Decl_List, const int);
ANEW ANN Exp new_exp_binary(const Exp, const Operator, const Exp, const int);
ANEW ANN Exp new_exp_post(const Exp, const Operator, const int);
ANN2(1) ANEW Exp new_exp_call(const Exp, const Exp args, const int pos);
ANEW ANN Exp new_exp_cast(Type_Decl*, const Exp, const int);
ANN2(1,2) ANEW Exp new_exp_if(const __restrict__ Exp, const __restrict__ Exp, const __restrict__ Exp, const int);
ANEW ANN Exp new_exp_dur(const Exp, const Exp, const int);
ANEW ANN Exp new_exp_dot(const Exp, const Symbol, const int);
ANEW ANN Exp new_exp_unary(const Operator, const Exp, const int);
ANEW ANN Exp new_exp_unary2(const Operator, Type_Decl*, const int);
ANEW ANN Exp new_exp_unary3(const Operator, const Stmt, const int);
ANEW ANN Exp prepend_exp(const __restrict__ Exp, const __restrict__ Exp);
ANN void free_exp(Exp exp);

typedef struct Decl_List_* Decl_List;
struct Decl_List_ {
  Exp self;
  Decl_List next;
};

ANN2(1) ANEW Decl_List new_decl_list(Exp d, Decl_List l);

typedef enum { ae_stmt_exp, ae_stmt_while, ae_stmt_until, ae_stmt_for, ae_stmt_auto, ae_stmt_loop,
               ae_stmt_if, ae_stmt_code, ae_stmt_switch, ae_stmt_break,
               ae_stmt_continue, ae_stmt_return, ae_stmt_case, ae_stmt_jump,
               ae_stmt_enum, ae_stmt_fptr, ae_stmt_type, ae_stmt_union,
#ifndef TINY_MODE
#ifdef TOOL_MODE
ae_stmt_pp
#endif
#endif
             } ae_stmt_t;

typedef struct Stmt_Exp_     * Stmt_Exp;
typedef struct Stmt_Code_    * Stmt_Code;
typedef struct Stmt_For_     * Stmt_For;
typedef struct Stmt_Flow_    * Stmt_Flow;
typedef struct Stmt_Auto_    * Stmt_Auto;
typedef struct Stmt_Loop_    * Stmt_Loop;
typedef struct Stmt_If_      * Stmt_If;
typedef struct Stmt_Switch_  * Stmt_Switch;
typedef struct Stmt_Jump_    * Stmt_Jump;
typedef struct Stmt_Enum_    * Stmt_Enum;
typedef struct Stmt_Fptr_    * Stmt_Fptr;
typedef struct Stmt_Type_    * Stmt_Type;
typedef struct Stmt_Union_   * Stmt_Union;
#ifndef TINY_MODE
typedef struct Stmt_PP_      * Stmt_PP;
#endif
struct Stmt_Basic_ {
  int pos;
};

struct Stmt_Exp_ {
  Exp val;
  Stmt self;
};

struct Stmt_Flow_ {
  Exp cond;
  Stmt body;
  Stmt self;
  unsigned is_do : 1;
};

struct Stmt_Code_ {
  Stmt_List stmt_list;
};

struct Stmt_For_ {
  Stmt c1;
  Stmt c2;
  Exp c3;
  Stmt body;
  Stmt self;
};

struct Stmt_Auto_ {
  Symbol sym;
  Exp exp;
  Stmt body;
  Value v;
  m_bool is_ptr;
  Stmt self;
};

struct Stmt_Loop_ {
  Exp cond;
  Stmt body;
  Stmt self;
};

struct Stmt_If_ {
  Exp cond;
  Stmt if_body;
  Stmt else_body;
};

struct Stmt_Jump_ {
  Symbol name;
  union stmt_goto_data {
    struct Vector_ v;
    Instr instr;
  } data;
  m_bool is_label;
  Stmt self;
};

struct Stmt_Switch_ {
  Exp val;
  Stmt stmt;
  Stmt self;
};

struct Stmt_Enum_ {
  ID_List list;
  Symbol xid;
  Type t;
  struct Vector_ values;
  Stmt self;
  ae_flag flag;
};

struct Stmt_Fptr_ {
  Type_Decl* td;
  Type       type;
  Symbol     xid;
  Arg_List   args;
  Type       ret_type;
  Func       func;
  Value      value;
};

struct Stmt_Type_ {
  Type_Decl* td;
  Type       type;
  Symbol     xid;
};

struct Stmt_Union_ {
  Decl_List l;
  struct Vector_ v;
  Symbol xid;
  Value value;
  m_uint s;
  m_uint o;
  Stmt self;
  ae_flag flag;
};

#ifndef TINY_MODE
enum ae_pp_type {
  ae_pp_comment,
  ae_pp_include,
  ae_pp_define,
  ae_pp_undef,
  ae_pp_ifdef,
  ae_pp_ifndef,
  ae_pp_else,
  ae_pp_endif,
  ae_pp_nl
};
struct Stmt_PP_ {
  enum ae_pp_type type;
  m_str data;
};
#endif
struct Stmt_ {
  union stmt_data {
    struct Stmt_Exp_        stmt_exp;
    struct Stmt_Code_       stmt_code;
    struct Stmt_Flow_       stmt_flow;
    struct Stmt_Loop_       stmt_loop;
    struct Stmt_For_        stmt_for;
    struct Stmt_Auto_       stmt_auto;
    struct Stmt_If_         stmt_if;
    struct Stmt_Jump_       stmt_jump;
    struct Stmt_Switch_     stmt_switch;
    struct Stmt_Enum_       stmt_enum;
    struct Stmt_Fptr_       stmt_fptr;
    struct Stmt_Type_       stmt_type;
    struct Stmt_Union_      stmt_union;
#ifndef TINY_MODE
    struct Stmt_PP_    stmt_pp;
#endif
  } d;
  int pos;
  ae_stmt_t stmt_type;
};

ANEW Stmt new_stmt(const ae_stmt_t, const int);
ANEW Stmt new_stmt_exp(const ae_stmt_t, const Exp, const int);
ANEW Stmt new_stmt_code(const Stmt_List, const int);
ANN2(1,2) ANEW Stmt new_stmt_if(const Exp, const __restrict__ Stmt, const __restrict__ Stmt, const int);
ANEW ANN Stmt new_stmt_flow(const ae_stmt_t, const Exp, const Stmt, const m_bool, const int);
ANN2(1,2,4) ANEW Stmt new_stmt_for(const __restrict__ Stmt, const __restrict__ Stmt, const Exp, const __restrict__ Stmt, const int);
ANEW ANN Stmt new_stmt_auto(const Symbol, const Exp, const Stmt, const m_bool, const int);
ANEW ANN Stmt new_stmt_loop(const Exp, const Stmt, const int pos);
ANEW ANN Stmt new_stmt_jump(const Symbol, const m_bool, const int);
ANN2(1) ANEW Stmt new_stmt_enum(const ID_List, const Symbol, const int);
ANEW ANN Stmt new_stmt_switch(Exp, Stmt, const int);
ANEW ANN Stmt new_stmt_union(const Decl_List, const int);
ANEW ANN Stmt new_stmt_fptr(const Symbol, Type_Decl*, const Arg_List, const ae_flag, const int);
ANEW ANN Stmt new_stmt_type(Type_Decl*, const Symbol, const int);
#ifndef TINY_MODE
ANEW     Stmt new_stmt_pp(const enum ae_pp_type, const m_str);
#endif
ANN void free_stmt(Stmt);
struct Stmt_List_ {
  Stmt stmt;
  Stmt_List next;
};
ANEW Stmt_List new_stmt_list(Stmt stmt, Stmt_List next);
ANN void free_stmt_list(Stmt_List);

typedef struct Class_Body_ * Class_Body;

typedef struct Tmpl_List_ {
  ID_List list;
  m_int  base;
} Tmpl_List;

struct Func_Def_ {
  Type_Decl* td;
  Type ret_type;
  Symbol name;
  Arg_List arg_list;
  m_uint stack_depth;
  Func func;
  union func_def_data {
    Stmt code;
    void* dl_func_ptr;
  } d;
  Tmpl_List* tmpl;
  ae_flag flag;
};
ANEW ANN Tmpl_List* new_tmpl_list(const ID_List, const m_int);
ANN void free_tmpl_list(Tmpl_List*);
m_bool tmpl_list_base(const Tmpl_List*);
ANEW Func_Def new_func_def(Type_Decl*, const Symbol, const Arg_List, const Stmt, const ae_flag);
ANN void free_func_def(Func_Def def);
ANN void free_func_def_simple(Func_Def def);

typedef enum { ae_section_stmt, ae_section_func, ae_section_class } ae_section_t;
typedef struct Section_ {
  union section_data {
    Stmt_List stmt_list;
    Class_Def class_def;
    Func_Def func_def;
  } d;
  ae_section_t section_type;
} Section;
ANEW ANN Section* new_section_stmt_list(const Stmt_List);
ANEW ANN Section* new_section_func_def(const Func_Def);
ANEW ANN Section* new_section_class_def(const Class_Def);

struct Class_Body_ {
  Section* section;
  Class_Body next;
};

typedef struct Tmpl_Class_ {
  Tmpl_List list;
  Type_List base;
} Tmpl_Class;
ANEW ANN Tmpl_Class* new_tmpl_class(const ID_List, const m_bool);
m_bool tmpl_class_base(const Tmpl_Class*);
ANN void free_tmpl_class(Tmpl_Class*);
struct Class_Def_ {
  ID_List name;
  Type_Decl* ext;
  Class_Body body;
  Type type;
  Tmpl_Class*  tmpl;
  ae_flag flag;
};
ANN2(2) ANEW Class_Def new_class_def(const ae_flag, const ID_List,
                        Type_Decl*, const Class_Body);
ANN void free_class_def(Class_Def);
ANN void free_class_def_simple(Class_Def);
ANN2(1) ANEW Class_Body new_class_body(Section*, const Class_Body);
ANN void free_class_body(Class_Body);

struct Ast_ {
  Section* section;
  Ast next;
};
ANN2(1) ANEW Ast new_ast(Section*, const Ast);
ANN void free_ast(Ast);
#endif
