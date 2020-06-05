#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"

#ifdef min
#undef min
#endif

#define MAX_DISTANCE 2
#define min2(a, b) ((a) < (b) ? (a) : (b))
#define min(a, b, c) (min2(min2((a), (b)), (c)))

static m_bool wagner_fisher(const char *s, const char* t) {
  const size_t m = strlen(s);
  const size_t n = strlen(t);
  unsigned int d[m][n];
  uint i, j;
  for(i = 0; i < m; ++i)
    d[i][0] = i;
  for(i = 0; i < n; ++i)
    d[0][i] = i;
  for(j = 1; j < n; ++j) {
    for(i = 1; i < m; ++i) {
      if(s[i] == t[j])
        d[i][j] = d[i-1][j-1];
      else
        d[i][j] = min(d[i-1][j] + 1, d[i][j-1] + 1, d[i-1][j-1] + 1);
      if(d[i][j] > MAX_DISTANCE + 1)
        return 0;
    }
  }
  return (i && j && d[m-1][n-1] < MAX_DISTANCE);
}

ANN static void ressembles(const Vector v, const Nspc nspc, const char* name) {
  struct scope_iter iter = { nspc->info->value, 0, 0 };
  Value value;
  while(scope_iter(&iter, &value) > 0) {
    if(!strcmp(name, value->name))
      continue;
    if(wagner_fisher(name, value->name))
      vector_add(v, (vtype)value->name);
  }
}

#define MAX_NAME_LEN 16
#define CHECK_LEN(name) if(strlen(name) > MAX_NAME_LEN) return;

#undef did_you_mean_nspc
ANN void did_you_mean_nspc(Nspc nspc, const char* name) {
  CHECK_LEN(name)
  struct Vector_ v;
  vector_init(&v);
  do ressembles(&v, nspc, name);
  while((nspc = nspc->parent));
  for(m_uint i = 0; i < vector_size(&v); ++i)
    gw_err(_("  (did you mean '%s'?)\n"), (m_str)vector_at(&v, i));
  vector_release(&v);
}

#undef did_you_mean_type
ANN void did_you_mean_type(Type type, const char* name) {
  CHECK_LEN(name)
  Type t = type;
  struct Vector_ v;
  vector_init(&v);
  do ressembles(&v, t->nspc, name);
  while((t = t->e->parent) && t->nspc);
  for(m_uint i = 0; i < vector_size(&v); ++i)
    gw_err(_("  (did you mean '%s'?)\n"), (m_str)vector_at(&v, i));
  did_you_mean_nspc(type->nspc, name);
  vector_release(&v);
}
