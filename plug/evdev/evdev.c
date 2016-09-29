#include "defs.h"
#include "type.h"
#include "err_msg.h"
#include "lang.h"
#include "import.h"

#include "object.h"
#include "shreduler.h"
#include <pthread.h>
#include <string.h>
#include <libevdev/libevdev.h>
#include <fcntl.h>
#include <unistd.h>



// TODO: release former fd if any


#define INFO(o) *(EvdevInfo**)(o->data + o_evdev_info)


#define VALUE(o) *(m_uint*)(o->data + o_evdev_value)
#define CODE(o) *(m_uint*)(o->data + o_evdev_code)
#define TYPE(o) *(m_uint*)(o->data + o_evdev_type)
#define TIME(o) *(m_float*)(o->data + o_evdev_time)

struct Type_ t_evdev = { "Evdev",     sizeof(m_uint), &t_event };

m_int o_evdev_info;

m_int o_evdev_time;
m_int o_evdev_type;
m_int o_evdev_code;
m_int o_evdev_value;

static void* evdev_process(void* arg);
typedef struct
{
  struct timeval time;
  m_uint type;
  m_uint code;
  m_int  value;
} EvdevArg;

typedef struct
{
  struct libevdev* evdev;
  int index;
  int fd;
  pthread_t thread;
  Vector args;
} EvdevInfo;

void evdev_ctor(M_Object o, VM_Shred shred)
{
  EvdevInfo* info = calloc(1, sizeof(EvdevInfo));
  info->evdev    =  libevdev_new();
  if(!info->evdev)
  {
    shred->is_running = 0;
    shred->is_done    = 1;
  }
  info->index    = -1;
  info->args     = new_Vector();
  INFO(o)        = info;
}

void evdev_dtor(M_Object o, VM_Shred shred)
{
  EvdevInfo* info = INFO(o);
  if(libevdev_get_fd(info->evdev) != -1)
  {
    pthread_cancel(info->thread);
    libevdev_free(info->evdev);
    close(info->fd);
    free(info->args);
    free(INFO(o));
  }
  free_Vector(info->args);
}

static void evdev_get_index(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  EvdevInfo* info = INFO(o);
  RETURN->v_uint = info->index;
}

static void evdev_name(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  M_Object ret = new_M_Object();
  EvdevInfo* info = INFO(o);
  ret = new_String((m_str)libevdev_get_name(info->evdev));
  RETURN->v_object = ret;
}

static void evdev_index(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
  char c[256];
  m_int index = *(m_int*)(shred->mem + SZ_INT);
  EvdevInfo* info = INFO(o);
  if(index < 0)
  {
    err_msg(INSTR_, 0, "EVDEV: negative index;");
    return;
  }
  sprintf(c, "/dev/input/event%i", index);
  if(info->index != -1)
  {
    int fd = libevdev_get_fd(info->evdev);
    close(fd);
    info->fd = open((const char*)c, O_RDONLY|O_NONBLOCK);
    libevdev_change_fd(info->evdev, info->fd);
  }
  else
  {
    info->fd = open((const char*)c, O_RDONLY|O_NONBLOCK);
    libevdev_set_fd(info->evdev, info->fd);
  }
  info->index = index;
  pthread_create(&info->thread, NULL, evdev_process, o);
  RETURN->v_uint = index;
}

static void evdev_recv(M_Object o, DL_Return * RETURN, VM_Shred shred)
{
#ifdef DEBUG_INSTR
  debug_msg("instr", "evdev recv");
#endif
  EvdevInfo* info = INFO(o);
  Vector v= info->args;
  EvdevArg* arg = (EvdevArg*)vector_front(v);
  if(!arg)
  {
    RETURN->v_uint = 0;
    return;
  }
  TIME(o)   = arg->time.tv_sec + (arg->time.tv_usec / 1000000.);
  TYPE(o)   = arg->type;
  CODE(o)   = arg->code;
  VALUE(o)  = arg->value;
  vector_remove(v, 0);
  free(arg);
  RETURN->v_uint = 1;
}
// loop thread
void* evdev_process(void* arg)
{  
  int rc;
  struct input_event event;
  M_Object o = (M_Object)arg;
  EvdevInfo* info = INFO(o);
  Vector v= info->args;
  do
  {
    rc = libevdev_next_event(info->evdev, LIBEVDEV_READ_FLAG_NORMAL, &event);
    if(rc == 0 && event.type)
    {
      EvdevArg* ev = malloc(sizeof(EvdevArg));
      ev->time  = event.time;
      ev->type  = event.type;
      ev->code  = event.code;
      ev->value = event.value;
      vector_append(v, ev);
      broadcast(o);   
    }
  } while((rc == 1 || rc == 0 || rc == -11));
  return NULL;

}
m_bool import(Env env)
{
  DL_Func* fun;

  CHECK_BB(add_global_type(env, &t_evdev))
  CHECK_BB(import_class_begin(env, &t_evdev, env->global_nspc, evdev_ctor, evdev_dtor))
	env->class_def->doc =  "Evdev interface";

  
  import_mvar(env, "int", "@dummy",  0, 0, "evdev info");
  
  o_evdev_info = import_mvar(env, "int", "@info",  0, 0, "evdev info");
  CHECK_BB(o_evdev_info)

  o_evdev_time  = import_mvar(env, "float", "time",  1, 0, "time");
  CHECK_BB(o_evdev_time)
  
  o_evdev_type  = import_mvar(env, "int", "type",  1, 0, "type");
  CHECK_BB(o_evdev_type)
  
  o_evdev_code  = import_mvar(env, "int", "code",  1, 0, "code");
  CHECK_BB(o_evdev_code)
  
  o_evdev_value = import_mvar(env, "int", "value",  1, 0, "value");
  CHECK_BB(o_evdev_value)
  
  fun = new_DL_Func("int", "index", (m_uint)evdev_get_index);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "index", (m_uint)evdev_index);
    dl_func_add_arg(fun, "int", "i");
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("string", "name", (m_uint)evdev_name);
  CHECK_OB(import_mfun(env, fun))

  fun = new_DL_Func("int", "recv", (m_uint)evdev_recv);
  CHECK_OB(import_mfun(env, fun))

  CHECK_BB(import_class_end(env))  
  return 1;
}