#ifndef __IMPORT_INTERNALS
#define __IMPORT_INTERNALS

#define GWI_ERR(a, ...)                                                        \
  { env_err(gwi->gwion->env, gwi->loc, (a), ##__VA_ARGS__); }
#define GWI_ERR_B(a, ...)                                                      \
  {                                                                            \
    GWI_ERR((a), ##__VA_ARGS__);                                               \
    return GW_ERROR;                                                           \
  }
#define GWI_ERR_O(a, ...)                                                      \
  {                                                                            \
    GWI_ERR((a), ##__VA_ARGS__);                                               \
    return NULL;                                                               \
  }
#define GWION_ERR(pos, a, ...)                                                 \
  { env_err(gwion->env, pos, (a), ##__VA_ARGS__); }
#define GWION_ERR_B(pos, a, ...)                                               \
  {                                                                            \
    GWION_ERR(pos, (a), ##__VA_ARGS__);                                        \
    return GW_ERROR;                                                           \
  }
#define GWION_ERR_O(pos, a, ...)                                               \
  {                                                                            \
    GWION_ERR(pos, (a), ##__VA_ARGS__);                                        \
    return NULL;                                                               \
  }
#define ENV_ERR_B(pos, a, ...)                                                 \
  {                                                                            \
    env_err(env, pos, (a), ##__VA_ARGS__);                                     \
    return GW_ERROR;                                                           \
  }
#define ENV_ERR_O(pos, a, ...)                                                 \
  {                                                                            \
    env_err(env, pos, (a), ##__VA_ARGS__);                                     \
    return NULL;                                                               \
  }

ANN void gwi_body(const Gwi, const Ast);

ANN Exp make_exp(const Gwi gwi, const m_str type, const m_str name);

ANN void gwi_reset(const Gwi gwi);
ANN VM *   gwi_vm(const Gwi);
ANN m_bool gwi_gack(const Gwi gwi, const Type type, const f_gack d);
#endif
