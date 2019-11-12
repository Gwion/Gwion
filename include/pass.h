#ifndef __GWIONPASS
#define __GWIONPASS

// change this to gwion ?
typedef m_bool (*compilation_pass)(Env, Ast);

ANN void pass_register(const Gwion, const m_str, const compilation_pass);
ANN void pass_default(const Gwion);
ANN m_bool pass_set(const Gwion, const Vector);
#endif
