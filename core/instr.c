#include <math.h>
#include <string.h>
#include "defs.h"
#include "err_msg.h"
#include "instr.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#define overflow_(c)       ( c >  ( c + (0x1 << SIZEOF_MEM)) - ((0x1 << SIZEOF_MEM) / MEM_STEP))

INSTR(EOC)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Shred [%i]: End of Code", shred->xid);
#endif
  shred->is_running = 0;
  shred->is_done = 1;
}
/* stacking */
INSTR(Reg_Pop_Word4)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] pop %i", instr->m_val);
#endif
  POP_REG(shred,  instr->m_val);
}

INSTR(Reg_Push_Imm)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push imm %p %i", (void*)instr->m_val, instr->m_val2);
#endif
  *(m_uint*)(shred->reg) = instr->m_val;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Push_Imm2)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push imm2 %f", instr->f_val);
#endif
  *(m_float*)(shred->reg) = instr->f_val;
  PUSH_REG(shred,  SZ_FLOAT);
}

INSTR(Reg_Push_ImmC)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push imm complex");
#endif
  POP_REG(shred,  SZ_FLOAT * 2);
  *(m_complex*)(shred->reg) = ((*(m_float*)shred->reg) + (*(m_float*)(shred->reg + SZ_FLOAT)) * I);
  PUSH_REG(shred,  SZ_COMPLEX);
}

INSTR(Reg_Push_Mem_Addr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push mem addr %i %i", instr->m_val, instr->m_val2);
#endif
  *(m_uint**)shred->reg = (m_uint*)((instr->m_val2 ? shred->base : shred->mem) + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Mem_Push_Imm)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[mem] push imm %i %i", instr->m_val, instr->m_val2);
#endif
  *(m_uint*)shred->mem = instr->m_val;
  PUSH_MEM(shred,  SZ_INT);
}

// test on 13/01/17
INSTR(Mem_Push_Ret)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[mem] push ret to mem[%u] %p", instr->m_val, (void*) * (M_Object*)(shred->reg - SZ_INT));
#endif
  *(M_Object*)(shred->mem + instr->m_val) = *(M_Object*)(shred->reg - SZ_INT);
}

INSTR(Mem_Set_Imm)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[mem] set imm [%i] %p", instr->m_val, instr->ptr);
#endif
  *(m_uint**)(shred->mem + instr->m_val) = (m_uint*)instr->ptr;
}
// template erase
INSTR(Free_Func)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "free template func '%p'", (void*)instr->m_val);
#endif
  Func f = (Func)instr->m_val;
//  if(!f->value_ref->owner_class)
//    free(f->def);

  free(f->value_ref->name);
  free(f->value_ref->m_type->name);
  free(f->value_ref->m_type->obj);
  free(f->value_ref->m_type);
  rem_ref(f->value_ref->obj, f->value_ref);
  free_VM_Code(f->code);
//f->def = NULL;
//  free_Func(f);
}

// func pointer
INSTR(assign_func)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "assign func");
#endif
  POP_REG(shred,  SZ_INT * 2);
  *(m_uint*)shred->reg = (**(m_uint**)(shred->reg + SZ_INT) = *(m_uint*)shred->reg);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Push_Mem)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] reg push '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(instr->m_val2) // global
    *(m_uint*)shred->reg = *(m_uint*)(shred->base + instr->m_val);
  else
    *(m_uint*)shred->reg = *(m_uint*)(shred->mem  + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Push_Mem2)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] reg push float '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(instr->m_val2) // global
    *(m_float*)shred->reg = *(m_float*)(shred->base + instr->m_val);
  else
    *(m_float*)shred->reg = *(m_float*)(shred->mem  + instr->m_val);
  PUSH_REG(shred,  SZ_FLOAT);
}

INSTR(Reg_Push_Mem_Complex)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] 'complex' push mem '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(instr->m_val2)
    *(m_complex*)shred->reg = *(m_complex*)(shred->base + instr->m_val);
  else
    *(m_complex*)shred->reg = *(m_complex*)(shred->mem  + instr->m_val);
  PUSH_REG(shred, SZ_COMPLEX);
}

INSTR(Reg_Push_Mem_Vec3)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] 'vec3' push mem '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(instr->m_val2)
    *(VEC3_T*)shred->reg = *(VEC3_T*)(shred->base + instr->m_val);
  else
    *(VEC3_T*)shred->reg = *(VEC3_T*)(shred->mem  + instr->m_val);
  PUSH_REG(shred, SZ_VEC3);
}

INSTR(Reg_Push_Mem_Vec4)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] 'vec4' push mem '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(instr->m_val2) // global
    *(VEC4_T*)shred->reg = *(VEC4_T*)(shred->base + instr->m_val);
  else
    *(VEC4_T*)shred->reg = *(VEC4_T*)(shred->mem  + instr->m_val);
  PUSH_REG(shred, SZ_VEC4);
}

INSTR(Reg_Push_Ptr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push ptr (%p)", (void*)instr->ptr);
#endif
  *(m_uint*)(shred->reg - SZ_INT) = (m_uint)instr->ptr;
}

INSTR(Reg_Push_Code)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push code [%i]", instr->m_val);
#endif
  Func f = *(Func*)(shred->mem + instr->m_val);
  *(VM_Code*)(shred->reg - SZ_INT) = f->code;
}

INSTR(Reg_Dup_Last)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] dup last %p", (void*) * (m_uint*)(shred->reg  - SZ_INT));
#endif
  *(m_uint*)shred->reg = *(m_uint*)(shred->reg  - SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_AddRef_Object3)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] add ref %i %p", instr->m_val, instr->m_val ? **(M_Object**)(shred->reg - SZ_INT) : (void*) * (M_Object*)(shred->reg - SZ_INT));
#endif
  M_Object obj = instr->m_val ? **(M_Object**)(shred->reg - SZ_INT) : *(M_Object*)(shred->reg - SZ_INT);
  if(obj)
    obj->ref++;
}

INSTR(Reg_Push_Me)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push me %p", (void*)shred->me);
#endif
  *(M_Object*)shred->reg = shred->me;
  PUSH_REG(shred, SZ_INT);
}

INSTR(Reg_Push_Now)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push now %f", get_now(vm->shreduler));
#endif
  *(m_float*)shred->reg = get_now(vm->shreduler);
  PUSH_REG(shred, SZ_FLOAT);
}

INSTR(Reg_Push_Maybe)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push maybe");
#endif
  *(m_uint*)shred->reg = (sp_rand(shred->vm_ref->bbq->sp) > (RAND_MAX / 2)) ? 1 : 0;
  PUSH_REG(shred, SZ_INT);
}

/* alloc */
INSTR(Alloc_Word)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc_word '%s' (%p)[%i]", instr->m_val2 ? "base" : "mem", (void*) * (m_uint*)(shred->mem + instr->m_val), instr->m_val);
#endif
  *(m_uint*)(shred->mem + instr->m_val) = 0; // since template
  *(m_uint**)shred->reg = &*(m_uint*)(shred->mem + instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

INSTR(Alloc_Word_Float)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word float %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_float*)(shred->mem + instr->m_val) = 0; // since template
  *(m_float**)shred->reg = &*(m_float*)(shred->mem + instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

INSTR(Alloc_Word_Complex)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word complex %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_complex**)shred->reg = &*(m_complex*)(shred->mem + instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

INSTR(Alloc_Word_Vec3)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word vec3 %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(VEC3_T**)shred->reg = &*(VEC3_T*)(shred->mem + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Alloc_Word_Vec4)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word vec4 %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(VEC4_T**)shred->reg = &*(VEC4_T*)(shred->mem + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

/* branching */
INSTR(Branch_Switch)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch switch %i %i", *(m_int*)(shred->reg - SZ_INT), instr->m_val);
#endif
  Map map = instr->ptr;
  POP_REG(shred,  SZ_INT);
  shred->next_pc = (m_int)map_get(map, (vtype) * (m_int*)shred->reg);
  if(!shred->next_pc)
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Eq_Int)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch eq int %i %i %i", instr->m_val,  *(m_int**)(shred->reg - SZ_INT * 2),  *(m_int**)(shred->reg - SZ_INT));
#endif
  POP_REG(shred,  SZ_INT * 2);
  m_int l = *(m_uint*)shred->reg;
  m_int r = *(m_uint*)(shred->reg + SZ_INT);
  if(l == r)
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Neq_Int)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch !=");
#endif
  POP_REG(shred,  SZ_INT * 2);
  m_int l = *(m_uint*)shred->reg;
  m_int r = *(m_uint*)(shred->reg + SZ_INT);
  if(l != r)
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Eq_Float)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch eq int %i %f %f", instr->m_val,  *(m_float*)(shred->reg - SZ_FLOAT * 2),  *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred,  SZ_FLOAT * 2);
  m_float l = *(m_float*)shred->reg;
  m_float r = *(m_float*)(shred->reg + SZ_FLOAT);
  if(l == r)
    shred->next_pc = instr->m_val;
}

INSTR(Branch_Neq_Float)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch != float %f %f", *(m_float*)(shred->reg - SZ_FLOAT * 2), *(m_float*)(shred->reg - SZ_FLOAT));
#endif
  POP_REG(shred,  SZ_FLOAT * 2);
  m_float l = *(m_float*)shred->reg;
  m_float r = *(m_float*)(shred->reg + SZ_FLOAT);
  if(l != r)
    shred->next_pc = instr->m_val;
}

INSTR(Init_Loop_Counter)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "loop: init counter");
#endif
  POP_REG(shred,  SZ_INT);
  m_int* sp = (m_int*)shred->reg;
  (*(m_int*)instr->m_val) =  (*sp >= 0 ? *sp : -*sp);
}

INSTR(Free_Loop_Counter)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "loop: free counter");
#endif
  free((m_uint*)instr->m_val);
}

INSTR(Reg_Push_Deref)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "reg  push deref %i", instr->m_val);
#endif
  *(m_int*)shred->reg = *(m_int*)instr->m_val;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Dec_int_Addr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "dec int addr");
#endif
  (*((m_int*)(instr->m_val)))--;
}

INSTR(Goto)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "goto %i", instr->m_val);
#endif
  shred->next_pc = instr->m_val;
}

/* casting */
INSTR(Cast_i2f)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "cast i2f");
#endif
  POP_REG(shred,  SZ_INT);
  *(m_float*)shred->reg = *(m_int*)shred->reg;
  PUSH_REG(shred,  SZ_FLOAT);
}

INSTR(Cast_f2i)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "castf2i");
#endif
  POP_REG(shred,  SZ_FLOAT);
  *(m_int*)shred->reg = *(m_float*)shred->reg;
  PUSH_REG(shred,  SZ_INT);
}

/* debugging */
INSTR(Gack)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "gack");
#endif
  Type type;
  Vector v = instr->ptr;
  m_uint i, j, size = vector_size(v);
  m_uint len, longest = 0;
  for(i = 0; i < size; i++) {
    type = (Type)vector_at(v, i);
    POP_REG(shred,  type->size);
    len = strlen(type->name);
    if(len > longest)
      longest = len;
  }

  for(i = size; i > 0; i--) {
#ifdef DEBUG
#ifdef COLOR
    fprintf(stdout, "\033[1m[\033[34mDEBUG\033[0m] [\033[1m\033[30m%li\033[0m] ", shred->xid);
#else
    fprintf(stdout, "[DEBUG] [%lu] ", shred->xid);
#endif
#endif
    type = (Type)vector_at(v, size - i);
    /*    offset -= type->size;*/
#ifdef COLOR
    fprintf(stdout, "\033[1m");
#endif
    m_str name;
    if(isa(type, &t_function) > 0 && isa(type, &t_func_ptr) < 0)
      name = strdup("@function");
    else
      name = strdup(type->name);
#ifdef COLOR
    fprintf(stdout, "(\033[32m%s\033[0m\033[1m", name);
#else
    fprintf(stdout, " (%s", name);
#endif
    for(j = 0; j < type->array_depth; j++)
      fprintf(stdout, "[]");
#ifdef COLOR
    fprintf(stdout, ")\033[0m ");
#else
    fprintf(stdout, ") ");
#endif
    int j;
    for(j = 0; j < longest - strlen(name); j++)
      fprintf(stdout, " ");
    /*exit(2);*/
    if(type->xid == t_int.xid)
#ifdef COLOR
      fprintf(stdout, "\033[1m%li\033[1m", *(m_uint*)(shred->reg));
#else
      fprintf(stdout, "%li", *(m_uint*)(shred->reg));
#endif
    else if(type->xid == t_float.xid || type->xid == t_dur.xid || isa(type, &t_time) > 0)
#ifdef COLOR
      fprintf(stdout, "\033[1m%.6f\033[0m", *(m_float*)(shred->reg));
#else
      fprintf(stdout, "%.6f", *(m_float*)(shred->reg));
#endif
    else if(type->xid == t_complex.xid) {
#ifdef COLOR
      fprintf(stdout, "\033[33m#(\033[0m\033[1m%.4f\033[0m\033[33m, \033[0m\033[1m%.4f\033[0m\033[33m)\033[0m",
#else
      fprintf(stdout, "#(%.4f, %.4f)",
#endif
              creal(*(m_complex*)(shred->reg)),
              cimag(*(m_complex*)(shred->reg)));
    } else if(type->xid == t_polar.xid) {
#ifdef COLOR
      fprintf(stdout, "\033[33m%%(\033[0m\033[1m%.4f\033[0m\033[33m, \033[0m\033[1m%.4f\033[36m*\033[34mpi\033[0m\033[33m)\033[0m",
#else
      fprintf(stdout, "%%(%.4f, %.4f*pi)",
#endif
              creal(*(m_complex*)(shred->reg)),
              cimag(*(m_complex*)(shred->reg)) / M_PI);
    } else if(type->xid == t_vec3.xid) {
      fprintf(stdout, "%%(%f %f %f)", *(m_float*)(shred->reg), *(m_float*)(shred->reg + SZ_FLOAT),
              *(m_float*)(shred->reg + SZ_FLOAT * 2));

    } else if(type->xid == t_vec4.xid) {
      fprintf(stdout, "%%(%f, %f, %f, %f)", *(m_float*)(shred->reg), *(m_float*)(shred->reg + SZ_FLOAT),
              *(m_float*)(shred->reg + SZ_FLOAT * 2), *(m_float*)(shred->reg + SZ_FLOAT * 3));
    } else if(type->xid == t_string.xid)
#ifdef COLOR
      fprintf(stdout, "\033[1m%s\033[0m", *(M_Object*)(shred->reg) ? STRING(*(M_Object*)(shred->reg)) : NULL);
#else
      fprintf(stdout, "%s", *(M_Object*)(shred->reg) ? STRING(*(M_Object*)(shred->reg)) : NULL);
#endif
    else if(type->xid == t_void.xid)
      fprintf(stdout, "(void)");
    else if(type->xid == t_function.xid)
      /*      fprintf(stdout, "%p %s", type, type->name, type->func ? type->func->name : "");*/
      fprintf(stdout, "%s %p %p", type->name, (void*) * (Func*)(shred->reg),
              (void*)(*(Func*)(shred->reg))->code);
    else if(isa(type, &t_func_ptr) > 0)
      fprintf(stdout, "%p %s  %p", (void*)type, type->name, (void*) * (Func*)(shred->reg));
    /*      fprintf(stdout, "%s %p %p", type->name, *(Func*)(shred->reg),*/
    /*      *(m_uint*)(shred->reg) ? (*(Func*)(shred->reg))->code : 0);*/
    else if(type->xid == t_class.xid)
      fprintf(stdout, "\033[1m%s\033[0m %p", type->actual_type->name, (void*) * (Func*)(shred->reg));
    else
      fprintf(stdout, "%p", (void*) * (M_Object*)(shred->reg));
#ifdef COLOR
    fprintf(stdout, "\033[0m\n");
#else
    fprintf(stdout, "\n");
#endif
    free(name);
    PUSH_REG(shred,  type->size);
  }
//  free_Vector(v);
}

/* function */
struct Vararg {
  Kindof* k; // k(ind)
  char* d;   // d(ata)
  m_uint o, i, s;  // o(ffset)
};

INSTR(MkVararg)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Make Vararg %i %p", instr->m_val, (void*)instr->m_val2);
#endif
  POP_REG(shred,  instr->m_val);
  m_uint i;
  Vector kinds = (Vector)instr->m_val2;
  struct Vararg* arg = malloc(sizeof(struct Vararg));
  if(instr->m_val) {
    arg->d = malloc(instr->m_val);
    memcpy(arg->d, shred->reg, instr->m_val);
  }  else arg->d = NULL;
  arg->s = vector_size(kinds);
  arg->k = calloc(arg->s, sizeof(Kindof));
  for(i = 0; i < arg->s; i++) {
    arg->k[i] = (Kindof)vector_at(kinds, i);
  }
  arg->o = 0;
  arg->i = 0;
  free_Vector(kinds);
  *(struct Vararg**)shred->reg = arg;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Spork)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Spork");
#endif
  m_uint this_ptr = 0;
  VM_Code code;
  Func func;

  POP_REG(shred,  SZ_INT);
  code = *(VM_Code*)shred->reg;
  VM_Shred sh = new_VM_Shred(code);
  sh->parent = shred;
  if(!shred->child)
    shred->child = new_Vector();
  vector_append(shred->child, (vtype)sh);
  sh->_mem = sh->base;
  sh->base = shred->base;
  sh->me = new_Shred(vm, sh);
  vm_add_shred(vm, sh);
  POP_REG(shred,  SZ_INT);
  func = *(Func*)shred->reg;
  if(func->is_member) {
    POP_REG(shred,  SZ_INT);
    this_ptr = *(m_uint*)shred->reg;
    // add to shred so it's ref counted, and released when shred done (1.3.1.2)
    /*    sh->add_parent_ref( (Chuck_Object *)this_ptr );*/
  }
  POP_REG(shred,  instr->m_val);
  memcpy(sh->reg, shred->reg, instr->m_val);
  /*  if(instr->ptr)*/
  /*    memcpy(sh->reg, shred->reg, sizeof(shred->reg));*/
  if(instr->ptr)
    memcpy(sh->mem, shred->mem, (m_uint)instr->ptr);
//  sh->reg += instr->m_val;
  PUSH_REG(sh, instr->m_val);

  if(func->is_member) {
    *(m_uint*)sh->reg = this_ptr;
//    sh->reg += SZ_INT;
    PUSH_REG(sh, SZ_INT);
  }
  if(instr->ptr && code->need_this) // sporked function
    *(m_uint*)sh->mem = this_ptr;
  *(Func*)sh->reg = func;
//  sh->reg += SZ_INT;
  PUSH_REG(sh, SZ_INT);
  *(M_Object*)shred->reg = sh->me;
  PUSH_REG(shred,  SZ_INT);
}

void handle_overflow(VM_Shred shred)
{
  fprintf( stderr,
           "[Gwion](VM): StackOverflow: shred[id=%lu:%s], PC=[%lu]\n",
           shred->xid, shred->name, shred->pc );
  // do something!
  shred->is_running = 0;
  shred->is_done = 1;
}

INSTR(Instr_Func_Call)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "func call");
#endif
  VM_Code func;
  m_uint local_depth, stack_depth, prev_stack = 0, push, next;

  POP_REG(shred,  SZ_INT * 2);
  func = *(VM_Code*)shred->reg;
  stack_depth = func->stack_depth;
  local_depth = *(m_uint*)(shred->reg + SZ_INT);
  prev_stack = instr ? instr->m_val : shred->mem == shred->base ? 0 : *(m_uint*)(shred->mem - SZ_INT);
  push = prev_stack + local_depth;
  next = shred->pc + 1;
  PUSH_MEM(shred, push);
  *(m_uint*)(shred->mem)  = push;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)(shred->mem)  = (m_uint)shred->code;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)(shred->mem)  = (m_uint)next;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)(shred->mem)  = (m_uint)stack_depth;
  PUSH_MEM(shred,  SZ_INT);
  shred->next_pc = 0;
  shred->code = func;
  if(stack_depth) {
    POP_REG(shred,  stack_depth);
    if(func->need_this) {
      *(m_uint*)(shred->mem) = *(m_uint*)(shred->reg + stack_depth - SZ_INT);
      PUSH_MEM(shred,  SZ_INT);
      stack_depth -= SZ_INT;
    }
    if(stack_depth) {
      memcpy(shred->mem, shred->reg, stack_depth);
    }
    if(func->need_this) {
      POP_MEM(shred,  SZ_INT);
    }
  }
  if(overflow_(shred->mem))
    handle_overflow(shred);
  return;
}

INSTR(Instr_Op_Call_Binary)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "binary call");
#endif
  VM_Code func;
  Type l = (Type)instr->m_val2;
  Type r = (Type)instr->ptr;
  m_uint local_depth, stack_depth, prev_stack = 0, push, next;

  POP_REG(shred,  SZ_INT * 2);
  func = *(VM_Code*)shred->reg;
  stack_depth = func->stack_depth;
  local_depth = *(m_uint*)(shred->reg + SZ_INT);
  prev_stack = instr->m_val;
  push = prev_stack + local_depth;
  next = shred->pc + 1;
  PUSH_MEM(shred, push);
  *(m_uint*)(shred->mem)  = push;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)(shred->mem)  = (m_uint)shred->code;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)(shred->mem)  = (m_uint)next;
  PUSH_MEM(shred,  SZ_INT);
  *(m_uint*)(shred->mem)  = (m_uint)stack_depth;
  PUSH_MEM(shred,  SZ_INT);
  shred->next_pc = 0;
  shred->code = func;
  POP_REG(shred,  stack_depth);
  if(func->need_this) {
    *(m_uint*)(shred->mem) = *(m_uint*)(shred->reg + stack_depth - SZ_INT);
    PUSH_MEM(shred,  SZ_INT);
    stack_depth -= SZ_INT;
  }
  if(isa(l, &t_object) > 0)
    release(*(M_Object*)(shred->reg), shred);
  if(isa(r, &t_object) > 0)
    release(**(M_Object**)(shred->reg + SZ_INT), shred);
  if(stack_depth) {
    Kindof kl = kindof(l); 
    Kindof kr = kindof(r); 
    if(kl == Kindof_Int)
      *(m_uint*)(shred->mem) = *(m_uint*)shred->reg;
    else if(kl == Kindof_Float)
      *(m_float*)(shred->mem) = *(m_float*)shred->reg;
    else if(kl == Kindof_Complex)
      *(m_complex*)(shred->mem) = *(m_complex*)shred->reg;
    else if(kl == Kindof_Vec3)
      *(VEC3_T*)(shred->mem) = *(VEC3_T*)shred->reg;
    else if(kl == Kindof_Vec4)
      *(VEC4_T*)(shred->mem) = *(VEC4_T*)shred->reg;
    if(kr == Kindof_Int)
       *(m_uint*)(shred->mem + l->size) = **(m_uint**)(shred->reg + l->size);
    else if(kr == Kindof_Float)
       *(m_float*)(shred->mem + l->size) = **(m_float**)(shred->reg + l->size);
    else if(kr == Kindof_Complex)
       *(m_complex*)(shred->mem + l->size) = **(m_complex**)(shred->reg + l->size);
    else if(kr == Kindof_Vec3)
       *(VEC3_T*)(shred->mem + l->size) = **(VEC3_T**)(shred->reg + l->size);
    else if(kr == Kindof_Vec4)
       *(VEC4_T*)(shred->mem + l->size) = **(VEC4_T**)(shred->reg + l->size);
    if(func->need_this) {
      POP_MEM(shred, SZ_INT);
    }
  }
  if(overflow_(shred->mem))
    handle_overflow(shred);
  return;
}

INSTR(Dot_Static_Func)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "dot static func %s", ((Func)instr->m_val)->name);
#endif
  *(m_uint*)(shred->reg - SZ_INT) = instr->m_val;
}

INSTR(Reg_Dup_Last_Vec3)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "dup last vec3");
#endif
  *(VEC3_T*)shred->reg = *(VEC3_T*)(shred->reg  - SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Reg_Dup_Last_Vec4)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "dup last vec3");
#endif
  *(VEC4_T*)shred->reg = *(VEC4_T*)(shred->reg  - SZ_INT);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(member_function)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "member function %p [%i] %p", instr->ptr, instr->m_val, vector_at((Vector)instr->ptr,
            instr->m_val));
#endif
  POP_REG(shred,  SZ_INT);
  /*
  M_Object obj = *(M_Object*)shred->reg;
  if(!obj)
  {
  	Except(shred);
  	return;
  }
  */
  *(VM_Code*)shred->reg = ((Func)vector_at((Vector)instr->ptr, instr->m_val))->code;
  PUSH_REG(shred,  SZ_INT);
#ifdef DEBUG_INSTR
  debug_msg("instr", "member function %i [%i]", instr->ptr, instr->m_val, vector_at((Vector)instr->ptr, instr->m_val));
#endif
  return;
}


INSTR(Dot_Member_Func)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "dot member func");
//  debug_msg("instr", "dot member func %p[%i] %p", *(M_Object*)(shred->reg - SZ_INT), instr->m_val, vector_at((*(M_Object*)(shred->reg - SZ_INT))->vtable, instr->m_val));
#endif
  POP_REG(shred,  SZ_INT);
  M_Object obj = *(M_Object*)shred->reg;
  if(!obj)
    Except(shred);
  *(Func*)shred->reg = (Func)vector_at(obj->vtable, instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Instr_Func_Call_Static)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "func call static");
#endif

  f_sfun f;
  m_uint local_depth, stack_depth;
  VM_Code func;
  DL_Return retval;

  POP_REG(shred,  SZ_INT * 2);
  func        = *(VM_Code*)shred->reg;
  f           = (f_sfun)func->native_func;
  local_depth = *(m_uint*)(shred->reg + SZ_INT);
  stack_depth = func->stack_depth;
  PUSH_MEM(shred,  local_depth);
  if(stack_depth) {
    POP_REG(shred,   stack_depth);
    memcpy(shred->mem + SZ_INT, shred->reg, stack_depth);
  }
  if(overflow_(shred->mem)) {
    handle_overflow(shred);
    return;
  }
  f(&retval, shred);
  dl_return_push(retval, shred, instr->m_val);
  POP_MEM(shred,  local_depth);
}

INSTR(Instr_Func_Call_Member)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "func call member");
#endif

  DL_Return retval;
  VM_Code func;
  m_uint local_depth, stack_depth;

  POP_REG(shred,  SZ_INT * 2);
  func = *(VM_Code*)shred->reg;
  local_depth =   *(m_uint*)(shred->reg + SZ_INT);
  stack_depth = func->stack_depth;
  PUSH_MEM(shred,  local_depth);
  if(stack_depth) {
    POP_REG(shred,  stack_depth);
    if(func->need_this) {
      *(m_uint*)shred->mem = *(m_uint*)(shred->reg - SZ_INT + stack_depth);
      PUSH_MEM(shred,  SZ_INT);
      stack_depth -= SZ_INT;
    }
    memcpy(shred->mem, shred->reg, stack_depth);
  }
  if(func->need_this)
    POP_MEM(shred,  SZ_INT);

  if(overflow_(shred->mem)) {
    handle_overflow(shred);
    return;
  }
  if(func->native_func_type == NATIVE_CTOR) {
    f_ctor f = (f_ctor)func->native_func;
    f(*(M_Object*)shred->mem, shred);
  } else {
    f_mfun f = (f_mfun)func->native_func;
    f((*(M_Object*)shred->mem), &retval, shred);
    dl_return_push(retval, shred, instr->m_val);
  }
  POP_MEM(shred,  local_depth);
}

INSTR(Func_Return)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "func return %p", (shred->mem));
//  debug_msg("instr", "func return %s", (*(VM_Code*)(shred->mem - SZ_INT * 3))->name);
#endif
  POP_MEM(shred,  SZ_INT * 2);
  shred->next_pc = *(m_uint*)shred->mem;
  POP_MEM(shred,  SZ_INT);
  VM_Code func = *(VM_Code*)shred->mem;
  POP_MEM(shred,  SZ_INT);
  POP_MEM(shred, *(m_uint*)shred->mem);
  shred->code = func;
}

/* object */
static void call_pre_constructor(VM * vm, VM_Shred shred, VM_Code pre_ctor, m_uint stack_offset)
{
  *(m_uint*)shred->reg = *(m_uint*)(shred->reg - SZ_INT); // ref dup last
  PUSH_REG(shred,  SZ_INT);
  *(VM_Code*)shred->reg = pre_ctor;
  PUSH_REG(shred,  SZ_INT);
  *(m_uint*)shred->reg = stack_offset;
  PUSH_REG(shred,  SZ_INT);
  if(pre_ctor->native_func_type != NATIVE_UNKNOWN)
    Instr_Func_Call_Member(vm, shred, NULL);
  else
    Instr_Func_Call(vm, shred, NULL);
}

INSTR(Reg_Push_This)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "push this %p", *(m_uint*)(shred->mem));
#endif
  *(m_uint*)shred->reg = *(m_uint*)shred->mem;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Pre_Constructor)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "pre constructor");
#endif
  call_pre_constructor(vm, shred, (VM_Code)instr->m_val, instr->m_val2);
}

static void instantiate_object(VM * vm, VM_Shred shred, Type type )
{

  M_Object object = new_M_Object();
  if(!object)
    goto error;
  initialize_object(object, type);
  *(M_Object*)shred->reg =  object;
  PUSH_REG(shred,  SZ_INT);
#ifdef DEBUG_INSTR
  debug_msg("instr", "instantiate object (internal)%p %s", object, type->name);
#endif
  return;

error:
  shred->is_running = 0;
  shred->is_done = 1;
}

INSTR(Instantiate_Object)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "instantiate object %p", instr->ptr);
#endif
  instantiate_object(vm, shred, instr->ptr);
}

INSTR(Alloc_Member_Word)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member word: %p[%i]", *(m_uint*)(shred->mem), instr->m_val);
#endif
  M_Object obj = *(M_Object*)(shred->mem);
  *(m_uint*)(obj->d.data + instr->m_val) = 0;
  *(m_uint**)shred->reg = &*(m_uint*)(obj->d.data + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Alloc_Member_Word_Float)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member float: %p[%i] = '%p'", *(m_uint*)(shred->mem - 1), instr->m_val, *(m_uint*)shred->mem);
#endif
  M_Object obj = *(M_Object*)shred->mem;
  *(m_float*)(obj->d.data + instr->m_val) = 0.0;
  *(m_float**)shred->reg = &*(m_float*)(obj->d.data + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Alloc_Member_Word_Complex)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member complex: %p[%i] = '%p'", *(m_uint*)(shred->mem - 1), instr->m_val, *(m_uint*)shred->mem);
#endif
  M_Object obj = *(M_Object*)shred->mem;
  *(m_complex*)(obj->d.data + instr->m_val) = 0.0;
  *(m_complex**)shred->reg = &*(m_complex*)(obj->d.data + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Alloc_Member_Word_Vec3)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member vec3: %p[%i] = '%p'", *(m_uint*)(shred->mem - 1), instr->m_val, *(m_uint*)shred->mem);
#endif
  M_Object obj = *(M_Object*)shred->mem;
  memset((obj->d.data + instr->m_val), 0, SZ_VEC3);
  *(VEC3_T**)shred->reg = &*(VEC3_T*)(obj->d.data + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Alloc_Member_Word_Vec4)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc member vec4: %p[%i] = '%p'", *(m_uint*)(shred->mem - 1), instr->m_val, *(m_uint*)shred->mem);
#endif
  M_Object obj = *(M_Object*)shred->mem;
  memset((obj->d.data + instr->m_val), 0, SZ_VEC4);
  *(VEC4_T**)shred->reg = &*(VEC4_T*)(obj->d.data + instr->m_val);
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Dot_Static_Data)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Dot STATIC DATA: [%i] (%i) (emit:%i)", instr->m_val, instr->m_val2, instr->ptr);
#endif
  Type t;

  POP_REG(shred,  SZ_INT);
  t = *(Type*)shred->reg;
  // take care of emit_addr ? (instr->ptr)
  if(instr->ptr) {
    *(m_uint**)shred->reg = &*(m_uint*)(t->info->class_data + instr->m_val);
    PUSH_REG(shred,  SZ_INT);
  }
  /* take care of Kind (instr->m_val2)*/
  else if(instr->m_val2 == Kindof_Int) {
    *(m_uint*)shred->reg = *(m_uint*)(t->info->class_data + instr->m_val);
    PUSH_REG(shred,  SZ_INT);
  } else if(instr->m_val2 == Kindof_Float) {
    *(m_float*)shred->reg = *(m_float*)(t->info->class_data + instr->m_val);
    PUSH_REG(shred,  SZ_FLOAT);
  } else if(instr->m_val2 == Kindof_Complex) {
    *(m_complex*)shred->reg = *(m_complex*)(t->info->class_data + instr->m_val);
    PUSH_REG(shred,  SZ_COMPLEX);
  } else if(instr->m_val2 == Kindof_Vec3) {
    *(VEC3_T*)shred->reg = *(VEC3_T*)(t->info->class_data + instr->m_val);
    PUSH_REG(shred,  SZ_VEC3);
  } else if(instr->m_val2 == Kindof_Vec4) {
    *(VEC4_T*)shred->reg = *(VEC4_T*)(t->info->class_data + instr->m_val);
    PUSH_REG(shred,  SZ_VEC4);
  }
}

INSTR(Dot_Static_Import_Data)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Dot STATIC Import DATA: %p", instr->ptr);
#endif
  // take care of emit_addr ? (instr->ptr)
  if(instr->ptr) {
    *(m_uint**)shred->reg = &*(m_uint*)instr->m_val;
    PUSH_REG(shred,  SZ_INT);
  }
  /* take care of Kind (instr->m_val2)*/
  if(instr->m_val2 == Kindof_Int) {
    *(m_uint*)shred->reg = *(m_uint*)instr->m_val;
    PUSH_REG(shred,  SZ_INT);
  } else if(instr->m_val2 == Kindof_Float) {
    *(m_float*)shred->reg = *(m_float*)instr->m_val;
    PUSH_REG(shred,  SZ_FLOAT);
  } else if(instr->m_val2 == Kindof_Complex) {
    *(m_complex*)shred->reg = *(m_complex*)instr->m_val;
    PUSH_REG(shred,  SZ_COMPLEX);
  } else if(instr->m_val2 == Kindof_Vec3) {
    *(VEC3_T*)shred->reg = *(VEC3_T*)instr->m_val;
    PUSH_REG(shred,  SZ_VEC3);
  } else if(instr->m_val2 == Kindof_Vec4) {
    *(VEC4_T*)shred->reg = *(VEC4_T*)instr->m_val;
    PUSH_REG(shred,  SZ_VEC4);
  }
}

INSTR(Dot_Member_Data)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "dot member data '%p'[%i] (%i) (emit:%i)", *(M_Object*)(shred->reg - SZ_INT), instr->m_val, instr->m_val2, instr->ptr);
#endif
  M_Object obj;

  POP_REG(shred,  SZ_INT);
  obj  = *(M_Object*)shred->reg;
  if(!obj)
    Except(shred);
  // take care of emit_addr ? (instr->ptr)
  if(instr->ptr) {
    *(m_uint**)shred->reg = &*(m_uint*)(obj->d.data + instr->m_val);
    PUSH_REG(shred,  SZ_INT);
  }
  /* take care of Kind (instr->m_val2)*/
  else if(instr->m_val2 == Kindof_Int) {
    *(m_uint*)shred->reg = *(m_uint*)(obj->d.data + instr->m_val);
    PUSH_REG(shred,  SZ_INT);
  } else if(instr->m_val2 == Kindof_Float) {
    *(m_float*)shred->reg = *(m_float*)(obj->d.data + instr->m_val);
    PUSH_REG(shred,  SZ_FLOAT);
  } else if(instr->m_val2 == Kindof_Complex) {
    *(m_complex*)shred->reg = *(m_complex*)(obj->d.data + instr->m_val);
    PUSH_REG(shred,  SZ_COMPLEX);
  } else if(instr->m_val2 == Kindof_Vec3) {
    *(VEC3_T*)shred->reg = *(VEC3_T*)instr->m_val;
    PUSH_REG(shred,  SZ_VEC3);
  } else if(instr->m_val2 == Kindof_Vec4) {
    *(VEC4_T*)shred->reg = *(VEC4_T*)instr->m_val;
    PUSH_REG(shred,  SZ_VEC4);
  }
}

INSTR(Release_Object2)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "release object %p %i", *(m_uint*)(shred->mem + instr->m_val), instr->m_val);
#endif
  release(*(M_Object*)(shred->mem + instr->m_val), shred);
}

/* array */
INSTR(Instr_Pre_Ctor_Array_Top)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc top");
#endif
  if(*(m_uint*)(shred->reg - SZ_INT * 2) >= *(m_uint*)(shred->reg - SZ_INT))
    shred->next_pc = instr->m_val;
  else
    instantiate_object(vm, shred, (Type)instr->ptr);
}

INSTR(Instr_Pre_Ctor_Array_Bottom)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc bottom");
#endif
  POP_REG(shred,  SZ_INT);
  M_Object obj = *(M_Object*)shred->reg;
  m_uint * array = *(m_uint**)(shred->reg - SZ_INT * 3);
  m_int i = *(m_int*)(shred->reg - SZ_INT * 2);
  *(m_uint*)array[i] = (m_uint)obj;
  (*(m_int*)(shred->reg - SZ_INT * 2))++;
  shred->next_pc = instr->m_val;
}

INSTR(Instr_Pre_Ctor_Array_Post)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc post");
#endif
  POP_REG(shred,  SZ_INT * 3);
  m_uint* array = *(m_uint**)shred->reg;
  free(array);
}

static M_Object do_alloc_array(VM_Shred shred, m_int capacity, const m_int top,
                               Type type, m_bool is_obj, m_uint* objs, m_int* index )
{
  M_Object base = NULL, next = NULL;
  m_int i = 0;
  m_int cap = *(m_int*)(shred->reg + capacity * SZ_INT);
  if(cap < 0)
    goto negative_array_size;
  type->obj->ref_count++;
  if(capacity >= top) {
    base = new_M_Array(type->array_type->size, cap, -capacity);
    if(!base)
      goto out_of_memory;
    base->type_ref=type; // /13/03/17
    if(is_obj && objs) {
      for( i = 0; i < cap; i++ ) {
        objs[*index] = (m_uint)i_vector_addr(base->d.array, i);
        (*index)++;
      }
    }
    return base;
  }
  base = new_M_Array(SZ_INT, cap, -capacity);
  if(!base)
    goto out_of_memory;
  base->type_ref=type;
  for( i = 0; i < cap; i++ ) {
    next = do_alloc_array(shred, capacity + 1, top, type, is_obj, objs, index);
    if(!next)
      goto error;
    // set that, with ref count
    i_vector_set(base->d.array, i, (m_uint)next);
  }
  return base;

out_of_memory:
  fprintf(stderr, "[chuck](VM): OutOfMemory: while allocating arrays...\n" );
  goto error;

negative_array_size:
  fprintf(stderr, "[chuck](VM): NegativeArraySize: while allocating arrays...\n" );
  goto error;

error:
  if(base)
    release(base, shred);
  return NULL;
}

INSTR(Instr_Array_Init) // for litteral array
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array init");
#endif
  m_uint i;
  VM_Array_Info* info = (VM_Array_Info*)instr->ptr;
  M_Object obj;
  POP_REG(shred,  SZ_INT * info->length);
  obj = new_M_Array(info->type->array_type->size, info->length, info->depth);
  obj->type_ref = info->type;
  for(i = 0; i < info->length; i++)
    i_vector_set(obj->d.array, i, *(m_uint*)(shred->reg + SZ_INT * i));
  info->type->obj->ref_count = 1;
  *(M_Object*)shred->reg = obj;
  *(M_Object*)(shred->mem + instr->m_val) = obj;
  PUSH_REG(shred,  SZ_INT);
}

INSTR(Instr_Array_Alloc)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc");
#endif
  VM_Array_Info* info = (VM_Array_Info*)instr->ptr;
  M_Object ref;
  m_uint num_obj = 0;
  m_int index = 0;
  m_float num = 1.0;
  m_uint* obj_array = NULL;

  if(info->is_obj && !info->is_ref) {
    m_int curr = -info->depth;
    m_int top = - 1;
    m_int tmp;
    num_obj = 1;
    num = 1.0;
    while( curr <= top ) {
      tmp = *(m_int*)(shred->reg + SZ_INT * curr);
      num_obj *= tmp;
      num *= (m_float)tmp;
      curr++;
    }
    if(num_obj > 0) {
      obj_array = (m_uint*)calloc(num_obj, sizeof(m_uint));
      if(!obj_array)
        goto out_of_memory;
    }
  }
  ref = do_alloc_array(shred, -info->depth, -1, info->type, info->is_obj, obj_array, &index);
  if(!ref)
    goto error;
  POP_REG(shred, SZ_INT * info->depth);

  *(M_Object*)shred->reg = ref;
  PUSH_REG(shred,  SZ_INT);
  if(info->is_obj && !info->is_ref) {
    *(m_uint**)shred->reg = obj_array;
    PUSH_REG(shred,  SZ_INT);
    *(m_uint*) shred->reg = 0;
    PUSH_REG(shred,  SZ_INT);
    *(m_uint*) shred->reg = num_obj;
    PUSH_REG(shred,  SZ_INT);
  }
  return;

out_of_memory:
  fprintf( stderr, "[Gwion](VM): OutOfMemory: while allocating arrays...\n" );
  goto error;
error:
  fprintf( stderr, "[Gwion](VM): (note: in shred[id=%lu:%s])\n", shred->xid, shred->name);
  shred->is_running = 0;
  shred->is_done = 1;
}


INSTR(Instr_Array_Access)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array access '%p'  (emit: %i) [%i] ", *(m_uint*)(shred->reg - SZ_INT * 2), instr->m_val, instr->m_val2);
#endif
  m_int i = 0;
  M_Object obj;
  POP_REG(shred,  SZ_INT * 2);
  obj = *(M_Object*)shred->reg;
  if(!obj)
    Except(shred);
  i = *(m_int*)(shred->reg + SZ_INT);
  if(i < 0 || i >= m_vector_size(obj->d.array))
    goto array_out_of_bound;
  // take care of emit_addr (instr->m_val)
  if(instr->m_val) {
    if(instr->m_val2 == Kindof_Int)
      *(m_uint**)shred->reg = i_vector_addr(obj->d.array, i);
    else if(instr->m_val2 == Kindof_Float)
      *(m_float**)shred->reg = f_vector_addr(obj->d.array, i);
    else if(instr->m_val2 == Kindof_Complex)
      *(m_complex**)shred->reg = c_vector_addr(obj->d.array, i);
    else if(instr->m_val2 == Kindof_Vec3)
      *(VEC3_T**)shred->reg = v3_vector_addr(obj->d.array, i);
    else if(instr->m_val2 == Kindof_Vec4)
      *(VEC4_T**)shred->reg = v4_vector_addr(obj->d.array, i);
    PUSH_REG(shred,  SZ_INT);
  }
  // take care of kind (instr->m_val2)
  else if(instr->m_val2 == Kindof_Int) {
    *(m_uint*)shred->reg = i_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_INT);
  } else if(instr->m_val2 == Kindof_Float) {
    *(m_float*)shred->reg = f_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_FLOAT);
  } else if(instr->m_val2 == Kindof_Complex) {
    *(m_complex*)shred->reg = c_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_COMPLEX);
  } else if(instr->m_val2 == Kindof_Vec3) {
    *(VEC3_T*)shred->reg = v3_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_VEC3);
  } else if(instr->m_val2 == Kindof_Vec4) {
    *(VEC4_T*)shred->reg = v4_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_VEC4);
  }
  return;

array_out_of_bound:
  fprintf( stderr,
           "[Gwion](VM): ArrayOutofBounds: in shred[id=%lu:%s], PC=[%lu], index=[%ld]\n",
           shred->xid, shred->name, shred->pc, i );
  shred->is_running = 0;
  shred->is_done = 1;
}

INSTR(Instr_Array_Access_Multi)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array access multi");
#endif
  m_int i, j;
  POP_REG(shred,  SZ_INT * (instr->m_val + 1));
  M_Object obj, *base = (M_Object*)shred->reg;
  obj = *base;
  if(!obj)
    Except(shred);
  for(j = 0; j < instr->m_val - 1; j++) {
    i = *(m_int*)(shred->reg + SZ_INT * (j + 1));
    if(i < 0 || i >= m_vector_size(obj->d.array))
      goto array_out_of_bound;
    obj = (M_Object)i_vector_at(obj->d.array, i);
    if(!obj)
      Except(shred);
  }
  i = *(m_int*)(shred->reg + SZ_INT * (j + 1));
  // take care of emit_addr (instr->ptr)
  if(instr->ptr) {
    if(instr->m_val2 == Kindof_Int)
      *(m_uint**)shred->reg = i_vector_addr(obj->d.array, i);
    if(instr->m_val2 == Kindof_Float)
      *(m_float**)shred->reg = f_vector_addr(obj->d.array, i);
    if(instr->m_val2 == Kindof_Complex)
      *(m_complex**)shred->reg = c_vector_addr(obj->d.array, i);
    else if(instr->m_val2 == Kindof_Vec3)
      *(VEC3_T**)shred->reg = v3_vector_addr(obj->d.array, i);
    else if(instr->m_val2 == Kindof_Vec4)
      *(VEC4_T**)shred->reg = v4_vector_addr(obj->d.array, i);
    PUSH_REG(shred,  SZ_INT);
  }
  // take care of kind (instr->m_val2)
  else if(instr->m_val2 == Kindof_Int) {
    *(m_uint*)shred->reg = i_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_INT);
  } else if(instr->m_val2 == Kindof_Float) {
    *(m_float*)shred->reg = f_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_FLOAT);
  } else if(instr->m_val2 == Kindof_Complex) {
    *(m_complex*)shred->reg = c_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_COMPLEX);
  } else if(instr->m_val2 == Kindof_Vec3) {
    *(VEC3_T*)shred->reg = v3_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_VEC3);
  } else if(instr->m_val2 == Kindof_Vec4) {
    *(VEC4_T*)shred->reg = v4_vector_at(obj->d.array, i);
    PUSH_REG(shred,  SZ_VEC4);
  }
  return;

array_out_of_bound:
  fprintf( stderr,
           "[Gwion](VM): ArrayOutofBounds: in shred[id=%lu:%s], PC=[%lu], index=[%ld]\n",
           shred->xid, shred->name, shred->pc, i );
  shred->is_running = 0;
  shred->is_done = 1;
}
