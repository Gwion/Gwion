#include <stdlib.h>
#include <string.h>
#include <math.h>
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

static void print_type(Type type) {
  const m_bool is_func = isa(type, t_function) > 0 && isa(type, t_func_ptr) < 0;
  const m_str name = is_func ? strdup("@function") : strdup(type->name);
  gw_out(BOLD "(" GREEN "%s" CLEAR BOLD, name);
  gw_out(") " CLEAR);
  free(name);
  if(GET_FLAG(type, ae_flag_typedef)) {
    gw_out(" aka ");
    print_type(type->parent);
  }
}

static inline void print_int(m_int i) {
  gw_out(BOLD "%" INT_F "" CLEAR , i);
}

static inline void print_float(m_float f) {
  gw_out(BOLD "%.4f" CLEAR, f);
}

#define describe_cmp(name, imag)\
static void print_##name(m_complex c) {\
  gw_out("#(");\
  print_float(creal(c));\
  gw_out(", ");\
  imag;\
  gw_out(")");\
}
describe_cmp(complex, print_float(cimag(c)))
describe_cmp(polar, print_float(cimag(c)/M_PI))

static void print_vec(m_bit* f, m_uint size) {
  gw_out("@(");
  for(m_uint i = 0; i < size; i++) {
    print_float(*(m_float*)(f + i * SZ_FLOAT));
    if(i < size - 1)
      gw_out(", ");
  }
  gw_out(")");
}

static inline void print_string1(m_str str) {
  gw_out(BOLD "%s" CLEAR, str);
}

static inline void print_string(M_Object obj) {
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
  else if(isa(type, t_float) > 0)
   print_float(*(m_float*)stack);
  else if(isa(type, t_complex) > 0)
    print_complex(*(m_complex*)stack);
  else if(isa(type, t_polar) > 0)
    print_polar(*(m_complex*)stack);
  else if(isa(type, t_vec3) > 0)
    print_vec(stack, 3);
  else
    print_vec(stack, 4);
}

INSTR(Gack) { GWDEBUG_EXE
  const Vector v = *(Vector*)instr->ptr;
  const m_uint size = instr->m_val2;
  POP_REG(shred, instr->m_val);
  m_uint offset = 0;
  for(m_uint i = size + 1; --i;) {
    const Type type = (Type)vector_at(v, size - i);
    if(isa(type, t_object) > 0)
      print_object(type, *(M_Object*)REG(offset));
    else if(isa(type, t_function) > 0)
      print_func(type, REG(offset));
    else if(isa(type, t_class) > 0)
      print_type(type->d.base_type);
    else if(isa(type, t_void) > 0)
      print_string1("void");
    else
      print_prim(type, REG(offset));
    gw_out(" " CLEAR);
    offset += type->size;
  }
  if(instr->m_val2 == 1) {
    gw_out(" :");
    print_type((Type)vector_front(v));
  }
  gw_out(CLEAR"\n");
}

#ifdef JIT
#include "code/gack.h"
#endif
