#ifndef __GWION_OPCODES__
#define __GWION_OPCODES__
enum {
  eRegSetImm,
  eRegPushImm,
  eRegPushImm2,
  eRegPushImm3,
  eRegPushImm4,
  eRegPushMem,
  eRegPushMem2,
  eRegPushMem3,
  eRegPushMem4,
  eRegPushMemDeref,
  eRegPushNow,
  eRegPushBase,
  eRegPushBase2,
  eRegPushBase3,
  eRegPushBase4,
  eReg2Reg,
  eReg2RegOther,
  eReg2RegAddr,
  eReg2RegDeref,
  eStructMember,
  eStructMemberFloat,
  eStructMemberOther,
  eStructMemberAddr,
  eMemSetImm,
  eMemAddImm,
  eRepeatIdx,
  eRepeat,
  eRegPushMe,
  eRegPushMaybe,
  eFuncReturn,
  eGoto,
  eAllocWord,
  eAllocWord2,
  eAllocWord3,
  eint_plus,
  eint_minus,
  eint_mul,
  eint_div,
  eint_modulo,
  eint_eq,
  eint_neq,
  eint_and,
  eint_or,
  eint_gt,
  eint_ge,
  eint_lt,
  eint_le,
  eint_sl,
  eint_sr,
  eint_sand,
  eint_sor,
  eint_xor,
  eint_negate,
  eIntNot,
  eint_cmp,
  eint_r_assign,
  eint_r_plus,
  eint_r_minus,
  eint_r_mul,
  eint_r_div,
  eint_r_modulo,
  eint_r_sl,
  eint_r_sr,
  eint_r_sand,
  eint_r_sor,
  eint_r_sxor,
  eint_pre_inc,
  eint_pre_dec,
  eint_post_inc,
  eint_post_dec,
  eFloatPlus,
  eFloatMinus,
  eFloatTimes,
  eFloatDivide,
  efloat_and,
  efloat_or,
  efloat_eq,
  efloat_neq,
  efloat_gt,
  efloat_ge,
  efloat_lt,
  efloat_le,
  efloat_negate,
  efloat_not,
  efloat_r_assign,
  efloat_r_plus,
  efloat_r_minus,
  efloat_r_mul,
  efloat_r_div,
  eint_float_plus,
  eint_float_minus,
  eint_float_mul,
  eint_float_div,
  eint_float_and,
  eint_float_or,
  eint_float_eq,
  eint_float_neq,
  eint_float_gt,
  eint_float_ge,
  eint_float_lt,
  eint_float_le,
  eint_float_r_assign,
  eint_float_r_plus,
  eint_float_r_minus,
  eint_float_r_mul,
  eint_float_r_div,
  efloat_int_plus,
  efloat_int_minus,
  efloat_int_mul,
  efloat_int_div,
  efloat_int_and,
  efloat_int_or,
  efloat_int_eq,
  efloat_int_neq,
  efloat_int_gt,
  efloat_int_ge,
  efloat_int_lt,
  efloat_int_le,
  efloat_int_r_assign,
  efloat_int_r_plus,
  efloat_int_r_minus,
  efloat_int_r_mul,
  efloat_int_r_div,
  eCastI2F,
  eCastF2I,
  eTime_Advance,
  eSetCode,
  eRegMove,
  eReg2Mem,
  eReg2Mem4,
  eOverflow,
  eFuncUsrEnd,
  eFuncMemberEnd,
  eSporkIni,
  eForkIni,
  eSporkFunc,
  eSporkMemberFptr,
  eSporkExp,
  eSporkEnd,
  eBranchEqInt,
  eBranchNeqInt,
  eBranchEqFloat,
  eBranchNeqFloat,
  eUnroll,
  eArrayAppend,
  eAutoUnrollInit,
  eAutoLoop,
  eArrayTop,
  eArrayAccess,
  eArrayGet,
  eArrayAddr,
  eObjectInstantiate,
  eRegAddRef,
  eRegAddRefAddr,
  eStructRegAddRef,
  eStructRegAddRefAddr,
  eObjectAssign,
  eAssign,
  eObjectRelease,
  eGWOP_EXCEPT,
  eAllocMember4,
  eDotMember,
  eDotMember2,
  eDotMember3,
  eDotMember4,
  eUnionCheck,
  eUnionMember,
  eUnionMember2,
  eUnionMember3,
  eUnionMember4,
  eDotStatic,
  eDotStatic2,
  eDotStatic3,
  eUpvalueInt,
  eUpvalueFloat,
  eUpvalueOther,
  eUpvalueAddr,
  eDotFunc,
  eGcIni,
  eGcAdd,
  eGcEnd,
  eGackType,
  eGackEnd,
  eGack,
  eTryIni,
  eTryEnd,
  eHandleEffect,
  ePerformEffect,
  eNoOp,
  eDebugLine,
  eDebugValue,
  eDebugPush,
  eDebugPop,
  eEOC,
  eUnroll2,
  eOP_MAX,
  eDotTmplVal,
};

#define  RegSetImm            (f_instr)eRegSetImm
#define  RegPushImm           (f_instr)eRegPushImm
#define  RegPushImm2          (f_instr)eRegPushImm2
#define  RegPushImm3          (f_instr)eRegPushImm3
#define  RegPushImm4          (f_instr)eRegPushImm4
#define  RegPushMem           (f_instr)eRegPushMem
#define  RegPushMem2          (f_instr)eRegPushMem2
#define  RegPushMem3          (f_instr)eRegPushMem3
#define  RegPushMem4          (f_instr)eRegPushMem4
#define  RegPushMemDeref      (f_instr)eRegPushMemDeref
#define  RegPushNow           (f_instr)eRegPushNow
#define  RegPushBase          (f_instr)eRegPushBase
#define  RegPushBase2         (f_instr)eRegPushBase2
#define  RegPushBase3         (f_instr)eRegPushBase3
#define  RegPushBase4         (f_instr)eRegPushBase4
#define  Reg2Reg              (f_instr)eReg2Reg
#define  Reg2RegOther         (f_instr)eReg2RegOther
#define  Reg2RegAddr          (f_instr)eReg2RegAddr
#define  Reg2RegDeref         (f_instr)eReg2RegDeref
#define  StructMember         (f_instr)eStructMember
#define  StructMemberFloat    (f_instr)eStructMemberFloat
#define  StructMemberOther    (f_instr)eStructMemberOther
#define  StructMemberAddr     (f_instr)eStructMemberAddr
#define  MemSetImm            (f_instr)eMemSetImm
#define  MemAddImm            (f_instr)eMemAddImm
#define  RepeatIdx            (f_instr)eRepeatIdx
#define  Repeat               (f_instr)eRepeat
#define  RegPushMe            (f_instr)eRegPushMe
#define  RegPushMaybe         (f_instr)eRegPushMaybe
#define  FuncReturn           (f_instr)eFuncReturn
#define  Goto                 (f_instr)eGoto
#define  AllocWord            (f_instr)eAllocWord
#define  AllocWord2           (f_instr)eAllocWord2
#define  AllocWord3           (f_instr)eAllocWord3
#define  int_plus             (f_instr)eint_plus
#define  int_minus            (f_instr)eint_minus
#define  int_mul              (f_instr)eint_mul
#define  int_div              (f_instr)eint_div
#define  int_modulo           (f_instr)eint_modulo
#define  int_eq               (f_instr)eint_eq
#define  int_neq              (f_instr)eint_neq
#define  int_and              (f_instr)eint_and
#define  int_or               (f_instr)eint_or
#define  int_gt               (f_instr)eint_gt
#define  int_ge               (f_instr)eint_ge
#define  int_lt               (f_instr)eint_lt
#define  int_le               (f_instr)eint_le
#define  int_sl               (f_instr)eint_sl
#define  int_sr               (f_instr)eint_sr
#define  int_sand             (f_instr)eint_sand
#define  int_sor              (f_instr)eint_sor
#define  int_xor              (f_instr)eint_xor
#define  int_negate           (f_instr)eint_negate
#define  IntNot               (f_instr)eIntNot
#define  int_cmp              (f_instr)eint_cmp
#define  int_r_assign         (f_instr)eint_r_assign
#define  int_r_plus           (f_instr)eint_r_plus
#define  int_r_minus          (f_instr)eint_r_minus
#define  int_r_mul            (f_instr)eint_r_mul
#define  int_r_div            (f_instr)eint_r_div
#define  int_r_modulo         (f_instr)eint_r_modulo
#define  int_r_sl             (f_instr)eint_r_sl
#define  int_r_sr             (f_instr)eint_r_sr
#define  int_r_sand           (f_instr)eint_r_sand
#define  int_r_sor            (f_instr)eint_r_sor
#define  int_r_sxor           (f_instr)eint_r_sxor
#define  int_pre_inc          (f_instr)eint_pre_inc
#define  int_pre_dec          (f_instr)eint_pre_dec
#define  int_post_inc         (f_instr)eint_post_inc
#define  int_post_dec         (f_instr)eint_post_dec
#define  FloatPlus            (f_instr)eFloatPlus
#define  FloatMinus           (f_instr)eFloatMinus
#define  FloatTimes           (f_instr)eFloatTimes
#define  FloatDivide          (f_instr)eFloatDivide
#define  float_and            (f_instr)efloat_and
#define  float_or             (f_instr)efloat_or
#define  float_eq             (f_instr)efloat_eq
#define  float_neq            (f_instr)efloat_neq
#define  float_gt             (f_instr)efloat_gt
#define  float_ge             (f_instr)efloat_ge
#define  float_lt             (f_instr)efloat_lt
#define  float_le             (f_instr)efloat_le
#define  float_negate         (f_instr)efloat_negate
#define  float_not            (f_instr)efloat_not
#define  float_r_assign       (f_instr)efloat_r_assign
#define  float_r_plus         (f_instr)efloat_r_plus
#define  float_r_minus        (f_instr)efloat_r_minus
#define  float_r_mul          (f_instr)efloat_r_mul
#define  float_r_div          (f_instr)efloat_r_div
#define  int_float_plus       (f_instr)eint_float_plus
#define  int_float_minus      (f_instr)eint_float_minus
#define  int_float_mul        (f_instr)eint_float_mul
#define  int_float_div        (f_instr)eint_float_div
#define  int_float_and        (f_instr)eint_float_and
#define  int_float_or         (f_instr)eint_float_or
#define  int_float_eq         (f_instr)eint_float_eq
#define  int_float_neq        (f_instr)eint_float_neq
#define  int_float_gt         (f_instr)eint_float_gt
#define  int_float_ge         (f_instr)eint_float_ge
#define  int_float_lt         (f_instr)eint_float_lt
#define  int_float_le         (f_instr)eint_float_le
#define  int_float_r_assign   (f_instr)eint_float_r_assign
#define  int_float_r_plus     (f_instr)eint_float_r_plus
#define  int_float_r_minus    (f_instr)eint_float_r_minus
#define  int_float_r_mul      (f_instr)eint_float_r_mul
#define  int_float_r_div      (f_instr)eint_float_r_div
#define  float_int_plus       (f_instr)efloat_int_plus
#define  float_int_minus      (f_instr)efloat_int_minus
#define  float_int_mul        (f_instr)efloat_int_mul
#define  float_int_div        (f_instr)efloat_int_div
#define  float_int_and        (f_instr)efloat_int_and
#define  float_int_or         (f_instr)efloat_int_or
#define  float_int_eq         (f_instr)efloat_int_eq
#define  float_int_neq        (f_instr)efloat_int_neq
#define  float_int_gt         (f_instr)efloat_int_gt
#define  float_int_ge         (f_instr)efloat_int_ge
#define  float_int_lt         (f_instr)efloat_int_lt
#define  float_int_le         (f_instr)efloat_int_le
#define  float_int_r_assign   (f_instr)efloat_int_r_assign
#define  float_int_r_plus     (f_instr)efloat_int_r_plus
#define  float_int_r_minus    (f_instr)efloat_int_r_minus
#define  float_int_r_mul      (f_instr)efloat_int_r_mul
#define  float_int_r_div      (f_instr)efloat_int_r_div
#define  CastI2F              (f_instr)eCastI2F
#define  CastF2I              (f_instr)eCastF2I
#define  Time_Advance         (f_instr)eTime_Advance
#define  SetCode              (f_instr)eSetCode
#define  RegMove              (f_instr)eRegMove
#define  Reg2Mem              (f_instr)eReg2Mem
#define  Reg2Mem4             (f_instr)eReg2Mem4
#define  Overflow             (f_instr)eOverflow
#define  FuncUsrEnd           (f_instr)eFuncUsrEnd
#define  FuncMemberEnd        (f_instr)eFuncMemberEnd
#define  SporkIni             (f_instr)eSporkIni
#define  ForkIni              (f_instr)eForkIni
#define  SporkFunc            (f_instr)eSporkFunc
#define  SporkMemberFptr      (f_instr)eSporkMemberFptr
#define  SporkExp             (f_instr)eSporkExp
#define  SporkEnd             (f_instr)eSporkEnd
#define  BranchEqInt          (f_instr)eBranchEqInt
#define  BranchNeqInt         (f_instr)eBranchNeqInt
#define  BranchEqFloat        (f_instr)eBranchEqFloat
#define  BranchNeqFloat       (f_instr)eBranchNeqFloat
#define  Unroll               (f_instr)eUnroll
#define  ArrayAppend          (f_instr)eArrayAppend
#define  AutoUnrollInit       (f_instr)eAutoUnrollInit
#define  AutoLoop             (f_instr)eAutoLoop
#define  ArrayTop             (f_instr)eArrayTop
#define  ArrayAccess          (f_instr)eArrayAccess
#define  ArrayGet             (f_instr)eArrayGet
#define  ArrayAddr            (f_instr)eArrayAddr
#define  ObjectInstantiate    (f_instr)eObjectInstantiate
#define  RegAddRef            (f_instr)eRegAddRef
#define  RegAddRefAddr        (f_instr)eRegAddRefAddr
#define  StructRegAddRef      (f_instr)eStructRegAddRef
#define  StructRegAddRefAddr  (f_instr)eStructRegAddRefAddr
#define  ObjectAssign         (f_instr)eObjectAssign
#define  Assign               (f_instr)eAssign
#define  ObjectRelease        (f_instr)eObjectRelease
#define  GWOP_EXCEPT          (f_instr)eGWOP_EXCEPT
#define  AllocMember4         (f_instr)eAllocMember4
#define  DotMember            (f_instr)eDotMember
#define  DotMember2           (f_instr)eDotMember2
#define  DotMember3           (f_instr)eDotMember3
#define  DotMember4           (f_instr)eDotMember4
#define  UnionCheck           (f_instr)eUnionCheck
#define  UnionMember          (f_instr)eUnionMember
#define  UnionMember2         (f_instr)eUnionMember2
#define  UnionMember3         (f_instr)eUnionMember3
#define  UnionMember4         (f_instr)eUnionMember4
#define  DotStatic            (f_instr)eDotStatic
#define  DotStatic2           (f_instr)eDotStatic2
#define  DotStatic3           (f_instr)eDotStatic3
#define  UpvalueInt           (f_instr)eUpvalueInt
#define  UpvalueFloat         (f_instr)eUpvalueFloat
#define  UpvalueOther         (f_instr)eUpvalueOther
#define  UpvalueAddr          (f_instr)eUpvalueAddr
#define  DotFunc              (f_instr)eDotFunc
#define  GcIni                (f_instr)eGcIni
#define  GcAdd                (f_instr)eGcAdd
#define  GcEnd                (f_instr)eGcEnd
#define  GackType             (f_instr)eGackType
#define  GackEnd              (f_instr)eGackEnd
#define  Gack                 (f_instr)eGack
#define  TryIni               (f_instr)eTryIni
#define  TryEnd               (f_instr)eTryEnd
#define  HandleEffect         (f_instr)eHandleEffect
#define  PerformEffect        (f_instr)ePerformEffect
#define  NoOp                 (f_instr)eNoOp
#define  DebugLine            (f_instr)eDebugLine
#define  DebugValue           (f_instr)eDebugValue
#define  DebugPush            (f_instr)eDebugPush
#define  DebugPop             (f_instr)eDebugPop
#define  EOC                  (f_instr)eEOC
#define  Unroll2              (f_instr)eUnroll2
#define  OP_MAX               (f_instr)eOP_MAX
#define  DotTmplVal           (f_instr)eDotTmplVal
#endif
