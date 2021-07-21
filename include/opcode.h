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
  eint_add,
  eint_sub,
  eint_mul,
  eint_div,
  eint_mod,
  eint_add_imm,
  eint_sub_imm,
  eint_mul_imm,
  eint_div_imm,
  eint_mod_imm,
  eint_eq,
  eint_neq,
  eint_and,
  eint_or,
  eint_gt,
  eint_ge,
  eint_lt,
  eint_le,
  eint_gt_imm,
  eint_ge_imm,
  eint_lt_imm,
  eint_le_imm,
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
  efloat_add,
  efloat_sub,
  efloat_mul,
  efloat_div,
  efloat_add_imm,
  efloat_sub_imm,
  efloat_mul_imm,
  efloat_div_imm,
  efloat_and,
  efloat_or,
  efloat_eq,
  efloat_neq,
  efloat_gt,
  efloat_ge,
  efloat_lt,
  efloat_le,
  efloat_gt_imm,
  efloat_ge_imm,
  efloat_lt_imm,
  efloat_le_imm,
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
  eRecurs,
  eSetCode,
  eRegMove,
  eReg2Mem,
  eReg2Mem4,
  eOverflow,
  eFuncUsrEnd,
  eFuncUsrEnd2,
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
#define  int_add              (f_instr)eint_add
#define  int_sub              (f_instr)eint_sub
#define  int_mul              (f_instr)eint_mul
#define  int_div              (f_instr)eint_div
#define  int_mod              (f_instr)eint_mod
#define  int_add_imm          (f_instr)eint_add_imm
#define  int_sub_imm          (f_instr)eint_sub_imm
#define  int_mul_imm          (f_instr)eint_mul_imm
#define  int_div_imm          (f_instr)eint_div_imm
#define  int_mod_imm          (f_instr)eint_mod_imm
#define  int_eq               (f_instr)eint_eq
#define  int_neq              (f_instr)eint_neq
#define  int_and              (f_instr)eint_and
#define  int_or               (f_instr)eint_or
#define  int_gt               (f_instr)eint_gt
#define  int_ge               (f_instr)eint_ge
#define  int_lt               (f_instr)eint_lt
#define  int_le               (f_instr)eint_le
#define  int_gt_imm           (f_instr)eint_gt_imm
#define  int_ge_imm           (f_instr)eint_ge_imm
#define  int_lt_imm           (f_instr)eint_lt_imm
#define  int_le_imm           (f_instr)eint_le_imm
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
#define  float_add            (f_instr)efloat_add
#define  float_sub            (f_instr)efloat_sub
#define  float_mul            (f_instr)efloat_mul
#define  float_div            (f_instr)efloat_div
#define  float_add_imm        (f_instr)efloat_add_imm
#define  float_sub_imm        (f_instr)efloat_sub_imm
#define  float_mul_imm        (f_instr)efloat_mul_imm
#define  float_div_imm        (f_instr)efloat_div_imm
#define  float_and            (f_instr)efloat_and
#define  float_or             (f_instr)efloat_or
#define  float_eq             (f_instr)efloat_eq
#define  float_neq            (f_instr)efloat_neq
#define  float_gt             (f_instr)efloat_gt
#define  float_ge             (f_instr)efloat_ge
#define  float_lt             (f_instr)efloat_lt
#define  float_le             (f_instr)efloat_le
#define  float_gt_imm         (f_instr)efloat_gt_imm
#define  float_ge_imm         (f_instr)efloat_ge_imm
#define  float_lt_imm         (f_instr)efloat_lt_imm
#define  float_le_imm         (f_instr)efloat_le_imm
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
#define  Recurs               (f_instr)eRecurs
#define  SetCode              (f_instr)eSetCode
#define  RegMove              (f_instr)eRegMove
#define  Reg2Mem              (f_instr)eReg2Mem
#define  Reg2Mem4             (f_instr)eReg2Mem4
#define  Overflow             (f_instr)eOverflow
#define  FuncUsrEnd           (f_instr)eFuncUsrEnd
#define  FuncUsrEnd2          (f_instr)eFuncUsrEnd2
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
ANN static inline void dump_opcodes(const VM_Code code) {
  gw_out("{Y}┏━━━━┓{0}{-Y} {+}%s{0}\n{Y}┃{0}\n", code->name);
  m_uint j = 0;
  for(m_uint i = 0; i < vector_size(&code->instr); i++) {
    const Instr instr = (Instr)vector_at(&code->instr, i);
    if (instr->opcode == eNoOp) continue;
    switch(instr->opcode) {
      case eRegSetImm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegSetImm   ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushImm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushImm  ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushImm2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushImm2 ", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushImm3:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushImm3 ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushImm4:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushImm4 ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushMem:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushMem  ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushMem2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushMem2 ", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushMem3:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushMem3 ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushMem4:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushMem4 ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushMemDeref:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushMemDeref", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRegPushNow:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushNow  ", j);
        gw_out("\n");
        break;
      case eRegPushBase:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushBase ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out("\n");
        break;
      case eRegPushBase2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushBase2", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case eRegPushBase3:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushBase3", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out("\n");
        break;
      case eRegPushBase4:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushBase4", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out("\n");
        break;
      case eReg2Reg:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Reg2Reg     ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eReg2RegOther:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Reg2RegOther", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eReg2RegAddr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Reg2RegAddr ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eReg2RegDeref:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Reg2RegDeref", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eStructMember:
        gw_out("{Y}┃{0}{-}% 4lu{0}: StructMember", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eStructMemberFloat:
        gw_out("{Y}┃{0}{-}% 4lu{0}: StructMemberFloat", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eStructMemberOther:
        gw_out("{Y}┃{0}{-}% 4lu{0}: StructMemberOther", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eStructMemberAddr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: StructMemberAddr", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eMemSetImm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: MemSetImm   ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
       gw_out(" {-M}%-14p{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eMemAddImm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: MemAddImm   ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"INT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRepeatIdx:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RepeatIdx   ", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eRepeat:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Repeat      ", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eRegPushMe:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushMe   ", j);
        gw_out("\n");
        break;
      case eRegPushMaybe:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegPushMaybe", j);
        gw_out("\n");
        break;
      case eFuncReturn:
        gw_out("{Y}┃{0}{-}% 4lu{0}: FuncReturn  ", j);
        gw_out("\n");
        break;
      case eGoto:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Goto        ", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eAllocWord:
        gw_out("{Y}┃{0}{-}% 4lu{0}: AllocWord   ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eAllocWord2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: AllocWord2  ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eAllocWord3:
        gw_out("{Y}┃{0}{-}% 4lu{0}: AllocWord3  ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eint_add:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_add     ", j);
        gw_out("\n");
        break;
      case eint_sub:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_sub     ", j);
        gw_out("\n");
        break;
      case eint_mul:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_mul     ", j);
        gw_out("\n");
        break;
      case eint_div:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_div     ", j);
        gw_out("\n");
        break;
      case eint_mod:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_mod     ", j);
        gw_out("\n");
        break;
      case eint_add_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_add_imm ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_sub_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_sub_imm ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_mul_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_mul_imm ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_div_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_div_imm ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_mod_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_mod_imm ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_eq:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_eq      ", j);
        gw_out("\n");
        break;
      case eint_neq:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_neq     ", j);
        gw_out("\n");
        break;
      case eint_and:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_and     ", j);
        gw_out("\n");
        break;
      case eint_or:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_or      ", j);
        gw_out("\n");
        break;
      case eint_gt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_gt      ", j);
        gw_out("\n");
        break;
      case eint_ge:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_ge      ", j);
        gw_out("\n");
        break;
      case eint_lt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_lt      ", j);
        gw_out("\n");
        break;
      case eint_le:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_le      ", j);
        gw_out("\n");
        break;
      case eint_gt_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_gt_imm  ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_ge_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_ge_imm  ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_lt_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_lt_imm  ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_le_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_le_imm  ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eint_sl:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_sl      ", j);
        gw_out("\n");
        break;
      case eint_sr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_sr      ", j);
        gw_out("\n");
        break;
      case eint_sand:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_sand    ", j);
        gw_out("\n");
        break;
      case eint_sor:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_sor     ", j);
        gw_out("\n");
        break;
      case eint_xor:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_xor     ", j);
        gw_out("\n");
        break;
      case eint_negate:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_negate  ", j);
        gw_out("\n");
        break;
      case eIntNot:
        gw_out("{Y}┃{0}{-}% 4lu{0}: IntNot      ", j);
        gw_out("\n");
        break;
      case eint_cmp:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_cmp     ", j);
        gw_out("\n");
        break;
      case eint_r_assign:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_assign", j);
        gw_out("\n");
        break;
      case eint_r_plus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_plus  ", j);
        gw_out("\n");
        break;
      case eint_r_minus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_minus ", j);
        gw_out("\n");
        break;
      case eint_r_mul:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_mul   ", j);
        gw_out("\n");
        break;
      case eint_r_div:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_div   ", j);
        gw_out("\n");
        break;
      case eint_r_modulo:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_modulo", j);
        gw_out("\n");
        break;
      case eint_r_sl:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_sl    ", j);
        gw_out("\n");
        break;
      case eint_r_sr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_sr    ", j);
        gw_out("\n");
        break;
      case eint_r_sand:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_sand  ", j);
        gw_out("\n");
        break;
      case eint_r_sor:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_sor   ", j);
        gw_out("\n");
        break;
      case eint_r_sxor:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_r_sxor  ", j);
        gw_out("\n");
        break;
      case eint_pre_inc:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_pre_inc ", j);
        gw_out("\n");
        break;
      case eint_pre_dec:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_pre_dec ", j);
        gw_out("\n");
        break;
      case eint_post_inc:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_post_inc", j);
        gw_out("\n");
        break;
      case eint_post_dec:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_post_dec", j);
        gw_out("\n");
        break;
      case efloat_add:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_add   ", j);
        gw_out("\n");
        break;
      case efloat_sub:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_sub   ", j);
        gw_out("\n");
        break;
      case efloat_mul:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_mul   ", j);
        gw_out("\n");
        break;
      case efloat_div:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_div   ", j);
        gw_out("\n");
        break;
      case efloat_add_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_add_imm", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case efloat_sub_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_sub_imm", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case efloat_mul_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_mul_imm", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case efloat_div_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_div_imm", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case efloat_and:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_and   ", j);
        gw_out("\n");
        break;
      case efloat_or:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_or    ", j);
        gw_out("\n");
        break;
      case efloat_eq:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_eq    ", j);
        gw_out("\n");
        break;
      case efloat_neq:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_neq   ", j);
        gw_out("\n");
        break;
      case efloat_gt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_gt    ", j);
        gw_out("\n");
        break;
      case efloat_ge:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_ge    ", j);
        gw_out("\n");
        break;
      case efloat_lt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_lt    ", j);
        gw_out("\n");
        break;
      case efloat_le:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_le    ", j);
        gw_out("\n");
        break;
      case efloat_gt_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_gt_imm", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case efloat_ge_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_ge_imm", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case efloat_lt_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_lt_imm", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case efloat_le_imm:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_le_imm", j);
        gw_out(" {-R}%-14f", instr->f);
        gw_out("\n");
        break;
      case efloat_negate:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_negate", j);
        gw_out("\n");
        break;
      case efloat_not:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_not   ", j);
        gw_out("\n");
        break;
      case efloat_r_assign:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_r_assign", j);
        gw_out("\n");
        break;
      case efloat_r_plus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_r_plus", j);
        gw_out("\n");
        break;
      case efloat_r_minus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_r_minus", j);
        gw_out("\n");
        break;
      case efloat_r_mul:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_r_mul ", j);
        gw_out("\n");
        break;
      case efloat_r_div:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_r_div ", j);
        gw_out("\n");
        break;
      case eint_float_plus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_plus", j);
        gw_out("\n");
        break;
      case eint_float_minus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_minus", j);
        gw_out("\n");
        break;
      case eint_float_mul:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_mul", j);
        gw_out("\n");
        break;
      case eint_float_div:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_div", j);
        gw_out("\n");
        break;
      case eint_float_and:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_and", j);
        gw_out("\n");
        break;
      case eint_float_or:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_or", j);
        gw_out("\n");
        break;
      case eint_float_eq:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_eq", j);
        gw_out("\n");
        break;
      case eint_float_neq:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_neq", j);
        gw_out("\n");
        break;
      case eint_float_gt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_gt", j);
        gw_out("\n");
        break;
      case eint_float_ge:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_ge", j);
        gw_out("\n");
        break;
      case eint_float_lt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_lt", j);
        gw_out("\n");
        break;
      case eint_float_le:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_le", j);
        gw_out("\n");
        break;
      case eint_float_r_assign:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_r_assign", j);
        gw_out("\n");
        break;
      case eint_float_r_plus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_r_plus", j);
        gw_out("\n");
        break;
      case eint_float_r_minus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_r_minus", j);
        gw_out("\n");
        break;
      case eint_float_r_mul:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_r_mul", j);
        gw_out("\n");
        break;
      case eint_float_r_div:
        gw_out("{Y}┃{0}{-}% 4lu{0}: int_float_r_div", j);
        gw_out("\n");
        break;
      case efloat_int_plus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_plus", j);
        gw_out("\n");
        break;
      case efloat_int_minus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_minus", j);
        gw_out("\n");
        break;
      case efloat_int_mul:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_mul", j);
        gw_out("\n");
        break;
      case efloat_int_div:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_div", j);
        gw_out("\n");
        break;
      case efloat_int_and:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_and", j);
        gw_out("\n");
        break;
      case efloat_int_or:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_or", j);
        gw_out("\n");
        break;
      case efloat_int_eq:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_eq", j);
        gw_out("\n");
        break;
      case efloat_int_neq:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_neq", j);
        gw_out("\n");
        break;
      case efloat_int_gt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_gt", j);
        gw_out("\n");
        break;
      case efloat_int_ge:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_ge", j);
        gw_out("\n");
        break;
      case efloat_int_lt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_lt", j);
        gw_out("\n");
        break;
      case efloat_int_le:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_le", j);
        gw_out("\n");
        break;
      case efloat_int_r_assign:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_r_assign", j);
        gw_out("\n");
        break;
      case efloat_int_r_plus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_r_plus", j);
        gw_out("\n");
        break;
      case efloat_int_r_minus:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_r_minus", j);
        gw_out("\n");
        break;
      case efloat_int_r_mul:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_r_mul", j);
        gw_out("\n");
        break;
      case efloat_int_r_div:
        gw_out("{Y}┃{0}{-}% 4lu{0}: float_int_r_div", j);
        gw_out("\n");
        break;
      case eCastI2F:
        gw_out("{Y}┃{0}{-}% 4lu{0}: CastI2F     ", j);
        gw_out("\n");
        break;
      case eCastF2I:
        gw_out("{Y}┃{0}{-}% 4lu{0}: CastF2I     ", j);
        gw_out("\n");
        break;
      case eTime_Advance:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Time_Advance", j);
        gw_out("\n");
        break;
      case eRecurs:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Recurs      ", j);
        gw_out("\n");
        break;
      case eSetCode:
        gw_out("{Y}┃{0}{-}% 4lu{0}: SetCode     ", j);
        gw_out("\n");
        break;
      case eRegMove:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegMove     ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eReg2Mem:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Reg2Mem     ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"INT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eReg2Mem4:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Reg2Mem4    ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eOverflow:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Overflow    ", j);
        gw_out("\n");
        break;
      case eFuncUsrEnd:
        gw_out("{Y}┃{0}{-}% 4lu{0}: FuncUsrEnd  ", j);
        gw_out("\n");
        break;
      case eFuncUsrEnd2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: FuncUsrEnd2 ", j);
        gw_out("\n");
        break;
      case eFuncMemberEnd:
        gw_out("{Y}┃{0}{-}% 4lu{0}: FuncMemberEnd", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eSporkIni:
        gw_out("{Y}┃{0}{-}% 4lu{0}: SporkIni    ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out("\n");
        break;
      case eForkIni:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ForkIni     ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out(" {-M}%-14"INT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eSporkFunc:
        gw_out("{Y}┃{0}{-}% 4lu{0}: SporkFunc   ", j);
        gw_out("\n");
        break;
      case eSporkMemberFptr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: SporkMemberFptr", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eSporkExp:
        gw_out("{Y}┃{0}{-}% 4lu{0}: SporkExp    ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eSporkEnd:
        gw_out("{Y}┃{0}{-}% 4lu{0}: SporkEnd    ", j);
        gw_out("\n");
        break;
      case eBranchEqInt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: BranchEqInt ", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eBranchNeqInt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: BranchNeqInt", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eBranchEqFloat:
        gw_out("{Y}┃{0}{-}% 4lu{0}: BranchEqFloat", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eBranchNeqFloat:
        gw_out("{Y}┃{0}{-}% 4lu{0}: BranchNeqFloat", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eUnroll:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Unroll      ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eArrayAppend:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ArrayAppend ", j);
        gw_out("\n");
        break;
      case eAutoUnrollInit:
        gw_out("{Y}┃{0}{-}% 4lu{0}: AutoUnrollInit", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eAutoLoop:
        gw_out("{Y}┃{0}{-}% 4lu{0}: AutoLoop    ", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eArrayTop:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ArrayTop    ", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eArrayAccess:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ArrayAccess ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eArrayGet:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ArrayGet    ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"INT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eArrayAddr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ArrayAddr   ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"INT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eObjectInstantiate:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ObjectInstantiate", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out(" {-B}%-14s"UINT_F"{0}", ((Type)instr->m_val2)->name);
        gw_out("\n");
        break;
      case eRegAddRef:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegAddRef   ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eRegAddRefAddr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: RegAddRefAddr", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eStructRegAddRef:
        gw_out("{Y}┃{0}{-}% 4lu{0}: StructRegAddRef", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eStructRegAddRefAddr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: StructRegAddRefAddr", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eObjectAssign:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ObjectAssign", j);
        gw_out("\n");
        break;
      case eAssign:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Assign      ", j);
        gw_out("\n");
        break;
      case eObjectRelease:
        gw_out("{Y}┃{0}{-}% 4lu{0}: ObjectRelease", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eGWOP_EXCEPT:
        gw_out("{Y}┃{0}{-}% 4lu{0}: GWOP_EXCEPT ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eAllocMember4:
        gw_out("{Y}┃{0}{-}% 4lu{0}: AllocMember4", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eDotMember:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotMember   ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eDotMember2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotMember2  ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eDotMember3:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotMember3  ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eDotMember4:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotMember4  ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eUnionCheck:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UnionCheck  ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eUnionMember:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UnionMember ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eUnionMember2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UnionMember2", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eUnionMember3:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UnionMember3", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eUnionMember4:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UnionMember4", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eDotStatic:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotStatic   ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out("\n");
        break;
      case eDotStatic2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotStatic2  ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out("\n");
        break;
      case eDotStatic3:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotStatic3  ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eUpvalueInt:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UpvalueInt  ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eUpvalueFloat:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UpvalueFloat", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eUpvalueOther:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UpvalueOther", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eUpvalueAddr:
        gw_out("{Y}┃{0}{-}% 4lu{0}: UpvalueAddr ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eDotFunc:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotFunc     ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out(" {-M}%-14"UINT_F"{0}", instr->m_val2);
        gw_out("\n");
        break;
      case eGcIni:
        gw_out("{Y}┃{0}{-}% 4lu{0}: GcIni       ", j);
        gw_out("\n");
        break;
      case eGcAdd:
        gw_out("{Y}┃{0}{-}% 4lu{0}: GcAdd       ", j);
        gw_out(" {-R}%-14"INT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eGcEnd:
        gw_out("{Y}┃{0}{-}% 4lu{0}: GcEnd       ", j);
        gw_out("\n");
        break;
      case eGackType:
        gw_out("{Y}┃{0}{-}% 4lu{0}: GackType    ", j);
        gw_out("\n");
        break;
      case eGackEnd:
        gw_out("{Y}┃{0}{-}% 4lu{0}: GackEnd     ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eGack:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Gack        ", j);
        gw_out("\n");
        break;
      case eTryIni:
        gw_out("{Y}┃{0}{-}% 4lu{0}: TryIni      ", j);
        gw_out("\n");
        break;
      case eTryEnd:
        gw_out("{Y}┃{0}{-}% 4lu{0}: TryEnd      ", j);
        gw_out("\n");
        break;
      case eHandleEffect:
        gw_out("{Y}┃{0}{-}% 4lu{0}: HandleEffect", j);
        gw_out(" {-B}=>%-12"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case ePerformEffect:
        gw_out("{Y}┃{0}{-}% 4lu{0}: PerformEffect", j);
        gw_out("\n");
        break;
      case eNoOp:
        gw_out("{Y}┃{0}{-}% 4lu{0}: NoOp        ", j);
        gw_out("\n");
        break;
      case eDebugLine:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DebugLine   ", j);
        gw_out(" {-R}%-14"UINT_F"{0}", instr->m_val);
        gw_out("\n");
        break;
      case eDebugValue:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DebugValue  ", j);
        gw_out("\n");
        break;
      case eDebugPush:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DebugPush   ", j);
        gw_out("\n");
        break;
      case eDebugPop:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DebugPop    ", j);
        gw_out("\n");
        break;
      case eEOC:
        gw_out("{Y}┃{0}{-}% 4lu{0}: EOC         ", j);
        gw_out("\n");
        break;
      case eUnroll2:
        gw_out("{Y}┃{0}{-}% 4lu{0}: Unroll2     ", j);
        gw_out("\n");
        break;
      case eOP_MAX:
        gw_out("{Y}┃{0}{-}% 4lu{0}: OP_MAX      ", j);
        gw_out(" {-R}%-14p{0}", instr->m_val);
        gw_out("\n");
        break;
      case eDotTmplVal:
        gw_out("{Y}┃{0}{-}% 4lu{0}: DotTmplVal  ", j);
        gw_out("\n");
        break;
    }
    j++;
  }
  gw_out("{Y}┃\n┗━━━━┛{0}\n");
}
#endif
