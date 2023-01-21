#!/bin/sh
header_name() {
  echo "$1" | sed 's/\./_/' | sed 's#/#_#g'
}

cat << EOF
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "compile.h"
#include "gwion.h"

static void compile_embed_gw(const Gwion gwion, const m_str filename,
                              const m_str content, const size_t sz)  {
  const m_str str = mp_malloc2(gwion->mp, sz + 1);
  memcpy(str, content, sz);
  str[sz] = 0;
  compile_string(gwion, filename, str);
  mp_free2(gwion->mp, sz + 1, str);
}

EOF

for file in $GWION_EMBED_GW
do
  name=$(header_name "$file")
  xxd -i "$file" > embed/"${name}.h"
  echo "#include \"${name}.h\""
done



echo "void embed_gw(const Gwion gwion) {"

for file in $GWION_EMBED_GW
do
  name=$(header_name "$file")
  xxd -i "$file" > "${name}.h"
  echo "  compile_embed_gw(gwion, \"$file\", ${name}, ${name}_len);"

done

echo "}"
