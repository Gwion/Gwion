#ifndef __DEF
#define __DEF

#define CHECK_BB(f) if(f < 0) return -1;
#define CHECK_OB(f) if(!f)    return -1;
#define CHECK_BO(f) if(f < 0) return NULL;
#define CHECK_OO(f) if(!f)    return NULL;

#define SET_FLAG(a, b) a->flag |= b
#define GET_FLAG(a, b) ((a->flag & b) == b)

#include "generated.h"

typedef struct VM_Object_ * VM_Object;
typedef struct Nspc_ * Nspc;
typedef struct Scope_     * Scope;
typedef struct Type_      * Type;
typedef struct Value_     * Value;
typedef struct Func_      * Func;
typedef struct Context_   * Context;
typedef struct Env_       * Env;

typedef struct Instr_     * Instr;
typedef struct ID_List_   * ID_List;
typedef struct Type_List_ * Type_List;

typedef struct Emitter_   * Emitter;
typedef struct M_Object_  * M_Object;
typedef struct M_Vector_  * M_Vector;
typedef struct UGen_      * UGen;

typedef enum {
  te_int, te_float, te_dur, te_time, te_now, te_string, te_void, te_function, te_func_ptr,
  te_object, te_class, /* te_user,*/ te_null,
  te_fileio, te_polar, te_complex, te_vec3, te_vec4, te_array, te_shred, te_machine, te_event,
  te_std,
  te_template,
  te_ugen,
  te_vararg,
  te_last
} te_type;
typedef enum { ae_func_spec_none, ae_func_spec_dtor, ae_func_spec_op } ae_func_spec;

typedef enum { Kindof_Void, Kindof_Int, Kindof_Float, Kindof_Complex, Kindof_Vec3, Kindof_Vec4} Kindof;

typedef enum {
// arithmetic
  op_assign, op_plus, op_minus, op_times, op_divide, op_percent,
// logical
  op_and, op_or, op_eq, op_neq,
  op_gt, op_ge, op_lt, op_le,
  // bitwise
  op_shift_left, op_shift_right,
  op_s_or, op_s_and, op_s_xor,
  // unary op
  op_plusplus, op_minusminus, op_exclamation, op_tilda,
  op_new, op_spork,
// reverse arithmetic
  op_chuck, op_plus_chuck, op_minus_chuck, op_times_chuck, op_divide_chuck, op_modulo_chuck,
// reverse logical
  op_rand, op_ror, op_req, op_rneq,
  op_rgt, op_rge, op_rlt, op_rle,
// reverse bitwise
  op_rsl, op_rsr, op_rsand, op_rsor, op_rsxor,
// unchuck
  op_unchuck,
// at
  op_at_chuck, op_at_unchuck,
// trig
  op_trig, op_untrig
} Operator;

#endif
