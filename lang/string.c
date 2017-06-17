#include <math.h>
#include "defs.h"
#include "err_msg.h"
#include "import.h"

struct Type_ t_string = { "string", SZ_INT, &t_object, te_string };
m_int o_string_data;

static INSTR(String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "string => string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  if(rhs && rhs->d.data) { // assigning with chuck a empty ref
    release(lhs, shred);
    release(rhs, shred);
    STRING(rhs) = lhs ? STRING(lhs) : NULL;
  } else
    **(M_Object**)(shred->reg + SZ_INT) = lhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(Int_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '=>' string");
#endif
  POP_REG(shred, SZ_INT*2);
  m_int lhs = *(m_int*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  if(!rhs)
    Except(shred, "NullStringException.");
  char str[1024];
  memset(str, 0, 1024);
  sprintf(str, "%li", lhs);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Float_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_FLOAT);
  char str[1024];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "%f", lhs);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Complex_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Complex '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex lhs = *(m_complex*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_COMPLEX);
  char str[1024];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%f, %f)", creal(lhs), cimag(lhs));
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Polar_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Polar '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex lhs = *(m_complex*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_COMPLEX);
  char str[1024];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%f, %f)",  creal(lhs),
      cimag(lhs) / M_PI);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Vec3_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec3 '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3 lhs = *(m_vec3*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_VEC3);
  char str[1024];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%f, %f, %f)", lhs.x, lhs.y, lhs.z);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Vec4_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec4 '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4 lhs = *(m_vec4*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_VEC4);
  char str[1024];
  if(!rhs)
    Except(shred, "NullStringException.");
  sprintf(str, "#(%f, %f, %f, %f)", lhs.x, lhs.y, lhs.z, lhs.w);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Object_String_Assign) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Object '=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  if(!rhs) {
    Except(shred, "NullStringException");
  }
  char str[12];
  str[11] = '\0';
  sprintf(str, "0x%08lu", (uintptr_t)lhs);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
  release(lhs, shred);
}

static INSTR(String_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "string '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  char str[(lhs ? strlen(STRING(lhs)) : 0) + (rhs ? strlen(STRING(rhs)): 0) + 1];
  sprintf(str, "%s%s", lhs ? STRING(lhs) : NULL , rhs ? STRING(rhs) : NULL);
  *(M_Object*)shred->reg = new_String(shred, str);
  PUSH_REG(shred, SZ_INT);
  release(lhs, shred);
  release(rhs, shred);
}

static INSTR(Int_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  char str[1024];
  sprintf(str, "%li%s", lhs, rhs ? STRING(rhs): NULL);
  *(M_Object*)shred->reg = new_String(shred,str);
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Float_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_FLOAT);
  char str[1024];
  sprintf(str, "%f%s", lhs, rhs ? STRING(rhs) : NULL);
  *(M_Object*)shred->reg = new_String(shred,str);
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Complex_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "complex '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex  lhs = *(m_complex*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_COMPLEX);
  char str[1024];
  sprintf(str, "#(%f, %f)%s", creal(lhs), cimag(lhs), rhs ? STRING(rhs) : NULL);
  *(M_Object*)shred->reg = new_String(shred,str);
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Polar_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "polar '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex  lhs = *(m_complex*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_COMPLEX);
  char str[1024];
  sprintf(str, "%%(%f, %f)%s", creal(lhs), cimag(lhs) / M_PI, rhs ? STRING(rhs) : NULL);
  *(M_Object*)shred->reg = new_String(shred,str);
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Vec3_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec3 '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3  lhs = *(m_vec3*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_VEC3);
  char str[1024];
  sprintf(str, "@(%f, %f, %f)%s", lhs.x, lhs.y, lhs.z, rhs ? STRING(rhs) : NULL);
  *(M_Object*)shred->reg = new_String(shred, str);
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Vec4_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec4 '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4  lhs = *(m_vec4*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_VEC4);
  char str[1024];
  sprintf(str, "@(%f, %f, %f, %f)%s", lhs.x, lhs.y, lhs.z, lhs.w, rhs ? STRING(rhs) : NULL);
  *(M_Object*)shred->reg = new_String(shred, str);
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Object_String) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Object '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  char str[11 + strlen(STRING(rhs))];
  sprintf(str, "0x%08lu%s", (uintptr_t)lhs, rhs ? STRING(rhs) : NULL);
  *(M_Object*)shred->reg = new_String(shred,str);
  PUSH_REG(shred, SZ_INT);
  release(lhs, shred);
  release(rhs, shred);
}

static INSTR(String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "string '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  if(!rhs) { // TODO release
    rhs = lhs;
    *(M_Object*)shred->reg = rhs;
    release(lhs, shred);
    PUSH_REG(shred, SZ_INT);
    return;
  }
  m_uint l_len = strlen(STRING(lhs));
  m_uint r_len = strlen(STRING(rhs));
  char c[l_len + r_len + 1];
  sprintf(c, "%s%s", STRING(rhs), lhs ? STRING(lhs) : NULL);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Int_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  if(!rhs)
    Except(shred, "NullStringException.");
  m_uint len = strlen(STRING(rhs)) + 1;
  m_uint tmp = labs(lhs);
  while(tmp /= 10)
    len++;
  char c[len];
  sprintf(c, "%s%li", STRING(rhs), lhs);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Float_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_FLOAT);
  m_uint len = (rhs ? strlen(STRING(rhs)) : 0) + 1 + 7;
  m_uint tmp = fabs(lhs);
  if(!rhs)
    Except(shred,"NullStringException");
  while(tmp /= 10)
    len++;
  char c[len];
  sprintf(c, "%s%f", STRING(rhs), lhs);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Complex_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "complex '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_COMPLEX);
  if(rhs)
    Except(shred,"NullStringException")
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13;
  m_uint tmp = fabs(creal(lhs));
  while(tmp /= 10)
    len++;
  tmp = fabs(cimag(lhs));
  while(tmp /= 10)
    len++;
  char c[len];
  sprintf(c, "%s#(%f, %f)", STRING(rhs), creal(lhs), cimag(lhs));
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Polar_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "polar '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_COMPLEX);
  if(rhs)
    Except(shred,"NullStringException")
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13;
  m_uint tmp = fabs(creal(lhs));
  while(tmp /= 10)
    len++;
  tmp = fabs(cimag(lhs));
  while(tmp /= 10)
    len++;
  char c[len];
  sprintf(c, "%s%%(%f, %f)", STRING(rhs), creal(lhs), cimag(lhs) / M_PI);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Vec3_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec3 '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC3);
  m_vec3 lhs = *(m_vec3*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_VEC3);
  if(rhs)
    Except(shred,"NullStringException")
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13 + 2;
  m_uint tmp = fabs(lhs.x);
  while(tmp /= 10)
    len++;
  tmp = fabs(lhs.y);
  while(tmp /= 10)
    len++;
  tmp = fabs(lhs.z);
  while(tmp /= 10)
    len++;
  char c[len];
  sprintf(c, "%s#(%f, %f, %f)", STRING(rhs), lhs.x, lhs.y, lhs.z);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Vec4_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec4 '+=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC4);
  m_vec4 lhs = *(m_vec4*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_VEC4);
  if(rhs)
    Except(shred,"NullStringException")
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13 + 4;
  m_uint tmp = fabs(lhs.x);
  while(tmp /= 10)
    len++;
  tmp = fabs(lhs.y);
  while(tmp /= 10)
    len++;
  tmp = fabs(lhs.z);
  while(tmp /= 10)
    len++;
  tmp = fabs(lhs.w);
  while(tmp /= 10)
    len++;
  len += 6*4; // 31/03/17
  char c[len];
  sprintf(c, "%s#(%f, %f, %f, %f)", STRING(rhs), lhs.x, lhs.y, lhs.z, lhs.w);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Object_String_Plus) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "Object '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  if(rhs)
    Except(shred,"NullStringException")
  m_uint len = (rhs ? strlen(STRING(rhs)) : 0 ) + 11;
  char c[len+1];
  c[len] = '\0';
  //  sprintf(c, "%s%p", STRING(rhs), (void*)lhs);
  sprintf(c, "%s0x%08lu", rhs ? STRING(rhs) : "" , (uintptr_t)lhs);
  if(rhs)
    STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(lhs, shred);
  release(rhs, shred);
}

INSTR(Reg_Push_Str) {
#ifdef DEBUG_INSTR
  debug_msg("instr", "push string %s", (m_str)instr->m_val);
#endif
  *(M_Object*)shred->reg = new_String(shred,(m_str)instr->m_val);
  PUSH_REG(shred, SZ_INT);
}

static CTOR(string_ctor) {
  STRING(o) = "this is the default string";
}

static MFUN(string_len) {
  RETURN->d.v_uint = strlen(STRING(o));
}

static MFUN(string_upper) {
  m_uint i;
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(i = 0; i < strlen(c); i++)
    if(c[i]  >= 'a' && c[i] <= 'z')
      c[i] += 'A' - 'a';
  RETURN->d.v_object = new_String(shred, c);
}

static MFUN(string_lower) {
  m_uint i;
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(i = 0; i < strlen(c); i++)
    if(c[i]  >= 'A' && c[i] <= 'Z')
      c[i] -= 'A' - 'a';
  RETURN->d.v_object = new_String(shred, c);
}

static MFUN(string_ltrim) {
  m_uint i = 0;
  m_str str = STRING(o);
  while(str[i] == ' ')
    i++;
  char c[strlen(str) - i + 1];
  strcpy(c, STRING(o) + i);
  RETURN->d.v_object = new_String(shred,c);
}

static MFUN(string_rtrim) {
  m_str str = STRING(o);
  m_uint len = strlen(str) -1;
  while(str[len] == ' ')
    len--;
  char c[len + 2];
  strncpy(c, str, len+1);
  c[len+1] = '\0';
  RETURN->d.v_object = new_String(shred, c);
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
    RETURN->d.v_uint = 0;
    return;
  }
  char c[len - start - end + 1];
  for(i = start; i < len - end; i++)
    c[i - start] = str[i];
  c[len - start - end ] = '\0';
  RETURN->d.v_object = new_String(shred, c);
}

static MFUN(string_charAt) {
  m_str str = STRING(o);
  m_int i = *(m_int*)(shred->mem + SZ_INT), len = 0;
  while(str[len] != '\0')
    len++;
  if(i < 0 || i >= len)
    RETURN->d.v_uint = -1;
  else
    RETURN->d.v_uint = str[i];
}

static MFUN(string_setCharAt) {
  m_str str = STRING(o);
  m_int i = *(m_int*)(shred->mem + SZ_INT), len = 0;
  m_int c = *(m_int*)(shred->mem + SZ_INT * 2);
  while(str[len] != '\0')
    len++;
  if(i < 0 || i >= len)
    RETURN->d.v_uint = -1;
  else {
    str[i] = c;
    STRING(o) = S_name(insert_symbol(str));
    RETURN->d.v_uint = c;
  }
}

static MFUN(string_substring) {
  m_uint i, len = 0;
  m_int  index = *(m_int*)(shred->mem + SZ_INT);
  m_str str = STRING(o);
  while(str[len] != '\0')
    len++;
  if(!len || index > len || (len-index+1) <= 0) {
    RETURN->d.v_object = NULL;
    return;
  }
  char c[len - index + 1];
  memset(c, 0, len - index + 1);
  for(i = index; i < len; i++)
    c[i - index] = str[i];
  RETURN->d.v_object = new_String(shred,c);
}

static MFUN(string_substringN) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, (STRING(o)));
  m_int i, len = 0, index = *(m_int*)(shred->mem + SZ_INT);
  m_int end = *(m_int*)(shred->mem + SZ_INT*2);
  while(str[len] != '\0')
    len++;
  if(end > len) {
    RETURN->d.v_object = NULL;
    return;
  }
  len -= end;
  char c[len - index + 1];
  for(i = index; i < len; i++)
    c[i - index] = str[i];
  c[i - index] = '\0';
  RETURN->d.v_object = new_String(shred,c);
}

static MFUN(string_insert) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, len_insert = 0, index = *(m_int*)(shred->mem + SZ_INT);
  M_Object arg = *(M_Object*)(shred->mem + SZ_INT * 2);
  char insert[strlen(STRING(arg)) + 1];
  if(!arg) {
    RETURN->d.v_object = NULL;
    return;
  }
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
  RETURN->d.v_object = new_String(shred,c);;
}

static MFUN(string_replace) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, len_insert = 0, index = *(m_int*)(shred->mem + SZ_INT);
  M_Object arg = *(M_Object*)(shred->mem + SZ_INT * 2);
  if(!arg) {
    RETURN->d.v_object = NULL;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  while(str[len] != '\0')
    len++;
  len_insert =  strlen(insert);
  if(index < 0 || !len || (index + len_insert + 1) <= 0) {
    RETURN->d.v_object = NULL;
    return;
  }
  char c[index + len_insert + 1];
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < len_insert; i++)
    c[i + index] = insert[i];
  c[index + len_insert] = '\0';
  release(arg, shred);
  RETURN->d.v_object = new_String(shred,c);;
}

static MFUN(string_replaceN) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, index = *(m_int*)(shred->mem + SZ_INT);
  M_Object arg = *(M_Object*)(shred->mem + SZ_INT * 3);
  m_int _len = *(m_int*)(shred->mem + SZ_INT * 2);
  if(!arg) {
    RETURN->d.v_object = NULL;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  while(str[len] != '\0')
    len++;
  len = len > index + _len ? len : index + _len;
  char c[len + 1];
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < _len; i++)
    c[i + index] = insert[i];
  for(i = index + _len; i < len; i++)
    c[i] = str[i];
  c[len] = '\0';
  release(arg, shred);
  RETURN->d.v_object = new_String(shred,c);;
}

static MFUN(string_find) {
  m_str str = STRING(o);
  m_int i = 0, ret = -1;
  char arg = *(m_int*)(shred->mem + SZ_INT);
  while(str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i++;
  }
  RETURN->d.v_uint = ret;
}

static MFUN(string_findStart) {
  m_str str = STRING(o);
  char pos = *(m_int*)(shred->mem + SZ_INT);
  char arg = *(m_int*)(shred->mem + SZ_INT * 2);
  m_int i = pos, ret = -1;
  if(!strlen(str)) {
    RETURN->d.v_object = NULL;
    return;
  }
  while(str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i++;
  }
  RETURN->d.v_uint = ret;
}

static MFUN(string_findStr) {
  if(!strlen(STRING(o))) {
    RETURN->d.v_object = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  if(!obj) {
    RETURN->d.v_uint = 0;
    return;
  }
  m_str arg = STRING(obj);
  m_int len  = strlen(str);
  m_int i = 0;
  m_int arg_len = strlen(arg);
  while(i < len) {
    if(!strncmp(str +i, arg, arg_len)) {
      ret = i;
      break;
    }
    i++;
  }
  release(obj, shred);
  RETURN->d.v_uint = ret;
}

static MFUN(string_findStrStart) {
  if(!strlen(STRING(o))) {
    RETURN->d.v_object = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  m_int start = *(m_int*)(shred->mem + SZ_INT);
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT * 2);
  if(!obj) {
    RETURN->d.v_object = NULL;
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
  RETURN->d.v_uint = ret;
}

static MFUN(string_rfind) {
  m_str str = STRING(o);
  m_int i = strlen(str) - 1, ret = -1;
  char arg = *(m_int*)(shred->mem + SZ_INT);
  while(i > -1 && str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i--;
  }
  RETURN->d.v_uint = ret;
}

static MFUN(string_rfindStart) {
  if(!strlen(STRING(o))) {
    RETURN->d.v_object = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  char pos = *(m_int*)(shred->mem + SZ_INT);
  char arg = *(m_int*)(shred->mem + SZ_INT * 2);
  m_int i = pos, ret = -1;
  while(i > 0 && str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i--;
  }
  RETURN->d.v_uint = ret;
}

static MFUN(string_rfindStr) {
  if(!strlen(STRING(o))) {
    RETURN->d.v_object = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
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
  RETURN->d.v_uint = ret;
}

static MFUN(string_rfindStrStart) {
  if(!strlen(STRING(o))) {
    RETURN->d.v_object = NULL;
    return;
  }
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int ret = -1;
  m_int start = *(m_int*)(shred->mem + SZ_INT);
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT * 2);
  if(!obj) {
    RETURN->d.v_uint = 0;
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
  RETURN->d.v_uint = ret;
}

static MFUN(string_erase) {
  m_str str = STRING(o);
  m_int i;
  m_int start = *(m_int*)(shred->mem + SZ_INT);
  m_int rem = *(m_int*)(shred->mem + SZ_INT * 2);
  m_int len = strlen(str);
  m_uint size = len -rem + 1;
  if((len-rem+1) <= 0) {
    RETURN->d.v_object = NULL;
    return;
  }
  char c[size];
  c[size -1] = '\0';
  for(i = 0; i < start; i++)
    c[i] = str[i];
  for(i = start + rem; i < len; i++)
    c[i - rem] = str[i];
  STRING(o) = S_name(insert_symbol(c));
}

static MFUN(string_toInt) {
  RETURN->d.v_uint = atoi(STRING(o));
}

static MFUN(string_toFloat) {
  RETURN->d.v_float = atof(STRING(o));
}

m_bool import_string(Env env) {
  CHECK_OB(import_class_begin(env, &t_string, env->global_nspc, string_ctor, NULL))
    env->class_def->doc = "chain of characters";

  o_string_data = import_mvar(env, "int", "@data",   1, 0, "place to hold the string");
  CHECK_BB(o_string_data)

    DL_Func* fun = new_dl_func("int", "size", (m_uint)string_len);
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "upper", (m_uint)string_upper);
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "lower", (m_uint)string_lower);
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "ltrim", (m_uint)string_ltrim);
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "rtrim", (m_uint)string_rtrim);
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "trim", (m_uint)string_trim);
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "charAt", (m_uint)string_charAt);
  dl_func_add_arg(fun, "int", "pos");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "charAt", (m_uint)string_setCharAt);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "substring", (m_uint)string_substring);
  dl_func_add_arg(fun, "int", "start");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "substring", (m_uint)string_substringN);
  dl_func_add_arg(fun, "int", "start");
  dl_func_add_arg(fun, "int", "end");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "insert", (m_uint)string_insert);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "string", "string");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "replace", (m_uint)string_replace);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "string", "string");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("string", "replace", (m_uint)string_replaceN);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "int", "n");
  dl_func_add_arg(fun, "string", "string");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "find", (m_uint)string_find);
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "find", (m_uint)string_findStart);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "find", (m_uint)string_findStr);
  dl_func_add_arg(fun, "string", "str");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "find", (m_uint)string_findStrStart);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "string", "str");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "rfind", (m_uint)string_rfind);
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "rfind", (m_uint)string_rfindStart);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "rfind", (m_uint)string_rfindStr);
  dl_func_add_arg(fun, "string", "str");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "rfind", (m_uint)string_rfindStrStart);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "string", "str");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("void",   "erase", (m_uint)string_erase);
  dl_func_add_arg(fun, "int", "start");
  dl_func_add_arg(fun, "int", "length");
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("int", "toInt", (m_uint)string_toInt);
  CHECK_OB(import_mfun(env, fun))

    fun = new_dl_func("float", "toFloat", (m_uint)string_toFloat);
  CHECK_OB(import_mfun(env, fun))

    CHECK_BB(import_op(env, op_chuck, "string",  "string", "string", String_Assign, 1))
    CHECK_BB(import_op(env, op_chuck, "int",     "string", "string", Int_String_Assign, 1))
    CHECK_BB(import_op(env, op_chuck, "float",   "string", "string", Float_String_Assign, 1))
    CHECK_BB(import_op(env, op_chuck, "complex", "string", "string", Complex_String_Assign, 1))
    CHECK_BB(import_op(env, op_chuck, "polar",   "string", "string", Polar_String_Assign, 1))
    CHECK_BB(import_op(env, op_chuck, "Vec3",    "string", "string", Vec3_String_Assign, 1))
    CHECK_BB(import_op(env, op_chuck, "Vec4",    "string", "string", Vec4_String_Assign, 1))
    CHECK_BB(import_op(env, op_chuck, "Object",  "string", "string", Object_String_Assign, 1))
    CHECK_BB(import_op(env, op_chuck, "@null",    "string", "string", Object_String_Assign, 1))

    CHECK_BB(import_op(env, op_plus, "string",  "string", "string", String_String, 1))
    CHECK_BB(import_op(env, op_plus, "int",     "string", "string", Int_String, 1))
    CHECK_BB(import_op(env, op_plus, "float",   "string", "string", Float_String, 1))
    CHECK_BB(import_op(env, op_plus, "complex", "string", "string", Complex_String, 1))
    CHECK_BB(import_op(env, op_plus, "polar",   "string", "string", Polar_String, 1))
    CHECK_BB(import_op(env, op_plus, "Vec3",    "string", "string", Vec3_String, 1))
    CHECK_BB(import_op(env, op_plus, "Vec4",    "string", "string", Vec4_String, 1))
    CHECK_BB(import_op(env, op_plus, "Object",  "string", "string", Object_String, 1))
    CHECK_BB(import_op(env, op_plus, "@null",  "string", "string", Object_String, 1))

    CHECK_BB(import_op(env, op_plus_chuck, "string",  "string", "string", String_Plus, 1))
    CHECK_BB(import_op(env, op_plus_chuck, "int",     "string", "string", Int_String_Plus, 1))
    CHECK_BB(import_op(env, op_plus_chuck, "float",   "string", "string", Float_String_Plus, 1))
    CHECK_BB(import_op(env, op_plus_chuck, "complex", "string", "string", Complex_String_Plus, 1))
    CHECK_BB(import_op(env, op_plus_chuck, "polar",   "string", "string", Polar_String_Plus, 1))
    CHECK_BB(import_op(env, op_plus_chuck, "Vec3",    "string", "string", Vec3_String_Plus, 1))
    CHECK_BB(import_op(env, op_plus_chuck, "Vec4",    "string", "string", Vec4_String_Plus, 1))
    CHECK_BB(import_op(env, op_plus_chuck, "Object",  "string", "string", Object_String_Plus, 1))
    CHECK_BB(import_op(env, op_plus_chuck, "@null",    "string", "string", Object_String_Plus, 1))
    CHECK_BB(import_class_end(env))
    return 1;
}
