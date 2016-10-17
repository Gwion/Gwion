#include "vm.h"
Shreduler new_Shreduler(VM* vm);
void free_Shreduler(Shreduler s);
//m_float get_now(Shreduler s) ;
m_float get_now(Shreduler s) ;
VM_Shred shreduler_get(Shreduler s);
m_bool shredule(Shreduler s, VM_Shred shred, m_float wake_time);
void shreduler_set_loop(Shreduler s, m_bool loop);
M_Object shreduler_list(Shreduler s, VM_Shred sh);
void sio_wakeup();
