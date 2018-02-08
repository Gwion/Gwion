#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "type.h"
#include "instr.h"

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

static void pop(VM_Shred shred, Vector v) {
  m_uint i, size = vector_size(v);
  for(i = 0; i < size; i++) {
    Type type = (Type)vector_at(v, i);
    POP_REG(shred,  type->size);
  }
}

static void print_type(Type type) {
  m_bool is_func = isa(type, &t_function) > 0 && isa(type, &t_func_ptr) < 0;
  m_str name;
  name = is_func ? strdup("@function") : strdup(type->name);
  fprintf(stdout, BOLD "(" GREEN "%s" CLEAR BOLD, name);
  fprintf(stdout, ") " CLEAR);
  free(name);
  if(GET_FLAG(type, ae_flag_typedef) && !GET_FLAG(type, ae_flag_unary)) {
    fprintf(stdout, " aka ");
    print_type(type->parent);
  }
}

static void print_int(m_int i) {
  fprintf(stdout, BOLD "%" INT_F "" CLEAR , i);
}

static void print_float(m_float f) {
  fprintf(stdout, BOLD "%.4f" CLEAR, f);
}


static void print_complex_inner(m_complex c) {
  print_float(creal(c));
  fprintf(stdout, ", ");
  print_float(cimag(c));
}

static void print_complex(m_complex c) {
  fprintf(stdout, "#(");
  print_complex_inner(c);
  fprintf(stdout, ")");
}

static void print_polar(m_complex c) {
  fprintf(stdout, "%%(");
  print_complex_inner(c);
  fprintf(stdout, "*pi)");
}

static void print_vec(char* f, m_uint size) {
  m_uint i;
  fprintf(stdout, "@(");
  for(i = 0; i < size; i++) {
    print_float(creal(*(m_float*)(f + i * SZ_FLOAT)));
    if(i < size - 1)
      fprintf(stdout, ", ");
  }
  fprintf(stdout, ")");
}

static void print_string1(m_str str) {
  fprintf(stdout, BOLD "%s" CLEAR, str);
}

static void print_string(M_Object obj) {
  print_string1(obj ? STRING(obj) : "(null string)");
}

static void print_object(Type type, M_Object obj) {
  if(isa(type, &t_string) > 0)
    print_string(obj);
  else
    fprintf(stdout, BOLD "%p" CLEAR, (void*)obj);
}

static void print_func(Type type, char* stack) {
    if(isa(type, &t_func_ptr) > 0)
      fprintf(stdout, BOLD "%p" CLEAR, (void*) * (Func*)stack);
    else
      fprintf(stdout, BOLD "%s" CLEAR, type->name);
}

static void print_prim(Type type, char* stack) {
    if(isa(type, &t_int) > 0)
      print_int(*(m_int*)stack);
    else if(type->xid == te_complex)
      print_complex(*(m_complex*)stack);
    else if(type->xid == te_polar)
      print_polar(*(m_complex*)stack);
    else if(type->xid == te_vec3)
      print_vec(stack, 3);
    else if(type->xid == te_vec4)
      print_vec(stack, 4);
    else
     print_float(*(m_float*)stack);
}

INSTR(Gack) {
  Type type;
  Vector v = *(Vector*)instr->ptr;
  m_uint i, size = vector_size(v);
  pop(shred, v);
  for(i = size + 1; --i;) {
    type = (Type)vector_at(v, size - i);
    if(size == 1 && type->xid != te_class)
      print_type(type);
    if(isa(type, &t_object) > 0)
      print_object(type, *(M_Object*)REG(0));
    else if(isa(type, &t_function) > 0)
      print_func(type, REG(0));
    else if(type->xid == te_class)
        print_type(type->d.base_type);
    else if(type->xid == te_void)
      print_string1("void");
    else
      print_prim(type, REG(0));
    if(i > 1)
      fprintf(stdout, ", " CLEAR);
    PUSH_REG(shred,  type->size);
  }
  fprintf(stdout, CLEAR"\n");
}
