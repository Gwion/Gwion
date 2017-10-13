#include "defs.h"
#include "map.h"
#include "map_private.h"
#include "arg.h"

void arg_init(Arg* arg) {
  vector_init(&arg->add);
  vector_init(&arg->rem);
  vector_init(&arg->lib);
  vector_add(&arg->lib, (vtype)GWION_ADD_DIR);
  arg->ref = &arg->add;
}

void arg_release(Arg* arg) {
  vector_release(&arg->add);
  vector_release(&arg->rem);
  vector_release(&arg->lib);
}
