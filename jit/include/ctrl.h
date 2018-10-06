typedef struct Jit *Jit;
typedef struct ctrl ctrl;
#define JIT_CTRL(a)                                                    \
ANN static inline void a##_ctrl(const Instr instr __attribute__((unused)), \
                      const ctrl* ctrl __attribute__((unused)))

#define ctrl_xet_def(a)                                                \
ANN m_bool ctrl_##a(const struct ctrl*);
ctrl_xet_def(pc)
ctrl_xet_def(ex)
//ctrl_xet_def(nf)
ANN void ctrl_set_pc(const struct ctrl*, const m_uint);
ANN void ctrl_set_ex(const struct ctrl*);
//ANN void ctrl_set_nf(const struct ctrl*);
ANN void ctrl_curr(const ctrl*);
ANN void ctrl_next(const ctrl*);
ANN void ctrl_set_skip(const ctrl*);
ANN m_bool ctrl_skip(const ctrl*);
ANEW struct ctrl* new_ctrl(m_uint);
ANN void free_ctrl(struct ctrl* ctrl);
ANN m_uint ctrl_idx(const struct ctrl* ctrl);
ANN void ctrl_inc_idx(const struct ctrl* ctrl);
ANN void ctrl_done(const struct ctrl* ctrl);
ANN m_uint ctrl_state(const struct ctrl* ctrl);
ANN void ctrl_ini(const vtype, const vtype);
ANN void ctrl_label(const struct ctrl*, void* data, void (*)(void*, vtype));
ANN Instr ctrl_run(const struct ctrl* ctrl, const Vector v);
ANN Instr ctrl_byte(const struct ctrl*);

typedef void (*_ctrl)(const Instr, const struct ctrl*);
_ctrl ctrl_get(f_instr exec);
void jit_ctrl_import(struct Jit*, f_instr, _ctrl);
//#include <jit/jit.h>
//ANN jit_label_t* ctrl_lbl(const struct ctrl*, const m_uint);
