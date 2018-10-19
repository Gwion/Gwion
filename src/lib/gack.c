#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include "type.h"
#include "object.h"
#include "err_msg.h"
#include "instr.h"

/* static */ void print_type(const Type type) {
  const m_bool is_func = isa(type, t_function) > 0 && isa(type, t_fptr) < 0;
  const m_str name = is_func ? strdup("@function") : strdup(type->name);
  gw_out("(%s) ", name);
  free(name);
  if(GET_FLAG(type, ae_flag_typedef)) {
    gw_out(" aka ");
    print_type(type->parent);
  }
}

/* static inline */ void print_int(const m_int i) {
  gw_out("%" INT_F "", i);
}

/* static inline */void print_float(const m_float f) {
  gw_out("%.4f", f);
}

/* static inline */void print_complex(const m_complex c) {
  gw_out("#(");
  print_float(creal(c));
  gw_out(", ");
  print_float(cimag(c));
  gw_out(")");
}

/*static inline */void print_polar(const m_complex c) {
  gw_out("%%(");
  print_float(creal(c));
  gw_out(", ");
  print_float(cimag(c) / M_PI);
  gw_out("*pi)");
}

/* static inline */ void print_vec(const m_bit* f, const m_uint size) {
  gw_out("@(");
  for(m_uint i = 0; i < size; i++) {
    print_float(creal(*(m_float*)(f + i * SZ_FLOAT)));
    if(i < size - 1)
      gw_out(", ");
  }
  gw_out(")");
}

/* static inline */void print_string1(const m_str str) {
  gw_out("%s", str);
}

/*static inline */void print_string(const M_Object obj) {
  print_string1(obj ? STRING(obj) : "(null string)");
}

/*static inline */void print_object(const Type type, const M_Object obj) {
  if(isa(type, t_string) > 0)
    print_string(obj);
  else
    gw_out("%p", (void*)obj);
}

/* static inline */ void print_func(const Type type, const m_bit* stack) {
  const Func func = isa(type, t_fptr) > 0 ?
    *(Func*)stack : type->d.func;
  gw_out("%s %p", type->name, (void*)func);
}

/*static */void print_prim(const Type type, const m_bit* stack) {
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
  const Vector v = *(Vector*)instr->ptr;
  const m_uint size = vector_size(v);
  m_uint offset = instr->m_val;
  for(m_uint i = size + 1; --i;) {
    const Type type = (Type)vector_at(v, size - i);
    if(size == 1)
      print_type(type);
    if(isa(type, t_object) > 0)
      print_object(type, *(M_Object*)REG(-offset));
    else if(isa(type, t_function) > 0)
      print_func(type, REG(-offset));
    else if(isa(type, t_class) > 0)
      print_type(type->d.base_type);
    else if(isa(type, t_void) > 0)
      print_string1("void");
    else
      print_prim(type, REG(-offset));
    offset -= type->size;
  }
  gw_out("\n");
}
