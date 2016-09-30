#include "defs.h"
#include "err_msg.h"
#include "instr.h"
#include <math.h>
void EOC(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Shred [%i]: End of Code", shred->xid); 
#endif
  shred->is_running = 0;
  shred->is_done = 1;
}
/* stacking */
void Reg_Pop_Word4(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] pop %i", instr->m_val);
#endif
  shred->reg -= instr->m_val;
}

void Reg_Push_Imm(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push imm");
  debug_msg("instr", "[reg] push imm %p %i", instr->m_val, instr->m_val2);
#endif
  *(m_uint*)(shred->reg) = instr->m_val;
  shred->reg += SZ_INT;
}

void Reg_Push_Imm2(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push imm2 %f", instr->f_val);
#endif
  *(m_float*)(shred->reg) = instr->f_val;
  shred->reg += SZ_FLOAT;
}

void Reg_Push_ImmC(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push imm complex %p", instr->c_val);
#endif
  shred->reg -= SZ_FLOAT * 2;
  *(complex*)(shred->reg) = ((*(m_float*)shred->reg) + (*(m_float*)(shred->reg + SZ_FLOAT)) * I);
  shred->reg += SZ_COMPLEX;
}

void Reg_Push_Mem_Addr(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push mem addr %i %i", instr->m_val, instr->m_val2);
#endif
  *(m_uint**)shred->reg= (m_uint*)((instr->m_val2 ? shred->base : shred->mem) + instr->m_val);
  shred->reg += SZ_INT;
}

void Mem_Push_Imm(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[mem] push imm");
  debug_msg("instr", "[mem] push imm %i %i", instr->m_val, instr->m_val2);
#endif
  *(m_uint*)shred->mem = instr->m_val;
  shred->mem += SZ_INT;
}

void Mem_Set_Imm(VM* vm, VM_Shred shred, Instr instr)
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
  debug_msg("instr", "free template func '%p'", instr->m_val);
#endif
  Func f = (Func)instr->m_val;
  free_Func_Def(f->def);
  free_Func(f);

}
// func pointer
void assign_func(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "assign func");
#endif
  shred->reg -= SZ_INT*2;
  *(m_uint*)shred->reg = (**(m_uint**)(shred->reg + SZ_INT) = *(m_uint*)shred->reg);
  shred->reg += SZ_INT;
}

void Reg_Push_Mem(VM* vm, VM_Shred shred, Instr instr)
{ 
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] reg push '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(instr->m_val2) // global
    *(m_uint*)shred->reg = *(m_uint*)(shred->base + instr->m_val);
  else
    *(m_uint*)shred->reg = *(m_uint*)(shred->mem  + instr->m_val);
  shred->reg += SZ_INT;
}

void Reg_Push_Mem2(VM* vm, VM_Shred shred, Instr instr)
{ 
  char* s = instr->m_val2 ? shred->base : shred->mem;
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] reg push float '%s' [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  if(instr->m_val2) // global
    *(m_float*)shred->reg = *(m_float*)(shred->base + instr->m_val);
  else
    *(m_float*)shred->reg = *(m_float*)(shred->mem  + instr->m_val);
  shred->reg += SZ_FLOAT;
}

void Reg_Push_Mem_Complex(VM* vm, VM_Shred shred, Instr instr)
{ 
  char* s = instr->m_val2 ? shred->base : shred->mem;
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] 'complex' push mem ");
#endif
  if(instr->m_val2) // global
    *(complex*)shred->reg = *(complex*)(shred->base + instr->m_val);
  else
    *(complex*)shred->reg = *(complex*)(shred->mem  + instr->m_val);
  shred->reg += SZ_COMPLEX;
}

void Reg_Push_Ptr(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push ptr (%p)", instr->ptr);
#endif
  *(m_uint*)(shred->reg - SZ_INT) = (m_uint)instr->ptr;
}

void Reg_Push_Code(VM* vm, VM_Shred shred, Instr instr)
{ 
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push code [%i]", instr->m_val);
#endif
  Func f = *(Func*)(shred->mem + instr->m_val);
  *(VM_Code*)(shred->reg - SZ_INT) = f->code;
}

void Reg_Dup_Last(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] dup last %p", *(m_uint*)(shred->reg  - SZ_INT));
#endif
  *(m_uint*)shred->reg = *(m_uint*)(shred->reg  - SZ_INT);
  shred->reg += SZ_INT;
}

void Reg_AddRef_Object3(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "[reg] add ref %i %p", instr->m_val, instr->m_val ? **(M_Object**)(shred->reg -SZ_INT) : *(M_Object*)(shred->reg -SZ_INT));
#endif
    M_Object obj = instr->m_val ? **(M_Object**)(shred->reg -SZ_INT) : *(M_Object*)(shred->reg -SZ_INT);
    if(obj)
      obj->ref++;
}

void Reg_Push_Me(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push me %p", shred->me);
#endif
  *(M_Object*)shred->reg = shred->me;
  shred->reg += SZ_INT;
}

void Reg_Push_Now(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push now %f", get_now(vm->shreduler));
#endif
  *(m_float*)shred->reg = get_now(vm->shreduler);
  shred->reg += SZ_FLOAT;
}

void Reg_Push_Maybe(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "[reg] push maybe");
#endif
  *(m_uint*)shred->reg = (rand() > (RAND_MAX/2)) ? 1 : 0;
  shred->reg += SZ_INT;
}

/* alloc */
void Alloc_Word(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "alloc_word '%s' (%p)[%i]", instr->m_val2 ? "base" : "mem", *(m_uint*)(shred->mem + instr->m_val), instr->m_val);
#endif
  *(m_uint*)(shred->mem + instr->m_val) = 0; // since template
  *(m_uint**)shred->reg = &*(m_uint*)(shred->mem + instr->m_val);
  shred->reg += SZ_INT;
}

void Alloc_Word_Float(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word float %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(m_float*)(shred->mem + instr->m_val) = 0; // since template
  *(m_float**)shred->reg = &*(m_float*)(shred->mem + instr->m_val);
  shred->reg += SZ_INT;
}

void Alloc_Word_Complex(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "instr alloc word complex %s [%i]", instr->m_val2 ? "base" : "mem", instr->m_val);
#endif
  *(complex**)shred->reg = &*(complex*)(shred->mem + instr->m_val);
  shred->reg += SZ_INT;
}

/* branching */
void Branch_Switch(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch switch %i %i", *(m_int*)(shred->reg - SZ_INT), instr->m_val);
#endif
  Map map = instr->ptr;
  shred->reg -= SZ_INT;
  shred->next_pc = (m_int)map_get(map, (void*)*(m_int*)shred->reg);
  if(!shred->next_pc)
    shred->next_pc = instr->m_val;
}

void Branch_Eq_Int(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch eq int %i %i %i", instr->m_val,  *(m_int**)(shred->reg - SZ_INT*2),  *(m_int**)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_INT*2;
  m_int l = *(m_uint*)shred->reg;
  m_int r = *(m_uint*)(shred->reg + SZ_INT);
  if(l == r)
    shred->next_pc = instr->m_val;
}

void Branch_Neq_Int(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "branch !=");
#endif
  shred->reg -= SZ_INT*2;
  m_int l = *(m_uint*)shred->reg;
  m_int r = *(m_uint*)(shred->reg + SZ_INT);
  if(l != r)
    shred->next_pc = instr->m_val;
}

void Branch_Eq_Float(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "branch eq int %i %i %i", instr->m_val,  *(m_int*)(shred->reg -SZ_INT*2),  *(m_int*)(shred->reg - SZ_INT));
#endif
  shred->reg -= SZ_FLOAT*2;
  m_float l = *(m_float*)shred->reg;
  m_float r = *(m_float*)(shred->reg + SZ_INT);
  if(l == r)
    shred->next_pc = instr->m_val;
}

void Branch_Neq_Float(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "branch != float");
#endif
  shred->reg -= SZ_FLOAT*2;
  m_float l = *(m_float*)shred->reg;
  m_float r = *(m_float*)(shred->reg + SZ_INT);
  if(l != r)
    shred->next_pc = instr->m_val;
}  

void Init_Loop_Counter(VM * vm, VM_Shred shred, Instr instr )
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "init loop counter");
#endif
  shred->reg -= SZ_INT;
  m_int* sp = (m_int*)shred->reg;
  (*(m_int*)instr->m_val) =  (*sp >= 0 ? *sp : -*sp);
}

void Reg_Push_Deref(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "reg  push deref %i", instr->m_val);
#endif
  *(m_int*)shred->reg = *(m_int*)instr->m_val;
  shred->reg += SZ_INT;
}

void Dec_int_Addr(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "dec int addr");
#endif
  (*((m_int*)(instr->m_val)))--;
}

void Goto(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "goto %i", instr->m_val); 
#endif
  shred->next_pc = instr->m_val;
}

/* casting */
void Cast_i2f(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "cast i2f"); 
#endif
  shred->reg -= SZ_INT;
  *(m_float*)shred->reg = *(m_int*)shred->reg;
  shred->reg += SZ_FLOAT;
}

void Cast_f2i(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "castf2i"); 
#endif
  shred->reg -= SZ_FLOAT;
  *(m_int*)shred->reg = *(m_float*)shred->reg;
  shred->reg += SZ_INT;
}

/* debugging */
void Gack(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "gack");
#endif
  Type type;
  Vector v = instr->ptr;
  m_uint i, j, size = vector_size(v);
  m_int offset = 0;;
  m_uint len, longest = 0;
  for(i = 0; i < size; i++)
  {
    type = vector_at(v, i);
    shred->reg -= type->size;
    len = strlen(type->name);
    if(len > longest)
      longest = len;
  }

  for(i = size; i > 0; i--)
  {
#ifdef DEBUG
#ifdef COLOR
    fprintf(stdout, "\033[1m[\033[34mDEBUG\033[0m] [\033[1m\033[30m%i\033[0m] ", shred->xid); 
#else
    fprintf(stdout, "[DEBUG] [%i] ", shred->xid); 
#endif
#endif
  type = vector_at(v, size-i);
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
    for(int j = 0; j < longest - strlen(name); j++)
      fprintf(stdout, " ");
/*exit(2);*/
    if(type->xid == t_int.xid)
#ifdef COLOR
      fprintf(stdout, "\033[1m%i\033[1m", *(m_uint*)(shred->reg));
#else
      fprintf(stdout, "%i", *(m_uint*)(shred->reg));
#endif
    else if(type->xid == t_float.xid || type->xid == t_dur.xid || isa(type, &t_time) > 0)
#ifdef COLOR
      fprintf(stdout, "\033[1m%.6f\033[0m", *(m_float*)(shred->reg));
#else
      fprintf(stdout, "%.6f", *(m_float*)(shred->reg));
#endif
    else if(type->xid == t_complex.xid)
    {
#ifdef COLOR
      fprintf(stdout, "\033[33m#(\033[0m\033[1m%.4f\033[0m\033[33m, \033[0m\033[1m%.4f\033[0m\033[33m)\033[0m", 
#else
      fprintf(stdout, "#(%.4f, %.4f)", 
#endif
        creal(*(complex*)(shred->reg)), 
        cimag(*(complex*)(shred->reg)));    
    }
    else if(type->xid == t_polar.xid)
    {
#ifdef COLOR
      fprintf(stdout, "\033[33m%%(\033[0m\033[1m%.4f\033[0m\033[33m, \033[0m\033[1m%.4f\033[36m*\033[34mpi\033[0m\033[33m)\033[0m", 
#else
      fprintf(stdout, "%%(%.4f, %.4f*pi)", 
#endif
        creal(*(complex*)(shred->reg)), 
        cimag(*(complex*)(shred->reg))/ M_PI);    
    }
    else if(type->xid == t_string.xid)
#ifdef COLOR
      fprintf(stdout, "\033[1m%s\033[0m", *(M_Object*)(shred->reg) ? STRING(*(M_Object*)(shred->reg)) : NULL);    
#else
      fprintf(stdout, "%s", *(M_Object*)(shred->reg) ? STRING(*(M_Object*)(shred->reg)) : NULL);    
#endif
    else if(type->xid == t_void.xid)
      fprintf(stdout, "(void)");
    else if(type->xid == t_function.xid)
/*      fprintf(stdout, "%p %s", type, type->name, type->func ? type->func->name : "");*/
      fprintf(stdout, "%s %p %p", type->name, *(Func*)(shred->reg),
      (*(Func*)(shred->reg))->code);
    else if(isa(type, &t_func_ptr) > 0)
      fprintf(stdout, "%p %s  %p", type, type->name, *(Func*)(shred->reg));
/*      fprintf(stdout, "%s %p %p", type->name, *(Func*)(shred->reg),*/
/*      *(m_uint*)(shred->reg) ? (*(Func*)(shred->reg))->code : 0);*/
    else if(type->xid == t_class.xid)
      fprintf(stdout, "\033[1m%s\033[0m", type->actual_type->name);
    else
      fprintf(stdout, "%p", *(m_uint*)(shred->reg));
#ifdef COLOR
    fprintf(stdout, "\033[0m\n");
#else
    fprintf(stdout, "\n");
#endif
    shred->reg += type->size;

  }
}

/* function */
struct Vararg
{
  Kindof* k; // k(ind)
  char* d;   // d(ata)
  m_uint o, i, s;  // o(ffset)
};
INSTR(MkVararg)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Make Vararg %i %p", instr->m_val, instr->m_val2); 
#endif
  shred->reg -= instr->m_val;
  m_uint i;
  Vector kinds = (Vector)instr->m_val2;
  struct Vararg* arg = malloc(sizeof(struct Vararg));
  if(instr->m_val)
    arg->d = malloc(instr->m_val);
  else arg->d = NULL;
  arg->s = vector_size(kinds);
  arg->k = calloc(arg->s, sizeof(Kindof));
  for(i = 0; i < arg->s; i++)
  {
    arg->k[i] = (Kindof)vector_at(kinds, i);
    printf("arg->k[i] %i\n", arg->k[i]);
  }
  arg->o = 0;
  arg->i = 0;
  memcpy(arg->d, shred->reg, instr->m_val);
  printf("vararg: %p %i\n", arg, vector_size(kinds));
  free_Vector(kinds);
  *(struct Vararg**)(shred->reg)= arg;
  shred->reg += SZ_INT;
}

void Spork(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Spork"); 
#endif
  m_uint i, this_ptr = 0;
  VM_Code code;
  Func func;

  shred->reg -= SZ_INT;
  code = *(VM_Code*)shred->reg;
  VM_Shred sh = new_VM_Shred(code);
  sh->name = code->name;
  sh->parent = shred;
  if(!shred->child)
    shred->child = new_Vector();
  vector_append(shred->child, sh);
  sh->base = shred->base;
  sh->me = new_Shred(vm, sh);
  sh->me->ref++;
  vm_add_shred(vm, sh);
  shred->reg -= SZ_INT;
  func = *(Func*)shred->reg;

  if(!func) // HACK
  {
    func = (Func)instr->m_val2;
/*    exit(89);*/
  }
  
  if(func->is_member)
  {
    shred->reg -= SZ_INT;
    this_ptr = *(m_uint*)shred->reg;
    // add to shred so it's ref counted, and released when shred done (1.3.1.2)
/*    sh->add_parent_ref( (Chuck_Object *)this_ptr );*/
  }
  shred->reg -= instr->m_val;
  memcpy(sh->reg, shred->reg, instr->m_val);
/*  if(instr->ptr)*/
/*    memcpy(sh->reg, shred->reg, sizeof(shred->reg));*/
  if(instr->ptr)
    memcpy(sh->mem, shred->mem, (m_uint)instr->ptr);
  sh->reg += instr->m_val;
  if(func->is_member)
  {
    *(m_uint*)sh->reg = this_ptr;
    sh->reg += SZ_INT;
  }
  if(instr->ptr && code->need_this) // sporked function
    *(m_uint*)sh->mem= this_ptr;
  *(Func*)sh->reg = func;
  sh->reg += SZ_INT;
  *(M_Object*)shred->reg = sh->me;
  shred->reg += SZ_INT;
}
#define overflow_(c)       ( c >  ( c + (0x1 << 16)) - ((0x1 << 16) / 16))
void handle_overflow(VM_Shred shred)
{
  fprintf( stderr,
    "[chuck](VM): StackOverflow: shred[id=%lu:%s], PC=[%lu]\n",
    shred->xid, shred->name, shred->pc );
  // do something!
  shred->is_running = 0;
  shred->is_done = 1;
}
void Instr_Func_Call(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "func call"); 
#endif
  m_uint i;
  VM_Code func;
  Vector _instr;
  m_uint local_depth, stack_depth, prev_stack = 0, push, next;

  shred->reg -= SZ_INT*2;
  func = *(VM_Code*)shred->reg;
  stack_depth = func->stack_depth;
  local_depth = *(m_uint*)(shred->reg + SZ_INT);
  prev_stack = *(m_uint*)(shred->mem - SZ_INT) == 65553 ? 0 : *(m_uint*)(shred->mem - SZ_INT); 
  push = prev_stack + local_depth;
  next = shred->pc + 1;
  m_uint* mem = (m_uint*)shred->mem;
  mem += (prev_stack + local_depth)/SZ_INT;
  *mem = push; mem++;
  *mem = (m_uint)shred->code; mem++;
  *mem = next; mem++;
  *mem = stack_depth; mem++;
  shred->next_pc = 0;
  shred->code = func;
  printf("stack_depth %i\n", stack_depth);
  if(stack_depth)
  {
    if(stack_depth == 7 || stack_depth == 5 || stack_depth == 6)
      stack_depth = SZ_INT;
    shred->reg -= stack_depth;
    if(func->need_this)
    {
      *mem = *(m_uint*)(shred->reg + stack_depth - SZ_INT); 
      mem++;
      stack_depth -= SZ_INT;
    }

    for(i = 0; i < stack_depth/SZ_INT; i++)
      *(mem + i) = *(m_uint*)(shred->reg + SZ_INT*i);
  }

  shred->mem += prev_stack + local_depth;
  shred->mem += 4 * SZ_INT;
  if(overflow_(shred->mem))
    handle_overflow(shred);
  return;
}

void Dot_Static_Func(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "dot static func %s", ((Func)instr->m_val)->name); 
#endif
  *(m_uint*)(shred->reg - SZ_INT) = instr->m_val;
}

/*
void member_function(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "string function %p [%i]", *(M_Object*)(shred->reg - SZ_INT), instr->m_val); 
#endif
  shred->reg -= SZ_INT;
  M_Object obj = *(M_Object*)shred->reg;
  if(!obj)
    goto error;
  *(m_uint**)shred->reg = vector_at((Type)instr->ptr, instr->m_val);
  shred->reg += SZ_INT;
  return;

error:
  fprintf( stderr,
     "[chuck](VM): NullPointerException: shred[id=%lu:%s], PC=[%lu]\n",
     shred->xid, shred->name, shred->pc );

  // do something!
  shred->is_running = 0;
  shred->is_done = 1;
}
*/
void Dot_Member_Func(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "dot member func"); 
  debug_msg("instr", "dot member func %p [%i]", *(M_Object*)(shred->reg - SZ_INT), instr->m_val); 
#endif
  shred->reg -= SZ_INT;
  M_Object obj = *(M_Object*)shred->reg;
  if(!obj)
    Except(shred);
  *(m_uint**)shred->reg = vector_at(obj->vtable, instr->m_val);
  shred->reg += SZ_INT;
}

void Instr_Func_Call_Static(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "func call static"); 
#endif

  f_sfun f;
  m_uint i, local_depth, stack_depth;
  VM_Code func;
  DL_Return retval;
  
  shred->reg -= SZ_INT*2;
  func        = *(VM_Code*)shred->reg;
  f           = (f_sfun)func->native_func;
  local_depth = *(m_uint*)(shred->reg + SZ_INT);
  stack_depth = func->stack_depth;
  shred->mem += local_depth;
  if(stack_depth)
  {
    shred->reg -=  stack_depth;
    for(i = 0; i < stack_depth/SZ_INT; i++)
      *(m_uint*)(shred->mem + SZ_INT + SZ_INT*i) = *(m_uint*)(shred->reg + SZ_INT*i);
  }
  if(overflow_(shred->mem))
  {
    handle_overflow(shred);
    return;
  }
  f(&retval, shred);
  dl_return_push(retval, shred, instr->m_val);
  shred->mem -= local_depth;
}

void Instr_Func_Call_Member(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "func call member");
#endif

  m_uint i;
  DL_Return retval;
  VM_Code func;
  m_uint local_depth, stack_depth;
  
  shred->reg -= SZ_INT*2;
  func = *(VM_Code*)shred->reg;
  local_depth =   *(m_uint*)(shred->reg + SZ_INT);
  stack_depth = func->stack_depth;
  shred->mem += local_depth;
  if(stack_depth)
  {
    shred->reg -= stack_depth;
    if(func->need_this)
    {
      *(m_uint*)shred->mem = *(m_uint*)(shred->reg - SZ_INT + stack_depth);
      shred->mem += SZ_INT;
      stack_depth -= SZ_INT;
    }
    for(i = 0; i < stack_depth/SZ_INT; i++)
      *(m_uint*)(shred->mem + SZ_INT*i) = *(m_uint*)(shred->reg + SZ_INT*i);
  }

  if(func->need_this)
    shred->mem -= SZ_INT;
    
  if(overflow_(shred->mem))
  {
    handle_overflow(shred);
    return;
  }
  if(func->native_func_type == NATIVE_CTOR)
  {
    f_ctor f = (f_ctor)func->native_func;
    f(*(M_Object*)shred->mem, shred);
  }
  else
  {
    f_mfun f = (f_mfun)func->native_func;
    f((*(M_Object*)shred->mem), &retval, shred);
    dl_return_push(retval, shred, instr->m_val);
  }
  shred->mem -= local_depth;
} 

void Func_Return(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "func return"); 
#endif
  shred->mem -= SZ_INT * 2;
  shred->next_pc = *(m_uint*)shred->mem;
  shred->mem -= SZ_INT;
  VM_Code func = *(VM_Code*)shred->mem;
  shred->mem -= SZ_INT;
  shred->mem -= *(m_uint*)shred->mem;
  shred->code = func;
}

/* object */
static void call_pre_constructor(VM * vm, VM_Shred shred, VM_Code pre_ctor, m_uint stack_offset)
{
  *(m_uint*)shred->reg = *(m_uint*)(shred->reg - SZ_INT); // ref dup last
  shred->reg += SZ_INT;
  *(VM_Code*)shred->reg = pre_ctor;
  shred->reg += SZ_INT;
  *(m_uint*)shred->reg = stack_offset;
  shred->reg += SZ_INT;
  if(pre_ctor->native_func_type != NATIVE_UNKNOWN)
    Instr_Func_Call_Member(vm, shred, NULL);
  else
    Instr_Func_Call(vm, shred, NULL);
}

void Reg_Push_This(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "push this %p", *(m_uint*)(shred->mem));
#endif
	*(m_uint*)shred->reg = *(m_uint*)shred->mem;
  shred->reg += SZ_INT;
}

void Pre_Constructor(VM * vm, VM_Shred shred, Instr instr)
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
  shred->reg += SZ_INT;
#ifdef DEBUG_INSTR
  debug_msg("instr", "instantiate object (internal)%p %s", object, type->name); 
#endif
  return;

error:
  shred->is_running = 0;
  shred->is_done = 1;
}

void Instantiate_Object(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "instantiate object %p", instr->ptr);
#endif
  instantiate_object(vm, shred, instr->ptr);
}

void Alloc_Member_Word(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "alloc member word: %p[%i] = '%p'", *(m_uint*)(shred->mem -1), instr->m_val, *(m_uint*)shred->mem);
#endif
  M_Object obj = *(M_Object*)shred->mem;
  *(m_uint*)(obj->data + instr->m_val) = 0;
  *(m_uint**)shred->reg = &*(m_uint*)(obj->data + instr->m_val);
  shred->reg += SZ_INT;
}

void Alloc_Member_Word_Float(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "alloc member float: %p[%i] = '%p'", *(m_uint*)(shred->mem -1), instr->m_val, *(m_uint*)shred->mem);
#endif
  M_Object obj = *(M_Object*)shred->mem;
  *(m_float*)(obj->data + instr->m_val) = 0.0;
  *(m_float**)shred->reg = &*(m_float*)(obj->data + instr->m_val);
  shred->reg += SZ_INT;
}

void Alloc_Member_Word_Complex(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "alloc member complex: %p[%i] = '%p'", *(m_uint*)(shred->mem -1), instr->m_val, *(m_uint*)shred->mem);
#endif
  M_Object obj = *(M_Object*)shred->mem;
  *(complex*)(obj->data + instr->m_val) = 0.0;
  *(complex**)shred->reg = &*(complex*)(obj->data + instr->m_val);
  shred->reg += SZ_INT;
}

void Dot_Static_Data(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "Dot STATIC DATA: [%i] (%i) (emit:%i)", instr->m_val, instr->m_val2, instr->ptr);
#endif
  Type t;
  
  shred->reg -= SZ_INT;
  t = *(Type*)shred->reg;
  // take care of emit_addr ? (instr->ptr)
  if(instr->ptr)
  {
    *(m_uint**)shred->reg = &*(m_uint*)(t->info->class_data + instr->m_val);
    shred->reg += SZ_INT;  
  }
  /* take care of Kind (instr->m_val2)*/
  else if(instr->m_val2 == Kindof_Int)
  {
    *(m_uint*)shred->reg = *(m_uint*)(t->info->class_data + instr->m_val);
    shred->reg += SZ_INT;
  }
  else if(instr->m_val2 == Kindof_Float)
  {
printf("dot static data %f", *(m_float*)(t->info->class_data + instr->m_val));
    *(m_float*)shred->reg = *(m_float*)(t->info->class_data + instr->m_val);
    shred->reg += SZ_FLOAT;
  }
  else if(instr->m_val2 == Kindof_Complex)
  {
    *(complex*)shred->reg = *(complex*)(t->info->class_data + instr->m_val);
    shred->reg += SZ_COMPLEX;
  }
}

void Dot_Static_Import_Data(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "Dot STATIC Import DATA: %p", instr->ptr);
#endif
  // take care of emit_addr ? (instr->ptr)
  if(instr->ptr)
  {
    *(m_uint**)shred->reg = &*(m_uint*)instr->m_val;
    shred->reg += SZ_INT;  
  }
  /* take care of Kind (instr->m_val2)*/
  if(instr->m_val2 == Kindof_Int)
  {
    *(m_uint*)shred->reg = *(m_uint*)instr->m_val;
    shred->reg += SZ_INT;
  }
  else if(instr->m_val2 == Kindof_Float)
  {
    *(m_float*)shred->reg = *(m_float*)instr->m_val;
    shred->reg += SZ_FLOAT;
  }
  else if(instr->m_val2 == Kindof_Complex)
  {
    *(complex*)shred->reg = *(complex*)instr->m_val;
    shred->reg += SZ_COMPLEX;
  }
  else if(instr->m_val2 == Kindof_Vec3)
  {
    *(VEC3_T*)shred->reg = *(VEC3_T*)instr->m_val;
    shred->reg += SZ_VEC3;
  }
  else if(instr->m_val2 == Kindof_Vec4)
  {
    *(VEC4_T*)shred->reg = *(VEC4_T*)instr->m_val;
    shred->reg += SZ_VEC4;
  }
}

void Dot_Member_Data(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "dot member data '%p'[%i] (%i) (emit:%i)", *(M_Object*)(shred->reg-SZ_INT), instr->m_val, instr->m_val2, instr->ptr);
#endif

  Type type = instr->ptr;
  m_uint  data;
  M_Object* tmp, obj;

  shred->reg -= SZ_INT;
  obj  = *(M_Object*)shred->reg;
  if(!obj)
    Except(shred);
  // take care of emit_addr ? (instr->ptr)
  if(instr->ptr)
  {
    *(m_uint**)shred->reg = &*(m_uint*)(obj->data + instr->m_val);  
    shred->reg += SZ_INT;
  }
  /* take care of Kind (instr->m_val2)*/
  else if(instr->m_val2 == Kindof_Int)
  {
    *(m_uint*)shred->reg = *(m_uint*)(obj->data + instr->m_val);
    shred->reg += SZ_INT;
  }
  else if(instr->m_val2 == Kindof_Float)
  {
    *(m_float*)shred->reg = *(m_float*)(obj->data + instr->m_val);
    shred->reg += SZ_FLOAT;
  }
  else if(instr->m_val2 == Kindof_Complex)
  {
    *(complex*)shred->reg = *(complex*)(obj->data + instr->m_val);
    shred->reg += SZ_COMPLEX;
  }
  else if(instr->m_val2 == Kindof_Vec3)
  {
    *(VEC3_T*)shred->reg = *(VEC3_T*)instr->m_val;
    shred->reg += SZ_VEC3;
  }
  else if(instr->m_val2 == Kindof_Vec4)
  {
    *(VEC4_T*)shred->reg = *(VEC4_T*)instr->m_val;
    shred->reg += SZ_VEC4;
  }
}

void Release_Object2(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "release object %p %i", *(m_uint*)(shred->mem + instr->m_val), instr->m_val);
#endif
  release(*(M_Object*)(shred->mem + instr->m_val), shred);
}
/* array */
void Instr_Pre_Ctor_Array_Top(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array alloc top");
#endif
  if(*(m_uint*)(shred->reg - SZ_INT * 2) >= *(m_uint*)(shred->reg - SZ_INT))
      shred->next_pc = instr->m_val;
  else
    instantiate_object(vm, shred, (Type)instr->ptr);
}

void Instr_Pre_Ctor_Array_Bottom(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "array alloc bottom");
#endif
  shred->reg -= SZ_INT;
  M_Object obj = *(M_Object*)shred->reg;
  m_uint * array = *(m_uint**)(shred->reg - SZ_INT * 3);
  m_int i = *(m_int*)(shred->reg - SZ_INT * 2);
  *(m_uint*)array[i] = (m_uint)obj;
  (*(m_int*)(shred->reg - SZ_INT * 2))++;
  shred->next_pc = instr->m_val;
}

void Instr_Pre_Ctor_Array_Post(VM * vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "array alloc post");
#endif
  shred->reg -= SZ_INT * 3;
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
  if(capacity >= top)
  {
    base = new_M_Array(type->size, cap);
    if(!base)
      goto out_of_memory;
    if(is_obj && objs)
    {
      for( i = 0; i < cap; i++ )
      {
        objs[*index] = (m_uint)i_vector_addr(base->array, i);
        (*index)++;
      }
    }
    return base;
  }
  base = new_M_Array(SZ_INT, cap);
  if(!base)
    goto out_of_memory;
  for( i = 0; i < cap; i++ )
  {
    next = do_alloc_array(shred, capacity + 1, top, type, is_obj, objs, index );
    if(!next)
      goto error;
    // set that, with ref count
    i_vector_set(base->array, i, (m_uint)next);
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

void Instr_Array_Init(VM* vm, VM_Shred shred, Instr instr) // for litteral array
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "array init");
#endif
  m_uint i;
  VM_Array_Info* info = (VM_Array_Info*)instr->ptr;
  M_Object obj;
  shred->reg -= SZ_INT*info->length;
  obj = new_M_Array(info->type->size, info->length);
  for(i = 0; i < info->length; i++)
    i_vector_set(obj->array, i, *(m_uint*)(shred->reg + SZ_INT*i));
  *(M_Object*)shred->reg = obj;
  shred->reg += SZ_INT;
}

void Instr_Array_Alloc(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR 
  debug_msg("instr", "array alloc");
#endif
  VM_Array_Info* info = (VM_Array_Info*)instr->ptr;
  m_uint ref = 0;
  m_uint num_obj = 0;
  m_int index = 0;
  m_float num = 1.0;
  m_uint* obj_array = NULL;
  m_uint obj_array_size = 0;
  m_uint depth = info->depth;
  
  if(info->is_obj && !info->is_ref)
  {    
    m_int curr = -info->depth;
    m_int top = - 1;
    m_int tmp;
    num_obj = 1;
    num = 1.0;
    while( curr <= top )
    {
      tmp = *(m_int*)(shred->reg + SZ_INT * curr);
      num_obj *= tmp;
      num *= (m_float)tmp;
      curr++;
    }
    if(num_obj > 0)
    {
      obj_array = (m_uint*)calloc(num_obj, sizeof(m_uint));
      if(!obj_array)
        goto out_of_memory;
      obj_array_size = num_obj;
    }
  }
  ref = (m_uint)do_alloc_array(shred, -info->depth, -1, info->type, info->is_obj, obj_array, &index);
  shred->reg -=  SZ_INT * info->depth;
  if(!ref)
    goto error;

  *(m_uint*)shred->reg = ref;
  shred->reg += SZ_INT;
  if(info->is_obj && !info->is_ref)
  {
    *(m_uint**)shred->reg = obj_array; shred->reg += SZ_INT;
    *(m_uint*) shred->reg = 0; shred->reg += SZ_INT;
    *(m_uint*) shred->reg = num_obj; shred->reg += SZ_INT;
  }
  return;

overflow:
  fprintf( stderr, "[chuck](VM): OverFlow: requested array size too big...\n" );
  goto error;
out_of_memory:
  fprintf( stderr, "[chuck](VM): OutOfMemory: while allocating arrays...\n" );
  goto error;
error:
  fprintf( stderr, "[chuck](VM): (note: in shred[id=%lu:%s])\n", shred->xid, shred->name);
  shred->is_running = 0;
  shred->is_done = 1;
}


void Instr_Array_Access(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array access '%p'  (emit: %i) [%i] ", *(m_uint*)(shred->reg - SZ_INT * 2), instr->m_val, instr->m_val2);
#endif
  m_int i = 0;
  M_Object obj;
  shred->reg -= SZ_INT * 2;
  obj = *(M_Object*)shred->reg;
  if(!obj)
    Except(shred);
  i = *(m_int*)(shred->reg + SZ_INT);
  if(i < 0 || i >= m_vector_size(obj->array))
    goto array_out_of_bound;
  // take care of emit_addr (instr->m_val)
  if(instr->m_val)
  {
    if(instr->m_val2 == Kindof_Int)
      *(m_uint**)shred->reg = i_vector_addr(obj->array, i);
    else if(instr->m_val2 == Kindof_Float)
      *(m_float**)shred->reg = f_vector_addr(obj->array, i);
    else if(instr->m_val2 == Kindof_Complex)
      *(complex**)shred->reg = c_vector_addr(obj->array, i);
    else if(instr->m_val2 == Kindof_Vec3)
      *(VEC3_T**)shred->reg = v3_vector_addr(obj->array, i);
    else if(instr->m_val2 == Kindof_Vec3)
      *(VEC4_T**)shred->reg = v4_vector_addr(obj->array, i);
      shred->reg += SZ_INT;
  }
  // take care of kind (instr->m_val2)
  else if(instr->m_val2 == Kindof_Int)
  {
    *(m_uint*)shred->reg = i_vector_at(obj->array, i);
    shred->reg += SZ_INT;
  }
  else if(instr->m_val2 == Kindof_Float)
  {
    *(m_float*)shred->reg = f_vector_at(obj->array, i);
    shred->reg += SZ_FLOAT;
  }
  else if(instr->m_val2 == Kindof_Complex)
  {
    *(complex*)shred->reg = c_vector_at(obj->array, i);
    shred->reg += SZ_COMPLEX;
  }
  else if(instr->m_val2 == Kindof_Vec3)
  {
    *(VEC3_T*)shred->reg = v3_vector_at(obj->array, i);
    shred->reg += SZ_VEC3;
  }
  else if(instr->m_val2 == Kindof_Vec4)
  {
    *(VEC4_T*)shred->reg = v4_vector_at(obj->array, i);
    shred->reg += SZ_VEC4;
  }
  return;

array_out_of_bound:
  fprintf( stderr,
    "[Gwion](VM): ArrayOutofBounds: in shred[id=%lu:%s], PC=[%lu], index=[%ld]\n",
    shred->xid, shred->name, shred->pc, i );
  shred->is_running = 0;
  shred->is_done = 1;
}

void Instr_Array_Access_Multi(VM* vm, VM_Shred shred, Instr instr)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "array access multi");
#endif
  m_int i , j;
  m_float fval = 0;
  shred->reg -= SZ_INT * (instr->m_val + 1);
  M_Object obj = NULL, *base = (M_Object*)shred->reg;
  obj = *base;
  if(!obj)
    Except(shred);
  for(j = 0; j < instr->m_val - 1; j++)
  {
    i = *(m_int*)(shred->reg + SZ_INT * (j + 1));
    if(i < 0 || i >= m_vector_size(obj->array))
      goto array_out_of_bound;
    obj = (M_Object)i_vector_at(obj->array, i);
    if(!obj)
      Except(shred);
  }
  i = *(m_int*)(shred->reg + SZ_INT * (j + 1));
  // take care of emit_addr (instr->ptr)
  if(instr->ptr)
  {
    if(instr->m_val2 == Kindof_Int)
      *(m_uint**)shred->reg = i_vector_addr(obj->array, i);
    if(instr->m_val2 == Kindof_Float)
      *(m_float**)shred->reg = f_vector_addr(obj->array, i);
    if(instr->m_val2 == Kindof_Complex)
      *(complex**)shred->reg = c_vector_addr(obj->array, i);
      shred->reg += SZ_INT;
  }
  // take care of kind (instr->m_val2)
  else if(instr->m_val2 == Kindof_Int)
  {
    *(m_uint*)shred->reg = i_vector_at(obj->array, i);
    shred->reg += SZ_INT;
  }
  else if(instr->m_val2 == Kindof_Float)
  {
    *(m_float*)shred->reg = f_vector_at(obj->array, i);
    shred->reg += SZ_FLOAT;
  }
  else if(instr->m_val2 == Kindof_Complex)
  {
    *(complex*)shred->reg = c_vector_at(obj->array, i);
    shred->reg += SZ_COMPLEX;
  }
  else if(instr->m_val2 == Kindof_Vec3)
  {
    *(VEC3_T*)shred->reg = v3_vector_at(obj->array, i);
    shred->reg += SZ_VEC3;
  }
  else if(instr->m_val2 == Kindof_Vec4)
  {
    *(VEC4_T*)shred->reg = v4_vector_at(obj->array, i);
    shred->reg += SZ_VEC4;
  }
  return;

array_out_of_bound:
  fprintf( stderr,
    "[Gwion](VM): ArrayOutofBounds: in shred[id=%lu:%s], PC=[%lu], index=[%ld]\n",
    shred->xid, shred->name, shred->pc, i );
    shred->is_running = 0;
    shred->is_done = 1;
}
