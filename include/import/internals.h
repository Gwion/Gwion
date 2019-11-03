#ifndef __IMPORT_INTERNALS
#define __IMPORT_INTERNALS

#define GWI_ERR_B(a,...) { env_err(gwi->gwion->env, gwi->loc, (a), ## __VA_ARGS__); return GW_ERROR; }
#define GWI_ERR_O(a,...) { env_err(gwi->gwion->env, gwi->loc, (a), ## __VA_ARGS__); return NULL; }
#define ENV_ERR_B(pos, a,...) { env_err(env, pos, (a), ## __VA_ARGS__); return GW_ERROR; }
#define ENV_ERR_O(pos, a,...) { env_err(env, pos, (a), ## __VA_ARGS__); return NULL; }

ANN void gwi_body(const Gwi, const Class_Body);

ANN m_bool name_valid(const Gwi, const m_str);

ANN Exp make_exp(const Gwi gwi, const m_str type, const m_str name);

ANN void gwi_reset(const Gwi gwi);
#endif
