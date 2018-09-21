typedef void (*_code)(CC jt, const Instr instr);
void jit_code_import(struct Jit*, f_instr, _code);
#define JIT_CODE(a)                                                        \
ANN2(1) static void jitcode_##a(const CC cc, const Instr instr __attribute__((unused)))
