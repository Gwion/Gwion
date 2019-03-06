#include <signal.h>
#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "compile.h"
#include "driver.h"
#include "gwion.h"
#include "arg.h"

static struct Gwion_ gwion;

static void sig(int unused __attribute__((unused))) {
  gwion.vm->is_running = 0;
}

int main(int argc, char** argv) {
  Arg arg = { .argc = argc, .argv=argv, .loop=-1 };
  DriverInfo di = { .in=2, .out=2, .sr=48000, .func=dummy_driver, .run=vm_run };
  arg_parse(&arg, &di);
  gwion_init(&gwion, &arg.lib);
  signal(SIGINT, sig);
  signal(SIGTERM, sig);
  if(gwion_audio(&gwion, &di) > 0 && gwion_engine(&gwion)) {
    plug_ini(&gwion, &arg.mod);
    for(m_uint i = 0; i < vector_size(&arg.add); i++)
      compile_filename(&gwion, (m_str)vector_at(&arg.add, i));
    shreduler_set_loop(gwion.vm->shreduler, arg.loop);
    gwion_run(&gwion, &di);
  }
  gwion_release(&gwion, &di);
  arg_release(&arg);
  return 0;
}
