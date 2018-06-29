extern struct Map_ codemap;
#define JIT_CODE(a)                                                        \
ANN static void jitcode_##a(JitCC*, const Instr);                          \
__attribute__((constructor(800))) static void                              \
a##_codeini() { map_set(&codemap, (vtype)a, (vtype)jitcode_##a); }         \
ANN static void jitcode_##a(JitCC* j, const Instr instr __attribute__((unused)))
