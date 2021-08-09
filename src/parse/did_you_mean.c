#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"

#ifdef min
#undef min
#endif

#define MAX_DISTANCE 2
#define min2(a, b)   ((a) < (b) ? (a) : (b))
#define min(a, b, c) (min2(min2((a), (b)), (c)))

static m_bool wagner_fisher(const char *s, const char *t) {
  const size_t m = strlen(s);
  const size_t n = strlen(t);
  unsigned int d[m][n];
  uint         i, j;
  for (i = 0; i < m; ++i) d[i][0] = i;
  for (i = 0; i < n; ++i) d[0][i] = i;
  for (j = 1; j < n; ++j) {
    for (i = 1; i < m; ++i) {
      if (s[i] == t[j])
        d[i][j] = d[i - 1][j - 1];
      else
        d[i][j] = min(d[i - 1][j] + 1, d[i][j - 1] + 1, d[i - 1][j - 1] + 1);
      if (d[i][j] > MAX_DISTANCE + 1) return 0;
    }
  }
  return (i && j && d[m - 1][n - 1] < MAX_DISTANCE);
}

ANN static void ressembles(const Scope scope, const char *name,
                           bool *const done) {
  struct scope_iter iter = { scope, 0, 0 };
  Value             value;
  while (scope_iter(&iter, &value) > 0) {
    if (wagner_fisher(name, value->name)) {
      if (!*done) {
        *done = true;
        gw_err("{-/}did you mean{0}:\n");
      }
      if (!vflag(value, vflag_builtin)) defined_here(value);
    }
  }
}

ANN static void trait_ressembles(const Scope scope, const char *name,
                           bool *const done) {
  struct scope_iter iter = { scope, 0, 0 };
  Trait trait;
  while (scope_iter(&iter, &trait) > 0) {
    if (wagner_fisher(name, trait->name)) {
      if (!*done) {
        *done = true;
        gw_err("{-/}did you mean{0}:\n");
      }
      if (trait->filename) // TODO: check why is that from check
        gwerr_secondary(_("defined here"), trait->filename, trait->loc);
    }
  }
}

#define MAX_NAME_LEN 16
#define CHECK_LEN(name)                                                        \
  if (strlen(name) > MAX_NAME_LEN) return;

#undef did_you_mean_nspc
ANN void did_you_mean_nspc(Nspc nspc, const char *name) {
  CHECK_LEN(name)
  bool done = false;
  do ressembles(nspc->info->value, name, &done);
  while ((nspc = nspc->parent));
}

#undef did_you_mean_type
ANN void did_you_mean_type(Type type, const char *name) {
  CHECK_LEN(name)
  bool done = false;
  do ressembles(type->nspc->info->value, name, &done);
  while ((type = type->info->parent) && type->nspc);
}

ANN void did_you_mean_trait(Nspc nspc, const char *name) {
  CHECK_LEN(name)
  bool done = false;
  do trait_ressembles(nspc->info->trait, name, &done);
  while ((nspc = nspc->parent));
}
