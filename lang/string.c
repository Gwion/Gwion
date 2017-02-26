#include "defs.h"
#include "vm.h"
#include "type.h"
#include "err_msg.h"
#include "import.h"
#include "instr.h"

// needed for polar
#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

struct Type_ t_string = { "string", SZ_INT, &t_object, te_string };
m_int o_string_data;

static INSTR(String_Assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "string => string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  /*
    if(!lhs) {
      lhs = new_M_Object();
      initialize_object(lhs, &t_string);
    }
  */
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  if(!rhs) {
    err_msg(INSTR_, 0, "assigning to empty string.");
    Except(shred);
    return;
//    rhs = new_M_Object();
//    initialize_object(rhs, &t_string);
  }
  STRING(rhs) = lhs ? STRING(lhs) : NULL;
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
}

static INSTR(Int_String_Assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  char str[1024];
  sprintf(str, "%li", lhs);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg =  (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

static INSTR(Float_String_Assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  char str[1024];
  sprintf(str, "%f", lhs);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Complex_String_Assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Complex '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex lhs = *(m_complex*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_COMPLEX);
  char str[1024];
  sprintf(str, "#(%f, %f)", creal(lhs), cimag(lhs));
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Polar_String_Assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Polar '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex lhs = *(m_complex*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_COMPLEX);
  char str[1024];
  sprintf(str, "#(%f, %f)",  creal(lhs),
              cimag(lhs) / M_PI);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Vec3_String_Assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec3 '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC3);
  VEC3_T lhs = *(VEC3_T*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_VEC3);
  char str[1024];
  sprintf(str, "#(%f, %f, %f)", lhs.x, lhs.y, lhs.z);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Vec4_String_Assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Vec4 '=>' string");
#endif
  POP_REG(shred, SZ_INT + SZ_VEC4);
  VEC4_T lhs = *(VEC4_T*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_VEC4);
  char str[1024];
  sprintf(str, "#(%f, %f, %f, %f)", lhs.x, lhs.y, lhs.z, lhs.w);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Object_String_Assign)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "Object '=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  char str[1024];
  sprintf(str, "%p", (void*)lhs);
  STRING(rhs) = S_name(insert_symbol(str));
  *(M_Object*)shred->reg = (M_Object)rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
  release(lhs, shred);
}

INSTR(String_String)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  char str[1024];
  sprintf(str, "%s%s", STRING(lhs), STRING(rhs));
  *(M_Object*)shred->reg = new_String(str);
  PUSH_REG(shred, SZ_INT);
  release(lhs, shred);
  release(rhs, shred);
}

INSTR(Int_String)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_int lhs = *(m_int*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  char str[1024];
  sprintf(str, "%li%s", lhs, STRING(rhs));
  *(M_Object*)shred->reg = new_String(str);
//  release(*(M_Object*)shred->reg, shred);
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Float_String)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_FLOAT);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_FLOAT);
  char str[1024];
  sprintf(str, "%f%s", lhs, STRING(rhs));
  *(M_Object*)shred->reg = new_String(str);
  PUSH_REG(shred, SZ_INT);
}

INSTR(Complex_String)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "complex '+' string");
#endif
  POP_REG(shred, SZ_INT + SZ_COMPLEX);
  m_complex  lhs = *(m_complex*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_COMPLEX);
  char str[1024];
  sprintf(str, "#(%f, %f)%s", creal(lhs), cimag(lhs), STRING(rhs));
  *(M_Object*)shred->reg = new_String(str);
  PUSH_REG(shred, SZ_INT);
}

INSTR(Object_String)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = *(M_Object*)(shred->reg + SZ_INT);
  char str[1024];
  sprintf(str, "%p%s", (void*)lhs, STRING(rhs));
  *(M_Object*)shred->reg = new_String(str);
  PUSH_REG(shred, SZ_INT);
}

INSTR(String_Plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "string '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  m_uint l_len = strlen(STRING(lhs));
  m_uint r_len = strlen(STRING(rhs));
  char c[l_len + r_len + 1];
  sprintf(c, "%s%s", STRING(rhs), STRING(lhs));
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
}

INSTR(Int_String_Plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "int '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_uint lhs = *(m_uint*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  m_uint len = strlen(STRING(rhs)) + 1;
  m_uint tmp = lhs;
  while(tmp /= 10)
    len++;
  char c[len];
  sprintf(c, "%s%li", STRING(rhs), lhs);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Float_String_Plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  m_uint len = strlen(STRING(rhs)) + 1 + 7;
  m_uint tmp = lhs;
  while(tmp /= 10)
    len++;
  char c[len];
  sprintf(c, "%s%f", STRING(rhs), lhs);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Complex_String_Plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "float '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  m_float lhs = *(m_float*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  m_uint len = strlen(STRING(rhs)) + 1 + 5 + 13;
  m_uint tmp = creal(lhs);
  while(tmp /= 10)
    len++;
  tmp = cimag(lhs);
  while(tmp /= 10)
    len++;
  char c[len];
  sprintf(c, "%s#(%f, %f)", STRING(rhs), creal(lhs), cimag(lhs));
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Object_String_Plus)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "string '+=>' string");
#endif
  POP_REG(shred, SZ_INT * 2);
  M_Object lhs = *(M_Object*)shred->reg;
  M_Object rhs = **(M_Object**)(shred->reg + SZ_INT);
  m_uint len = strlen(STRING(rhs)) + 9;
  char c[len];
  sprintf(c, "%s%p", STRING(rhs), (void*)lhs);
  STRING(rhs) = S_name(insert_symbol(c));
  *(M_Object*)shred->reg = rhs;
  PUSH_REG(shred, SZ_INT);
  release(rhs, shred);
}

INSTR(Reg_Push_Str)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "push string %s", (m_str)instr->m_val);
#endif
  // modified 13/01/17 'get rid of litteral strings'
  *(M_Object*)shred->reg = new_String((m_str)instr->m_val);
  *(M_Object*)(shred->mem + instr->m_val2) = *(M_Object*)shred->reg;
  (*(M_Object*)shred->reg)->ref++;
  PUSH_REG(shred, SZ_INT);
#ifdef DEBUG_INSTR
  debug_msg("instr", "push string");
#endif
}

void string_ctor(M_Object o, VM_Shred shred)
{
  STRING(o) = "this is the default string";
}

MFUN(string_len)
{
  RETURN->d.v_uint = strlen(STRING(o));
}

MFUN(string_upper)
{
  m_uint i;
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(i = 0; i < strlen(c); i++)
    if(c[i]  >= 'a' && c[i] <= 'z')
      c[i] += 'A' - 'a';
  STRING(obj) = S_name(insert_symbol(c));
  RETURN->d.v_object = obj;
}

MFUN(string_lower)
{
  m_uint i;
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for(i = 0; i < strlen(c); i++)
    if(c[i]  >= 'A' && c[i] <= 'Z')
      c[i] -= 'A' - 'a';
  STRING(obj) = S_name(insert_symbol(c));
  RETURN->d.v_object = obj;
}

MFUN(string_ltrim)
{
  m_uint i = 0;
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  m_str str = STRING(obj) = strdup(STRING(o));
  while(*str || *str == ' ') {
    i++;
    str++;
  }
  RETURN->d.v_object = obj;
}

MFUN(string_rtrim)
{
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  STRING(obj) = strdup(STRING(o));
  RETURN->d.v_object = obj;
}

MFUN(string_trim)
{
  m_str str = STRING(o);
  m_int i, start = 0, end = 0, len = 0;
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  STRING(obj) = str;
  while(str[len] != '\0')
    len++;
  for(i = 0; i < len; i++) {
    if(str[i] == ' ')
      start++;
    else break;
  }
  /*  exit(2);*/
  for(i = len - 1; i >= 0; i--) {
    if(str[i] == ' ')
      end++;
    else break;
  }
  char c[len - start - end + 1];
  for(i = start; i < len - end; i++)
    c[i - start] = str[i];
  c[len - start - end ] = '\0';
  STRING(obj) = strdup(c);
  RETURN->d.v_object = obj;
}

MFUN(string_charAt)
{
  m_str str = STRING(o);
  m_int i = *(m_int*)(shred->mem + SZ_INT), len = 0;
  while(str[len] != '\0')
    len++;
  if(i < 0 || i >= len)
    RETURN->d.v_uint = -1;
  else
    RETURN->d.v_uint = str[i];
}


MFUN(string_setCharAt)
{
  m_str str = STRING(o);
  m_int i = *(m_int*)(shred->mem + SZ_INT), len = 0;
  m_int c = *(m_int*)(shred->mem + SZ_INT * 2);
  while(str[len] != '\0')
    len++;
  if(i < 0 || i >= len)
    RETURN->d.v_uint = -1;
  else {
    str[i] = c;
    STRING(o) = strdup(str);
    RETURN->d.v_uint = c;
  }
}

MFUN(string_substring)
{
  m_str str = strdup(STRING(o));
  m_int i, len = 0, index = *(m_int*)(shred->mem + SZ_INT);
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  STRING(obj) = str;
  while(str[len] != '\0')
    len++;
  char c[len - index];
  for(i = index; i < len; i++)
    c[i - index] = str[i];
  STRING(obj) = strdup(c);
  RETURN->d.v_object = obj;
}

MFUN(string_substringN)
{
  m_str str = strdup(STRING(o));
  m_int i, len = 0, index = *(m_int*)(shred->mem + SZ_INT * 2);
  m_int end = *(m_int*)(shred->mem + SZ_INT);
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  STRING(obj) = str;
  while(str[len] != '\0')
    len++;
  len -= end;
  char c[len - index + 1];
  for(i = index; i < len; i++)
    c[i - index] = str[i];
  c[i - index] = '\0';
  STRING(obj) = strdup(c);
  RETURN->d.v_object = obj;
}

MFUN(string_insert)
{
  m_str str = strdup(STRING(o));
  m_int i, len = 0, len_insert = 0, index = *(m_int*)(shred->mem + SZ_INT);
  M_Object arg = *(M_Object*)(shred->mem + SZ_INT * 2);
  m_str insert = strdup(STRING(arg));
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
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
  STRING(obj) = S_name(insert_symbol(c));
  free(str);
  free(insert);
  RETURN->d.v_object = obj;
}

MFUN(string_replace)
{
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, len_insert = 0, index = *(m_int*)(shred->mem + SZ_INT);
  M_Object arg = *(M_Object*)(shred->mem + SZ_INT * 2);
  m_str insert = strdup(STRING(arg));
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
  while(str[len] != '\0')
    len++;
  len_insert =  strlen(insert);
  char c[index + len_insert + 1];
  for(i = 0; i < index; i++)
    c[i] = str[i];
  for(i = 0; i < len_insert; i++)
    c[i + index] = insert[i];
  c[index + len_insert] = '\0';
  free(insert);
  STRING(obj) = S_name(insert_symbol(c));
  RETURN->d.v_object = obj;
}

MFUN(string_replaceN)
{
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int i, len = 0, index = *(m_int*)(shred->mem + SZ_INT);
  M_Object arg = *(M_Object*)(shred->mem + SZ_INT * 3);
  m_int _len = *(m_int*)(shred->mem + SZ_INT * 2);
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  M_Object obj = new_M_Object();
  initialize_object(obj, &t_string);
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
  STRING(obj) = S_name(insert_symbol(c));
  RETURN->d.v_object = obj;
}

MFUN(string_find)
{
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

MFUN(string_findStart)
{
  m_str str = STRING(o);
  char pos = *(m_int*)(shred->mem + SZ_INT);
  char arg = *(m_int*)(shred->mem + SZ_INT * 2);
  m_int i = pos, ret = -1;
  while(str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i++;
  }
  RETURN->d.v_uint = ret;
}

MFUN(string_findStr)
{
  m_str str = strdup(STRING(o));
  m_int ret = -1;
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str arg = STRING(obj);
  m_int len  = strlen(str);
  m_int i = 0;
  m_int arg_len = strlen(arg);
  while(i < len) {
    if(!strncmp(str, arg, arg_len)) {
      ret = i;
      break;
    }
    i++;
    str++;
  }
  str -= i;
  free(str);
  release(obj, shred);
  RETURN->d.v_uint = ret;
}

MFUN(string_findStrStart)
{
  m_str str = strdup(STRING(o));
  m_int ret = -1;
  m_int start = *(m_int*)(shred->mem + SZ_INT);
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT * 2);
  m_str arg = STRING(obj);
  m_int len  = strlen(str);
  m_int i = start;
  m_int arg_len = strlen(arg);
  while(i < len) {
    if(!strncmp(str, arg, arg_len)) {
      ret = i;
      break;
    }
    i++;
    str++;
  }
  str -= (i - 1);
  free(str);
  release(obj, shred);
  RETURN->d.v_uint = ret;
}

MFUN(string_rfind)
{
  m_str str = STRING(o);
  m_int i = strlen(str), ret = -1;
  char arg = *(m_int*)(shred->mem + SZ_INT);
  while(str[i] != '\0') {
    if(str[i] == arg) {
      ret = i;
      break;
    }
    i--;
  }
  RETURN->d.v_uint = ret;
}

MFUN(string_rfindStart)
{
  m_str str = STRING(o);
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

MFUN(string_rfindStr)
{
  m_str str = strdup(STRING(o));
  m_int ret = -1;
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT);
  m_str arg = STRING(o);
  m_int len  = strlen(str);
  m_int i = len;
  m_int arg_len = strlen(arg);
  str += len - 1;
  while(i) {
    if(!strncmp(str, arg, arg_len)) {
      ret = i;
      break;
    }
    i--;
    str--;
  }
  str -= (i - 1);
  free(str);
  release(obj, shred);
  RETURN->d.v_uint = ret;
}

MFUN(string_rfindStrStart)
{
  m_str str = strdup(STRING(o));
  m_int ret = -1;
  m_int start = *(m_int*)(shred->mem + SZ_INT);
  M_Object obj = *(M_Object*)(shred->mem + SZ_INT * 2);
  m_str arg = STRING(obj);

  printf("here: %s %s\n", STRING(o), STRING(obj));
  printf("here: %s %s\n", str, arg);

//  m_str str = strdup(STRING(o));
  m_str buf = str;
//  m_int ret = -1;
//  m_int start = *(m_int*)(shred->mem + SZ_INT);
//  M_Object obj = *(M_Object*)(shred->mem + SZ_INT *2);
//  m_str arg = STRING(obj);
  m_int len  = strlen(str);
  m_int i = start;
  m_int arg_len = strlen(arg);
  str += len - 1;
//  while(i < len - 1) {
  while(i > -1) {
    if(!strncmp(str, arg, arg_len)) {
      ret = i;
      break;
    }
    i--;
    str--;
  }
//  str -= i ? i : 0;
  free(buf);
  release(obj, shred);
  RETURN->d.v_uint = ret;
}

MFUN(string_erase)
{
  m_str str = STRING(o);
  m_int i;
  m_int start = *(m_int*)(shred->mem + SZ_INT);
  m_int rem = *(m_int*)(shred->mem + SZ_INT * 2);
  m_int len = strlen(str);
  char c[len - rem];
  memset(c, 0, len - rem);
  for(i = 0; i < start; i++)
    c[i] = str[i];
  for(i = start + rem; i < len; i++)
    c[i - rem] = str[i];
}

MFUN(string_toInt)
{
  RETURN->d.v_uint = atoi(STRING(o));
}

MFUN(string_toFloat)
{
  RETURN->d.v_float = atof(STRING(o));
}

m_bool import_string(Env env)
{
  CHECK_BB(add_global_type(env, &t_string));
  CHECK_OB(import_class_begin(env, &t_string, env->global_nspc, string_ctor, NULL))
  env->class_def->doc = "chain of characters";

  o_string_data = import_mvar(env, "int", "@data",   1, 0, "place to hold the string");
  CHECK_BB(o_string_data)

  DL_Func* fun = new_DL_Func("int", "size", (m_uint)string_len);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "upper", (m_uint)string_upper);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "lower", (m_uint)string_lower);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "ltrim", (m_uint)string_ltrim);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "rtrim", (m_uint)string_rtrim);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "trim", (m_uint)string_trim);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "charAt", (m_uint)string_charAt);
  dl_func_add_arg(fun, "int", "pos");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "charAt", (m_uint)string_setCharAt);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "substring", (m_uint)string_substring);
  dl_func_add_arg(fun, "int", "start");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "substring", (m_uint)string_substringN);
  dl_func_add_arg(fun, "int", "start");
  dl_func_add_arg(fun, "int", "end");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "insert", (m_uint)string_insert);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "string", "string");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "replace", (m_uint)string_replace);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "string", "string");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "replace", (m_uint)string_replaceN);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "int", "n");
  dl_func_add_arg(fun, "string", "string");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "find", (m_uint)string_find);
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "find", (m_uint)string_findStart);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "find", (m_uint)string_findStr);
  dl_func_add_arg(fun, "string", "str");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "find", (m_uint)string_findStrStart);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "string", "str");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "rfind", (m_uint)string_rfind);
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "rfind", (m_uint)string_rfindStart);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "int", "char");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "rfind", (m_uint)string_rfindStr);
  dl_func_add_arg(fun, "string", "str");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "rfind", (m_uint)string_rfindStrStart);
  dl_func_add_arg(fun, "int", "pos");
  dl_func_add_arg(fun, "string", "str");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("void",   "erase", (m_uint)string_erase);
  dl_func_add_arg(fun, "int", "start");
  dl_func_add_arg(fun, "int", "length");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "toInt", (m_uint)string_toInt);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("float", "toFloat", (m_uint)string_toFloat);
  CHECK_OB(import_mfun(env, fun))

  CHECK_BB(add_binary_op(env, op_chuck, &t_string,  &t_string, &t_string, String_Assign, 1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_int,     &t_string, &t_string, Int_String_Assign, 1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_float,   &t_string, &t_string, Float_String_Assign, 1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_complex, &t_string, &t_string, Complex_String_Assign, 1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_polar,   &t_string, &t_string, Polar_String_Assign, 1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_vec3,    &t_string, &t_string, Vec3_String_Assign, 1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_vec4,    &t_string, &t_string, Vec4_String_Assign, 1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_object,  &t_string, &t_string, Object_String_Assign, 1))
  CHECK_BB(add_binary_op(env, op_chuck, &t_null,    &t_string, &t_string, Object_String_Assign, 1))

  CHECK_BB(add_binary_op(env, op_plus, &t_string,  &t_string, &t_string, String_String, 1))
  CHECK_BB(add_binary_op(env, op_plus, &t_int,     &t_string, &t_string, Int_String, 1))
  CHECK_BB(add_binary_op(env, op_plus, &t_float,   &t_string, &t_string, Float_String, 1))
  CHECK_BB(add_binary_op(env, op_plus, &t_complex, &t_string, &t_string, Complex_String, 1))
  CHECK_BB(add_binary_op(env, op_plus, &t_object,  &t_string, &t_string, Object_String, 1))

  CHECK_BB(add_binary_op(env, op_plus_chuck, &t_string,  &t_string, &t_string, String_Plus, 1))
  CHECK_BB(add_binary_op(env, op_plus_chuck, &t_int,     &t_string, &t_string, Int_String_Plus, 1))
  CHECK_BB(add_binary_op(env, op_plus_chuck, &t_float,   &t_string, &t_string, Float_String_Plus, 1))
  CHECK_BB(add_binary_op(env, op_plus_chuck, &t_complex, &t_string, &t_string, Complex_String_Plus, 1))
  CHECK_BB(add_binary_op(env, op_plus_chuck, &t_object,  &t_string, &t_string, Object_String_Plus, 1))
  CHECK_BB(import_class_end(env))
  return 1;
}
