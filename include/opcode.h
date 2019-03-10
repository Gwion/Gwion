#ifndef __GWION_OPCODES__
#define __GWION_OPCODES__
enum {
  RegSetImm,
  RegPushImm,
  RegPushImm2,
  RegPushImm3,
  RegPushImm4,
  RegPushMem,
  RegPushMem2,
  RegPushMem3,
  RegPushMem4,
  RegPushNow,
  RegPushBase,
  RegPushBase2,
  RegPushBase3,
  RegPushBase4,
  RegDup,
  RegDup2,
  MemSetImm,
  RegPop,
  RegPushMe,
  RegPushMaybe,
  FuncReturn,
  Goto,
  AllocWord,
  AllocWord2,
  AllocWord3,
  AllocWord4,
  int_plus,
  int_minus,
  int_mul,
  int_div,
  int_modulo,
  int_eq,
  int_neq,
  int_and,
  int_or,
  int_gt,
  int_ge,
  int_lt,
  int_le,
  int_sl,
  int_sr,
  int_sand,
  int_sor,
  int_xor,
  int_negate,
  IntNot,
  int_cmp,
  int_r_assign,
  int_r_plus,
  int_r_minus,
  int_r_mul,
  int_r_div,
  int_r_modulo,
  int_r_sl,
  int_r_sr,
  int_r_sand,
  int_r_sor,
  int_r_sxor,
  int_pre_inc,
  int_pre_dec,
  int_post_inc,
  int_post_dec,
  FloatPlus,
  FloatMinus,
  FloatTimes,
  FloatDivide,
  float_and,
  float_or,
  float_eq,
  float_neq,
  float_gt,
  float_ge,
  float_lt,
  float_le,
  float_negate,
  float_not,
  float_r_assign,
  float_r_plus,
  float_r_minus,
  float_r_mul,
  float_r_div,
  int_float_plus,
  int_float_minus,
  int_float_mul,
  int_float_div,
  int_float_and,
  int_float_or,
  int_float_eq,
  int_float_neq,
  int_float_gt,
  int_float_ge,
  int_float_lt,
  int_float_le,
  int_float_r_assign,
  int_float_r_plus,
  int_float_r_minus,
  int_float_r_mul,
  int_float_r_div,
  float_int_plus,
  float_int_minus,
  float_int_mul,
  float_int_div,
  float_int_and,
  float_int_or,
  float_int_eq,
  float_int_neq,
  float_int_gt,
  float_int_ge,
  float_int_lt,
  float_int_le,
  float_int_r_assign,
  float_int_r_plus,
  float_int_r_minus,
  float_int_r_mul,
  float_int_r_div,
  CastI2F,
  CastF2I,
  Time_Advance,
  FuncUsr,
  FuncMember,
  FuncStatic,
  SporkIni,
  SporkFunc,
  SporkThis,
  SporkExp,
  SporkEnd,
  FuncPtr,
  BranchEqInt,
  BranchNeqInt,
  BranchEqFloat,
  BranchNeqFloat,
  DecIntAddr,
  InitLoopCounter,
  ObjectInstantiate,
  RegAddRef,
  ObjectAssign,
  ObjectRelease,
  GWOP_EXCEPT,
  AllocMember4,
  DotMember,
  DotMember2,
  DotMember3,
  DotMember4,
  DotStatic,
  DotStatic2,
  DotStatic3,
  DotFunc,
  DotStaticFunc,
  PushStaticCode,
  RegPushStr,
  GcIni,
  GcAdd,
  GcEnd,
  Gack,
  OP_MAX,
};

#define  RegSetImm           (f_instr)RegSetImm
#define  RegPushImm          (f_instr)RegPushImm
#define  RegPushImm2         (f_instr)RegPushImm2
#define  RegPushImm3         (f_instr)RegPushImm3
#define  RegPushImm4         (f_instr)RegPushImm4
#define  RegPushMem          (f_instr)RegPushMem
#define  RegPushMem2         (f_instr)RegPushMem2
#define  RegPushMem3         (f_instr)RegPushMem3
#define  RegPushMem4         (f_instr)RegPushMem4
#define  RegPushNow          (f_instr)RegPushNow
#define  RegPushBase         (f_instr)RegPushBase
#define  RegPushBase2        (f_instr)RegPushBase2
#define  RegPushBase3        (f_instr)RegPushBase3
#define  RegPushBase4        (f_instr)RegPushBase4
#define  RegDup              (f_instr)RegDup
#define  RegDup2             (f_instr)RegDup2
#define  MemSetImm           (f_instr)MemSetImm
#define  RegPop              (f_instr)RegPop
#define  RegPushMe           (f_instr)RegPushMe
#define  RegPushMaybe        (f_instr)RegPushMaybe
#define  FuncReturn          (f_instr)FuncReturn
#define  Goto                (f_instr)Goto
#define  AllocWord           (f_instr)AllocWord
#define  AllocWord2          (f_instr)AllocWord2
#define  AllocWord3          (f_instr)AllocWord3
#define  AllocWord4          (f_instr)AllocWord4
#define  int_plus            (f_instr)int_plus
#define  int_minus           (f_instr)int_minus
#define  int_mul             (f_instr)int_mul
#define  int_div             (f_instr)int_div
#define  int_modulo          (f_instr)int_modulo
#define  int_eq              (f_instr)int_eq
#define  int_neq             (f_instr)int_neq
#define  int_and             (f_instr)int_and
#define  int_or              (f_instr)int_or
#define  int_gt              (f_instr)int_gt
#define  int_ge              (f_instr)int_ge
#define  int_lt              (f_instr)int_lt
#define  int_le              (f_instr)int_le
#define  int_sl              (f_instr)int_sl
#define  int_sr              (f_instr)int_sr
#define  int_sand            (f_instr)int_sand
#define  int_sor             (f_instr)int_sor
#define  int_xor             (f_instr)int_xor
#define  int_negate          (f_instr)int_negate
#define  IntNot              (f_instr)IntNot
#define  int_cmp             (f_instr)int_cmp
#define  int_r_assign        (f_instr)int_r_assign
#define  int_r_plus          (f_instr)int_r_plus
#define  int_r_minus         (f_instr)int_r_minus
#define  int_r_mul           (f_instr)int_r_mul
#define  int_r_div           (f_instr)int_r_div
#define  int_r_modulo        (f_instr)int_r_modulo
#define  int_r_sl            (f_instr)int_r_sl
#define  int_r_sr            (f_instr)int_r_sr
#define  int_r_sand          (f_instr)int_r_sand
#define  int_r_sor           (f_instr)int_r_sor
#define  int_r_sxor          (f_instr)int_r_sxor
#define  int_pre_inc         (f_instr)int_pre_inc
#define  int_pre_dec         (f_instr)int_pre_dec
#define  int_post_inc        (f_instr)int_post_inc
#define  int_post_dec        (f_instr)int_post_dec
#define  FloatPlus           (f_instr)FloatPlus
#define  FloatMinus          (f_instr)FloatMinus
#define  FloatTimes          (f_instr)FloatTimes
#define  FloatDivide         (f_instr)FloatDivide
#define  float_and           (f_instr)float_and
#define  float_or            (f_instr)float_or
#define  float_eq            (f_instr)float_eq
#define  float_neq           (f_instr)float_neq
#define  float_gt            (f_instr)float_gt
#define  float_ge            (f_instr)float_ge
#define  float_lt            (f_instr)float_lt
#define  float_le            (f_instr)float_le
#define  float_negate        (f_instr)float_negate
#define  float_not           (f_instr)float_not
#define  float_r_assign      (f_instr)float_r_assign
#define  float_r_plus        (f_instr)float_r_plus
#define  float_r_minus       (f_instr)float_r_minus
#define  float_r_mul         (f_instr)float_r_mul
#define  float_r_div         (f_instr)float_r_div
#define  int_float_plus      (f_instr)int_float_plus
#define  int_float_minus     (f_instr)int_float_minus
#define  int_float_mul       (f_instr)int_float_mul
#define  int_float_div       (f_instr)int_float_div
#define  int_float_and       (f_instr)int_float_and
#define  int_float_or        (f_instr)int_float_or
#define  int_float_eq        (f_instr)int_float_eq
#define  int_float_neq       (f_instr)int_float_neq
#define  int_float_gt        (f_instr)int_float_gt
#define  int_float_ge        (f_instr)int_float_ge
#define  int_float_lt        (f_instr)int_float_lt
#define  int_float_le        (f_instr)int_float_le
#define  int_float_r_assign  (f_instr)int_float_r_assign
#define  int_float_r_plus    (f_instr)int_float_r_plus
#define  int_float_r_minus   (f_instr)int_float_r_minus
#define  int_float_r_mul     (f_instr)int_float_r_mul
#define  int_float_r_div     (f_instr)int_float_r_div
#define  float_int_plus      (f_instr)float_int_plus
#define  float_int_minus     (f_instr)float_int_minus
#define  float_int_mul       (f_instr)float_int_mul
#define  float_int_div       (f_instr)float_int_div
#define  float_int_and       (f_instr)float_int_and
#define  float_int_or        (f_instr)float_int_or
#define  float_int_eq        (f_instr)float_int_eq
#define  float_int_neq       (f_instr)float_int_neq
#define  float_int_gt        (f_instr)float_int_gt
#define  float_int_ge        (f_instr)float_int_ge
#define  float_int_lt        (f_instr)float_int_lt
#define  float_int_le        (f_instr)float_int_le
#define  float_int_r_assign  (f_instr)float_int_r_assign
#define  float_int_r_plus    (f_instr)float_int_r_plus
#define  float_int_r_minus   (f_instr)float_int_r_minus
#define  float_int_r_mul     (f_instr)float_int_r_mul
#define  float_int_r_div     (f_instr)float_int_r_div
#define  CastI2F             (f_instr)CastI2F
#define  CastF2I             (f_instr)CastF2I
#define  Time_Advance        (f_instr)Time_Advance
#define  FuncUsr             (f_instr)FuncUsr
#define  FuncMember          (f_instr)FuncMember
#define  FuncStatic          (f_instr)FuncStatic
#define  SporkIni            (f_instr)SporkIni
#define  SporkFunc           (f_instr)SporkFunc
#define  SporkThis           (f_instr)SporkThis
#define  SporkExp            (f_instr)SporkExp
#define  SporkEnd            (f_instr)SporkEnd
#define  FuncPtr             (f_instr)FuncPtr
#define  BranchEqInt         (f_instr)BranchEqInt
#define  BranchNeqInt        (f_instr)BranchNeqInt
#define  BranchEqFloat       (f_instr)BranchEqFloat
#define  BranchNeqFloat      (f_instr)BranchNeqFloat
#define  DecIntAddr          (f_instr)DecIntAddr
#define  InitLoopCounter     (f_instr)InitLoopCounter
#define  ObjectInstantiate   (f_instr)ObjectInstantiate
#define  RegAddRef           (f_instr)RegAddRef
#define  ObjectAssign        (f_instr)ObjectAssign
#define  ObjectRelease       (f_instr)ObjectRelease
#define  GWOP_EXCEPT         (f_instr)GWOP_EXCEPT
#define  AllocMember4        (f_instr)AllocMember4
#define  DotMember           (f_instr)DotMember
#define  DotMember2          (f_instr)DotMember2
#define  DotMember3          (f_instr)DotMember3
#define  DotMember4          (f_instr)DotMember4
#define  DotStatic           (f_instr)DotStatic
#define  DotStatic2          (f_instr)DotStatic2
#define  DotStatic3          (f_instr)DotStatic3
#define  DotFunc             (f_instr)DotFunc
#define  DotStaticFunc       (f_instr)DotStaticFunc
#define  PushStaticCode      (f_instr)PushStaticCode
#define  RegPushStr          (f_instr)RegPushStr
#define  GcIni               (f_instr)GcIni
#define  GcAdd               (f_instr)GcAdd
#define  GcEnd               (f_instr)GcEnd
#define  Gack                (f_instr)Gack
#define  OP_MAX              (f_instr)OP_MAX
#endif
