#ifdef GWREPL
extern void repl_init(VM* vm) ANN;
extern void repl_fini(void);
#define GWREPL_INI(a) if(arg.repl) repl_init((a));
#define GWREPL_END() if(arg.repl) repl_fini();
INSTR(EOC2);
#else
#define GWREPL_INI(a)
#define GWREPL_END()
#endif
