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

ANN static void push_string(const VM_Shred shred, const M_Object obj, const m_str c) {
  STRING(obj) = s_name(insert_symbol(c));
  *(M_Object*)REG(-SZ_INT) = (M_Object)obj;
  _release(obj, shred);
}

ANN static void push_new_string(const VM_Shred shred, const m_str c) {
  const M_Object obj = new_String(shred, c);
  *(M_Object*)REG(-SZ_INT) = (M_Object)obj;
}

#define describe_string_logical(name, action)    \
static INSTR(String_##name) { GWDEBUG_EXE        \
  POP_REG(shred, SZ_INT);                        \
  const M_Object lhs = *(M_Object*)REG(-SZ_INT); \
  const M_Object rhs = *(M_Object*)REG(0);       \
  *(m_int*)REG(-SZ_INT) = action;                \
  release(lhs, shred);                           \
  release(rhs, shred);                           \
}
describe_string_logical(eq, (lhs && rhs && !strcmp(STRING(lhs), STRING(rhs))) || (!lhs && !rhs))
describe_string_logical(neq, (lhs && rhs && strcmp(STRING(lhs), STRING(rhs))) ||
    (lhs && !rhs) || (!lhs && rhs))

#define describe_string_assign(name, type, offset, opt, len, format, ...) \
static INSTR(name##String_Assign) { GWDEBUG_EXE                      \
  POP_REG(shred, offset);                                            \
  const type lhs = *(type*)REG(-SZ_INT);                             \
  const M_Object rhs = *(M_Object*)REG(offset - SZ_INT);             \
  opt;                                                               \
  if(!rhs)                                                           \
    Except(shred, "NullStringException.");                           \
  char str[(len)];                                                   \
  sprintf(str, format, __VA_ARGS__);                                 \
  push_string(shred, rhs, str);                                     \
}

static INSTR(String_Assign) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const M_Object lhs = *(M_Object*)REG(-SZ_INT);
  const M_Object rhs = *(M_Object*)REG(0);
  release(lhs, shred);
  if(!rhs)
    Except(shred, "NullStringException.");
  push_string(shred, rhs, lhs ? STRING(lhs) : "");
}

describe_string_assign(Int_, m_int, SZ_INT,,
  num_digit(labs(lhs)) + 1,
  "%" INT_F "", lhs)
describe_string_assign(Float_, m_float, SZ_FLOAT,,
  num_digit((m_uint)lhs) + 5,
  "%.4f", lhs)
describe_string_assign(Complex_, m_complex, SZ_COMPLEX,,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 16,
  "#(%.4f, %.4f)", creal(lhs), cimag(lhs))
describe_string_assign(Polar_, m_complex, SZ_COMPLEX,,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 16,
  "#(%.4f, %.4f)", creal(lhs), cimag(lhs)/M_PI)
describe_string_assign(Vec3_, m_vec3, SZ_VEC3,,
  num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + 23,
  "#(%.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z)
describe_string_assign(Vec4_, m_vec4, SZ_VEC4,,
  num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + num_digit(lhs.w) + 30,
  "#(%.4f, %.4f, %.4f, %.4f)", lhs.x, lhs.y, lhs.z, lhs.w)
describe_string_assign(Object_, M_Object, SZ_INT, release(lhs, shred),
  16,
  "%p", (void*)lhs)

static INSTR(String_String) { GWDEBUG_EXE
  POP_REG(shred, SZ_INT);
  const M_Object lhs = *(M_Object*)REG(-SZ_INT);
  const M_Object rhs = *(M_Object*)REG(0);
  char str[(lhs ? strlen(STRING(lhs)) : 0) + (rhs ? strlen(STRING(rhs)) : 0) + 1];
  sprintf(str, "%s%s", lhs ? STRING(lhs) : "", rhs ? STRING(rhs) : "");
  push_new_string(shred, str);
  release(rhs, shred);
  release(lhs, shred);
}

#define describe_string(name, type, offset, len, opt, format, ...) \
static INSTR(name##_String) { GWDEBUG_EXE \
  POP_REG(shred, offset); \
  const type lhs = *(type*)REG(-SZ_INT);\
  const M_Object rhs = *(M_Object*)REG(offset-SZ_INT);\
  char str[(len)];\
  sprintf(str, format, __VA_ARGS__);\
  push_new_string(shred, str);\
  opt; \
  release(rhs, shred);\
}
describe_string(Int, m_int, SZ_INT,
  num_digit(lhs) + (rhs ? strlen(STRING(rhs)) : 0) + 1,,
  "%" INT_F "%s", lhs, rhs ? STRING(rhs) : "")
describe_string(Float, m_float, SZ_FLOAT,
  (num_digit((m_uint)lhs) + 5 + (rhs ? strlen(STRING(rhs)) : 0) + 1),,
  "%.4f%s", lhs, rhs ? STRING(rhs) : "")
describe_string(Complex, m_complex, SZ_COMPLEX,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12,,
  "#(%.4f, %.4f)%s", creal(lhs), cimag(lhs), rhs ? STRING(rhs) : "")
describe_string(Polar, m_complex, SZ_COMPLEX,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + (rhs ? strlen(STRING(rhs)) : 0) +  12,,
  "%%(%f, %f*pi)%s", creal(lhs), cimag(lhs) / M_PI, rhs ? STRING(rhs) : "")
describe_string(Vec3, m_vec3, SZ_VEC3,
  (rhs ? strlen(STRING(rhs)) : 0) + 23 + num_digit((m_uint)lhs.x) +
                                      num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z),,
  "@(%.4f, %.4f, %.4f)%s", lhs.x, lhs.y, lhs.z, rhs ? STRING(rhs) : "")
describe_string(Vec4, m_vec4, SZ_VEC4,
  (rhs ? strlen(STRING(rhs)) : 0) + 28 + num_digit((m_uint)lhs.x) +
  num_digit((m_uint)lhs.y) + num_digit((m_uint)lhs.z) + num_digit((m_uint)lhs.w),,
  "@(%f, %f, %f, %f)%s", lhs.x, lhs.y, lhs.z, lhs.w, rhs ? STRING(rhs) : "")
describe_string(Object, M_Object, SZ_INT,
  11 + (rhs ? strlen(STRING(rhs)) : 0), release(lhs, shred),
  "%p%s", (void*)lhs, rhs ? STRING(rhs) : "")


#define describe_string_plus(name, offset, type, opt, len, format, ...) \
static INSTR(name##String_Plus) { GWDEBUG_EXE       \
  POP_REG(shred, offset);                  \
  const type lhs = *(type*)REG(-SZ_INT);                  \
  const M_Object rhs = *(M_Object*)REG(offset - SZ_INT);     \
  opt;                                              \
  if(!rhs)                                          \
    Except(shred, "NullPtrException");              \
  char c[strlen(STRING(rhs)) + (len) + 1];          \
  sprintf(c, "%s"format, STRING(rhs), __VA_ARGS__); \
  push_string(shred, rhs, c);                       \
}
describe_string_plus(,SZ_INT, M_Object, release(lhs, shred),
  strlen(STRING(rhs)), "%s", lhs ? STRING(lhs) : "")
describe_string_plus(Int_, SZ_INT, m_int,,
  num_digit(lhs), "%"INT_F, lhs)
describe_string_plus(Float_, SZ_FLOAT, m_float,,
  num_digit(lhs) + 6, "%s%.4f", STRING(rhs), lhs)
describe_string_plus(Complex_, SZ_COMPLEX, m_complex,,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 18, "#(%.4f, %.4f)",
  creal(lhs), cimag(lhs))
describe_string_plus(Polar_, SZ_COMPLEX, m_complex,,
  num_digit(creal(lhs)) + num_digit(cimag(lhs)) + 18, "%%(%.4f, %.4f)",
  creal(lhs), cimag(lhs) / M_PI)
describe_string_plus(Vec3_, SZ_VEC3, m_vec3,,
  num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + 20, "@(%.4f, %.4f, %.4f)",
  lhs.x, lhs.y, lhs.z)
describe_string_plus(Vec4_, SZ_VEC4, m_vec4,,
  num_digit(lhs.x) + num_digit(lhs.y) + num_digit(lhs.z) + num_digit(lhs.z) + 28, "@(%.4f, %.4f, %.4f, %.4f)",
  lhs.x, lhs.y, lhs.z, lhs.w)
describe_string_plus(Object_, SZ_INT, M_Object, release(lhs, shred),
  11, "%p", (void*)lhs)

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
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(m_uint i = 0; i < strlen(c); i++)
    if(c[i]  >= 'a' && c[i] <= 'z')
      c[i] += 'A' - 'a';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_lower) {
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(m_uint i = 0; i < strlen(c); i++)
    if(c[i]  >= 'A' && c[i] <= 'Z')
      c[i] -= 'A' - 'a';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_ltrim) {
  m_uint i = 0;
  const m_str str = STRING(o);
  while(str[i] == ' ')
    i++;
  char c[strlen(str) - i + 1];
  strcpy(c, STRING(o) + i);
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_rtrim) {
  const m_str str = STRING(o);
  m_uint len = strlen(str) - 1;
  while(str[len] == ' ')
    len--;
  char c[len + 2];
  strncpy(c, str, len + 1);
  c[len + 1] = '\0';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_trim) {
  const m_str str = STRING(o);
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
  const m_str str = STRING(o);
  const m_int i = *(m_int*)MEM(SZ_INT);
  const m_uint len = strlen(str);
  if(i < 0 || (m_uint)i >= len)
    *(m_uint*)RETURN = -1;
  else
    *(m_uint*)RETURN = str[i];
}

static MFUN(string_setCharAt) {
  const m_str str = STRING(o);
  const m_int i = *(m_int*)MEM(SZ_INT);
  const m_int c = *(m_int*)MEM(SZ_INT * 2);
  const m_uint len = strlen(str);
  if(i < 0 || (m_uint)i >= len)
    *(m_uint*)RETURN = -1;
  else {
    str[i] = c;
    STRING(o) = s_name(insert_symbol(str));
    *(m_uint*)RETURN = c;
  }
}

static MFUN(string_substring) {
  m_int  index = *(m_int*)MEM(SZ_INT);
  m_str str = STRING(o);
  const m_uint len = strlen(str);
  if(!len || (m_uint)index > len || (len - index + 1) <= 0) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char c[len - index + 1];
  memset(c, 0, len - index + 1);
  for(m_uint i = index; i < len; i++)
    c[i - index] = str[i];
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_substringN) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, (STRING(o)));
  m_int i, index = *(m_int*)MEM(SZ_INT);
  m_int end = *(m_int*)MEM(SZ_INT * 2);
  m_uint len = strlen(str);
  if(end > (m_int)len) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  len -= end;
  char c[len - index + 1];
  for(i = index; i < (m_int)len; i++)
    c[i - index] = str[i];
  c[i - index] = '\0';
  *(M_Object*)RETURN = new_String(shred, c);
}

static MFUN(string_insert) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len_insert = 0, index = *(m_int*)MEM(SZ_INT);
  const M_Object arg = *(M_Object*)MEM(SZ_INT * 2);

  if(!arg) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  const m_uint len = strlen(str);
  len_insert =  strlen(insert);
  char c[len + len_insert + 1];
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < len_insert; i++)
    c[i + index] = insert[i];
  for(i = index; i < (m_int)len; i++)
    c[i + len_insert] = str[i];
  c[len + len_insert] = '\0';
  release(arg, shred);
  *(M_Object*)RETURN = new_String(shred, c);;
}

static MFUN(string_replace) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len_insert = 0, index = *(m_int*)MEM(SZ_INT);
  const M_Object arg = *(M_Object*)MEM(SZ_INT * 2);
  if(!arg) {
    *(M_Object*)RETURN = o;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  const m_uint len = strlen(str);
  len_insert =  strlen(insert);
  if(index >= (m_int)len  || index < 0 || (index + len_insert + 1) <= 0) {
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
  m_int i, index = *(m_int*)MEM(SZ_INT);
  const M_Object arg = *(M_Object*)MEM(SZ_INT * 3);
  const m_int _len = *(m_int*)MEM(SZ_INT * 2);
  if(!arg || index > (m_int)strlen(STRING(o)) || _len > (m_int)strlen(STRING(arg))) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  const m_uint len = strlen(str);
  memset(insert, 0, len + 1);
  strcpy(insert, STRING(arg));
  str[len] = '\0';
  if(index > (m_int)len)
    index = len - 1;
  char c[len + _len];
  memset(c, 0, len + _len);
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < _len; i++)
    c[i + index] = insert[i];
  for(i = index + _len; i < (m_int)len; i++)
    c[i] = str[i];
  c[len + _len - 1] = '\0';
  release(arg, shred);
  *(M_Object*)RETURN = new_String(shred, c);;
}

static MFUN(string_find) {
  const m_str str = STRING(o);
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
  const m_str str = STRING(o);
  const char pos = *(m_int*)MEM(SZ_INT);
  const char arg = *(m_int*)MEM(SZ_INT * 2);
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
  const M_Object obj = *(M_Object*)MEM(SZ_INT);
  if(!obj) {
    *(m_uint*)RETURN = 0;
    return;
  }
  const m_str arg = STRING(obj);
  const m_int len  = strlen(str);
  m_int i = 0;
  const m_int arg_len = strlen(arg);
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
  const m_int start = *(m_int*)MEM(SZ_INT);
  const M_Object obj = *(M_Object*)MEM(SZ_INT * 2);
  if(!obj) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  const m_str arg = STRING(obj);
  const m_int len  = strlen(str);
  m_int i = start;
  const m_int arg_len = strlen(arg);
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
  const m_str str = STRING(o);
  m_int i = strlen(str) - 1, ret = -1;
  const char arg = *(m_int*)MEM(SZ_INT);
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
  const char pos = *(m_int*)MEM(SZ_INT);
  const char arg = *(m_int*)MEM(SZ_INT * 2);
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
  const M_Object obj = *(M_Object*)MEM(SZ_INT);
  const m_str arg = STRING(o);
  const m_int len  = strlen(str);
  m_int i = len - 1;
  const m_int arg_len = strlen(arg);
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
  const M_Object obj = *(M_Object*)MEM(SZ_INT * 2);
  if(!obj) {
    *(m_uint*)RETURN = 0;
    return;
  }
  m_str arg = STRING(obj);

  m_int i = start;
  const m_int arg_len = strlen(arg);
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
  const m_str str = STRING(o);
  const m_int start = *(m_int*)MEM(SZ_INT);
  const m_int rem = *(m_int*)MEM(SZ_INT * 2);
  const m_int len = strlen(str);
  const m_int size = len - rem + 1;
  if(start >= len || size <= 0) {
    *(M_Object*)RETURN = NULL;
    return;
  }
  char c[size];
  c[size - 1] = '\0';
  for(m_int i = 0; i < start; i++)
    c[i] = str[i];
  for(m_int i = start + rem; i < len; i++)
    c[i - rem] = str[i];
  STRING(o) = s_name(insert_symbol(c));
}

static MFUN(string_toInt) {
  *(m_uint*)RETURN = strtol(STRING(o), NULL, 10);
}

static MFUN(string_toFloat) {
  *(m_float*)RETURN = atof(STRING(o));
}

ANN m_bool import_string(const Gwi gwi) {
  CHECK_OB((t_string = gwi_mk_type(gwi, "string", SZ_INT, t_object)))
  CHECK_BB(gwi_class_ini(gwi,  t_string, string_ctor, NULL))

  gwi_item_ini(gwi, "int", "@data");
  CHECK_BB(gwi_item_end(gwi,   ae_flag_const, NULL))

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
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       String_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "string",  "string", "int"))
  CHECK_BB(gwi_oper_end(gwi, op_eq,       String_eq))
  CHECK_BB(gwi_oper_end(gwi, op_neq,       String_neq))

  CHECK_BB(gwi_oper_ini(gwi, "int",     "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Int_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Int_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Int_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "float",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Float_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Float_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Float_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "complex", "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Complex_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Complex_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Complex_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "polar",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Polar_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Polar_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Polar_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi,"Vec3",     "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Vec3_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Vec3_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Vec3_String_Plus))
  
  CHECK_BB(gwi_oper_ini(gwi, "Vec4",    "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Vec4_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Vec4_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Vec4_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "Object",  "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Object_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Object_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Object_String_Plus))

  CHECK_BB(gwi_oper_ini(gwi, "@null",   "string", "string"))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_chuck,      Object_String_Assign))
  CHECK_BB(gwi_oper_end(gwi, op_plus,       Object_String))
  CHECK_BB(gwi_oper_add(gwi, opck_const_rhs))
  CHECK_BB(gwi_oper_end(gwi, op_plus_chuck, Object_String_Plus))
  return 1;
}

#ifdef JIT
#include "ctrl/string.h"
#include "code/string.h"
#endif
