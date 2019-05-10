#include <stdlib.h>
#include <string.h>
#include <complex.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "type.h"
#include "func.h"
#include "object.h"
#include "instr.h"

static void print_type(const Type type) {
  const m_bool is_func = isa(type, t_function) > 0 && isa(type, t_fptr) < 0;
  const m_str name = is_func ? strdup("@function") : strdup(type->name);
  gw_out("(%s) ", name);
  free(name);
  if(GET_FLAG(type, typedef)) {
    gw_out(" aka ");
    print_type(type->parent);
  }
}

static inline void print_bool(const m_int i) {
  gw_out("%s", i ? "true" : "false");
}

static inline void print_int(const m_int i) {
  gw_out("%" INT_F "", i);
}

static inline void print_float(const m_float f) {
  gw_out("%.4f", f);
}

static inline void print_complex(const m_complex c) {
  gw_out("#(");
  print_float(creal(c));
  gw_out(", ");
  print_float(cimag(c));
  gw_out(")");
}

static inline void print_polar(const m_complex c) {
  gw_out("%%(");
  print_float(creal(c));
  gw_out(", ");
  print_float((m_float)cimag(c) / (m_float)M_PI);
  gw_out("*pi)");
}

ANN static inline void print_vec(const m_bit* f, const m_uint size) {
  gw_out("@(");
  for(m_uint i = 0; i < size; i++) {
    print_float(creal(*(m_float*)(f + i * SZ_FLOAT)));
    if(i < size - 1)
      gw_out(", ");
  }
  gw_out(")");
}

static inline void print_string1(const m_str str) {
  gw_out("%s", str);
}

static inline void print_string(const M_Object obj) {
  print_string1(obj ? STRING(obj) : "(null string)");
}

ANN2(1) static inline void print_object(const Type type, const M_Object obj) {
  if(isa(type, t_string) > 0)
    print_string(obj);
  else
    gw_out("%p", (void*)obj);
}

ANN static inline void print_func(const Type type, const m_bit* stack) {
  if(type->d.func) {
    const VM_Code code = isa(type, t_fptr) > 0 ?
      *(VM_Code*)stack : type->d.func->code;
    gw_out("%s %s %p", type->name, (void*)code ? code->name : NULL, code);
  } else
    gw_out("%s %p", type->name, NULL);
}

ANN static void print_prim(const Type type, const m_bit* stack) {
  if(isa(type, t_bool) > 0)
    print_bool(*(m_int*)stack);
  else if(isa(type, t_int) > 0)
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

ANN void gack(const m_bit* reg, const Instr instr) {
  m_uint offset = instr->m_val;
  const Vector v = (Vector)instr->m_val2;
  const m_uint size = vector_size(v);
  for(m_uint i = size + 1; --i;) {
    const Type type = (Type)vector_at(v, size - i);
    if(size == 1)
      print_type(type);
    if(isa(type, t_object) > 0)
      print_object(type, *(M_Object*)(reg-offset));
    else if(isa(type, t_function) > 0)
      print_func(type, (reg-offset));
    else if(type == t_class)
      print_type(type);
    else if(isa(type, t_class) > 0)
      print_type(type->d.base_type);
    else if(isa(type, t_void) > 0)
      print_string1("void");
    else
      print_prim(type, (reg-offset));
    offset -= type->size;
  }
  gw_out("\n");
}
