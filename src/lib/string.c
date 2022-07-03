#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "emit.h"
#include "specialid.h"
#include "gwi.h"
#include "gack.h"
#include "array.h"

#define describe_string_logical(name, action)                                  \
  static INSTR(String_##name) {                                                \
    POP_REG(shred, SZ_INT);                                                    \
    const M_Object lhs     = *(M_Object *)REG(-SZ_INT);                        \
    const M_Object rhs     = *(M_Object *)REG(0);                              \
    *(m_int *)REG(-SZ_INT) = action;                                           \
  }
describe_string_logical(eq, (!strcmp(STRING(lhs), STRING(rhs))))
    describe_string_logical(neq, (strcmp(STRING(lhs), STRING(rhs))))

        static inline uint is_const_str(const Exp exp) {
  return exp->exp_type == ae_exp_primary &&
         exp->d.prim.prim_type == ae_prim_str;
}

#define opck_str(name, __exp__)                                                \
  OP_CHECK(opck_string_##name) {                                               \
    Exp_Binary *bin = (Exp_Binary *)data;                                      \
    if (!is_const_str(bin->lhs) || !is_const_str(bin->rhs))                    \
      return env->gwion->type[et_bool];                                        \
    const int ret = __exp__;                                                   \
    free_exp(env->gwion->mp, bin->lhs);                                        \
    free_exp(env->gwion->mp, bin->rhs);                                        \
    const Exp e         = exp_self(bin);                                       \
    e->exp_type         = ae_exp_primary;                                      \
    e->d.prim.prim_type = ae_prim_num;                                         \
    e->d.prim.d.num     = ret;                                                 \
    return env->gwion->type[et_bool];                                          \
  }

opck_str(eq, !strcmp(bin->lhs->d.prim.d.string.data, bin->rhs->d.prim.d.string.data))
    opck_str(neq, strcmp(bin->lhs->d.prim.d.string.data, bin->rhs->d.prim.d.string.data))

        static CTOR(string_ctor) {
  STRING(o) = _mp_calloc(shred->info->mp, 1);
}

static DTOR(string_dtor) { free_mstr(shred->info->mp, STRING(o)); }

ID_CHECK(check_filepp) {
  ((Exp_Primary *)prim)->prim_type = ae_prim_str;
  ((Exp_Primary *)prim)->d.string.data     = env->name;
  ((Exp_Primary *)prim)->value = global_string(env, prim->d.string.data, prim_pos(prim));
  return prim->value->type;
}

ID_CHECK(check_funcpp) {
  ((Exp_Primary *)prim)->prim_type = ae_prim_str;
  ((Exp_Primary *)prim)->d.string.data     = env->func        ? env->func->name
                                     : env->class_def ? env->class_def->name
                                                      : env->name;
// handle delim?
  ((Exp_Primary *)prim)->value = global_string(env, prim->d.string.data, prim_pos(prim));
  return prim->value->type;
}

ID_CHECK(check_linepp) {
  ((Exp_Primary *)prim)->prim_type = ae_prim_num;
  ((Exp_Primary *)prim)->d.num = prim_pos(prim).first.line;
  return env->gwion->type[et_int];
}

static GACK(gack_string) {
  const M_Object obj = *(M_Object *)VALUE;
  INTERP_PRINTF("%s", obj ? STRING(obj) : "(null string)");
}

static inline m_bool bounds(const m_str str, const m_int i) {
  CHECK_BB(i);
  return (m_uint)i <= strlen(str) ? GW_OK : GW_ERROR;
}

static INSTR(StringSlice) {
  shred->reg -= SZ_INT * 2;
  const M_Object obj   = *(M_Object *)REG(-SZ_INT);
  m_str          str   = STRING(obj);
  const m_int    start = *(m_uint *)REG(0);
  const size_t   strsz = strlen(str);
  m_int          end   = *(m_uint *)REG(SZ_INT);
  if (end < 0) end = strsz + end - 1;
  if (bounds(str, start) < 0 || bounds(str, end) < 0) {
    handle(shred, "OutOfBoundsStringSlice");
    return;
  }
  const m_int  op = start < end ? 1 : -1;
  const m_uint sz = (op > 0 ? end - start : start - end) + 1;
  if(sz >= SIZE_MAX/SZ_INT) {
    handle(shred, "SliceTooBig");
    return;
  }
  char         c[sz + 1];
  for (m_int i = start, j = 0; j < (m_int)sz; i += op, ++j) c[j] = str[i];
  c[sz]                     = '\0';
  *(M_Object *)REG(-SZ_INT) = new_string(shred->info->vm->gwion, c);
}

static MFUN(string_len) { *(m_uint *)RETURN = strlen(STRING(o)); }

static MFUN(string_upper) {
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for (m_uint i = 0; i < strlen(c); i++)
    if (c[i] >= 'a' && c[i] <= 'z') c[i] += 'A' - 'a';
  *(M_Object *)RETURN = new_string(shred->info->vm->gwion, c);
}

static MFUN(string_lower) {
  char c[strlen(STRING(o)) + 1];
  strcpy(c, STRING(o));
  for (m_uint i = 0; i < strlen(c); i++)
    if (c[i] >= 'A' && c[i] <= 'Z') c[i] -= 'A' - 'a';
  *(M_Object *)RETURN = new_string(shred->info->vm->gwion, c);
}

static MFUN(string_ltrim) {
  m_uint      i   = 0;
  const m_str str = STRING(o);
  while (str[i] == ' ') i++;
  char c[strlen(str) - i + 1];
  strcpy(c, STRING(o) + i);
  *(M_Object *)RETURN = new_string(shred->info->vm->gwion, c);
}

static MFUN(string_rtrim) {
  const m_str  str = STRING(o);
  const size_t sz  = strlen(str);
  if (sz) {
    m_uint len = strlen(str) - 1;
    while (str[len] == ' ') len--;
    char c[len + 2];
    strncpy(c, str, len + 1);
    c[len + 1]          = '\0';
    *(M_Object *)RETURN = new_string(shred->info->vm->gwion, c);
  } else {
    ++o->ref;
    *(M_Object *)RETURN = o;
  }
}

static MFUN(string_trim) {
  const m_str str = STRING(o);
  m_int       i, start = 0, end = 0, len = 0;
  while (str[len] != '\0') len++;
  for (i = 0; i < len; i++) {
    if (str[i] == ' ')
      start++;
    else
      break;
  }
  for (i = len - 1; i >= 0; i--) {
    if (str[i] == ' ')
      end++;
    else
      break;
  }
  if (len - start - end <= 0) {
    handle(shred, "InvalidStringTrimRequest");
    return;
  }
  char c[len - start - end + 1];
  for (i = start; i < len - end; i++) c[i - start] = str[i];
  c[len - start - end] = '\0';
  *(M_Object *)RETURN  = new_string(shred->info->vm->gwion, c);
}

static MFUN(string_insert) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int          i, len_insert = 0, index = *(m_int *)MEM(SZ_INT);
  const M_Object arg = *(M_Object *)MEM(SZ_INT * 2);
  char           insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  const m_uint len = strlen(str);
  len_insert       = strlen(insert);
  char c[len + len_insert + 1];
  for (i = 0; i < index; i++) c[i] = str[i];
  for (i = 0; i < len_insert; i++) c[i + index] = insert[i];
  for (i = index; i < (m_int)len; i++) c[i + len_insert] = str[i];
  c[len + len_insert] = '\0';
  *(M_Object *)RETURN = new_string(shred->info->vm->gwion, c);
  ;
}

static MFUN(string_replace) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int          i, len_insert = 0, index = *(m_int *)MEM(SZ_INT);
  const M_Object arg = *(M_Object *)MEM(SZ_INT * 2);
  if (!arg) {
    *(M_Object *)RETURN = o;
    return;
  }
  char insert[strlen(STRING(arg)) + 1];
  strcpy(insert, STRING(arg));
  const m_uint len = strlen(str);
  len_insert       = strlen(insert);
  if (index >= (m_int)len || index < 0 || (index + len_insert + 1) <= 0) {
    handle(shred, "InvalidStringReplace");
    return;
  }
  char c[index + len_insert + 1];
  for (i = 0; i < index; i++) c[i] = str[i];
  for (i = 0; i < len_insert; i++) c[i + index] = insert[i];
  c[index + len_insert] = '\0';
  *(M_Object *)RETURN   = new_string(shred->info->vm->gwion, c);
  ;
}

static MFUN(string_replaceN) {
  char str[strlen(STRING(o)) + 1];
  strcpy(str, STRING(o));
  m_int          i, index = *(m_int *)MEM(SZ_INT);
  const M_Object arg  = *(M_Object *)MEM(SZ_INT * 3);
  const m_int    _len = *(m_int *)MEM(SZ_INT * 2);
  if (!arg || index > (m_int)strlen(STRING(o)) ||
      _len > (m_int)strlen(STRING(arg))) {
    handle(shred, "InvalidStringReplace");
    return;
  }
  char         insert[strlen(STRING(arg)) + 1];
  const m_uint len = strlen(str);
  //  memset(insert, 0, len + 1);
  strcpy(insert, STRING(arg));
  str[len] = '\0';
  if (index > (m_int)len) index = len - 1;
  char c[len + _len];
  memset(c, 0, len + _len);
  for (i = 0; i < index; i++) c[i] = str[i];
  for (i = 0; i < _len; i++) c[i + index] = insert[i];
  for (i = index + _len; i < (m_int)len; i++) c[i] = str[i];
  c[len + _len - 1]   = '\0';
  *(M_Object *)RETURN = new_string(shred->info->vm->gwion, c);
  ;
}

static MFUN(string_find) {
  const m_str base  = STRING(o);
  char        c     = *(m_int *)MEM(SZ_INT);
  char *      str   = strchr(base, c);
  *(m_uint *)RETURN = str ? str - base : -1;
}

static MFUN(string_findStart) {
  const m_str  base = STRING(o);
  const size_t sz   = strlen(base);
  const int   pos  = *(m_int *)MEM(SZ_INT * 2);
  if (pos >= 0 && (size_t)pos < sz) {
    const char arg    = *(m_int *)MEM(SZ_INT);
    char *     str    = strchr(base + pos, arg);
    *(m_uint *)RETURN = str ? str - pos - base : -1;
  } else
    *(m_uint *)RETURN = -1;
}

static MFUN(string_findStr) {
  const m_str    base = STRING(o);
  const size_t   sz   = strlen(base);
  const M_Object obj  = *(M_Object *)MEM(SZ_INT);
  if (sz) {
    const m_str arg   = STRING(obj);
    const m_str str   = strstr(base, arg);
    *(m_uint *)RETURN = str ? str - base : -1;
  } else
    *(m_uint *)RETURN = -1;
}

static MFUN(string_findStrStart) {
  const m_str    base = STRING(o);
  const size_t   sz   = strlen(base);
  const int      pos  = *(m_int *)MEM(SZ_INT * 2);
  const M_Object obj  = *(M_Object *)MEM(SZ_INT);
  if (pos >= 0 && (size_t)pos < sz) {
    const m_str arg   = STRING(obj);
    const m_str str   = strstr(base + pos, arg);
    *(m_uint *)RETURN = str ? str - pos - base : -1;
  } else
    *(m_uint *)RETURN = -1;
}

static MFUN(string_rfind) {
  const m_str base  = STRING(o);
  char        c     = *(m_int *)MEM(SZ_INT);
  char *      str   = strrchr(base, c);
  *(m_uint *)RETURN = str ? str - base : -1;
}

static MFUN(string_rfindStart) {
  const m_str  base = STRING(o);
  const size_t sz   = strlen(base);
  const int   pos  = *(m_int *)MEM(SZ_INT);
  if (pos >= 0 && (size_t)pos < sz) {
    const char arg    = *(m_int *)MEM(SZ_INT * 2);
    char *     str    = strrchr(base + pos, arg);
    *(m_uint *)RETURN = str ? str - pos - base : -1;
  } else
    *(m_uint *)RETURN = -1;
}

static MFUN(string_rfindStr) {
  const m_str    base = STRING(o);
  const size_t   sz   = strlen(base);
  const M_Object obj  = *(M_Object *)MEM(SZ_INT);
  if (sz) {
    const m_str arg = STRING(obj);
    m_str       tmp = base, str = NULL;
    while ((tmp = strstr(tmp, arg))) str = tmp++;
    *(m_uint *)RETURN = str ? str - base : -1;
  } else
    *(m_uint *)RETURN = -1;
}

static MFUN(string_rfindStrStart) {
  const m_str    base = STRING(o);
  const size_t   sz   = strlen(base);
  const m_int    pos  = *(m_int *)MEM(SZ_INT * 2);
  const M_Object obj  = *(M_Object *)MEM(SZ_INT);
  if (sz && (size_t)pos < sz) {
    const m_str arg = STRING(obj);
    m_str       tmp = base + pos, str = NULL;
    while ((tmp = strstr(tmp, arg))) str = tmp++;
    *(m_uint *)RETURN = str ? str - pos - base : -1;
  } else
    *(m_uint *)RETURN = -1;
}

static MFUN(string_erase) {
  const m_str str   = STRING(o);
  const m_int _start = *(m_int *)MEM(SZ_INT);
  const m_int rem   = *(m_int *)MEM(SZ_INT * 2);
  const m_int len   = strlen(str);
  const m_int size  = len - rem + 1;
  const m_int start = _start >= 0 ? _start : len - _start;
  if (start >= len || size <= 0) {
    handle(shred, "InvalidStringErase");
    return;
  }
  char c[size];
  c[size - 1] = '\0';
  for (m_int i = 0; i < start; i++) c[i] = str[i];
  for (m_int i = start + rem; i < len; i++) c[i - rem] = str[i];
  STRING(o) = s_name(insert_symbol(shred->info->vm->gwion->st, c));
}

static MFUN(string_save) {
  const m_str path = STRING(*(M_Object*)MEM(SZ_INT));
  FILE *f = fopen(path, "w");
  if(!f) {
    handle(shred, "StringLoadException");
    return;
  }
  const m_str str = STRING(o);
  fprintf(f, "%s", str);
  fclose(f);
}

static SFUN(string_load) {
  const m_str path = STRING(*(M_Object*)MEM(0));
  FILE *f = fopen(path, "r");
  if(!f) {
    handle(shred, "StringLoadException");
    return;
  }
  fseek(f, 0, SEEK_END);
  const size_t sz = ftell(f);
  char c[sz + 1];
  rewind(f);
  (void)fread(c, 1, sz, f);
  fclose(f);
  *(M_Object*)RETURN = new_string(shred->info->vm->gwion, c);
}

static MFUN(string_atoi) {
  const M_Object obj = *(M_Object*)MEM(0);
  const m_str str = STRING(obj);
  *(m_int*)RETURN = atoi(str);
}

static MFUN(string_atof) {
  const M_Object obj = *(M_Object*)MEM(0);
  const m_str str = STRING(obj);
  *(m_float*)RETURN = (m_float)atof(str);
}
/*
static MFUN(string_atoi2) {
  const M_Object obj = *(M_Object*)MEM(0);
  const m_str str = STRING(obj);
  char *endptr;
  *(m_int*)RETURN = strtol(str, &endptr, 10);
  **(m_uint**)MEM(SZ_INT) = endptr - str;
}
*/

ANN Type check_array_access(const Env env, const Array_Sub array);

static OP_CHECK(opck_string_access) {
  const Array_Sub array = (Array_Sub)data;
  const Exp exp = array->exp;
  if(!exp->next)
    return env->gwion->type[et_char];
  struct Array_Sub_ next = { exp->next, env->gwion->type[et_char], array->depth - 1 };
  return check_array_access(env, &next);
}

static INSTR(string_at) {
  POP_REG(shred, SZ_INT);
  const m_str  str = STRING(*(M_Object*)REG(-SZ_INT));
  const m_int  i   = *(m_int *)REG(0);
  const m_uint len = strlen(str);
  if (i < 0 || (m_uint)i >= len) {
    handle(shred, "invalid string access");
    return;
  } else
    *(m_int *)REG(-SZ_INT) = str[i];
}

static INSTR(string_at_set) {
  POP_REG(shred, SZ_INT);
  const m_str  str = STRING((*(M_Object*)REG(-SZ_INT)));
  const m_int  i   = *(m_int *)REG(0);
  const m_uint len = strlen(str);
  if (i < 0 || (m_uint)i >= len) {
    handle(shred, "StringAccessException");
    return;
  }
  const m_int c = *(m_int*)REG(-SZ_INT*2);
  if(c == '\0') {
    handle(shred, "StringAccessException");
    return;
  }
  *(char**)REG(-SZ_INT) = str + i;
  memcpy(REG(-SZ_INT*2 + 1), str + i + 1, SZ_INT - 1);
}


static OP_EMIT(opem_string_access) {
  struct ArrayAccessInfo *info = (struct ArrayAccessInfo*)data;
  const Exp exp = info->array.exp;
  const Exp next = exp->next;
  CHECK_BB(emit_exp(emit, exp));
  exp->next = next;
  emit_add_instr(emit, !info->is_var ? string_at : string_at_set);
  return GW_OK;
}

GWION_IMPORT(string) {
  const Type t_string         = gwi_class_ini(gwi, "string", NULL);
  gwi->gwion->type[et_string] = t_string; // use func
  gwi_class_xtor(gwi, string_ctor, string_dtor);
  GWI_BB(gwi_gack(gwi, t_string, gack_string))
  t_string->nspc->offset += SZ_INT;

  gwi_func_ini(gwi, "int", "size");
  GWI_BB(gwi_func_end(gwi, string_len, ae_flag_none))

  gwi_func_ini(gwi, "string", "upper");
  GWI_BB(gwi_func_end(gwi, string_upper, ae_flag_none))

  gwi_func_ini(gwi, "string", "lower");
  GWI_BB(gwi_func_end(gwi, string_lower, ae_flag_none))

  gwi_func_ini(gwi, "string", "ltrim");
  GWI_BB(gwi_func_end(gwi, string_ltrim, ae_flag_none))

  gwi_func_ini(gwi, "string", "rtrim");
  GWI_BB(gwi_func_end(gwi, string_rtrim, ae_flag_none))

  gwi_func_ini(gwi, "string", "trim");
  GWI_BB(gwi_func_end(gwi, string_trim, ae_flag_none))

  gwi_func_ini(gwi, "string", "insert");
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "string", "str");
  GWI_BB(gwi_func_end(gwi, string_insert, ae_flag_none))

  gwi_func_ini(gwi, "string", "replace");
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "string", "str");
  GWI_BB(gwi_func_end(gwi, string_replace, ae_flag_none))

  gwi_func_ini(gwi, "string", "replace");
  gwi_func_arg(gwi, "int", "pos");
  gwi_func_arg(gwi, "int", "n");
  gwi_func_arg(gwi, "string", "str");
  GWI_BB(gwi_func_end(gwi, string_replaceN, ae_flag_none))

  gwi_func_ini(gwi, "int", "find");
  gwi_func_arg(gwi, "char", "c");
  GWI_BB(gwi_func_end(gwi, string_find, ae_flag_none))

  gwi_func_ini(gwi, "int", "find");
  gwi_func_arg(gwi, "char", "c");
  gwi_func_arg(gwi, "int", "pos");
  GWI_BB(gwi_func_end(gwi, string_findStart, ae_flag_none))

  gwi_func_ini(gwi, "int", "find");
  gwi_func_arg(gwi, "string", "str");
  GWI_BB(gwi_func_end(gwi, string_findStr, ae_flag_none))

  gwi_func_ini(gwi, "int", "find");
  gwi_func_arg(gwi, "string", "str");
  gwi_func_arg(gwi, "int", "pos");
  GWI_BB(gwi_func_end(gwi, string_findStrStart, ae_flag_none))

  gwi_func_ini(gwi, "int", "rfind");
  gwi_func_arg(gwi, "char", "c");
  GWI_BB(gwi_func_end(gwi, string_rfind, ae_flag_none))

  gwi_func_ini(gwi, "int", "rfind");
  gwi_func_arg(gwi, "char", "c");
  gwi_func_arg(gwi, "int", "pos");
  GWI_BB(gwi_func_end(gwi, string_rfindStart, ae_flag_none))

  gwi_func_ini(gwi, "int", "rfind");
  gwi_func_arg(gwi, "string", "str");
  GWI_BB(gwi_func_end(gwi, string_rfindStr, ae_flag_none))

  gwi_func_ini(gwi, "int", "rfind");
  gwi_func_arg(gwi, "string", "str");
  gwi_func_arg(gwi, "int", "pos");
  GWI_BB(gwi_func_end(gwi, string_rfindStrStart, ae_flag_none))

  gwi_func_ini(gwi, "void", "erase");
  gwi_func_arg(gwi, "int", "start");
  gwi_func_arg(gwi, "int", "length");
  GWI_BB(gwi_func_end(gwi, string_erase, ae_flag_none))

  gwi_func_ini(gwi, "void", "save");
  gwi_func_arg(gwi, "string", "path");
  GWI_BB(gwi_func_end(gwi, string_save, ae_flag_none))

  gwi_func_ini(gwi, "string", "load");
  gwi_func_arg(gwi, "string", "path");
  GWI_BB(gwi_func_end(gwi, string_load, ae_flag_static))

  gwi_func_ini(gwi, "int", "atoi");
  GWI_BB(gwi_func_end(gwi, string_atoi, ae_flag_none))

  gwi_func_ini(gwi, "float", "atof");
  GWI_BB(gwi_func_end(gwi, string_atof, ae_flag_none))

  GWI_BB(gwi_class_end(gwi))

  GWI_BB(gwi_oper_ini(gwi, "int", "string", NULL))
  GWI_BB(gwi_oper_add(gwi, opck_string_access))
  GWI_BB(gwi_oper_emi(gwi, opem_string_access))
  GWI_BB(gwi_oper_end(gwi, "@array", NULL))

  GWI_BB(gwi_oper_ini(gwi, "string", "string", "bool"))
  GWI_BB(gwi_oper_add(gwi, opck_string_eq))
  GWI_BB(gwi_oper_end(gwi, "==", String_eq))
  GWI_BB(gwi_oper_add(gwi, opck_string_neq))
  GWI_BB(gwi_oper_end(gwi, "!=", String_neq))

  GWI_BB(gwi_oper_ini(gwi, "int", "string", "string"))
  GWI_BB(gwi_oper_end(gwi, "@slice", StringSlice))

  struct SpecialId_ file_spid = {
      .ck = check_filepp, .is_const = 1};
  gwi_specialid(gwi, "__file__", &file_spid);
  struct SpecialId_ func_spid = {
      .ck = check_funcpp, .is_const = 1};
  gwi_specialid(gwi, "__func__", &func_spid);
  struct SpecialId_ line_spid = {
      .ck = check_linepp, .is_const = 1};
  gwi_specialid(gwi, "__line__", &line_spid);
  return GW_OK;
}
