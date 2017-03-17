#include "defs.h"
#include "err_msg.h"
//#include "emit.h"
#include "instr.h"

static Operator operators[] = {
// arithmetic
  op_assign, op_plus, op_minus, op_times, op_divide, op_percent,
// logical
  op_and, op_or, op_eq, op_neq,
  op_gt, op_ge, op_lt, op_le,
// bitwise
  op_shift_left, op_shift_right,
  op_s_or, op_s_and, op_s_xor,
// unary
  op_plusplus, op_minusminus, op_exclamation, op_tilda,
  op_new, op_spork, op_typeof, op_sizeof,
// reverse arithmetic
  op_chuck, op_plus_chuck, op_minus_chuck, op_times_chuck, op_divide_chuck, op_modulo_chuck,
// reverse logical
  op_rand, op_ror, op_req, op_rneq,
  op_rgt, op_rge, op_rlt, op_rle,
// reverse bitwise
  op_rsl, op_rsr, op_rsand, op_rsor, op_rsxor,
// unchuck and others
  op_unchuck, op_rinc, op_rdec, op_runinc, op_rundec,
//at_chuck
  op_at_chuck, op_at_unchuck,
// trig
  op_trig, op_untrig
};

static m_str op_str[] = {
// arithmetic
  "=", "+", "-", "*", "/", "%",
// logical
  "&&", "||",	"==", "!=",
  ">", ">=", "<", "<=",
// bitwise
  "<<", ">>",
  "|", "&", "^",
// unary
  "++", "--", "!", "~",
  "new", "spork", "typeof", "sizeof",
// reverse arithmetic
  "=>", "+=>", "-=>", "*=>", "/=>", "%=>",
// reverse logical
  "&&=>", "||=>", "==>", "!=>",
  ">=>", ">==>", "<=<", "<==<",
// reverse bitwise
  "<<=>", ">>=>", "&=>", "|=>", "^=>",
// unchuck an others
  "=<", "++=>", "--=>", "++=<", "--=<",
// at_chuck
  "@=>", "@=<",
// trig
  "]=>", "]=<", NULL
};

static m_str op_name[] = {
// arithmetic
  "assign", "plus", "minus", "times", "divide", "modulo",
// logical
  "and", "or", "eq", "neq",
  "gt", "ge", "lt", "le",
// bitwise
  "shiftleft", "shiftright",
  "s_or", "s_and", "s_xor",
// unary
  "plusplus", "minusminus", "exclamation", "tilda",
  "new", "spork", "typeof", "sizeof",
// reverse arithmetic
  "chuck", "pluschuck", "minuschuck", "timeschuck", "dividechuck", "modulochuck",
// reverse logical
  "rand", "ror", "req", "rneq",
  "rgt", "rge", "rlt", "rle",
// reverse bitwise
  "rsl", "rsr", "rsand", "rsor", "rsxor",
// unchuck and others
  "unchuck", "rinc", "rdec", "runinc", "runcdec",
// at
  "at_chuck", "at_unchuck",
// trig
  "trig", "untrig", NULL
  // more to come
};

typedef Type (*f_type)(Env env, Expression exp);
typedef struct {
  Type lhs, rhs, ret;
  f_instr instr;
  Func func;
  f_type  type_func;
  m_str doc;
  m_bool is_new;
} M_Operator;

static M_Operator* last = NULL;
m_bool operator_set_doc(m_str doc)
{
  if(!last)
    return -1;
  last->doc = doc;
  return 1;
}

//Operatorname2op(m_str name)
m_int name2op(m_str name)
{
  m_uint i = 0;
//	for(i = 0; i < (sizeof(operators) / sizeof(Operator)) - 1; i++)
  while(op_name[i] && op_str[i]) {
    if(!strcmp(op_name[i], name)) {
      return operators[i];
    }
    if(!op_name[i + 1]) {
      return -1;
    }
    i++;
  }
  return -1;
}

m_str op2str(Operator op)
{
  return op_str[op];
}

Map new_Operator_Map()
{
  m_uint i;
  Map map = new_Map();
  for(i = 0; i < (sizeof(operators) / sizeof(Operator)); i++)
    map_set(map, (vtype)operators[i], (vtype)new_Vector());
  return map;
}

void free_Operator_Map(Map map)
{
  m_uint i;
  Vector v;
  for(i = 0; i < (sizeof(operators) / sizeof(Operator)); i++) {
    m_uint j;
    v = (Vector)map_get(map, (vtype)operators[i]);
    for(j = 0; j < vector_size(v); j++)
      free((M_Operator*)vector_at(v, j));
    free_Vector(v);
  }
  free_Map(map);
}

static M_Operator* operator_find(Vector v, Type lhs, Type rhs)
{
  m_uint i;
  for(i = 0; i < vector_size(v); i++) {
    M_Operator* mo = (M_Operator*)vector_at(v, i);
    if((lhs && mo->lhs && mo->lhs->xid == lhs->xid) &&
        (rhs && mo->rhs && mo->rhs->xid == rhs->xid))
      return mo;
    /*    if((rhs && mo->rhs && mo->rhs->xid == rhs->xid) && !mo->lhs && !lhs)*/
    /*      return mo;*/
    /*    if((lhs && mo->lhs && mo->lhs->xid == lhs->xid) && !mo->rhs && !rhs)*/
    /*      return mo;*/
  }
  return NULL;
}

m_bool add_binary_op(Env env, Operator op, Type lhs, Type rhs, Type ret, f_instr f, m_bool global, m_bool is_new)
{
// is_new unused for now
#ifdef DEBUG_OPERATOR
  debug_msg(" op  ", "import operator '%s' for type '%s' and '%s', in  '%s'",
            op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL, env->curr->name);
#endif

  NameSpace nspc = env->curr;
  Vector v;
  M_Operator* mo;

  last  = NULL;
  if(global && nspc->parent)
    /*		nspc = nspc->parent;*/
    nspc = env->global_nspc;
  v = (Vector)map_get(nspc->operator, (vtype)op);

  if(!v) {
    err_msg(TYPE_, 0, "failed to import operator '%s', for type '%s' and '%s'. reason: no such operator",
            op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL);
    return -1;
  }
  if((mo = operator_find(v, lhs, rhs))) {
    err_msg(TYPE_, 0, "operator '%s', for type '%s' and '%s' already imported",
            op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL);
    return -1;
  }

  mo = malloc(sizeof(M_Operator));
  mo->lhs       = lhs;
  mo->rhs       = rhs;
  mo->ret       = ret;
  mo->instr     = f;
  mo->func      = NULL;
  mo->type_func = NULL;
  mo->doc       = NULL;
  mo->is_new = is_new;
  vector_append(v, (vtype)mo);
  last = mo;
  return 1;
}

Type get_return_type(Env env, Operator op, Type lhs, Type rhs)
{
#ifdef DEBUG_OPERATOR
  debug_msg(" op", "get return type for operator '%s', for type '%s' and '%s'",
            op2str(op), lhs ? lhs->name : NULL, rhs ? rhs->name : NULL);
#endif
  Type t, l = lhs, r = lhs;
/*  NameSpace nspc = env->curr; */
  NameSpace nspc = env->global_nspc;
  M_Operator* mo;
  while(nspc) {
    Vector v = (Vector)map_get(nspc->operator, (vtype)op);

    if((mo = operator_find(v, lhs, rhs)))
      return mo->ret;
    l = l ? l->parent : NULL;
    while(l) {
      if((t = get_return_type(env, op, l, rhs)))
        return t;
      r = rhs;
      while(r) {
        if((t = get_return_type(env, op, l, r)))
          return t;
        r = r->parent;
      }
      l = l->parent;
    }
    nspc = nspc->parent;
  }
  nspc = env->global_nspc;
  while(nspc) {
    Vector v = (Vector)map_get(nspc->operator, (vtype)op);

    if((mo = operator_find(v, lhs, rhs)))
      return mo->ret;
    r = rhs->parent;
    while(r) {
      if((t = get_return_type(env, op, lhs, r)))
        return t;
      l = lhs;
      while(l) {
        if((t = get_return_type(env, op, l, r)))
          return t;
        l = l->parent;
      }
      r = r->parent;
    }
    nspc = nspc->parent;
  }
  return NULL;
}

// use for C operator
m_bool operator_set_type_func(f_type  f)
{
#ifdef DEBUG_OPERATOR
  debug_msg(" op", 0, "set type func");
#endif
  if(!last) {
    err_msg(TYPE_, 0, "no last operator. aborting");
    return -1;
  }
  last->type_func = f;
  return 1;
}

// use for in code operator
m_bool operator_set_func(Env env, Func f, Type lhs, Type rhs)
{
#ifdef DEBUG_OPERATOR
  debug_msg(" op", 0, "set func'");
#endif
  NameSpace nspc = env->curr;
  while(nspc) {
    M_Operator* mo;
    Vector v = (Vector)map_get(nspc->operator, (vtype)name2op(S_name(f->def->name)));
    if((mo = operator_find(v, lhs, rhs))) {
      mo->func = f;
      return 1;
    }
    nspc = nspc->parent;
  }
  return -1;
}

m_bool get_instr(Emitter emit, Operator op, Type lhs, Type rhs)
{
#ifdef DEBUG_OPERATOR
  debug_msg(" op", "get instr for operator '%s', for type '%s' and '%s'",
            op2str(op), lhs->name, rhs->name);
#endif
  Type l = lhs, r = rhs;
  Vector v;
  NameSpace nspc = emit->env->curr;
  while(nspc) {
    M_Operator* mo;
    v = (Vector)map_get(nspc->operator, (vtype)op);
    if((mo = operator_find(v, lhs, rhs))) {
      if(mo->func) {
        add_instr(emit, Reg_Push_Imm); //do we need to set offset ?
        if(emit_Func_Call1(emit, mo->func, mo->func->def->ret_type, 0) < 0)
          return -1;
        return 1;
      } else {
        Instr instr = add_instr(emit, mo->instr);
        if(mo->is_new) {
          // should only be M_Object, so use SZ_INT
		  Local* l = frame_alloc_local(emit->code->frame, SZ_INT, "operator return value", 0, 1);
          instr->m_val = l->offset;
        }
      }
      return 1;
    }
    if(l && l->parent)
      if(get_instr(emit, op, l->parent, rhs) > 0)
        return 1;

    if(r && r->parent)
      if(get_instr(emit, op, lhs, r->parent) > 0)
        return 1;
    nspc = nspc->parent;
  }
  return -1;
}

void operator_doc(Vector v, FILE* file)
{
#ifdef DEBUG_OPERATOR
  debug_msg(" op", "making doc");
#endif

  m_uint i;

  fprintf(file, "@itemize @bullet\n");
  for(i = 0; i < vector_size(v); i++) {
    M_Operator* mo = (M_Operator*)vector_at(v, i);
    fprintf(file, "@item (%s) '%s' %s '%s'\n", mo->ret->name, mo->lhs->name, op2str(i), mo->rhs->name);
    if(mo->doc)
      fprintf(file, "'%s'\n", mo->doc);
  }
  fprintf(file, "@end itemize\n");
}
