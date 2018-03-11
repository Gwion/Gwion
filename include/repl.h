#ifdef GWREPL
#include <readline/readline.h>
extern void* repl_init(VM* vm, pthread_t*) ANN;
#define GWREPL_THREAD pthread_t repl_thread;
#define GWREPL_INI(a, b) if(arg.repl)repl_init(a, b);
#define GWREPL_END(a) if(arg.repl) pthread_join(a, NULL);


#else
#define GWREPL_THREAD
#define GWREPL_INI(a, b)
#define GWREPL_END(a)
#endif
