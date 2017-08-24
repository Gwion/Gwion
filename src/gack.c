#include "type.h"
#include "instr.h"
#include "func.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif

#ifdef COLOR
#define BOLD  "\033[1m"
#define CLEAR "\033[0m"
#define GREEN "\033[32m"
#define BLUE  "\033[34m"
#else
#define BOLD  ""
#define CLEAR ""
#define GREEN ""
#define BLUE  ""
#endif

static m_uint get_longest(VM_Shred shred, Vector v) {
  m_uint i, len, longest = 0;
  m_uint size = vector_size(v);
  for(i = 0; i < size; i++) {
    Type type = (Type)vector_at(v, i);
    if(type->xid == te_function && GET_FLAG(type->d.func, ae_flag_member))
      POP_REG(shred, SZ_INT);
    POP_REG(shred,  type->size);
    len = strlen(type->name);
    if(len > longest)
      longest = len;
  }
  return longest;
}

static void print_type(Type type, m_uint longest) {
  m_uint j;
  m_bool is_func = isa(type, &t_function) > 0 && isa(type, &t_func_ptr) < 0;
  m_str name = is_func ? strdup("@function") : strdup(type->name);
  fprintf(stdout, "%s(%s%s%s%s", BOLD, GREEN, name, CLEAR, BOLD);
  for(j = 0; j < type->array_depth; j++)
    fprintf(stdout, "[]");
  fprintf(stdout, ")%s ", CLEAR);
  for(j = 0; j < longest - (name ? strlen(name) : 0); j++)
    fprintf(stdout, " ");
  free(name);
}

static void print_int(m_int i) {
      fprintf(stdout, "%s%li%s", BOLD, i, CLEAR);
}

static void print_float(m_float f) {
      fprintf(stdout, "%s%.4f%s", BOLD, f, CLEAR);
}

static void print_complex(m_complex c) {
  printf("#(");
  print_float(creal(c));
  print_float(cimag(c));
  printf(")");
}

static void print_polar(m_complex c) {
  printf("%%(");
  print_float(creal(c));
  print_float(cimag(c));
  printf("*pi)");
}

static void print_vec(char* f, m_uint size) {
  m_uint i;
  printf("@(");
  for(i = 0; i < size; i++)
    print_float(creal(*(m_float*)(f + i * SZ_FLOAT)));
  printf(")");
}

static void print_string(M_Object obj) {
  fprintf(stdout, "%s%s%s", BOLD, obj ? STRING(obj) : "(null string)", CLEAR);
}

INSTR(Gack) {
  Type type;
  Vector v = *(Vector*)instr->ptr;
  m_uint i, size = vector_size(v);
  m_uint longest = get_longest(shred, v);
  for(i = size + 1; --i;) {
#ifdef DEBUG
    fprintf(stdout, "%s%s[DEBUG]%s [%s%lu%s] ", BOLD, BLUE, CLEAR,
        BOLD, shred->xid, CLEAR);
#endif
    type = (Type)vector_at(v, size - i);
    print_type(type, longest);
    if(isa(type, &t_int) > 0)
      print_int(*(m_int*)REG(0));
    else if(type->xid == te_float || type->xid == te_dur ||
            type->xid == te_time  || type->xid == te_now)
      print_float(*(m_float*)REG(0));
    else if(type->xid == te_complex)
      print_complex(*(m_complex*)REG(0));
    else if(type->xid == te_polar)
      print_polar(*(m_complex*)REG(0));
    else if(type->xid == te_vec3)
      print_vec(REG(0), 3);
    else if(type->xid == te_vec4)
      print_vec(REG(0), 4);
    else if(isa(type, &t_string) > 0)
      print_string(*(M_Object*)REG(0));
    else if(type->xid == te_void)
      fprintf(stdout, "(%svoid%s)", BOLD, CLEAR);
    else if(type->xid == te_function)
        fprintf(stdout, "%s%s%s", BOLD, type->name, CLEAR);
    else if(isa(type, &t_func_ptr) > 0)
      fprintf(stdout, "%p %s  %p", (void*)type, type->name, (void*) * (Func*)REG(0));
    else if(type->xid == te_class)
      fprintf(stdout, "%s%s%s", BOLD, type->d.actual_type->name, CLEAR);
    else
      fprintf(stdout, "%p", (void*) * (M_Object*)REG(0));
    fprintf(stdout, "%s\n", CLEAR);
    PUSH_REG(shred,  type->size);
  }
}
