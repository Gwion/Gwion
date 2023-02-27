#!/usr/bin/bash
mkdir -p embed
rm -f embed/*

#json=$(jq -c '.' "$1")
json=$(cat "$1")

cat  << EOF >> embed/embed_head
#include "gwion_util.h"
#include "gwion_ast.h"
#include "gwion_env.h"
#include "vm.h"
#include "instr.h"
#include "emit.h"
#include "compile.h"
#include "gwion.h"
#include "object.h"
#include "operator.h"
#include "import.h"
#include "gwi.h"

EOF

header() {
  echo "$1" >> embed/embed_head
}

config() {
  echo "$1" >> embed/embed.mk
}

has_func() {
  nm "$2" | grep "${1}_${3}" > /dev/null
}
plugin() {
  has_func "gwimport" "$1" "$2" && {
    header "extern m_bool gwimport_${2}(const Gwi);"
    echo "  plug->plugin = gwimport_${2};"
  }
}
driver() {
  has_func "gwdriver" "$1" "$2" && {
    header "extern void *gwdriver_$2(struct DriverData_*);"
    echo "  plug->driver = gwdriver_$2;"
  }
}
modini() {
  has_func "gwmodini" "$1" "$2" && {
    header "extern void *gwmodini_${2}(const struct Gwion_ *, struct Vector_ *const);"
    echo "  plug->modini = gwmodini_${2};"
  }
}
modend() {
  has_func "gwmodend" "$1" "$2" && {
    header "extern void* gwmodend_${2}(struct Gwion_ *const, void *);"
    echo "  plug->modend = gwmodend_${2};"
  }
}

handle_lib() {
  cat << EOF >> embed/embed_body
ANN static void embed_${name}(const Gwion gwion) {
  Plug plug = new_plug(gwion->mp);
$(modini "$1" "$2")$(modend "$1" "$2")$(plugin "$1" "$2")$(driver "$1" "$2")
  map_set(&gwion->data->plugs->map, (vtype)strdup("${2}"), (vtype)plug);
}

EOF
}

cat << EOF >> embed/embed_foot
ANN static void compile_script(const Gwion gwion, const m_str filename,
                              const m_str content, const size_t sz)  {
  const m_str str = mp_malloc2(gwion->mp, sz + 1);
  memcpy(str, content, sz);
  str[sz] = 0;
  compile_string(gwion, filename, str);
  mp_free2(gwion->mp, sz + 1, str);
}

EOF

libraries=$(jq -rc '.libraries' <<< "$json")
echo "ANN void gwion_embed(const Gwion gwion) {" >> embed/embed_foot
jq -rc '.[]' <<< "$libraries" |
  while read -r lib
  do
    path=$(jq -c '.path' <<< "$lib" | sed -e 's/^"//' -e 's/"$//')
    names=$(jq -c '.names' <<< "$lib")
    cflags=$(jq -c '.cflags' <<< "$lib")
    if [ "$cflags" != "null" ]
    then config "CFLAGS += $cflags"
    fi
    config "LDFLAGS += $path"
    if [ "$names" != "null" ]
    then
      jq -c '.[]' <<< "$names" | sed -e 's/^"//' -e 's/"$//' |
      while read -r name
      do
        echo "  embed_${name}(gwion);" >> embed/embed_foot
        handle_lib "$path" "$name"
      done
    fi
    ldflags=$(jq -c '.ldflags' <<< "$lib")
    if [ "$ldflags" != "null" ]
    then config "LDFLAGS += $ldflags"
    fi
  done

handle_script() {
  name="script$2"
  xxd -name "$name" -i "$1" > "embed/${name}.h"
  header "#include \"${name}.h\""
  echo "  compile_script(gwion, \"$name\", ${name}, ${name}_len);"
}

scripts=$(jq -r '.scripts' <<< "$json")
handle_scripts() {
  i=0
  jq -r '.[]' <<< "$scripts"  |
    while read -r name;
    do handle_script "$name" "$i"; i=$((i+1));
    done
}

handle_scripts >> embed/embed_foot
echo "}" >> embed/embed_foot

cat embed/embed_head embed/embed_body embed/embed_foot > embed/embed.c
rm embed/embed_head embed/embed_body embed/embed_foot

audio=$(jq -rc '.audio' <<< "$json")
in=$(jq -rc '.in' <<< "$audio")
out=$(jq -rc '.out' <<< "$audio")
samplerate=$(jq -rc '.samplerate' <<< "$audio")

{
[ "$in" != "null" ] && echo "CFLAGS += -DGWION_DEFAULT_NIN=$in"
[ "$out" != "null" ] && echo "CFLAGS += -DGWION_DEFAULT_NOUT=$out"
[ "$samplerate" != "null" ] && echo "CFLAGS += -DGWION_DEFAULT_SAMPLERATE=$samplerate"
} >> embed/embed.mk

args=$(jq -rc '.args' <<< "$json")
[ "$args" != "null" ] && {
  count=0
  echo "CFLAGS += -DGWION_CONFIG_ARGS" >> embed/embed.mk
  echo "static const char *config_argv[] = {"
  jq -rc '.[]' <<< "$args" |
  while read -r arg 
  do
    echo "  \"$arg\", "
  done
  echo "};"
  count=$((count+1))
  echo "static const int config_argc = $count;"
cat << EOF
ANN const char** config_args(int *argc, char **const argv) {
  const int nargs = config_argc + *argc;
  const char **  args = malloc(nargs * SZ_INT);
  for(int i = 0; i < config_argc; i++) {
    args[i] = config_argv[i];
  }
  for(int i = 0; i < *argc; i++) {
  puts(argv[i]);
    args[i + config_argc] = argv[i];
  }
  *argc = nargs;
  return args;
}
EOF
} >> embed/embed.c

[ "$libraries" != "null" ] || [ "$scripts" != "null" ] && {
  echo "CFLAGS += -DGWION_EMBED"
} >> embed/embed.mk

cflags=$(jq -rc '.cflags' <<< "$json")
[ "$cflags" != "null" ] && {
  jq -rc '.[]' <<< "$cflags" |
  while read -r cflag
  do
    echo "CFLAGS += $cflag "
  done
} >> embed/embed.mk
ldflags=$(jq -rc '.ldflags' <<< "$json")
[ "$ldflags" != "null" ] && {
  jq -rc '.[]' <<< "$ldflags" |
  while read -r ldflag
  do
    echo "LDFLAGS += $ldflag "
  done
} >> embed/embed.mk
standalone=$(jq -rc '.standalone' <<< "$json")
[ "$standalone" = "true" ] && {
  echo "CFLAGS += -DGWION_STANDALONE"
} >> embed/embed.mk
