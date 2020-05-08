#include <time.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "object.h"
#include "ugen.h"
#include "shreduler_private.h"
#include "emit.h"
#include "gwion.h"
#include "operator.h"
#include "import.h"
#include "map_private.h"
#include "gack.h"
#include "array.h"


static inline uint64_t splitmix64_stateless(uint64_t index) {
  uint64_t z = (index + UINT64_C(0x9E3779B97F4A7C15));
  z = (z ^ (z >> 30)) * UINT64_C(0xBF58476D1CE4E5B9);
  z = (z ^ (z >> 27)) * UINT64_C(0x94D049BB133111EB);
  return z ^ (z >> 31);
}

static inline uint32_t rotl(const uint32_t x, int k) {
  return (x << k) | (x >> (32 -k));
}

void gw_seed(uint32_t rnd[2], const uint64_t s) {
  uint64_t seed = splitmix64_stateless(s);
  memcpy(rnd, &seed, sizeof(uint64_t));
}

/*xoroshiro32** */
uint32_t gw_rand(uint32_t s[2]) {
  const uint32_t s0 = s[0];
  const uint32_t s1 = s[1] ^ s0;
  const uint32_t ret = rotl(s0 * 0x9E3779BB, 5) * 5;
  s[0] = rotl(s0, 26) ^ s1 ^ (s1 << 9);
  s[1] = rotl(s1, 13);
  return ret;
}

void vm_remove(const VM* vm, const m_uint index) {
  const Vector v = (Vector)&vm->shreduler->shreds;
  LOOP_OPTIM
  for(m_uint i = vector_size(v) + 1; --i;) {
    const VM_Shred sh = (VM_Shred)vector_at(v, i - 1);
    if(sh && sh->tick->xid == index)
       Except(sh, "MsgRemove");
  }
}

ANN void free_vm(VM* vm) {
  vector_release(&vm->shreduler->shreds);
  vector_release(&vm->ugen);
  if(vm->bbq)
    free_driver(vm->bbq, vm);
  MUTEX_CLEANUP(vm->shreduler->mutex);
  mp_free(vm->gwion->mp, Shreduler, vm->shreduler);
  mp_free(vm->gwion->mp, VM, vm);
}

ANN void vm_add_shred(const VM* vm, const VM_Shred shred) {
  shred->info->vm = (VM*)vm;
  shred->info->me = new_shred(shred, 1);
  shreduler_add(vm->shreduler, shred);
}

ANN void vm_ini_shred(const VM* vm, const VM_Shred shred) {
  shred->info->vm = (VM*)vm;
  shred->info->me = new_shred(shred, 1);
  shreduler_ini(vm->shreduler, shred);
}

ANN void vm_lock(VM const *vm) {
  if(vm->parent)
    vm_lock(vm->parent);
  MUTEX_LOCK(vm->shreduler->mutex);
}

ANN void vm_unlock(VM const *vm) {
  do MUTEX_UNLOCK(vm->shreduler->mutex);
  while((vm = vm->parent));
}

ANN m_bool vm_running(VM const *vm) {
  if(!vm->shreduler->bbq->is_running)
    return 0;
  if(!vm->parent)
    return 1;
  return vm->shreduler->bbq->is_running = vm_running(vm->parent);
}

__attribute__((hot))
ANN static inline void vm_ugen_init(const VM* vm) {
  const Vector v = (Vector)&vm->ugen;
  LOOP_OPTIM
  for(m_uint i = vector_size(v) + 1; --i;) {
    const UGen u = (UGen)vector_at(v, i - 1);
    u->done = 0;
    if(u->multi) {
      struct ugen_multi_* m = u->connect.multi;
      LOOP_OPTIM
      for(m_uint j = m->n_chan + 1; --j;)
        UGEN(m->channel[j - 1])->done = 0;
    }
  }
  const UGen hole = (UGen)vector_at(v, 0);
  hole->compute(hole);
}

#ifdef DEBUG_STACK
#define VM_INFO                                                              \
  gw_err("shred[%" UINT_F "] mem[%" INT_F"] reg[%" INT_F"]\n", \
  shred->tick->xid, \
  mem - ((m_bit*)shred + sizeof(struct VM_Shred_) + SIZEOF_REG), reg - ((m_bit*)shred + sizeof(struct VM_Shred_)));
#else
#define VM_INFO
#endif

ANN static inline m_bool overflow_(const m_bit* mem, const VM_Shred c) {
  return mem > (((m_bit*)c + sizeof(struct VM_Shred_) + SIZEOF_REG) + (SIZEOF_MEM) - (MEM_STEP*16));
}

ANN static inline VM_Shred init_spork_shred(const VM_Shred shred, const VM_Code code) {
  const VM_Shred sh = new_shred_base(shred, code);
  vm_add_shred(shred->info->vm, sh);
  vector_add(&shred->gc, (vtype)sh->info->me);
  sh->tick->parent = shred->tick;
  if(!shred->tick->child.ptr)
    vector_init(&shred->tick->child);
  vector_add(&shred->tick->child, (vtype)sh);
  return sh;
}

ANN static VM_Shred init_fork_shred(const VM_Shred shred, const VM_Code code, const m_uint retsz) {
  VM* parent = shred->info->vm;
  const VM_Shred sh = new_shred_base(shred, code);
  VM* vm = (sh->info->vm = gwion_cpy(parent));
  vm->parent = parent;
  const M_Object o = sh->info->me = new_shred(sh, 0);
  ++sh->info->me->ref;
  if(!parent->gwion->data->child.ptr)
    vector_init(&parent->gwion->data->child);
  vector_add(&parent->gwion->data->child, (vtype)o);
  shreduler_add(vm->shreduler, sh);
  fork_launch(sh->info->me, retsz);
  return sh;
}

#define TEST0(t, pos) if(!*(t*)(reg-pos)){ shred->pc = PC; exception(shred, "ZeroDivideException"); break; }

#define ADVANCE() byte += BYTECODE_SZ;

#define SDISPATCH() goto *dispatch[*(m_bit*)byte];
#define IDISPATCH() { VM_INFO; SDISPATCH(); }

#define SET_BYTE(pc)	(byte = bytecode + (pc) * BYTECODE_SZ)

#define PC_DISPATCH(pc)\
  SET_BYTE((pc));\
  IDISPATCH();

#define DISPATCH()\
  ADVANCE();\
  IDISPATCH();


#define ADVANCE() byte += BYTECODE_SZ;

#define ADISPATCH() { ADVANCE(); SDISPATCH(); }

#define PC ((*(unsigned short*)(byte + 2)) + 1)

#define OP(t, sz, op, ...) \
  reg -= sz;\
  __VA_ARGS__\
  *(t*)(reg - sz) op##= *(t*)reg;\
  DISPATCH();

#define INT_OP(op, ...) OP(m_int, SZ_INT, op, __VA_ARGS__)
#define FLOAT_OP(op, ...) OP(m_float, SZ_FLOAT, op, __VA_ARGS__)

#define LOGICAL(t, sz0, sz, op)\
reg -= sz0;\
*(m_int*)(reg-SZ_INT) = (*(t*)(reg - SZ_INT) op *(t*)(reg+sz));\
DISPATCH()

#define INT_LOGICAL(op) LOGICAL(m_int, SZ_INT, 0, op)

#define FLOAT_LOGICAL(op) LOGICAL(m_float, SZ_FLOAT * 2 - SZ_INT, \
  SZ_FLOAT - SZ_INT, op)

#define SELF(t, sz,op) \
  *(t*)(reg - sz) = op*(t*)(reg - sz);\
  DISPATCH();

// check me
#define R(t, sz, op, ...) \
reg -= SZ_INT;\
__VA_ARGS__\
*(t*)(reg-sz) = (**(t**)reg op##= (*(t*)(reg-sz)));\
DISPATCH()
#define INT_R(op, ...) R(m_int, SZ_INT, op, __VA_ARGS__)
#define FLOAT_R(op, ...) R(m_float, SZ_FLOAT, op)

#define INT_PRE(op) \
/*assert(*(m_int**)(reg-SZ_INT));*/\
*(m_int*)(reg- SZ_INT) = op(**(m_int**)(reg-SZ_INT));\
DISPATCH()

#define INT_POST(op) \
/*assert(*(m_int**)(reg-SZ_INT));*/\
*(m_int*)(reg- SZ_INT) = (**(m_int**)(reg-SZ_INT))op;\
DISPATCH()

#define IF_OP(op) \
  reg -=SZ_INT;\
    *(m_float*)(reg-SZ_FLOAT) = (m_float)*(m_int*)(reg-SZ_FLOAT) op \
    *(m_float*)(reg + SZ_INT - SZ_FLOAT); \
  DISPATCH()

#define IF_LOGICAL(op)\
  reg -= SZ_FLOAT; \
  *(m_int*)(reg-SZ_INT) = (m_int)(*(m_int*)(reg-SZ_INT) op *(m_float*)reg); \
  DISPATCH()
__attribute__((hot))

#define IF_R(op) \
  reg -= SZ_INT * 2 - SZ_FLOAT; \
  *(m_float*)(reg-SZ_FLOAT) = (**(m_float**)(reg +SZ_INT - SZ_FLOAT) op##= \
    (m_float)*(m_int*)(reg-SZ_FLOAT)); \
  DISPATCH()

#define FI_OP(op)\
  reg -= SZ_INT; \
  *(m_float*)(reg-SZ_FLOAT) op##= (m_float)*(m_int*)reg; \
  DISPATCH()

#define FI_LOGICAL(op) \
  reg -= SZ_FLOAT; \
  *(m_int*)(reg-SZ_INT) = (m_int)(*(m_float*)(reg-SZ_INT) op\
    *(m_int*)(reg + SZ_FLOAT-SZ_INT)); \
  DISPATCH()

#define FI_R(op, ...) \
  reg -= SZ_FLOAT; \
  __VA_ARGS__ \
  *(m_int*)(reg-SZ_INT) = (**(m_int**)(reg+SZ_FLOAT -SZ_INT) op##= \
    /*(m_int)*/(*(m_float*)(reg-SZ_INT))); \
  DISPATCH()


#define STRINGIFY_NX(a) #a
#define STRINGIFY(a) STRINGIFY_NX(a)
#define PPCAT_NX(A, B) A ## B
#define PPCAT(A, B) PPCAT_NX(A, B)

#if defined(__clang__)
#define COMPILER clang
#define UNINITIALIZED "-Wuninitialized")
#elif defined(__GNUC__) || defined(__GNUG__)
#define COMPILER GCC
#define UNINITIALIZED "-Wmaybe-uninitialized")
#endif

#define PRAGMA_PUSH() \
_Pragma(STRINGIFY(COMPILER diagnostic push)) \
_Pragma(STRINGIFY(COMPILER diagnostic ignored UNINITIALIZED)
#define PRAGMA_POP() _Pragma(STRINGIFY(COMPILER diagnostic pop)) \

#define VAL (*(m_uint*)(byte + SZ_INT))
#define FVAL (*(m_float*)(byte + SZ_INT))
#define VAL2 (*(m_uint*)(byte + SZ_INT*2))

#define BRANCH_DISPATCH(check) \
  if(check) SET_BYTE(VAL);\
  else ADVANCE(); \
  IDISPATCH();

#define VM_OUT shred->code = code; shred->reg = reg; shred->mem = mem; shred->pc = PC;

__attribute__ ((hot, optimize("-O2")))
ANN void vm_run(const VM* vm) { // lgtm [cpp/use-of-goto]
  static const void* dispatch[] = {
    &&regsetimm,
    &&regpushimm, &&regpushfloat, &&regpushother, &&regpushaddr,
    &&regpushmem, &&regpushmemfloat, &&regpushmemother, &&regpushmemaddr, &&regpushmemderef,
    &&pushnow,
    &&baseint, &&basefloat, &&baseother, &&baseaddr,
    &&regtoreg, &&regtoregaddr, &&regtoregderef,
    &&structmember, &&structmemberaddr,
    &&memsetimm,
    &&regpushme, &&regpushmaybe,
    &&funcreturn,
    &&_goto,
    &&allocint, &&allocfloat, &&allocother,
    &&intplus, &&intminus, && intmul, &&intdiv, &&intmod,
    // int relationnal
    &&inteq, &&intne, &&intand, &&intor,
    &&intgt, &&intge, &&intlt, &&intle,
    &&intsl, &&intsr, &&intsand, &&intsor, &&intxor,
    &&intnegate, &&intnot, &&intcmp,
    &&intrassign,
    &&intradd, &&intrsub, &&intrmul, &&intrdiv, &&intrmod,
    &&intrsl, &&intrsr, &&intrsand, &&intrsor, &&intrxor,
    &&preinc, &&predec,
    &&postinc, &&postdec,
    &&floatadd, &&floatsub, &&floatmul, &&floatdiv,
// logical
    &&floatand, &&floator, &&floateq, &&floatne,
    &&floatgt, &&floatge, &&floatlt, &&floatle,
    &&floatneg, &&floatnot,
    &&floatrassign, &&floatradd, &&floatrsub, &&floatrmul, &&floatrdiv,
    &&ifadd, &&ifsub, &&ifmul, &&ifdiv,
    &&ifand, &&ifor, &&ifeq, &&ifne, &&ifgt, &&ifge, &&iflt, &&ifle,
    &&ifrassign, &&ifradd, &&ifrsub, &&ifrmul, &&ifrdiv,
    &&fiadd, &&fisub, &&fimul, &&fidiv,
    &&fiand, &&fior, &&fieq, &&fine, &&figt, &&fige, &&filt, &&file,
    &&firassign, &&firadd, &&firsub, &&firmul, &&firdiv,
    &&itof, &&ftoi,
    &&timeadv,
    &&setcode,
    &&regpop, &&regpush, &&regtomem, &&regtomemother, &&overflow, &&funcusrend, &&funcmemberend,
    &&sporkini, &&forkini, &&sporkfunc, &&sporkmemberfptr, &&sporkexp, &&sporkend,
    &&brancheqint, &&branchneint, &&brancheqfloat, &&branchnefloat,
    &&arrayappend, &&autoloop, &&autoloopptr, &&autoloopcount, &&arraytop, &&arrayaccess, &&arrayget, &&arrayaddr, &&arrayvalid,
    &&newobj, &&addref, &&addrefaddr, &&objassign, &&assign, &&remref,
    &&setobj, &&except, &&allocmemberaddr, &&dotmember, &&dotfloat, &&dotother, &&dotaddr,
    &&staticint, &&staticfloat, &&staticother,
    &&dotfunc, &&dotstaticfunc, &&pushstaticcode,
    &&gcini, &&gcadd, &&gcend,
    &&gacktype, &&gackend, &&gack, &&noop, &&regpushimm, &&other, &&eoc
  };
  const Shreduler s = vm->shreduler;
  register VM_Shred shred;
  register m_bit next;

  while((shred = shreduler_get(s))) {
    register VM_Code code = shred->code;
    register m_bit* bytecode = code->bytecode;
    register m_bit* byte = bytecode + shred->pc * BYTECODE_SZ;
    register m_bit* reg = shred->reg;
    register m_bit* mem = shred->mem;
    register union {
      M_Object obj;
      VM_Code code;
    } a;
PRAGMA_PUSH()
    register VM_Shred child;
PRAGMA_POP()
  MUTEX_LOCK(s->mutex);
  do {
    SDISPATCH();
regsetimm:
  *(m_uint*)(reg + (m_int)VAL2) = VAL;
  DISPATCH();
regpushimm:
  *(m_uint*)reg = VAL;
  reg += SZ_INT;
  DISPATCH();
regpushfloat:
  *(m_float*)reg = FVAL;
  reg += SZ_FLOAT;
  DISPATCH();
regpushother:
//  LOOP_OPTIM
  for(m_uint i = 0; i <= VAL2; i+= SZ_INT)
    *(m_bit**)(reg+i) = (m_bit*)(VAL + i);
  reg += VAL2;
  DISPATCH();
regpushaddr:
  *(m_uint**)reg =  &VAL;
  reg += SZ_INT;
  DISPATCH()
regpushmem:
  *(m_uint*)reg = *(m_uint*)(mem + (m_int)VAL);
  reg += SZ_INT;
  DISPATCH();
regpushmemfloat:
  *(m_float*)reg = *(m_float*)(mem + (m_int)VAL);
  reg += SZ_FLOAT;
  DISPATCH();
regpushmemother:
  for(m_uint i = 0; i <= VAL2; i+= SZ_INT)
    *(m_uint*)(reg+i) = *(m_uint*)((m_bit*)(mem + (m_int)VAL) + i);
  reg += VAL2;
  DISPATCH();
regpushmemaddr:
  *(m_bit**)reg = &*(m_bit*)(mem + (m_int)VAL);
  reg += SZ_INT;
  DISPATCH()
regpushmemderef:
  memcpy(reg, *(m_uint**)(mem+(m_int)VAL), VAL2);
  reg += VAL2;
  DISPATCH()
pushnow:
  *(m_float*)reg = vm->bbq->pos;
  reg += SZ_FLOAT;
  DISPATCH();
baseint:
  *(m_uint*)reg = *(m_uint*)(shred->base + VAL);
  reg += SZ_INT;
  DISPATCH();
basefloat:
  *(m_float*)reg = *(m_float*)(shred->base + VAL);
  reg += SZ_FLOAT;
  DISPATCH();
baseother:
//  LOOP_OPTIM
  for(m_uint i = 0; i <= VAL2; i+= SZ_INT)
    *(m_uint*)(reg+i) = *(m_uint*)((shred->base + VAL) + i);
  reg += VAL2;
  DISPATCH();
baseaddr:
  *(m_uint**)reg = &*(m_uint*)(shred->base + (m_int)VAL);
  reg += SZ_INT;
  DISPATCH();
regtoreg:
  *(m_uint*)(reg + (m_int)VAL) = *(m_uint*)(reg + (m_int)VAL2);
  DISPATCH()
regtoregaddr:
  *(m_uint**)(reg + (m_int)VAL) = &*(m_uint*)(reg + (m_int)VAL2);
  DISPATCH()
regtoregderef:
  memcpy(*(m_bit**)(reg - SZ_INT), *(m_bit**)(reg + (m_int)VAL), VAL2);
  DISPATCH()
structmember:
  *(m_bit**)(reg-SZ_INT) =  *(m_bit**)(*(m_bit**)(reg-SZ_INT) + (m_int)VAL2);
  DISPATCH()
structmemberaddr:
  *(m_bit**)(reg-SZ_INT) =  &*(*(m_bit**)(reg-SZ_INT) + (m_int)VAL2);
  DISPATCH()
memsetimm:
  *(m_uint*)(mem+VAL) = VAL2;
  DISPATCH();
regpushme:
  *(M_Object*)reg = shred->info->me;
  reg += SZ_INT;
  DISPATCH()
regpushmaybe:
  *(m_uint*)reg = gw_rand((uint32_t*)vm->rand) > (UINT32_MAX / 2);
  reg += SZ_INT;
  DISPATCH();
funcreturn:
{
  register const m_uint pc = *(m_uint*)(mem-SZ_INT*2);
  bytecode = (code = *(VM_Code*)(mem-SZ_INT*3))->bytecode;
  mem -= (*(m_uint*)(mem-SZ_INT*4) + SZ_INT*4);
  PC_DISPATCH(pc);
}
_goto:
  PC_DISPATCH(VAL);
allocint:
  *(m_uint*)reg = *(m_uint*)(mem+VAL) = 0;
  reg += SZ_INT;
  DISPATCH()
allocfloat:
  *(m_float*)reg = *(m_float*)(mem+VAL) = 0;
  reg += SZ_FLOAT;
  DISPATCH()
allocother:
//  LOOP_OPTIM
  for(m_uint i = 0; i <= VAL; i += SZ_INT)
    *(m_uint*)(reg+i) = (*(m_uint*)(mem+VAL+i) = 0);
  reg += VAL2;
  DISPATCH()

intplus:  INT_OP(+)
intminus: INT_OP(-)
intmul:   INT_OP(*)
intdiv:   INT_OP(/, TEST0(m_int, 0))
intmod:   INT_OP(%, TEST0(m_int, 0))

inteq:   INT_LOGICAL(==)
intne:   INT_LOGICAL(!=)
intand:  INT_LOGICAL(&&)
intor:   INT_LOGICAL(||)
intgt:   INT_LOGICAL(>)
intge:   INT_LOGICAL(>=)
intlt:   INT_LOGICAL(<)
intle:   INT_LOGICAL(<=)
intsl:   INT_LOGICAL(<<)
intsr:   INT_LOGICAL(>>)
intsand: INT_LOGICAL(&)
intsor:  INT_LOGICAL(|)
intxor:  INT_LOGICAL(^)

intnegate:
  *(m_int*)(reg - SZ_INT) *= -1;
  DISPATCH()
intnot: SELF(m_int, SZ_INT, !)
intcmp: SELF(m_int, SZ_INT, ~)

intrassign:
  reg -= SZ_INT;
  **(m_int**)reg = *(m_int*)(reg-SZ_INT);
  DISPATCH()

intradd: INT_R(+)
intrsub: INT_R(-)
intrmul: INT_R(*)
//intrdiv: INT_R(/, TEST0(m_int, -SZ_INT))
//intrmod: INT_R(%, TEST0(m_int, -SZ_INT))
intrdiv: INT_R(/, TEST0(m_int, SZ_INT))
intrmod: INT_R(%, TEST0(m_int, SZ_INT))
intrsl: INT_R(<<)
intrsr: INT_R(>>)
intrsand: INT_R(&)
intrsor: INT_R(|)
intrxor: INT_R(^)

preinc: INT_PRE(++)
predec: INT_PRE(--)

postinc: INT_POST(++)
postdec: INT_POST(--)

floatadd: FLOAT_OP(+)
floatsub: FLOAT_OP(-)
floatmul: FLOAT_OP(*)
floatdiv: FLOAT_OP(/)

floatand: FLOAT_LOGICAL(&&)
floator: FLOAT_LOGICAL(||)
floateq: FLOAT_LOGICAL(==)
floatne: FLOAT_LOGICAL(!=)
floatgt: FLOAT_LOGICAL(>)
floatge: FLOAT_LOGICAL(>=)
floatlt: FLOAT_LOGICAL(<)
floatle: FLOAT_LOGICAL(<=)

floatneg: SELF(m_float, SZ_FLOAT, -)

floatnot:
  reg -= SZ_FLOAT - SZ_INT;
  *(m_int*)(reg - SZ_INT) = !*(m_float*)(reg - SZ_INT);
  DISPATCH()

floatrassign:
  reg -= SZ_INT;
  **(m_float**)reg = *(m_float*)(reg-SZ_FLOAT);
  DISPATCH()

floatradd: FLOAT_R(+)
floatrsub: FLOAT_R(-)
floatrmul: FLOAT_R(*)
floatrdiv: FLOAT_R(/)

ifadd: IF_OP(+)
ifsub: IF_OP(-)
ifmul: IF_OP(*)
ifdiv: IF_OP(/)

ifand: IF_LOGICAL(&&)
ifor: IF_LOGICAL(||)
ifeq: IF_LOGICAL(==)
ifne: IF_LOGICAL(!=)
ifgt: IF_LOGICAL(>)
ifge: IF_LOGICAL(>=)
iflt: IF_LOGICAL(<)
ifle: IF_LOGICAL(<=)

ifrassign: IF_R()
ifradd: IF_R(+)
ifrsub: IF_R(-)
ifrmul: IF_R(*)
ifrdiv: IF_R(/)

fiadd: FI_OP(+)
fisub: FI_OP(-)
fimul: FI_OP(*)
fidiv: FI_OP(/)

fiand: FI_LOGICAL(&&)
fior: FI_LOGICAL(||)
fieq: FI_LOGICAL(==)
fine: FI_LOGICAL(!=)
figt: FI_LOGICAL( >)
fige: FI_LOGICAL(>=)
filt: FI_LOGICAL( <)
file: FI_LOGICAL(<=)

firassign:
  reg -=SZ_FLOAT;
  *(m_int*)(reg-SZ_INT) = **(m_int**)(reg + SZ_FLOAT-SZ_INT) =
    (m_int)*(m_float*)(reg-SZ_INT);
  DISPATCH()

firadd: FI_R(+)
firsub: FI_R(-)
firmul: FI_R(*)
firdiv: FI_R(/, TEST0(m_float, SZ_INT))

itof:
  reg -= SZ_INT - SZ_FLOAT;
  *(m_float*)(reg-SZ_FLOAT) = (m_float)*(m_int*)(reg-SZ_FLOAT);
  DISPATCH()
ftoi:
  reg -= SZ_FLOAT - SZ_INT;
  *(m_int*)(reg-SZ_INT) = (m_int)*(m_float*)(reg-SZ_INT);
  DISPATCH()
timeadv:
  reg -= SZ_FLOAT;
  shredule(s, shred, *(m_float*)(reg-SZ_FLOAT));
  *(m_float*)(reg-SZ_FLOAT) += vm->bbq->pos;
  VM_OUT
  break;
setcode:
PRAGMA_PUSH()
  reg -= SZ_INT;
  a.code = *(VM_Code*)reg;
  if(!GET_FLAG((VM_Code)a.code, builtin)) {
    register const m_uint push = *(m_uint*)(reg + SZ_INT) + *(m_uint*)(mem-SZ_INT);
    mem += push;
    *(m_uint*)  mem = push;mem += SZ_INT;
    *(VM_Code*) mem = code; mem += SZ_INT;
    *(m_uint*)  mem = PC + VAL2; mem += SZ_INT;
    *(m_uint*) mem = a.code->stack_depth; mem += SZ_INT;
    next = eFuncUsrEnd;
  } else {
    mem += *(m_uint*)(reg + SZ_INT);
    next = eFuncMemberEnd;
  }
PRAGMA_POP()
regpop:
  reg -= VAL;
  DISPATCH();
regpush:
  reg += VAL;
  DISPATCH();
regtomem:
  *(m_uint*)(mem+VAL) = *(m_uint*)(reg+(m_int)VAL2);
  DISPATCH()
regtomemother:
  memcpy(mem+VAL, reg, VAL2);
  DISPATCH()
overflow:
  if(overflow_(mem, shred)) {
    shred->pc = PC;
    exception(shred, "StackOverflow");
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
funcmemberend:
  VM_OUT
  {
    register const m_uint val = VAL;
    register const m_uint val2 = VAL2;
    ((f_mfun)a.code->native_func)((*(M_Object*)mem), reg, shred);
    reg += val;
    shred->mem = (mem -= val2);
    if(!s->curr)break;
  }
  PC_DISPATCH(shred->pc)
sporkini:
  child = init_spork_shred(shred, (VM_Code)VAL);
  DISPATCH()
forkini:
  child = init_fork_shred(shred, (VM_Code)VAL, VAL2),
  DISPATCH()
sporkfunc:
//  LOOP_OPTIM
PRAGMA_PUSH()
  for(m_uint i = 0; i < VAL; i+= SZ_INT)
    *(m_uint*)(child->reg + i) = *(m_uint*)(reg + i + (m_int)VAL2);
  child->reg += VAL;
  DISPATCH()
PRAGMA_POP()
sporkmemberfptr:
  for(m_uint i = 0; i < VAL; i+= SZ_INT)
    *(m_uint*)(child->reg + i) = *(m_uint*)(reg - VAL + i);
  *(M_Object*)(child->reg + VAL) = a.obj;
  *(m_uint*)(child->reg + VAL + SZ_INT) = *(m_uint*)(reg + VAL - SZ_INT*2);
  child->reg += VAL + SZ_INT*2;
  DISPATCH()
sporkexp:
//  LOOP_OPTIM
  for(m_uint i = 0; i < VAL; i+= SZ_INT)
    *(m_uint*)(child->mem + i) = *(m_uint*)(mem+i);
  DISPATCH()
sporkend:
  assert(!VAL); // spork are not mutable
  *(M_Object*)(reg-SZ_INT) = child->info->me;
  DISPATCH()
brancheqint:
  reg -= SZ_INT;
  BRANCH_DISPATCH(!*(m_uint*)reg);
branchneint:
  reg -= SZ_INT;
  BRANCH_DISPATCH(*(m_uint*)reg);
brancheqfloat:
  reg -= SZ_FLOAT;
  BRANCH_DISPATCH(!*(m_float*)reg);
branchnefloat:
  reg -= SZ_FLOAT;
  BRANCH_DISPATCH(*(m_float*)reg);
arrayappend:
  m_vector_add(ARRAY(*(M_Object*)(reg-SZ_INT)), reg);
  release(*(M_Object*)(reg-SZ_INT), shred);
  DISPATCH()
autoloop:
//  m_vector_get(ARRAY(a.obj), *(m_uint*)(mem + VAL), mem + VAL + SZ_INT);
  m_vector_get(ARRAY(*(M_Object*)(reg-SZ_INT)), *(m_uint*)(mem + VAL), mem + VAL + SZ_INT);
  goto autoloopcount;
autoloopptr:
  *(m_bit**)(*(M_Object*)(mem + VAL + SZ_INT))->data = m_vector_addr(ARRAY(*(M_Object*)(reg-SZ_INT)), *(m_uint*)(mem + VAL));
//  *(m_bit**)(*(M_Object*)(mem + VAL + SZ_INT))->data = m_vector_addr(ARRAY(a.obj), *(m_uint*)(mem + VAL));
autoloopcount:
  *(m_uint*)reg = m_vector_size(ARRAY(*(M_Object*)(reg-SZ_INT))) - (*(m_uint*)(mem + VAL))++;
//  *(m_uint*)reg = m_vector_size(ARRAY(a.obj)) - (*(m_uint*)(mem + VAL))++;
  reg += SZ_INT;
  DISPATCH()
arraytop:
  if(*(m_uint*)(reg - SZ_INT * 2) < *(m_uint*)(reg-SZ_INT))
    goto newobj;
  else
    goto _goto;
arrayaccess:
{
  register const m_int idx = *(m_int*)(reg + SZ_INT * VAL);
  if(idx < 0 || (m_uint)idx >= m_vector_size(ARRAY(a.obj))) {
//  if(idx < 0 || (m_uint)idx >= m_vector_size(ARRAY(*(M_Object*)(reg-SZ_INT)))) {
    gw_err(_("  ... at index [%" INT_F "]\n"), idx);
    gw_err(_("  ... at dimension [%" INT_F "]\n"), VAL);
    VM_OUT
    exception(shred, "ArrayOutofBounds");
    continue; // or break ?
  }
  DISPATCH()
}
arrayget:
  m_vector_get(ARRAY(a.obj), *(m_int*)(reg + SZ_INT * VAL), (reg + (m_int)VAL2));
  DISPATCH()
arrayaddr:
  *(m_bit**)(reg + (m_int)VAL2) = m_vector_addr(ARRAY(a.obj), *(m_int*)(reg + SZ_INT * VAL));
  DISPATCH()
arrayvalid:
// are we sure it is the array ?
// rather increase ref
  vector_pop(&shred->gc);
  goto regpush;
newobj:
  *(M_Object*)reg = new_object(vm->gwion->mp, shred, (Type)VAL2);
  reg += SZ_INT;
  DISPATCH()
addref:
  a.obj = *((M_Object*)(reg+(m_int)VAL) + (m_int)VAL2);
  goto addrefcommon;
addrefaddr:
  a.obj = *(*(M_Object**)(reg+(m_int)VAL) + (m_int)VAL2);
addrefcommon:
  if(a.obj)
    ++a.obj->ref;
  DISPATCH()
objassign:
  a.obj = **(M_Object**)(reg -SZ_INT);
  if(a.obj) {
    --a.obj->ref;
    _release(a.obj, shred);
  }
assign:
  reg -= SZ_INT;
  **(M_Object**)reg = *(M_Object*)(reg-SZ_INT);
  DISPATCH()
remref:
  release(*(M_Object*)(mem + VAL), shred);
  DISPATCH()
setobj:
  a.obj  = *(M_Object*)(reg-SZ_INT-(m_int)VAL);
  DISPATCH();
except:
/* TODO: Refactor except instruction             *
 * so that                                       *
 *  VAL = offset (no default SZ_INT)             *
 *  VAL2 = error message                         *
 * grep for GWOP_EXCEPT and Except, exception... */
  if(!(a.obj  = *(M_Object*)(reg-SZ_INT-VAL))) {
    shred->pc = PC;
    exception(shred, "NullPtrException");
    continue;
  }
  DISPATCH();
allocmemberaddr:
  a.obj = *(M_Object*)mem;
  *(m_bit**)reg = a.obj->data + VAL;
  reg += SZ_INT;
  DISPATCH()
dotmember:
  *(m_uint*)(reg-SZ_INT) = *(m_uint*)(a.obj->data + VAL);
  DISPATCH()
dotfloat:
  *(m_float*)(reg-SZ_INT) = *(m_float*)(a.obj->data + VAL);
  reg += SZ_FLOAT - SZ_INT;
  DISPATCH()
dotother:
//  LOOP_OPTIM
PRAGMA_PUSH()
  for(m_uint i = 0; i <= VAL2; i += SZ_INT)
    *(m_uint*)(reg+i-SZ_INT) = *(m_uint*)((a.obj->data + VAL) + i);
PRAGMA_POP()
  reg += VAL2 - SZ_INT;
  DISPATCH()
dotaddr:
  *(m_bit**)(reg-SZ_INT) = (a.obj->data + VAL);
  DISPATCH()
staticint:
  *(m_uint*)reg = *(m_uint*)VAL;
  reg += SZ_INT;
  DISPATCH()
staticfloat:
  *(m_float*)reg = *(m_float*)VAL;
  reg += SZ_FLOAT;
  DISPATCH()
staticother:
//  LOOP_OPTIM
//  for(m_uint i = 0; i <= VAL2; i += SZ_INT)
//    *(m_uint*)(reg+i) = *(m_uint*)((m_bit*)VAL + i);
  memcpy(reg, (m_bit*)VAL, VAL2);
  reg += VAL2;
  DISPATCH()
dotfunc:
  assert(a.obj);
  reg += SZ_INT;
dotstaticfunc:
PRAGMA_PUSH()
  *(VM_Code*)(reg-SZ_INT) = ((Func)vector_at(a.obj->vtable, VAL))->code;
PRAGMA_POP()
  DISPATCH()
pushstaticcode:
  *(m_bit*)byte = eRegSetImm;
  VAL = (*(m_uint*)(reg-SZ_INT) = (m_uint)((Func)VAL)->code);
  VAL2 = -SZ_INT;
  DISPATCH()
gcini:
  vector_add(&shred->gc, 0);
  DISPATCH();
gcadd:
  vector_add(&shred->gc, *(vtype*)(reg-SZ_INT));
  DISPATCH();
gcend:
  while((a.obj = (M_Object)vector_pop(&shred->gc)))
    _release(a.obj, shred);
  DISPATCH()
gacktype:
{
  const M_Object o = *(M_Object*)(reg - SZ_INT);
  if(o)
    *(Type*)reg = o->type_ref;
  DISPATCH()
}
gackend:
{
  m_str str = *(m_str*)(reg - SZ_INT);
  if(!VAL)
    gw_out("%s\n", str);
  else
    *(M_Object*)(reg - SZ_INT)= new_string(vm->gwion->mp, shred, str);
  if(str)
    mp_free2(vm->gwion->mp, strlen(str), str);
  DISPATCH();
}
gack:
  VM_OUT
  gack(shred, (Instr)VAL);
  goto in;
noop:
  DISPATCH();
other:
  VM_OUT
  ((f_instr)VAL2)(shred, (Instr)VAL);
in:
  if(!s->curr)
    break;
  bytecode = (code = shred->code)->bytecode;
  reg = shred->reg;
  mem = shred->mem;
  PC_DISPATCH(shred->pc)
eoc:
  VM_OUT
  vm_shred_exit(shred);
    } while(s->curr);
  MUTEX_UNLOCK(s->mutex);
  }
}

static void vm_run_audio(const VM *vm) {
  vm_run(vm);
  vm_ugen_init(vm);
}

VM* new_vm(MemPool p, const m_bool audio) {
  VM* vm = (VM*)mp_calloc(p, VM);
  vector_init(&vm->ugen);
  vm->bbq = new_driver(p);
  vm->bbq->run = audio ? vm_run_audio : vm_run;
  vm->shreduler  = (Shreduler)mp_calloc(p, Shreduler);
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
