#include "gwion_util.h"
#include "gwion_ast.h"
#include "oo.h"
#include "vm.h"
#include "env.h"
#include "gwion.h"

GWMODSTR(basic_module)

GWMODINI(basic_module) {
  puts("ini module");
  if(args) {
    puts("have arguments");
    for(vtype i = 0; i < vector_size(args); ++i)
      puts((m_str)vector_at(args, i));
  }
  return NULL;
}

GWMODEND(basic_module) {
  puts("end module");
}
