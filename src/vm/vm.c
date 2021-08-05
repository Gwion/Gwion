#include <time.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "shreduler_private.h"
#include "emit.h"
#include "gwion.h"
#include "object.h"
#include "ugen.h"
#include "operator.h"
#include "import.h"
#include "gack.h"
#include "array.h"

static inline uint64_t splitmix64_stateless(uint64_t index) {
  uint64_t z = (index + UINT64_C(0x9E3779B97F4A7C15));
  z          = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z          = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

static inline uint32_t rotl(const uint32_t x, int k) {
  return (x << k) | (x >> (32 - k));
}

void gw_seed(uint32_t rnd[2], const uint64_t s) {
  uint64_t seed = splitmix64_stateless(s);
  memcpy(rnd, &seed, sizeof(uint64_t));
}

/*xoroshiro32** */
uint32_t gw_rand(uint32_t s[2]) {
  const uint32_t s0  = s[0];
  const uint32_t s1  = s[1] ^ s0;
  const uint32_t ret = rotl(s0 * 0x9E3779BB, 5) * 5;
  s[0]               = rotl(s0, 26) ^ s1 ^ (s1 << 9);
  s[1]               = rotl(s1, 13);
  return ret;
}

ANN static inline void shred_unwind(const VM_Shred shred) {
  register frame_t *frame = &*(frame_t *)(shred->mem - sizeof(frame_t));
  shred->code             = frame->code;
  shred->pc               = frame->pc;
  shred->mem -= frame->push;
}

ANN static bool unwind(VM_Shred shred, const Symbol effect, const m_uint size) {
  const VM_Code code = shred->code;
  if (code->live_values.ptr) {
    const uint16_t pc = shred->pc;
    for (m_uint i = 0; i < m_vector_size(&code->live_values); i++) {
      VMValue *vmval = (VMValue *)m_vector_addr(&code->live_values, i);
      if (pc <= vmval->start) break;
      if (pc >= vmval->end) continue;
      m_bit *const data = &*(m_bit *)(shred->mem + vmval->offset);
      compound_release(shred, vmval->t, data);
    }
  }
  if (!size) return true;
  if (code->handlers.ptr) {
    const m_uint start = VKEY(&shred->info->frame, size - 1);
    if (start > shred->pc) return true;
    const Map m  = &code->handlers;
    m_uint    pc = 0;
    for (m_uint i = 0; i < map_size(m); i++) {
      if (start > shred->pc) break;
      if (start < shred->pc && VKEY(m, i) > shred->pc) {
        const m_uint next  = VKEY(m, i);
        const Instr  instr = (Instr)vector_at(&code->instr, next + 1);
        if (!instr->m_val2 || (Symbol)instr->m_val2 == effect) {
          pc = next + 1;
          break;
        }
      }
    }
    if (!pc) // outside of a try statement
      return true;
    shred->reg =
        (m_bit *)VPTR(&shred->info->frame, VLEN(&shred->info->frame) - 1);
    shredule(shred->tick->shreduler, shred, 0);
    shred->pc = pc; // VKEY(m, i);
    // should we pop?
    VLEN(&shred->info->frame) = size;
    return false;
  }
  // there might be no more stack to unwind
  if (shred->mem == (m_bit *)shred + sizeof(struct VM_Shred_) + SIZEOF_REG)
    return true;
  shred_unwind(shred);
  return unwind(shred, effect, size - 1);
}

ANN static void trace(VM_Shred shred, const m_uint size) {
  const m_uint line = vector_at(&shred->info->line, size - 1);
  m_uint       i;
  bool         can_skip = false;
  for (i = size; --i;) {
    const m_uint val = VPTR(&shred->info->line, i - 1);
    if (!val)
      can_skip = true;
    else if (can_skip && val)
      break;
  }
  loc_t loc = {.first = {.line = line, .column = 1},
               .last  = {.line = line, .column = 1}};
  gwerr_secondary("called from here", code_name(shred->code->name, true), loc);
  gw_err("      {M}┗━╸{0} {-}in code{0} {+W}%s{0}{-}:{0}\n", shred->code->name);
  if (shred->mem == (m_bit *)shred + sizeof(struct VM_Shred_) + SIZEOF_REG)
    return;
  shred_unwind(shred);
  trace(shred, i);
}

ANN void handle(VM_Shred shred, const m_str effect) {
  m_bit *const reg = shred->reg;
  m_bit *const mem = shred->mem;
  const m_uint size =
      shred->info->frame.ptr ? vector_size(&shred->info->frame) : 0;
  const VM_Code code = shred->code;
  shreduler_remove(shred->tick->shreduler, shred, false);
  if (!unwind(shred, insert_symbol(shred->info->vm->gwion->st, effect), size))
    return;
  gw_err("{-C}shred{W}[{Y}% 5" UINT_F "{W}]{-}: {-R}Unhandled {+R}%s{0}\n",
         shred->tick->xid, effect);
  if (shred->info->line.ptr) { // trace if available
    shred->reg  = reg;
    shred->mem  = mem;
    shred->code = code;
    gw_err("\n{-/}here is the trace:\n");
    trace(shred, vector_size(&shred->info->line));
  }
  vm_shred_exit(shred);
}

ANN bool vm_remove(const VM *vm, const m_uint index) {
  const Vector v = (Vector)&vm->shreduler->shreds;
  LOOP_OPTIM
  for (m_uint i = vector_size(v) + 1; --i;) {
    const VM_Shred sh = (VM_Shred)vector_at(v, i - 1);
    if (sh && sh->tick->xid == index) {
      handle(sh, "MsgRemove");
      return true;
    }
  }
  return false;
}

ANN void free_vm(VM *vm) {
  vector_release(&vm->shreduler->shreds);
  vector_release(&vm->ugen);
  if (vm->bbq) free_driver(vm->bbq, vm);
  MUTEX_CLEANUP(vm->shreduler->mutex);
  mp_free(vm->gwion->mp, Shreduler, vm->shreduler);
  mp_free(vm->gwion->mp, VM, vm);
}

ANN void vm_add_shred(const VM *vm, const VM_Shred shred) {
  shred->info->vm = (VM *)vm;
  shred->info->me = new_shred(shred);
  shreduler_add(vm->shreduler, shred);
}

ANN void vm_ini_shred(const VM *vm, const VM_Shred shred) {
  shred->info->vm = (VM *)vm;
  shred->info->me = new_shred(shred);
  shreduler_ini(vm->shreduler, shred);
}

ANN void vm_lock(VM const *vm) {
  if (vm->parent) vm_lock(vm->parent);
  MUTEX_LOCK(vm->shreduler->mutex);
}

ANN void vm_unlock(VM const *vm) {
  do MUTEX_UNLOCK(vm->shreduler->mutex);
  while ((vm = vm->parent));
}

ANN bool vm_running(VM const *vm) {
  if (!vm->shreduler->bbq->is_running) return 0;
  if (!vm->parent) return 1;
  return vm->shreduler->bbq->is_running = vm_running(vm->parent);
}

__attribute__((hot)) ANN static inline void vm_ugen_init(const VM *vm) {
  const Vector v = (Vector)&vm->ugen;
  LOOP_OPTIM
  for (m_uint i = vector_size(v) + 1; --i;) {
    const UGen u = (UGen)vector_at(v, i - 1);
    u->done      = 0;
    if (u->multi) {
      struct ugen_multi_ *m = u->connect.multi;
      LOOP_OPTIM
      for (m_uint j = m->n_chan + 1; --j;) UGEN(m->channel[j - 1])->done = 0;
    }
  }
  const UGen hole = (UGen)vector_at(v, 0);
  hole->compute(hole);
}

#ifdef DEBUG_STACK
#define VM_INFO                                                                \
  gw_err("{-C}shred{W}[{Y}% 5" UINT_F "{W}] {C}mem{W}[{Y}% 5" INT_F            \
         "{W}] {C}reg{W}[{Y}% 5" INT_F "{W}]{0}\n",                            \
         shred->tick->xid,                                                     \
         mem - ((m_bit *)shred + sizeof(struct VM_Shred_) + SIZEOF_REG),       \
         reg - ((m_bit *)shred + sizeof(struct VM_Shred_)));
#else
#define VM_INFO
#endif

ANN static inline bool overflow_(const m_bit *mem, const VM_Shred c) {
  return mem > (((m_bit *)c + sizeof(struct VM_Shred_) + SIZEOF_REG) +
                (SIZEOF_MEM));
}

ANN /*static inline */ VM_Shred init_spork_shred(const VM_Shred shred,
                                                 const VM_Code  code) {
  const VM_Shred sh = new_shred_base(shred, code);
  vm_add_shred(shred->info->vm, sh);
  sh->tick->parent = shred->tick;
  if (!shred->tick->child.ptr) vector_init(&shred->tick->child);
  vector_add(&shred->tick->child, (vtype)sh);
  vector_add(&shred->gc, (vtype)sh->info->me);
  return sh;
}

ANN M_Object        new_fork(const VM_Shred, const VM_Code code, const Type);
ANN static VM_Shred init_fork_shred(const VM_Shred shred, const VM_Code code,
                                    const Type t, const m_uint retsz) {
  const M_Object o  = new_fork(shred, code, t);
  VM *           vm = shred->info->vm;
  if (!vm->gwion->data->child.ptr) vector_init(&vm->gwion->data->child);
  vector_add(&vm->gwion->data->child, (vtype)o);
  fork_launch(o, retsz);
  return ME(o);
}

#define TEST0(t, pos)                                                          \
  if (!*(t *)(reg - pos)) {                                                    \
    shred->pc = PC;                                                            \
    handle(shred, "ZeroDivideException");                                      \
    break;                                                                     \
  }

#define ADVANCE() byte += BYTECODE_SZ;

#define SDISPATCH() goto *dispatch[*(m_bit *)byte];
#define IDISPATCH()                                                            \
  {                                                                            \
    VM_INFO;                                                                   \
    SDISPATCH();                                                               \
  }

#define SET_BYTE(pc) (byte = bytecode + (pc)*BYTECODE_SZ)

#define PC_DISPATCH(pc)                                                        \
  SET_BYTE((pc));                                                              \
  IDISPATCH();

#define DISPATCH()                                                             \
  ADVANCE();                                                                   \
  IDISPATCH();

#define ADVANCE() byte += BYTECODE_SZ;

#define ADISPATCH()                                                            \
  {                                                                            \
    ADVANCE();                                                                 \
    SDISPATCH();                                                               \
  }

#define PC (*(unsigned *)(byte + 1))

#define OP(t, sz, op, ...)                                                     \
  reg -= sz;                                                                   \
  __VA_ARGS__                                                                  \
  *(t *)(reg - sz) op## = *(t *)reg;                                           \
  DISPATCH();

#define INT_OP(op, ...)   OP(m_int, SZ_INT, op, __VA_ARGS__)
#define INT_IMM_OP(op)   *(m_int*)(reg-SZ_INT) op VAL; DISPATCH()
#define FLOAT_OP(op, ...) OP(m_float, SZ_FLOAT, op, __VA_ARGS__)
#define FLOAT_IMM_OP(op)   *(m_float*)(reg-SZ_FLOAT) op VAL; DISPATCH()

#define LOGICAL(t, sz0, sz, op)                                                \
  reg -= sz0;                                                                  \
  *(m_uint *)(reg - SZ_INT) = (*(t *)(reg - SZ_INT) op * (t *)(reg + sz));      \
  DISPATCH()

#define INT_LOGICAL(op) LOGICAL(m_int, SZ_INT, 0, op)
#define INT_IMM_LOGICAL(op) *(m_int *)(reg - SZ_INT) = *(m_int*)(reg-SZ_INT) op IVAL; DISPATCH();

#define FLOAT_LOGICAL(op)                                                      \
  LOGICAL(m_float, SZ_FLOAT * 2 - SZ_INT, SZ_FLOAT - SZ_INT, op)

#define FLOAT_IMM_LOGICAL(op) \
  reg -= SZ_FLOAT - SZ_INT;\
  *(m_uint *)(reg - SZ_INT) = *(m_float*)(reg-SZ_INT) op FVAL; DISPATCH();

#define SELF(t, sz, op)                                                        \
  *(t *)(reg - sz) = op * (t *)(reg - sz);                                     \
  DISPATCH();

// check me
#define R(t, sz, op, ...)                                                      \
  reg -= SZ_INT;                                                               \
  __VA_ARGS__                                                                  \
  *(t *)(reg - sz) = (**(t **)reg op## = (*(t *)(reg - sz)));                  \
  DISPATCH()
#define INT_R(op, ...)   R(m_int, SZ_INT, op, __VA_ARGS__)
#define FLOAT_R(op, ...) R(m_float, SZ_FLOAT, op)

#define INT_PRE(op)                                                            \
  /*assert(*(m_int**)(reg-SZ_INT));*/                                          \
  *(m_int *)(reg - SZ_INT) = op(**(m_int **)(reg - SZ_INT));                   \
  DISPATCH()

#define INT_POST(op)                                                           \
  /*assert(*(m_int**)(reg-SZ_INT));*/                                          \
  *(m_int *)(reg - SZ_INT) = (**(m_int **)(reg - SZ_INT)) op;                  \
  DISPATCH()

#define IF_OP(op)                                                              \
  reg -= SZ_INT;                                                               \
  *(m_float *)(reg - SZ_FLOAT) = (m_float) * (m_int *)(reg - SZ_FLOAT) op *    \
                                 (m_float *)(reg + SZ_INT - SZ_FLOAT);         \
  DISPATCH()

#define IF_LOGICAL(op)                                                         \
  reg -= SZ_FLOAT;                                                             \
  *(m_int *)(reg - SZ_INT) =                                                   \
      (m_int)(*(m_int *)(reg - SZ_INT) op * (m_float *)reg);                   \
  DISPATCH()
__attribute__((hot))

#define IF_R(op)                                                               \
  reg -= SZ_INT * 2 - SZ_FLOAT;                                                \
  *(m_float *)(reg - SZ_FLOAT) =                                               \
      (**(m_float **)(reg + SZ_INT - SZ_FLOAT) op## =                          \
           (m_float) * (m_int *)(reg - SZ_FLOAT));                             \
  DISPATCH()

#define FI_OP(op)                                                              \
  reg -= SZ_INT;                                                               \
  *(m_float *)(reg - SZ_FLOAT) op## = (m_float) * (m_int *)reg;                \
  DISPATCH()

#define FI_LOGICAL(op)                                                         \
  reg -= SZ_FLOAT;                                                             \
  *(m_int *)(reg - SZ_INT) = (m_int)(*(m_float *)(reg - SZ_INT) op *           \
                                     (m_int *)(reg + SZ_FLOAT - SZ_INT));      \
  DISPATCH()

#define FI_R(op, ...)                                                          \
  reg -= SZ_FLOAT;                                                             \
  __VA_ARGS__                                                                  \
  *(m_int *)(reg - SZ_INT) = (**(m_int **)(reg + SZ_FLOAT - SZ_INT) op## =     \
                                  /*(m_int)*/ (*(m_float *)(reg - SZ_INT)));   \
  DISPATCH()

#define STRINGIFY_NX(a) #a
#define STRINGIFY(a)    STRINGIFY_NX(a)
#define PPCAT_NX(A, B)  A##B
#define PPCAT(A, B)     PPCAT_NX(A, B)

#if defined(__clang__)
#define COMPILER clang
#define UNINITIALIZED "-Wuninitialized")
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER GCC
#define UNINITIALIZED "-Wmaybe-uninitialized")
#endif

#define PRAGMA_PUSH()                                                          \
_Pragma(STRINGIFY(COMPILER diagnostic push)) \
_Pragma(STRINGIFY(COMPILER diagnostic ignored UNINITIALIZED)
#define PRAGMA_POP() _Pragma(STRINGIFY(COMPILER diagnostic pop))

#define VMSZ (SZ_INT > SZ_FLOAT ? SZ_INT : SZ_FLOAT)

#define VAL   (*(m_uint *)(byte + VMSZ))
#define IVAL  (*(m_int *)(byte + VMSZ))
#define FVAL  (*(m_float *)(byte + VMSZ))
#define VAL2  (*(m_uint *)(byte + SZ_INT + SZ_INT))
#define IVAL2  (*(m_int *)(byte + SZ_INT + SZ_INT))
#define SVAL  (*(uint16_t *)(byte + SZ_INT + SZ_INT))
#define SVAL2 (*(uint16_t *)(byte + SZ_INT + SZ_INT + sizeof(uint16_t)))

#define BRANCH_DISPATCH(check)                                                 \
  if (check)                                                                   \
    SET_BYTE(VAL);                                                             \
  else                                                                         \
    ADVANCE();                                                                 \
  IDISPATCH();

#define VM_OUT                                                                 \
  shred->code = code;                                                          \
  shred->reg  = reg;                                                           \
  shred->mem  = mem;                                                           \
  shred->pc   = PC;

__attribute__((hot)) ANN void
vm_run(const VM *vm) { // lgtm [cpp/use-of-goto]
  static const void *dispatch[] = {
      &&regsetimm, &&regpushimm, &&regpushfloat, &&regpushother, &&regpushaddr,
      &&regpushmem, &&regpushmemfloat, &&regpushmemother, &&regpushmemaddr,
      &&regpushmemderef, &&pushnow, &&baseint, &&basefloat, &&baseother,
      &&baseaddr, &&regtoreg, &&regtoregother, &&regtoregaddr, &&regtoregderef,
      &&structmember, &&structmemberfloat, &&structmemberother,
      &&structmemberaddr, &&memsetimm, &&memaddimm, &&repeatidx, &&repeat,
      &&regpushme, &&regpushmaybe, &&funcreturn, &&_goto, &&allocint,
      &&allocfloat, &&allocother,
      &&intplus, &&intminus, &&intmul, &&intdiv, &&intmod,
      &&intplusimm, &&intminusimm, &&intmulimm, &&intdivimm, &&intmodimm,
      // int relationnal
      &&inteq, &&intne, &&intand, &&intor,
      &&intgt, &&intge, &&intlt, &&intle,
      &&intgtimm, &&intgeimm, &&intltimm, &&intleimm,
      &&intsl, &&intsr, &&intsand, &&intsor, &&intxor, &&intnegate, &&intnot,
      &&intcmp, &&intrassign, &&intradd, &&intrsub, &&intrmul, &&intrdiv,
      &&intrmod, &&intrsl, &&intrsr, &&intrsand, &&intrsor, &&intrxor, &&preinc,
      &&predec, &&postinc, &&postdec,
      &&floatadd, &&floatsub, &&floatmul, &&floatdiv,
      &&floataddimm, &&floatsubimm, &&floatmulimm, &&floatdivimm,
      // logical
      &&floatand, &&floator, &&floateq, &&floatne,
      &&floatgt, &&floatge, &&floatlt, &&floatle,
      &&floatgtimm, &&floatgeimm, &&floatltimm, &&floatleimm,
      &&floatneg, &&floatnot, &&floatrassign, &&floatradd,
      &&floatrsub, &&floatrmul, &&floatrdiv, &&ifadd, &&ifsub, &&ifmul, &&ifdiv,
      &&ifand, &&ifor, &&ifeq, &&ifne, &&ifgt, &&ifge, &&iflt, &&ifle,
      &&ifrassign, &&ifradd, &&ifrsub, &&ifrmul, &&ifrdiv, &&fiadd, &&fisub,
      &&fimul, &&fidiv, &&fiand, &&fior, &&fieq, &&fine, &&figt, &&fige, &&filt,
      &&file, &&firassign, &&firadd, &&firsub, &&firmul, &&firdiv, &&itof,
      &&ftoi, &&timeadv, &&recurs, &&setcode, &&regmove,
      &&regtomem, &&regtomemother,
      &&overflow,
      &&funcusrend, &&funcusrend2, &&funcmemberend,
      &&sporkini, &&forkini, &&sporkfunc, &&sporkmemberfptr, &&sporkexp,
      &&sporkend, &&brancheqint, &&branchneint, &&brancheqfloat,
      &&branchnefloat, &&unroll, &&arrayappend, &&autounrollinit, &&autoloop,
      &&arraytop, &&arrayaccess, &&arrayget, &&arrayaddr, &&newobj, &&addref,
      &&addrefaddr, &&structaddref, &&structaddrefaddr, &&objassign, &&assign,
      &&remref, &&except, &&allocmemberaddr, &&dotmember, &&dotfloat,
      &&dotother, &&dotaddr, &&unioncheck, &&unionint, &&unionfloat,
      &&unionother, &&unionaddr, &&staticint, &&staticfloat, &&staticother,
      &&upvalueint, &&upvaluefloat, &&upvalueother, &&upvalueaddr, &&dotfunc,
      &&gcini, &&gcadd, &&gcend, &&gacktype, &&gackend, &&gack, &&try_ini,
      &&try_end, &&handleeffect, &&performeffect, &&noop, &&debugline,
      &&debugvalue, &&debugpush, &&debugpop, &&eoc, &&unroll2, &&other,
      &&regpushimm};
  const Shreduler   s = vm->shreduler;
  register VM_Shred shred;
  register m_bit    next;

  while ((shred = shreduler_get(s))) {
    register VM_Code code     = shred->code;
    register m_bit * bytecode = code->bytecode;
    register m_bit * byte     = bytecode + shred->pc * BYTECODE_SZ;
    register m_bit * reg      = shred->reg;
    register m_bit * mem      = shred->mem;
    register union {
      M_Object obj;
      VM_Code  code;
    } a;
    PRAGMA_PUSH()
    register VM_Shred child;
    PRAGMA_POP()
    MUTEX_LOCK(s->mutex);
    do {
      SDISPATCH();
    regsetimm:
      *(m_uint *)(reg + IVAL2) = VAL;
      DISPATCH();
    regpushimm:
      *(m_uint *)reg = VAL;
      reg += SZ_INT;
      DISPATCH();
    regpushfloat:
      *(m_float *)reg = FVAL;
      reg += SZ_FLOAT;
      DISPATCH();
    regpushother:
      //  LOOP_OPTIM
      for (m_uint i = 0; i <= VAL2; i += SZ_INT)
        *(m_bit **)(reg + i) = *(m_bit **)(VAL + i);
      reg += VAL2;
      DISPATCH();
    regpushaddr:
      *(m_uint **)reg = &VAL;
      reg += SZ_INT;
      DISPATCH()
    regpushmem:
      *(m_uint *)reg = *(m_uint *)(mem + IVAL);
      reg += SZ_INT;
      DISPATCH();
    regpushmemfloat:
      *(m_float *)reg = *(m_float *)(mem + IVAL);
      reg += SZ_FLOAT;
      DISPATCH();
    regpushmemother:
      for (m_uint i = 0; i <= VAL2; i += SZ_INT)
        *(m_uint *)(reg + i) = *(m_uint *)((m_bit *)(mem + IVAL) + i);
      reg += VAL2;
      DISPATCH();
    regpushmemaddr:
      *(m_bit **)reg = &*(m_bit *)(mem + IVAL);
      reg += SZ_INT;
      DISPATCH()
    regpushmemderef:
      memcpy(reg, *(m_uint **)(mem + IVAL), VAL2);
      reg += VAL2;
      DISPATCH()
    pushnow:
      *(m_float *)reg = vm->bbq->pos;
      reg += SZ_FLOAT;
      DISPATCH();
    baseint:
      *(m_uint *)reg = *(m_uint *)(shred->base + VAL);
      reg += SZ_INT;
      DISPATCH();
    basefloat:
      *(m_float *)reg = *(m_float *)(shred->base + VAL);
      reg += SZ_FLOAT;
      DISPATCH();
    baseother:
      //  LOOP_OPTIM
      for (m_uint i = 0; i <= VAL2; i += SZ_INT)
        *(m_uint *)(reg + i) = *(m_uint *)((shred->base + VAL) + i);
      reg += VAL2;
      DISPATCH();
    baseaddr:
      *(m_uint **)reg = &*(m_uint *)(shred->base + IVAL);
      reg += SZ_INT;
      DISPATCH();
    regtoreg:
      *(m_uint *)(reg + IVAL) = *(m_uint *)(reg + IVAL2);
      DISPATCH()
    regtoregother:
      memcpy(*(m_bit **)(reg - SZ_INT), reg + IVAL, VAL2);
      DISPATCH()
    regtoregaddr:
      *(m_uint **)(reg + IVAL) = &*(m_uint *)(reg + IVAL2);
      DISPATCH()
    regtoregderef:
      memcpy(*(m_bit **)(reg - SZ_INT), *(m_bit **)(reg + IVAL), VAL2);
      DISPATCH()
    structmember:
      *(m_bit **)(reg - SZ_INT) =
          *(m_bit **)(*(m_bit **)(reg - SZ_INT) + IVAL);
      DISPATCH()
    structmemberfloat:
      *(m_bit **)(reg - SZ_INT) =
          *(m_bit **)(*(m_bit **)(reg - SZ_INT) + IVAL);
      DISPATCH()
    structmemberother:
      *(m_bit **)(reg - SZ_INT) =
          *(m_bit **)(*(m_bit **)(reg - SZ_INT) + IVAL);
      DISPATCH()
    structmemberaddr:
      *(m_bit **)(reg - SZ_INT) = &*(*(m_bit **)(reg - SZ_INT) + IVAL);
      DISPATCH()
    memsetimm:
      *(m_uint *)(mem + VAL) = VAL2;
      DISPATCH();
    memaddimm:
      *(m_int *)(mem + VAL) += IVAL2;
      //  (*(m_int*)(mem+VAL))--;
      DISPATCH();
    repeatidx:
      BRANCH_DISPATCH(*(m_int *)(mem + VAL2 + SZ_INT) ==
                      ++(*(m_int *)(mem + VAL2)));
    repeat:
      BRANCH_DISPATCH(!(*(m_uint *)(mem + VAL2))--);
    regpushme:
      *(M_Object *)reg = shred->info->me;
      reg += SZ_INT;
      DISPATCH()
    regpushmaybe:
      *(m_uint *)reg = gw_rand((uint32_t *)vm->rand) > (UINT32_MAX / 2);
      reg += SZ_INT;
      DISPATCH();
    funcreturn : {
      register frame_t frame = *(frame_t *)(mem - sizeof(frame_t));
      bytecode               = (code = frame.code)->bytecode;
      mem -= frame.push;
      PC_DISPATCH(frame.pc);
    }
    _goto:
      PC_DISPATCH(VAL);
    allocint:
      *(m_uint *)reg = *(m_uint *)(mem + VAL) = 0;
      reg += SZ_INT;
      DISPATCH()
    allocfloat:
      *(m_float *)reg = *(m_float *)(mem + VAL) = 0;
      reg += SZ_FLOAT;
      DISPATCH()
    allocother:
      //  LOOP_OPTIM
      for (m_uint i = 0; i <= VAL2; i += SZ_INT)
        *(m_uint *)(reg + i) = (*(m_uint *)(mem + VAL + i) = 0);
      reg += VAL2;
      DISPATCH()

    intplus:
      INT_OP(+)
    intminus:
      INT_OP(-)
    intmul:
      INT_OP(*)
    intdiv:
      INT_OP(/, TEST0(m_int, 0))
    intmod:
      INT_OP(%, TEST0(m_int, 0))

    intplusimm:
      INT_IMM_OP(+=)
    intminusimm:
      INT_IMM_OP(-=)
    intmulimm:
      INT_IMM_OP(*=)
    intdivimm:
      INT_IMM_OP(/=)
    intmodimm:
      INT_IMM_OP(%=)

    inteq:
      INT_LOGICAL(==)
    intne:
      INT_LOGICAL(!=)
    intand:
      INT_LOGICAL(&&)
    intor:
      INT_LOGICAL(||)
    intgt:
      INT_LOGICAL(>)
    intge:
      INT_LOGICAL(>=)
    intlt:
      INT_LOGICAL(<)
    intle:
      INT_IMM_LOGICAL(<=)
    intgtimm:
      INT_IMM_LOGICAL(>)
    intgeimm:
      INT_IMM_LOGICAL(>=)
    intltimm:
      INT_IMM_LOGICAL(<)
    intleimm:
      INT_IMM_LOGICAL(<=)
    intsl:
      INT_LOGICAL(<<)
    intsr:
      INT_LOGICAL(>>)
    intsand:
      INT_LOGICAL(&)
    intsor:
      INT_LOGICAL(|)
    intxor:
      INT_LOGICAL(^)

    intnegate:
      *(m_int *)(reg - SZ_INT) *= -1;
      DISPATCH()
    intnot:
      SELF(m_int, SZ_INT, !)
    intcmp:
      SELF(m_int, SZ_INT, ~)

    intrassign:
      reg -= SZ_INT;
      **(m_int **)reg = *(m_int *)(reg - SZ_INT);
      DISPATCH()

    intradd:
      INT_R(+)
    intrsub:
      INT_R(-)
    intrmul:
      INT_R(*)
    // intrdiv: INT_R(/, TEST0(m_int, -SZ_INT))
    // intrmod: INT_R(%, TEST0(m_int, -SZ_INT))
    intrdiv:
      INT_R(/, TEST0(m_int, SZ_INT))
    intrmod:
      INT_R(%, TEST0(m_int, SZ_INT))
    intrsl:
      INT_R(<<)
    intrsr:
      INT_R(>>)
    intrsand:
      INT_R(&)
    intrsor:
      INT_R(|)
    intrxor:
      INT_R(^)

    preinc:
      INT_PRE(++)
    predec:
      INT_PRE(--)

    postinc:
      INT_POST(++)
    postdec:
      INT_POST(--)

    floatadd:
      FLOAT_OP(+)
    floatsub:
      FLOAT_OP(-)
    floatmul:
      FLOAT_OP(*)
    floatdiv:
      FLOAT_OP(/)

    floataddimm:
      FLOAT_IMM_OP(+=)
    floatsubimm:
      FLOAT_IMM_OP(-=)
    floatmulimm:
      FLOAT_IMM_OP(*=)
    floatdivimm:
      FLOAT_IMM_OP(/=)

    floatand:
      FLOAT_LOGICAL(&&)
    floator:
      FLOAT_LOGICAL(||)
    floateq:
      FLOAT_LOGICAL(==)
    floatne:
      FLOAT_LOGICAL(!=)
    floatgt:
      FLOAT_LOGICAL(>)
    floatge:
      FLOAT_LOGICAL(>=)
    floatlt:
      FLOAT_LOGICAL(<)
    floatle:
      FLOAT_LOGICAL(<=)
    floatgtimm:
      FLOAT_IMM_LOGICAL(>)
    floatgeimm:
      FLOAT_IMM_LOGICAL(>=)
    floatltimm:
      FLOAT_IMM_LOGICAL(<)
    floatleimm:
      FLOAT_IMM_LOGICAL(<=)

    floatneg:
      SELF(m_float, SZ_FLOAT, -)

    floatnot:
      reg -= SZ_FLOAT - SZ_INT;
      *(m_int *)(reg - SZ_INT) = !*(m_float *)(reg - SZ_INT);
      DISPATCH()

    floatrassign:
      reg -= SZ_INT;
      **(m_float **)reg = *(m_float *)(reg - SZ_FLOAT);
      DISPATCH()

    floatradd:
      FLOAT_R(+)
    floatrsub:
      FLOAT_R(-)
    floatrmul:
      FLOAT_R(*)
    floatrdiv:
      FLOAT_R(/)

    ifadd:
      IF_OP(+)
    ifsub:
      IF_OP(-)
    ifmul:
      IF_OP(*)
    ifdiv:
      IF_OP(/)

    ifand:
      IF_LOGICAL(&&)
    ifor:
      IF_LOGICAL(||)
    ifeq:
      IF_LOGICAL(==)
    ifne:
      IF_LOGICAL(!=)
    ifgt:
      IF_LOGICAL(>)
    ifge:
      IF_LOGICAL(>=)
    iflt:
      IF_LOGICAL(<)
    ifle:
      IF_LOGICAL(<=)

    ifrassign:
      IF_R()
    ifradd:
      IF_R(+)
    ifrsub:
      IF_R(-)
    ifrmul:
      IF_R(*)
    ifrdiv:
      IF_R(/)

    fiadd:
      FI_OP(+)
    fisub:
      FI_OP(-)
    fimul:
      FI_OP(*)
    fidiv:
      FI_OP(/)

    fiand:
      FI_LOGICAL(&&)
    fior:
      FI_LOGICAL(||)
    fieq:
      FI_LOGICAL(==)
    fine:
      FI_LOGICAL(!=)
    figt:
      FI_LOGICAL(>)
    fige:
      FI_LOGICAL(>=)
    filt:
      FI_LOGICAL(<)
    file:
      FI_LOGICAL(<=)

    firassign:
      reg -= SZ_FLOAT;
      *(m_int *)(reg - SZ_INT) = **(m_int **)(reg + SZ_FLOAT - SZ_INT) =
          (m_int) * (m_float *)(reg - SZ_INT);
      DISPATCH()

    firadd:
      FI_R(+)
    firsub:
      FI_R(-)
    firmul:
      FI_R(*)
    firdiv:
      FI_R(/, TEST0(m_float, SZ_INT))

    itof:
      reg -= SZ_INT - SZ_FLOAT;
      *(m_float *)(reg - SZ_FLOAT) = (m_float) * (m_int *)(reg - SZ_FLOAT);
      DISPATCH()
    ftoi:
      reg -= SZ_FLOAT - SZ_INT;
      *(m_int *)(reg - SZ_INT) = (m_int) * (m_float *)(reg - SZ_INT);
      DISPATCH()
    timeadv:
      reg -= SZ_FLOAT;
      shredule(s, shred, *(m_float *)(reg - SZ_FLOAT));
      *(m_float *)(reg - SZ_FLOAT) += vm->bbq->pos;
      VM_OUT
      break;
    recurs : {
      register const uint push = SVAL2;
      mem += push;
      *(frame_t *)(mem - sizeof(frame_t)) =
          (frame_t) {.code = code, .pc = SVAL, .push = push};
      reg += IVAL;
      next = eFuncUsrEnd2;
    }
      DISPATCH();
    setcode:
      PRAGMA_PUSH()
      a.code = *(VM_Code *)(reg - SZ_INT);
      if (!a.code->builtin) {
        register const uint push =
            *(m_uint *)reg /*+ code->stack_depth*/ + sizeof(frame_t);
        mem += push;
        *(frame_t *)(mem - sizeof(frame_t)) =
            (frame_t) {.code = code, .pc = PC + SVAL, .push = push};
        next = eFuncUsrEnd;
      } else {
        mem += *(m_uint *)reg;
        next = eFuncMemberEnd;
      }
      PRAGMA_POP()
    regmove:
      reg += IVAL;
      DISPATCH();
    regtomem:
      *(m_uint *)(mem + VAL) = *(m_uint *)(reg + IVAL2);
      DISPATCH()
    regtomemother:
      memcpy(mem + VAL, reg, VAL2);
      DISPATCH()
    overflow:
      if (overflow_(mem + VAL2, shred)) {
        shred->pc = PC;
        handle(shred, "StackOverflow");
        continue;
      }
      PRAGMA_PUSH()
      goto *dispatch[next];
      PRAGMA_POP()
    funcusrend:
      PRAGMA_PUSH()
      byte = bytecode = (code = a.code)->bytecode;
      PRAGMA_POP()
      SDISPATCH();
    funcusrend2:
      byte = bytecode;
      SDISPATCH();
    funcmemberend:
      VM_OUT {
        register const m_uint val  = VAL;
        register const m_uint val2 = VAL2;
        ((f_mfun)a.code->native_func)((*(M_Object *)mem), reg, shred);
        reg += val;
        shred->mem = (mem -= val2);
        if (!s->curr) break;
      }
      PC_DISPATCH(shred->pc)
    sporkini:
      child = init_spork_shred(shred, (VM_Code)VAL);
      DISPATCH()
    forkini:
      reg -= SZ_INT;
      child = init_fork_shred(shred, (VM_Code)VAL, *(Type *)reg, VAL2),
      DISPATCH()
    sporkfunc:
      PRAGMA_PUSH()
      //  LOOP_OPTIM
      for (m_uint i = 0; i < VAL; i += SZ_INT)
        *(m_uint *)(child->reg + i) = *(m_uint *)(reg + i + IVAL2);
      child->reg += VAL;
      DISPATCH()
      PRAGMA_POP()
    sporkmemberfptr:
      for (m_uint i = SZ_INT; i < VAL; i += SZ_INT)
        *(m_uint *)(child->reg + i) = *(m_uint *)(reg - VAL + i);
      *(M_Object *)(child->reg + VAL) = *(M_Object *)(reg + VAL + SZ_INT);
      *(m_uint *)(child->reg + VAL + SZ_INT) =
          *(m_uint *)(reg + VAL - SZ_INT * 2);
      child->reg += VAL + SZ_INT * 2;
      DISPATCH()
    sporkexp:
      //  LOOP_OPTIM
      for (m_uint i = 0; i < VAL; i += SZ_INT)
        *(m_uint *)(child->mem + i) = *(m_uint *)(mem + i);
      DISPATCH()
    sporkend:
      assert(!VAL); // spork are not mutable
      PRAGMA_PUSH()
      *(M_Object *)(reg - SZ_INT) = child->info->me;
      PRAGMA_POP()
      DISPATCH()
    brancheqint:
      reg -= SZ_INT;
      BRANCH_DISPATCH(!*(m_uint *)reg);
    branchneint:
      reg -= SZ_INT;
      BRANCH_DISPATCH(*(m_uint *)reg);
    brancheqfloat:
      reg -= SZ_FLOAT;
      BRANCH_DISPATCH(!*(m_float *)reg);
    branchnefloat:
      reg -= SZ_FLOAT;
      BRANCH_DISPATCH(*(m_float *)reg);
    unroll : {
      const m_uint n   = *(m_uint *)(mem + VAL - SZ_INT);
      const m_uint idx = *(m_uint *)(mem + VAL);
      if (idx > 0) {
        if (idx >= n) {
          *(m_uint *)(mem + VAL) -= n;
          DISPATCH()
        }
        *(m_uint *)(mem + VAL) = 0;
        PC_DISPATCH(PC + VAL2 * (idx + 1));
      }
      PC_DISPATCH(PC + VAL2 * n + 1);
    }
    arrayappend:
      m_vector_add(ARRAY(*(M_Object *)(reg - SZ_INT)), reg);
      DISPATCH()
    autounrollinit:
      *(m_uint *)(mem + VAL) =
          m_vector_size(ARRAY(*(M_Object *)(mem + VAL + SZ_INT)));
      DISPATCH()
    autoloop:
      *(m_bit **)(mem + VAL2 + SZ_INT) =
          m_vector_addr(ARRAY(*(M_Object *)(mem + VAL2 - SZ_INT)),
                        ++*(m_uint *)(mem + VAL2));
      BRANCH_DISPATCH(
          m_vector_size(ARRAY(*(M_Object *)(mem + VAL2 - SZ_INT))) ==
          *(m_uint *)(mem + VAL2));
    arraytop:
      if (*(m_uint *)(reg - SZ_INT * 2) < *(m_uint *)(reg - SZ_INT))
        goto newobj;
      else
        goto _goto;
    arrayaccess : {
      register const m_int idx = *(m_int *)(reg + VAL);
      a.obj                    = *(M_Object *)(reg - VAL2);
      if (idx < 0 || (m_uint)idx >= m_vector_size(ARRAY(a.obj))) {
        gw_err(_("{-}  ... at index {W}[{Y}%" INT_F "{W}]{0}\n"), idx);
        //    gw_err(_("  ... at dimension [%" INT_F "]\n"), VAL);
        VM_OUT
        handle(shred, "ArrayOutofBounds");
        continue; // or break ?
      }
      DISPATCH()
    }
    arrayget:
      PRAGMA_PUSH()
      m_vector_get(ARRAY(a.obj), *(m_int *)(reg + VAL), (reg + IVAL2));
      PRAGMA_POP()
      DISPATCH()
    arrayaddr:
      PRAGMA_PUSH()
      *(m_bit **)(reg + IVAL2) =
          m_vector_addr(ARRAY(a.obj), *(m_int *)(reg + VAL));
      PRAGMA_POP()
      DISPATCH()
    newobj:
      *(M_Object *)reg = new_object(vm->gwion->mp, NULL, (Type)VAL2);
      reg += SZ_INT;
      DISPATCH()
    addref : {
      const M_Object o = *(M_Object *)(reg + IVAL);
      //    if(o)
      ++o->ref;
    }
      DISPATCH()
    addrefaddr : {
      const M_Object o = **(M_Object **)(reg + IVAL);
      if (o) ++o->ref;
    }
      DISPATCH()
    structaddref:
      struct_addref(vm->gwion, (Type)VAL2, *(m_bit **)(reg + IVAL));
      DISPATCH()
    structaddrefaddr:
      struct_addref(vm->gwion, (Type)VAL2, **(m_bit ***)(reg + IVAL));
      DISPATCH()
    objassign : {
      const M_Object o = **(M_Object **)(reg - SZ_INT);
      release(o, shred);
    }
    assign:
      reg -= SZ_INT;
      **(M_Object **)reg = *(M_Object *)(reg - SZ_INT);
      DISPATCH()
    remref:
      release(*(M_Object *)(mem + VAL), shred);
      DISPATCH()
    except:
      /* TODO: Refactor except instruction             *
       * so that                                       *
       *  VAL = offset (no default SZ_INT)             *
       *  VAL2 = error message                         *
       * grep for GWOP_EXCEPT and Except, exception... */
      if (!*(m_bit **)(reg + (m_int)(VAL))) {
        shred->pc = PC;
        handle(shred, "NullPtrException");
        continue;
      }
      DISPATCH();
    allocmemberaddr:
      *(m_bit **)reg = (*(M_Object *)mem)->data + VAL;
      reg += SZ_INT;
      DISPATCH()
    dotmember:
      *(m_uint *)(reg - SZ_INT) =
          *(m_uint *)((*(M_Object *)(reg - SZ_INT))->data + VAL);
      DISPATCH()
    dotfloat:
      *(m_float *)(reg - SZ_INT) =
          *(m_float *)((*(M_Object *)(reg - SZ_INT))->data + VAL);
      reg += SZ_FLOAT - SZ_INT;
      DISPATCH()
    dotother:
      //  LOOP_OPTIM
      PRAGMA_PUSH()
      for (m_uint i = 0; i <= VAL2; i += SZ_INT)
        *(m_uint *)(reg + i - SZ_INT) =
            *(m_uint *)(((*(M_Object *)(reg - SZ_INT))->data + VAL) + i);
      PRAGMA_POP()
      reg += VAL2 - SZ_INT;
      DISPATCH()
    dotaddr:
      *(m_bit **)(reg - SZ_INT) = ((*(M_Object *)(reg - SZ_INT))->data + VAL);
      DISPATCH()

#define UNION_CHECK                                                            \
  register const m_bit *data = (*(M_Object *)(reg - SZ_INT))->data;            \
  if (*(m_uint *)data != VAL) {                                                \
    handle(shred, "invalid union acces");                                      \
    continue;                                                                  \
  }

    unioncheck : {
      if (*(m_uint *)(*(M_Object *)(reg - SZ_INT))->data != VAL2) {
        reg -= SZ_INT;
        PC_DISPATCH(VAL);
      }
      DISPATCH()
    }
    unionint : {
      UNION_CHECK
      *(m_uint *)(reg - SZ_INT) = *(m_uint *)(data + SZ_INT);
      DISPATCH()
    }
    unionfloat : {
      UNION_CHECK
      *(m_float *)(reg - SZ_INT) = *(m_float *)(data + SZ_INT);
      reg += SZ_FLOAT - SZ_INT;
      DISPATCH()
    }
    unionother : {
      UNION_CHECK
      PRAGMA_PUSH()
      for (m_uint i = 0; i <= VAL2; i += SZ_INT)
        *(m_uint *)(reg + i - SZ_INT) = *(m_uint *)(data + SZ_INT + i);
      PRAGMA_POP()
      reg += VAL2 - SZ_INT;
      DISPATCH()
    }
    unionaddr : {
      *(m_uint *)(*(M_Object *)(reg - SZ_INT))->data = VAL;
      *(m_bit **)(reg - SZ_INT) =
          &*(m_bit *)((*(M_Object *)(reg - SZ_INT))->data + SZ_INT);
      DISPATCH()
    }
    staticint:
      *(m_uint *)reg = *(m_uint *)VAL;
      reg += SZ_INT;
      DISPATCH()
    staticfloat:
      *(m_float *)reg = *(m_float *)VAL;
      reg += SZ_FLOAT;
      DISPATCH()
    staticother:
      //  LOOP_OPTIM
      //  for(m_uint i = 0; i <= VAL2; i += SZ_INT)
      //    *(m_uint*)(reg+i) = *(m_uint*)((m_bit*)VAL + i);
      memcpy(reg, (m_bit *)VAL, VAL2);
      reg += VAL2;
      DISPATCH()
    upvalueint:
      *(m_uint *)reg = *(m_uint *)(code->closure->data + VAL);
      reg += SZ_INT;
      DISPATCH()
    upvaluefloat:
      *(m_float *)reg = *(m_float *)(code->closure->data + VAL);
      reg += SZ_FLOAT;
      DISPATCH()
    upvalueother:
      memcpy(reg, code->closure->data + VAL, VAL2);
      reg += VAL2;
      DISPATCH()
    upvalueaddr:
      *(m_uint **)reg = (m_uint *)(code->closure->data + VAL);
      reg += SZ_INT;
      DISPATCH()
    dotfunc:
      *(VM_Code *)(reg + (m_uint)VAL2) =
          ((Func)(*(M_Object *)(reg - SZ_INT))->vtable.ptr[OFFSET + VAL])->code;
      DISPATCH()
    gcini:
      vector_add(&shred->gc, 0);
      DISPATCH();
    gcadd:
      vector_add(&shred->gc, *(vtype *)(reg + IVAL));
      DISPATCH();
    gcend : {
      M_Object o;
      while ((o = (M_Object)vector_pop(&shred->gc))) _release(o, shred);
    }
      DISPATCH()
    gacktype : {
      const M_Object o = *(M_Object *)(reg - SZ_INT);
      if (o) *(Type *)reg = o->type_ref;
    }
      DISPATCH()
    gackend : {
      m_str str = *(m_str *)(reg - SZ_INT);
      if (!VAL)
        gw_out("%s\n", str);
      else
        *(M_Object *)(reg - SZ_INT) = new_string(vm->gwion->mp, shred, str);
      if (str) mp_free2(vm->gwion->mp, strlen(str), str);
      DISPATCH();
    }
    gack:
      VM_OUT
      gack(shred, VAL);
      goto in;
    try_ini:
      if (!shred->info->frame.ptr) // ???
        vector_init(&shred->info->frame);
      vector_add(&shred->info->frame, PC);
      vector_add(&shred->info->frame, (m_uint)reg);
      DISPATCH();
    try_end:
      vector_pop(&shred->info->frame);
      vector_pop(&shred->info->frame);
    handleeffect:
      // this should check the *xid* of the exception
      DISPATCH();
    performeffect:
      VM_OUT
      handle(shred, (m_str)VAL);
      break;
    noop:
      DISPATCH();
    other:
      VM_OUT((f_instr)VAL2)(shred, (Instr)VAL);
    unroll2:
    in:
      if (!s->curr) break;
      bytecode = (code = shred->code)->bytecode;
      reg      = shred->reg;
      mem      = shred->mem;
      PC_DISPATCH(shred->pc)
    debugline:
      if (!shred->info->line.ptr) { // from a problem with spork it seems
        vector_init(&shred->info->line);
        vector_add(&shred->info->line, VAL);
      } else if (!VAL) {
        vector_add(&shred->info->line, VAL);
      } else {
        const m_uint sz = vector_size(&shred->info->line);
        vector_set(&shred->info->line, sz - 1, VAL);
      }
      DISPATCH();
    debugvalue:
      DISPATCH();
    debugpush:
      if (!shred->info->line.ptr) vector_init(&shred->info->line);
      vector_add(&shred->info->line, 0);
      DISPATCH();
    debugpop:
      vector_pop(&shred->info->line);
      DISPATCH();
    eoc:
      VM_OUT
      vm_shred_exit(shred);
    } while (s->curr);
    MUTEX_UNLOCK(s->mutex);
  }
}

ANN void next_bbq_pos(const VM *vm) {
  Driver *const di = vm->bbq;
  if(++di->pos == 16777216-1) {
    for(m_uint i = 0; i < vector_size(&vm->shreduler->shreds); i++) {
      const VM_Shred shred = (VM_Shred)vector_at(&vm->shreduler->shreds, i);
      shred->tick->wake_time -= 16777216.0;
    }
    di->pos = 0;
  }
}

static void vm_run_audio(const VM *vm) {
  vm_run(vm);
  vm_ugen_init(vm);
}

VM *new_vm(MemPool p, const bool audio) {
  VM *vm = (VM *)mp_calloc(p, VM);
  vector_init(&vm->ugen);
  vm->bbq       = new_driver(p);
  vm->bbq->run  = audio ? vm_run_audio : vm_run;
  vm->shreduler = (Shreduler)mp_calloc(p, Shreduler);
  vector_init(&vm->shreduler->shreds);
  MUTEX_SETUP(vm->shreduler->mutex);
  vm->shreduler->bbq = vm->bbq;
#ifndef __AFL_COMPILER
  gw_seed(vm->rand, (uint64_t)time(NULL));
#else
  gw_seed(vm->rand, 0);
#endif
  return vm;
}
