#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <complex.h>
#include <math.h>
#include "defs.h"
#include "err_msg.h"
#include "type.h"
#include "instr.h"
#include "import.h"

struct Type_ t_string = { "string", SZ_INT, &t_object };
m_int o_string_data;

static void push_string(VM_Shred shred, M_Object obj, m_str c) {
  STRING(obj) = s_name(insert_symbol(c));
  *(M_Object*)REG(0) = (M_Object)obj;
  PUSH_REG(shred, SZ_INT);
  release(obj, shred);
}

static void push_new_string(VM_Shred shred, m_str c) {
  M_Object obj = new_String(shred, c);
  *(M_Object*)REG(0) = (M_Object)obj;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(rhs && rhs->data) { // assigning with chuck a empty ref
    release(lhs, shred);
    release(rhs, shred);
    STRING(rhs) = lhs ? STRING(lhs) : NULL;
  } else
    **(M_Object**)REG(SZ_INT) = lhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(String_eq) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  *(m_int*)REG(0) = (lhs && rhs && !strcmp(STRING(lhs), STRING(rhs))) || (!lhs && !rhs);
  PUSH_REG(shred, SZ_INT);
  release(lhs, shred);
  release(rhs, shred);
}

static INSTR(String_neq) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  *(m_int*)REG(0) = (lhs && rhs && strcmp(STRING(lhs), STRING(rhs))) ||
    (lhs && !rhs) || (!lhs && rhs);
  PUSH_REG(shred, SZ_INT);
  release(lhs, shred);
  release(rhs, shred);
}

static INSTR(Int_String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs)
    Except(shred, "NullStringException.");
  char str[num_digit(labs(lhs)) + strlen(STRING(rhs)) + 2];
  sprintf(str, "%" INT_F "", lhs);
  push_string(shred, rhs, str);
}

static INSTR(Float_String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_FLOAT);
  char str[num_digit((m_uint)lhs) + 6];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, ".4%f", lhs);
  push_string(shred, rhs, str);
}

static INSTR(Complex_String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex lhs = *(m_complex*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_COMPLEX);
  char str[num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 16];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%.4f, %.4f)", creal(lhs), cimag(lhs));
  push_string(shred, rhs, str);
}

static INSTR(Polar_String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex lhs = *(m_complex*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_COMPLEX);
  char str[num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 16];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%f, %f)",  creal(lhs),
          cimag(lhs) / M_PI);
  push_string(shred, rhs, str);
}

static INSTR(Vec3_String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3 lhs = *(m_vec3*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_VEC3);
  char str[num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + 23];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z);
  push_string(shred, rhs, str);
}

static INSTR(Vec4_String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4 lhs = *(m_vec4*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_VEC4);
  char str[num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + 30];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%.4f, %.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z, lhs.w);
  push_string(shred, rhs, str);
}

static INSTR(Object_String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs)
    Except(shred, "NullStringException");

  char str[16];
  sprintf(str, "0x%08" UINT_F, (uintptr_t)lhs);
  push_string(shred, rhs, str);
  release(lhs, shred);
}

static INSTR(String_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  char str[(lhs ? strlen(STRING(lhs)) : 0) + (rhs ? strlen(STRING(rhs)) : 0) + 1];
  sprintf(str, "%s%s", lhs ? STRING(lhs) : NULL, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
  release(lhs, shred);
}

static INSTR(Int_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  char str[num_digit(lhs) + (rhs ? strlen(STRING(rhs)) : 0) + 1];
  sprintf(str, "%" INT_F "%s", lhs, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Float_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_FLOAT);
  char str[num_digit((m_uint)lhs) + 5 + (rhs ? strlen(STRING(rhs)) : 0) + 1];
  sprintf(str, "%.4f%s", lhs, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Complex_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex  lhs = *(m_complex*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_COMPLEX);
  char str[num_digit(creal(lhs)) + num_digit(cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12];
  sprintf(str, "#(%.4f, %.4f)%s", creal(lhs), cimag(lhs), rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Polar_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex  lhs = *(m_complex*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_COMPLEX);
  char str[num_digit(creal(lhs)) + num_digit(cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12];
  sprintf(str, "%%(%f, %f)%s", creal(lhs), cimag(lhs) / M_PI, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Vec3_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3  lhs = *(m_vec3*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_VEC3);
  char str[(rhs ? strlen(STRING(rhs)) : 0) + 23 + num_digit((m_uint)lhs.x) +
                                      num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) ];
  sprintf(str, "@(%.4f, %.4f, %.4f)%s", lhs.x, lhs.y, lhs.z, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Vec4_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4  lhs = *(m_vec4*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_VEC4);
  char str[(rhs ? strlen(STRING(rhs)) : 0) + 28 + num_digit((m_uint)lhs.x) +
                                      num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + num_digit((m_uint)lhs.w)];
  sprintf(str, "@(%f, %f, %f, %f)%s", lhs.x, lhs.y, lhs.z, lhs.w, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
}

static INSTR(Object_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = *(M_Object*)REG(SZ_INT);
  char str[11 + (rhs ? strlen(STRING(rhs)) : 0)];
  sprintf(str, "0x%08" UINT_F "%s", (uintptr_t)lhs, rhs ? STRING(rhs) : NULL);
  push_new_string(shred, str);
  release(rhs, shred);
  release(lhs, shred);
}

static INSTR(String_Plus) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs) { // TODO release
    rhs = lhs;
    *(M_Object*)REG(0) = rhs;
    release(lhs, shred);
    PUSH_REG(shred, SZ_INT);
    return;
  }
  m_uint l_len = strlen(STRING(lhs));
  m_uint r_len = strlen(STRING(rhs));
  char c[l_len + r_len + 1];
  sprintf(c, "%s%s", STRING(rhs), lhs ? STRING(lhs) : NULL);
  push_string(shred, rhs, c);
  release(lhs, shred);
}

static INSTR(Int_String_Plus) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs)
    Except(shred, "NullStringException.");
  m_uint len = strlen(STRING(rhs)) + 1;
  char c[len + num_digit(lhs) + 1];
  sprintf(c, "%s%" INT_F "", STRING(rhs), lhs);
  push_string(shred, rhs, c);
}

static INSTR(Float_String_Plus) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_FLOAT);
  m_uint len = (rhs ? STRING(rhs) ? strlen(STRING(rhs)) : 0 : 0) + 1 + 7;
  if(!rhs)
    Except(shred, "NullStringException");
  char c[len + num_digit(lhs) + 6];
  sprintf(c, "%s%.4f", STRING(rhs), lhs);
  push_string(shred, rhs, c);
}

static INSTR(Complex_String_Plus) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_COMPLEX);
  if(!rhs)
    Except(shred, "NullStringException")
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13;
  char c[len + num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 18];
  sprintf(c, "%s#(%.4f, %.4f)", STRING(rhs), creal(lhs), cimag(lhs));
  push_string(shred, rhs, c);
}

static INSTR(Polar_String_Plus) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_float lhs = *(m_float*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_COMPLEX);
  if(!rhs)
    Except(shred, "NullStringException")
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13;
  char c[len + num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 18];
  sprintf(c, "%s%%(%.4f, %.4f)", STRING(rhs), creal(lhs), cimag(lhs) / M_PI);
  push_string(shred, rhs, c);
}

static INSTR(Vec3_String_Plus) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3 lhs = *(m_vec3*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_VEC3);
  if(!rhs)
    Except(shred, "NullStringException")
  char c[num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + strlen(STRING(rhs)) + 20];
  sprintf(c, "%s#(%.4f, %.4f, %.4f)", STRING(rhs), lhs.x, lhs.y, lhs.z);
  push_string(shred, rhs, c);
}

static INSTR(Vec4_String_Plus) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4 lhs = *(m_vec4*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_VEC4);
  if(!rhs)
    Except(shred, "NullStringException")
  char c[num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + num_digit(lhs.z) + strlen(STRING(rhs)) + 28];
  sprintf(c, "%s#(%.4f, %.4f, %.4f, %.4f)", STRING(rhs), lhs.x, lhs.y, lhs.z, lhs.w);
  push_string(shred, rhs, c);
}

static INSTR(Object_String_Plus) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)REG(0);
  M_Object rhs = **(M_Object**)REG(SZ_INT);
  if(!rhs) {
    if(lhs)
      release(rhs, shred);
    Except(shred, "NullStringException")
  }
  m_uint len = strlen(STRING(rhs)) + 11;
  char c[len + 1];
  c[len] = '\0';
  sprintf(c, "%s0x%08" UINT_F, STRING(rhs), (uintptr_t)lhs);
  push_string(shred, rhs, c);
  release(lhs, shred);
}

INSTR(Reg_Push_Str) { GWDEBUG_EXE
  *(M_Object*)REG(0) = new_String(shred, (m_str)instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

static CTOR(string_ctor) {
  STRING(o) = "this is the default string";
}

static MFUN(string_len) {
  *(m_uint*)RETURN = strlen(STRING(o));
}

static MFUN(string_upper) {
  m_uint i;
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(i = 0; i < strlen(c); i++)
    if(c[i]  >= 'a' && c[i] <= 'z')
      c[i] += 'A' - 'a';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_lower) {
  m_uint i;
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(i = 0; i < strlen(c); i++)
    if(c[i]  >= 'A' && c[i] <= 'Z')
      c[i] -= 'A' - 'a';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_ltrim) {
  m_uint i = 0;
  m_str str = STRING(o);
  while(str[i] == ' ')
    i++;
  char c[strlen(str) - i + 1];
  strcpy(c, STRING(o) + i);
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_rtrim) {
  m_str str = STRING(o);
  m_uint len = strlen(str) - 1;
  while(str[len] == ' ')
    len--;
  char c[len + 2];
  strncpy(c, str, len + 1);
  c[len + 1] = '\0';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_trim) {
  m_str str = STRING(o);
  m_int i, start = 0, end = 0, len = 0;
  while(str[len] != '\0')
    len++;
  for(i = 0; i < len; i++) {
    if(str[i] == ' ')
      start++;
    else break;
  }
  for(i = len - 1; i >= 0; i--) {
    if(str[i] == ' ')
      end++;
    else break;
  }
  if(len - start - end <= 0) {
    *(m_uint*)RETURN = 0;
    return;
  }
  char c[len - start - end + 1];
  for(i = start; i < len - end; i++)
    c[i - start] = str[i];
  c[len - start - end ] = '\0';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_charAt) {
  m_str str = STRING(o);
  m_int i = *(m_int*)MEM(SZ_INT), len = 0;
  while(str[len] != '\0')
    len++;
  if(i < 0 || i >= len)
    *(m_uint*)RETURN = -1;
  else
    *(m_uint*)RETURN = str[i];
}

static MFUN(string_setCharAt) {
  m_str str = STRING(o);
  m_int i = *(m_int*)MEM(SZ_INT), len = 0;
  m_int c = *(m_int*)MEM(SZ_INT * 2);
  while(str[len] != '\0')
    len++;
  if(i < 0 || i >= len)
    *(m_uint*)RETURN = -1;
  else {
    str[i] = c;
    STRING(o) = s_name(insert_symbol(str));
    *(m_uint*)RETURN = c;
  }
}

static MFUN(string_substring) {
  m_uint i, len = 0;
  m_int  index = *(m_int*)MEM(SZ_INT);
  m_str str = STRING(o);
  while(str[len] != '\0')
    len++;
  if(!len || index > len || (len - index + 1) <= 0) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char c[len - index + 1];
  memset(c, 0, len - index + 1);
  for(i = index; i < len; i++)
    c[i - index] = str[i];
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_substringN) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, (STRING(o)));
  m_int i, len = 0, index = *(m_int*)MEM(SZ_INT);
  m_int end = *(m_int*)MEM(SZ_INT * 2);
  while(str[len] != '\0')
    len++;
  if(end > len) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  len -= end;
  char c[len - index + 1];
  for(i = index; i < len; i++)
    c[i - index] = str[i];
  c[i - index] = '\0';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_insert) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, len_insert = 0, index = *(m_int*)MEM(SZ_INT);
  M_Object arg = *(M_Object*)MEM(SZ_INT * 2);

  if(!arg) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  while(str[len] != '\0')
    len++;
  len_insert =  strlen(insert);
  char c[len + len_insert + 1];
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < len_insert; i++)
    c[i + index] = insert[i];
  for(i = index; i < len; i++)
    c[i + len_insert] = str[i];
  c[len + len_insert] = '\0';
  release(arg, shred);
  *(M_Object*)RETURN = new_String(shred, c);;
}

static MFUN(string_replace) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, len_insert = 0, index = *(m_int*)MEM(SZ_INT);
  M_Object arg = *(M_Object*)MEM(SZ_INT * 2);
  if(!arg) {
    *(M_Object*)RETURN = o;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  len = strlen(str);
  len_insert =  strlen(insert);
  if(index >= len  || index < 0 || len < 0 || (index + len_insert + 1) <= 0) {
    release(arg, shred);
    *(M_Object*)RETURN = NULL;
    return;
  }
  char c[index + len_insert + 1];
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < len_insert; i++)
    c[i + index] = insert[i];
  c[index + len_insert] = '\0';
  release(arg, shred);
  *(M_Object*)RETURN = new_String(shred, c);;
}

static MFUN(string_replaceN) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, index = *(m_int*)MEM(SZ_INT);
  M_Object arg = *(M_Object*)MEM(SZ_INT * 3);
  m_int _len = *(m_int*)MEM(SZ_INT * 2);
  if(!arg || index > strlen(STRING(o)) || _len > strlen(STRING(arg))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  memset(insert, 0, len + 1);
  strcpy(insert, STRING(arg));
  len = strlen(str);
  str[len] = '\0';
  if(index > len)
    index = len - 1;
  char c[len + _len];
  memset(c, 0, len + _len);
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < _len; i++)
    c[i + index] = insert[i];
  for(i = index + _len; i < len; i++)
    c[i] = str[i];
  c[len + _len - 1] = '\0';
  release(arg, shred);
  *(M_Object*)RETURN = new_String(shred, c);;
}

static MFUN(string_find) {
  m_str str = STRING(o);
  m_int i = 0, ret = -1;
  char arg = *(m_int*)MEM(SZ_INT);
  while(str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i++;
  }
  *(m_uint*)RETURN = ret;
}

static MFUN(string_findStart) {
  m_str str = STRING(o);
  char pos = *(m_int*)MEM(SZ_INT);
  char arg = *(m_int*)MEM(SZ_INT * 2);
  m_int i = pos, ret = -1;
  if(!strlen(str)) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  while(str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i++;
  }
  *(m_uint*)RETURN = ret;
}

static MFUN(string_findStr) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_str arg = STRING(obj);
  m_int len  = strlen(str);
  m_int i = 0;
  m_int arg_len = strlen(arg);
  while(i < len) {
    if(!strncmp(str + i, arg, arg_len)) {
      ret = i;
      break;
    }
    i++;
  }
  release(obj, shred);
  *(m_uint*)RETURN = ret;
}

static MFUN(string_findStrStart) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  m_int start = *(m_int*)MEM(SZ_INT);
  M_Object obj = *(M_Object*)MEM(SZ_INT * 2);
  if(!obj) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  m_str arg = STRING(obj);
  m_int len  = strlen(str);
  m_int i = start;
  m_int arg_len = strlen(arg);
  while(i < len) {
    if(!strncmp(str + i, arg, arg_len)) {
      ret = i;
      break;
    }
    i++;
  }
  release(obj, shred);
  *(m_uint*)RETURN = ret;
}

static MFUN(string_rfind) {
  m_str str = STRING(o);
  m_int i = strlen(str) - 1, ret = -1;
  char arg = *(m_int*)MEM(SZ_INT);
  while(i > -1 && str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i--;
  }
  *(m_uint*)RETURN = ret;
}

static MFUN(string_rfindStart) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  char pos = *(m_int*)MEM(SZ_INT);
  char arg = *(m_int*)MEM(SZ_INT * 2);
  m_int i = pos, ret = -1;
  while(i > 0 && str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i--;
  }
  *(m_uint*)RETURN = ret;
}

static MFUN(string_rfindStr) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  M_Object obj = *(M_Object*)MEM(SZ_INT);
  m_str arg = STRING(o);
  m_int len  = strlen(str);
  m_int i = len - 1;
  m_int arg_len = strlen(arg);
  while(i) {
    if(!strncmp(str + i, arg, arg_len)) {
      ret = i;
      break;
    }
    i--;
  }
  release(obj, shred);
  *(m_uint*)RETURN = ret;
}

static MFUN(string_rfindStrStart) {
  if(!strlen(STRING(o))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  m_int start = *(m_int*)MEM(SZ_INT);
  M_Object obj = *(M_Object*)MEM(SZ_INT * 2);
  if(!obj) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_str arg = STRING(obj);

  m_int i = start;
  m_int arg_len = strlen(arg);
  while(i > -1) {
    if(!strncmp(str + i, arg, arg_len)) {
      ret = i;
      break;
    }
    i--;
  }
  release(obj, shred);
  *(m_uint*)RETURN = ret;
}

static MFUN(string_erase) {
  m_str str = STRING(o);
  m_int i;
  m_int start = *(m_int*)MEM(SZ_INT);
  m_int rem = *(m_int*)MEM(SZ_INT * 2);
  m_int len = strlen(str);
  m_int size = len - rem + 1;
  if(start >= len || size <= 0) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char c[size];
  c[size - 1] = '\0';
  for(i = 0; i < start; i++)
    c[i] = str[i];
  for(i = start + rem; i < len; i++)
    c[i - rem] = str[i];
  STRING(o) = s_name(insert_symbol(c));
}

static MFUN(string_toInt) {
  m_str endptr;
  *(m_uint*)RETURN = strtol(STRING(o), &endptr, 10);
}

static MFUN(string_toFloat) {
  *(m_float*)RETURN = atof(STRING(o));
}

m_bool import_string(Gwi gwi) {
  CHECK_BB(gwi_class_ini(gwi,  &t_string, string_ctor, NULL))

	gwi_item_ini(gwi, "int", "@data");
  o_string_data = gwi_item_end(gwi,   ae_flag_const, NULL);
  CHECK_BB(o_string_data)

  gwi_func_ini(gwi, "int", "size", string_len);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "upper", string_upper);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "lower", string_lower);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "ltrim", string_ltrim);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "rtrim", string_rtrim);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "trim", string_trim);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "charAt", string_charAt);
  gwi_func_arg(gwi, "int", "pos");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "charAt", string_setCharAt);
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "int", "char");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "substring", string_substring);
  gwi_func_arg(gwi, "int", "start");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "substring", string_substringN);
  gwi_func_arg(gwi, "int", "start");
  gwi_func_arg(gwi, "int", "end");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "insert", string_insert);
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "string", "string");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "replace", string_replace);
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "string", "string");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "string", "replace", string_replaceN);
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "int", "n");
  gwi_func_arg(gwi, "string", "string");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "find", string_find);
  gwi_func_arg(gwi, "int", "char");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "find", string_findStart);
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "int", "char");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "find", string_findStr);
  gwi_func_arg(gwi, "string", "str");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "find", string_findStrStart);
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "string", "str");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "rfind", string_rfind);
  gwi_func_arg(gwi, "int", "char");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "rfind", string_rfindStart);
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "int", "char");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "rfind", string_rfindStr);
  gwi_func_arg(gwi, "string", "str");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "rfind", string_rfindStrStart);
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "string", "str");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "void",   "erase", string_erase);
  gwi_func_arg(gwi, "int", "start");
  gwi_func_arg(gwi, "int", "length");
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "int", "toInt", string_toInt);
  CHECK_BB(gwi_func_end(gwi, 0))

  gwi_func_ini(gwi, "float", "toFloat", string_toFloat);
  CHECK_BB(gwi_func_end(gwi, 0))

  CHECK_BB(gwi_class_end(gwi))
  CHECK_BB(gwi_oper_ini(gwi, "string",  "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       String_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "string",  "string", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq,       String_eq))
  CHECK_BB(gwi_oper_end(gwi, op_neq,       String_neq))

  CHECK_BB(gwi_oper_ini(gwi, "int",     "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Int_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Int_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Int_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "float",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Float_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Float_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Float_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "complex", "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Complex_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Complex_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Complex_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "polar",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Polar_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Polar_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Polar_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi,"Vec3",     "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Vec3_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Vec3_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Vec3_String_Plus))
  
  CHECK_BB(gwi_oper_ini(gwi, "Vec4",    "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Vec4_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Vec4_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Vec4_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "Object",  "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Object_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Object_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Object_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "@null",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Object_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Object_String))
  CHECK_BB(gwi_oper_add(gwi, opck_rassign))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Object_String_Plus))
  return 1;
}
