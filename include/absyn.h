#ifndef __ABSYN
#define __ABSYN

#include "type.h"

#include "map.h" //only for MyArg;

typedef struct Expression_ * Expression;
typedef struct Var_Decl_* Var_Decl;
typedef struct Var_Decl_List_* Var_Decl_List;
typedef struct Arg_List_ * Arg_List;

extern Map scan_map;

typedef enum {
  ae_key_this, ae_key_me, ae_key_func, ae_key_public, ae_key_protected,
  ae_key_private, ae_key_static, ae_key_instance, ae_key_abstract, ae_key_variadic
} ae_Keyword;

typedef struct {
  void* scanner;
  Vector doc;
  char doc_str[4096];
  Ast ast;
  unsigned int line;
  unsigned int pos;
//	unsigned int fd;
  FILE* file;
  m_str filename;
} MyArg;

typedef struct {
  Expression base;
  Type t_base;
  S_Symbol xid;
  int pos;
  Expression self;
} Dot_Member;
struct NameSpace;

struct Array_Sub_ {
  m_uint depth;
  Expression exp_list;
  int pos;
  Expression self;
  Type type;
  int err_num;
  int err_pos;
};
Array_Sub new_array_sub(Expression exp, int pos);
Array_Sub prepend_array_sub(Array_Sub array, Expression exp, int pos);
void free_Array_Sub(Array_Sub a);
typedef struct {
  Expression base;
  Array_Sub indices;
  int pos;
  Expression self;
} Array_Expression;

typedef struct {
  Expression base;
  Array_Sub indices;
  int pos;
  Expression self;
} Array;

Expression new_Array(Expression base, Array_Sub indices, int pos );

struct Var_Decl_ {
  S_Symbol xid;
  Value value;
  Array_Sub array;
  void* addr;
  int pos;
};
Var_Decl new_Var_Decl(m_str name, Array_Sub array, int pos);

struct Var_Decl_List_ {
  Var_Decl self;
  Var_Decl_List next;
  int pos;
  char* doc;
};

Var_Decl_List new_Var_Decl_List(Var_Decl decl, Var_Decl_List list, int pos);

typedef struct {
  ID_List xid;
  Array_Sub array;
  Dot_Member* dot;
  int ref;
  int pos;
  m_str doc;
} Type_Decl;
Type_Decl* new_Type_Decl(ID_List name, int ref, int pos);
Type_Decl* new_Type_Decl_from_dot(Dot_Member* dot, int ref, int pos);
void free_Type_Decl(Type_Decl* a);

typedef struct {
  Type_Decl* type;
  Type m_type;
  Var_Decl_List list;
  int pos;
  int num_decl;
  int is_static;
  Expression self;
} Decl_Expression;
Expression new_Decl_Expression(Type_Decl* type, Var_Decl_List list, m_bool is_static, int pos);
Expression new_Binary_Expression(Expression lhs, Operator op, Expression rhs, int pos);
Type_Decl* add_type_decl_array(Type_Decl* a, Array_Sub array, int pos );

typedef struct {
  Operator op;
  int pos;
  Expression exp;
  Expression self;
} Postfix_Expression;

typedef struct {
  Type_Decl* type;
  Expression exp;
  int pos;
  Expression self;
  Func func;
} Cast_Expression;

typedef struct {
  Expression re;
  Expression im;
  int pos;
  Expression self;
} Complex;
Complex* new_complex( Expression re, int pos );
Expression new_exp_from_complex(Complex* exp, int pos);

typedef struct {
  Expression mod;
  Expression phase;
  int pos;
  Expression self;
} Polar;
Expression new_exp_from_polar(Polar* exp, int pos);
Expression new_exp_from_char( c_str chr, int pos );

typedef struct Vec_* Vec;
struct Vec_ {
  Expression args;
  m_uint numdims;
  m_uint pos;
  Expression self;
};
Vec new_Vec( Expression e, int pos );
Expression new_exp_from_vec( Vec a, int pos );
typedef enum { ae_primary_var, ae_primary_num, ae_primary_float,
               ae_primary_str, ae_primary_array,
               ae_primary_hack, ae_primary_complex, ae_primary_polar, ae_primary_vec,
               ae_primary_char, ae_primary_nil
             } ae_Exp_Primary_Type;
typedef struct {
  ae_Exp_Primary_Type type;
  Value value;
  union {
    S_Symbol var;
    long num;
    m_float fnum;
    m_str chr;
    m_str str;
    Array_Sub array;
    Expression exp;
    Complex* cmp;
    Polar* polar;
    Vec vec;
  } d;
  int pos;
  Expression self;
} Primary_Expression;

typedef struct {
  Expression func;
  Expression args;
  Type ret_type;
  Func m_func;
  VM_Code vm_code;
  int pos;
  Expression self;
  Type_List types;
  ID_List base;// hack for template
  Func    base_func;// hack for template //hack
} Func_Call;


Expression new_Primary_Expression_from_int(long i, int pos);
Expression new_Primary_Expression_from_float(m_float num, int pos);
Expression new_Primary_Expression_from_ID(m_str s, int pos);
Expression new_Primary_Expression_from_string(m_str s, int pos);
Expression new_exp_from_array_lit(Array_Sub exp_list, int pos );
Polar* new_polar(Expression mod, int pos);
Expression new_Postfix_Expression(Expression exp, Operator op, int pos);
Expression prepend_Expression(Expression exp, Expression next, int pos);
Expression new_Hack_Expression(Expression exp, int pos);
Expression new_Func_Call(Expression base, Expression args, int pos);
Expression new_Primary_Expression_from_nil( int pos );
Expression new_Cast_Expression(Type_Decl* type, Expression exp, int pos);
Expression new_If_Expression(Expression cond, Expression if_exp, Expression else_exp, int pos);
Expression new_Exp_Dur(Expression base, Expression unit, int pos);
Expression new_exp_from_member_dot(Expression base, m_str xid, int pos);
typedef struct {
  Expression lhs, rhs;
  Operator op;
  Func func;
  int pos;
  Expression self;
} Binary_Expression;

typedef struct {
  Expression cond;
  Expression if_exp;
  Expression else_exp;
  int pos;
  Expression self;
} If_Expression;

typedef struct {
  Expression base;
  Expression unit;
  int pos;
  Expression self;
} Exp_Dur;

typedef enum { Decl_Expression_type, Binary_Expression_type, Unary_Expression_type, Primary_Expression_type,
               Cast_Expression_type, Postfix_Expression_type, Func_Call_type, Array_Expression_type,
               If_Expression_type, Dot_Member_type, Dur_Expression_type
             } Expression_type;

typedef enum { ae_meta_var, ae_meta_value } ae_Exp_Meta;

typedef struct Stmt_List_       * Stmt_List;
typedef struct Stmt_Code_       * Stmt_Code;
typedef struct Stmt_Return_     * Stmt_Return;
typedef struct Stmt_Continue_   * Stmt_Continue;
typedef struct Stmt_Break_      * Stmt_Break;
typedef struct Stmt_While_      * Stmt_While;
typedef struct Stmt_Until_      * Stmt_Until;
typedef struct Stmt_For_        * Stmt_For;
typedef struct Stmt_Loop_       * Stmt_Loop;
typedef struct Stmt_If_         * Stmt_If;
typedef struct Stmt_Switch_     * Stmt_Switch;
typedef struct Stmt_Case_       * Stmt_Case;
typedef struct Stmt_Goto_Label_ * Stmt_Goto_Label;
typedef struct Stmt_Enum_       * Stmt_Enum;
typedef enum { ae_stmt_exp, ae_stmt_while, ae_stmt_until, ae_stmt_for, ae_stmt_loop,
               ae_stmt_if, ae_stmt_code, ae_stmt_switch, ae_stmt_break,
               ae_stmt_continue, ae_stmt_return, ae_stmt_case, ae_stmt_gotolabel,
               ae_stmt_enum, ae_stmt_funcptr, ae_stmt_union
             } ae_Stmt_Type;

typedef struct {
  Type_Decl*    type;
  Type    m_type;
  S_Symbol   xid;
  ae_Keyword key;
  Arg_List   args;
  Type       ret_type;
  Func       func;
  Value      value;
  int        pos;
  Expression self;
//    Type ref;
} Func_Ptr;

typedef struct Decl_List_* Decl_List;
struct Decl_List_ {
  Decl_Expression* self;
  Decl_List next;
};

Decl_List new_Decl_List(Decl_Expression* d, Decl_List l);
typedef struct {
  Decl_List l;
  Vector v;
  m_uint s;
  m_uint o;
} Union;

Union* new_Union(Decl_List l);
typedef struct {
  ae_Stmt_Type type;
  union {
    Expression       stmt_exp;
    Stmt_Code        stmt_code;
    Stmt_While       stmt_while;
    Stmt_Until       stmt_until;
    Stmt_Loop        stmt_loop;
    Stmt_For         stmt_for;
    Stmt_If          stmt_if;
    Stmt_Break       stmt_break;
    Stmt_Continue    stmt_continue;
    Stmt_Return      stmt_return;
    Stmt_Goto_Label  stmt_gotolabel;
    Stmt_Switch      stmt_switch;
    Stmt_Case        stmt_case;
    Stmt_Enum        stmt_enum;
    Func_Ptr*        stmt_funcptr;
    Union*           stmt_union;
  } d;
  int pos;
} Stmt;


typedef struct {
  Operator op;
  Expression exp;
  Type_Decl* type;
  Array_Sub array;
  Stmt* code;
  m_uint code_depth;
  int pos;
  Expression self;
} Unary_Expression;
Expression new_exp_from_unary(Operator oper, Expression exp, int pos );
Expression new_exp_from_unary2(Operator oper, Type_Decl* type, Array_Sub array, int pos );
Expression new_exp_from_unary3(Operator oper, Stmt* code, int pos );

struct Arg_List_ {
  Type_Decl* type_decl;
  Var_Decl var_decl;
  Type type;
  Arg_List  next;
  m_str doc;
  int pos;
};
Arg_List new_Arg_List(Type_Decl* type_decl, Var_Decl var_decl, Arg_List arg_list, int pos);
void free_Arg_List(Arg_List a);
Stmt* new_Func_Ptr_Stmt(ae_Keyword key, m_str type, Type_Decl* decl, Arg_List args, int pos);

struct Expression_ {
  Expression_type exp_type;
  ae_Exp_Meta meta;
  int emit_var;
  Type type;
  Type cast_to;
  union {
    Postfix_Expression*   exp_postfix;
    Primary_Expression*   exp_primary;
    Decl_Expression*      exp_decl;
    Unary_Expression*     exp_unary;
    Binary_Expression*    exp_binary;
    Cast_Expression*      exp_cast;
    Func_Call*            exp_func;
    If_Expression*        exp_if;
    Dot_Member*           exp_dot;
    Array*                exp_array;
    Exp_Dur*              exp_dur;
  } d;
  int pos;
  Expression next;
};


struct Stmt_Code_ {
  Stmt_List stmt_list;
  int pos;
  Stmt* self;
};

struct Stmt_Return_ {
  Expression val;
  int pos;
  Stmt* self;
};

struct Stmt_Continue_ {
  int pos;
  Stmt* self;
};

struct Stmt_Break_ {
  int pos;
  Stmt* self;
};

struct Stmt_While_ {
  int is_do;
  Expression cond;
  Stmt* body;
  int pos;
  Stmt* self;
};
struct Stmt_Until_ {
  int is_do;
  Expression cond;
  Stmt* body;
  int pos;
  Stmt* self;
};

struct Stmt_For_ {
  Stmt* c1;
  Stmt* c2;
  Expression c3;
  Stmt* body;
  int pos;
  Stmt* self;
};

struct Stmt_Loop_ {
  Expression cond;
  Stmt* body;
  int pos;
  Stmt* self;
};
struct Stmt_If_ {
  Expression cond;
  Stmt* if_body;
  Stmt* else_body;
  int pos;
  Stmt* self;
};
struct Stmt_Goto_Label_ {
  S_Symbol name;
  m_bool is_label;
  union {
    Vector v;
    Instr instr;
  } data;
  int pos;
  Stmt* self;
};

struct Stmt_Switch_ {
  Expression val;
  Stmt* stmt;
  int pos;
  Stmt* self;
};

struct  Stmt_Case_ {
  Expression val;
  int pos;
  Stmt* self;
};

struct Stmt_Enum_ {
  ID_List list;
  S_Symbol xid;
  Vector values;
  int pos;
  Stmt* self;
};

Stmt* new_Stmt_from_Expression(Expression exp, int pos);
Stmt* new_stmt_from_code( Stmt_List stmt_list, int pos );
Stmt* new_stmt_from_while(Expression cond, Stmt* body, m_bool is_do, int pos);
Stmt* new_stmt_from_return(Expression exp, int pos);
Stmt* new_stmt_from_break(int pos);
Stmt* new_stmt_from_continue(int pos);
Stmt* new_stmt_from_if(Expression cond, Stmt* if_body, Stmt* else_body, int pos);
Stmt* new_stmt_from_until(Expression cond, Stmt* body, m_bool is_do, int pos);
Stmt* new_stmt_from_for(Stmt* c1, Stmt* c2, Expression c3, Stmt* body, int pos);
Stmt* new_stmt_from_loop(Expression cond, Stmt* body, int pos);
Stmt* new_stmt_from_gotolabel(m_str xid, m_bool is_label, int pos);
Stmt* new_stmt_from_case(Expression exp, int pos);
Stmt* new_stmt_from_enum(ID_List list, m_str type, int pos);
Stmt* new_stmt_from_switch(Expression val, Stmt* stmt, int pos);

Stmt* new_stmt_from_Union(Union* u, int pos);

struct Stmt_List_ {
  Stmt* stmt;
  Stmt_List next;
  int pos;
};
Stmt_List new_Stmt_List(Stmt* stmt, Stmt_List next, int pos);


typedef struct Class_Ext_ * Class_Ext;
typedef struct Class_Body_ * Class_Body;

struct Class_Def_ {
  ae_Keyword decl;
  ID_List name;
  Class_Ext ext;
  Class_Body body;
  Type type;
  int iface;
  NameSpace home;
  m_str doc;
  int pos;
};


typedef enum { ae_section_stmt, ae_section_func, ae_section_class
             } ae_Section_Type;

typedef enum { ae_func_builtin, ae_func_user} ae_func_type;
struct Func_Def_ {
  Type_Decl* type_decl;
  Type ret_type;
  ae_func_type s_type;
  S_Symbol name;
  Arg_List arg_list;
  Stmt* code;
  Func func;
  int pos;
  m_uint stack_depth;
  ae_Keyword func_decl;
  ae_Keyword static_decl;
  m_bool has_code;
  void* dl_func_ptr;
  ae_func_spec spec;// try to implement dtor in parser
  ID_List types;
  ID_List base; // 13/03/17
  m_bool is_template;
  m_bool is_variadic;
};

Func_Def new_Func_Def(ae_Keyword func_decl, ae_Keyword static_decl, Type_Decl* type_decl, m_str name, Arg_List arg_list, Stmt* code, int pos);
void free_Func_Def(Func_Def def);
m_bool scan1_Func_Def(Env env, Func_Def f);
m_bool scan2_Func_Def(Env env, Func_Def f);
m_bool check_Func_Def(Env env, Func_Def f);
typedef struct {
  ae_Section_Type type;

  union {
    Stmt_List stmt_list;
    Class_Def class_def;
    Func_Def func_def;
  } d;
  int pos;
} Section;
Section* new_section_Stmt_List(Stmt_List list, int pos);
Section* new_section_Func_Def( Func_Def func_def, int pos );


struct Class_Ext_  {
  ID_List extend_id;
  ID_List impl_list;
  int pos;
};
struct Class_Body_ {
  Section* section;
  Class_Body next;
  int pos;
};
struct ID_List_    {
  S_Symbol xid;
  ID_List next;
  int pos;
};
struct Type_List_  {
  ID_List list;
  Type_List next;
  int pos;
};  // call template list
Type_List new_type_list(ID_List list, Type_List next, int pos);
void free_Type_List(Type_List a);
Class_Def new_class_def( ae_Keyword class_decl, ID_List name,
                         Class_Ext ext, Class_Body body, int pos );
Class_Def new_iface_def( ae_Keyword class_decl, ID_List name,
                         Class_Ext ext, Class_Body body, int pos );
Class_Body new_class_body( Section* section, int pos );
Class_Body prepend_class_body(Section* section, Class_Body body, int pos );
Class_Ext new_class_ext(ID_List extend_id, ID_List impl_list, int pos );
ID_List new_id_list(const m_str xid, int pos);
ID_List prepend_id_list(const m_str xid, ID_List list, int pos);
void free_ID_List(ID_List a);
Section* new_section_Class_Def(Class_Def class_def, int pos);


struct Ast_ {
  Section* section;
  Ast next;
  int pos;
  m_str doc;
};

Ast new_Ast(Section* section, Ast next, int pos);
Ast parse(m_str);
void free_Ast();

#endif
