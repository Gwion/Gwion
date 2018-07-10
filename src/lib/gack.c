#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "type.h"
#include "err_msg.h"
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
  m_bool is_func = isa(type, t_function) > 0 && isa(type, t_func_ptr) < 0;
  m_str name;
  name = is_func ? strdup("@function") : strdup(type->name);
  gw_out(BOLD "(" GREEN "%s" CLEAR BOLD, name);
  gw_out(") " CLEAR);
  free(name);
  if(GET_FLAG(type, ae_flag_typedef)) {
    gw_out(" aka ");
    print_type(type->parent);
  }
}

static void print_int(m_int i) {
  gw_out(BOLD "%" INT_F "" CLEAR , i);
}

static void print_float(m_float f) {
  gw_out(BOLD "%.4f" CLEAR, f);
}


static void print_complex_inner(m_complex c) {
  print_float(creal(c));
  gw_out(", ");
  print_float(cimag(c));
}

static void print_complex(m_complex c) {
  gw_out("#(");
  print_complex_inner(c);
  gw_out(")");
}

static void print_polar(m_complex c) {
  gw_out("%%(");
  print_complex_inner(c);
  gw_out("*pi)");
}

static void print_vec(m_bit* f, m_uint size) {
  m_uint i;
  gw_out("@(");
  for(i = 0; i < size; i++) {
    print_float(creal(*(m_float*)(f + i * SZ_FLOAT)));
    if(i < size - 1)
      gw_out(", ");
  }
  gw_out(")");
}

static void print_string1(m_str str) {
  gw_out(BOLD "%s" CLEAR, str);
}

static void print_string(M_Object obj) {
  print_string1(obj ? STRING(obj) : "(null string)");
}

static void print_object(Type type, M_Object obj) {
  if(isa(type, t_string) > 0)
    print_string(obj);
  else
    gw_out(BOLD "%p" CLEAR, (void*)obj);
}

static void print_func(Type type, m_bit* stack) {
    if(isa(type, t_func_ptr) > 0)
      gw_out(BOLD "%p" CLEAR, (void*) * (Func*)stack);
    else
      gw_out(BOLD "%s" CLEAR, type->name);
}

static void print_prim(Type type, m_bit* stack) {
    if(isa(type, t_int) > 0)
      print_int(*(m_int*)stack);
    else if(isa(type, t_complex) > 0)
      print_complex(*(m_complex*)stack);
    else if(isa(type, t_polar) > 0)
      print_polar(*(m_complex*)stack);
    else if(isa(type, t_vec3) > 0)
      print_vec(stack, 3);
    else if(isa(type, t_vec4) > 0)
      print_vec(stack, 4);
    else
     print_float(*(m_float*)stack);
}

INSTR(Gack) { GWDEBUG_EXE
  Type type;
  Vector v = *(Vector*)instr->ptr;
  m_uint i, size = vector_size(v);
  pop(shred, v);
  for(i = size + 1; --i;) {
    type = (Type)vector_at(v, size - i);
    if(size == 1 && isa(type, t_class) < 0)
      print_type(type);
    if(isa(type, t_object) > 0)
      print_object(type, *(M_Object*)REG(0));
    else if(isa(type, t_function) > 0)
      print_func(type, REG(0));
    else if(isa(type, t_class) > 0)
        print_type(type->d.base_type);
    else if(isa(type, t_void) > 0)
      print_string1("void");
    else
      print_prim(type, REG(0));
    if(i > 1)
      gw_out(", " CLEAR);
    PUSH_REG(shred,  type->size);
  }
  gw_out(CLEAR"\n");
}
