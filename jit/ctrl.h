extern struct Map_ ctrlmap;
extern struct Vector_ sfunvec;
extern struct Vector_ mfunvec;

typedef struct ctrl ctrl;
/*
#define JIT_SFUN(a)                             \
__attribute__((constructor(800))) static void   \
a##_ctrlini() { vector_add(&sfunvec, (vtype)a); }

#define JIT_MFUN(a)                             \
__attribute__((constructor(800))) static void   \
a##_ctrlini() { vector_add(&mfunvec, (vtype)a); }
*/
#define JIT_CTRL(a)                                                    \
ANN static void jitctrl_##a(const Instr, const ctrl*);          \
__attribute__((constructor(800))) static void                          \
a##_ctrlini() { map_set(&ctrlmap, (vtype)a, (vtype)jitctrl_##a); }     \
ANN static void jitctrl_##a(const Instr instr __attribute__((unused)), \
                      const ctrl* ctrl __attribute__((unused)))

#define ctrl_xet_def(a)                                                \
ANN m_bool ctrl_##a(const struct ctrl*);
ctrl_xet_def(pc)
ctrl_xet_def(ex)
ANN void ctrl_set_pc(const struct ctrl*, const m_uint);
ANN void ctrl_set_ex(const struct ctrl*);
ANN void ctrl_curr(const ctrl*);
ANN void ctrl_next(const ctrl*);
ANEW struct ctrl* new_ctrl(m_uint);
//ANN void free_ctrl(struct ctrl*);
ANN m_uint ctrl_idx(const struct ctrl* ctrl);
ANN m_uint ctrl_state(const struct ctrl* ctrl);
ANN Instr ctrl_run(struct ctrl* ctrl, struct Vector_* v);
