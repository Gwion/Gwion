#include "type.h"
#include "instr.h"
#include "import.h"
#include "emit.h"

#include "func.h"


static struct Type_ t_ev = { "Ev", SZ_INT };

static CTOR(ev_ctor) { printf(" %p this to test ctor\n", (void*)o); 
}

IMPORT {
  CHECK_BB(importer_class_ini(importer, &t_ev, ev_ctor, NULL))
//  CHECK_BB(importer_class_ini(importer, &t_ev, NULL, NULL))
  Type_Decl* td = new_type_decl(new_id_list(insert_symbol("Event"), 0), 0, 0);
  CHECK_BB(importer_class_ext(importer, td))
  CHECK_BB(importer_class_end(importer))
  return 1;
}
