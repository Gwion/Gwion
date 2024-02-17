#!/usr/bin/bash
mkdir -p embed
rm -f embed/*
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

cflag() {
  config "CFLAGS += $1"
}

has_func() {
  nm "$2" | grep "${1}_${3}" > /dev/null
}
plugin() {
  has_func "gwimport" "$1" "$2" && {
    header "extern bool gwimport_${2}(const Gwi);"
    echo "  plug->plugin = gwimport_${2};"
  }
}
driver() {
  has_func "gwdriver" "$1" "$2" && {
    header "extern void gwdriver_$2(struct DriverData_*);"
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
    header "extern void* gwmodend_${2}(const struct Gwion_ *, void *);"
    echo "  plug->modend = gwmodend_${2};"
  }
}

depend() {
  has_func "gwdepend" "$1" "$2" && {
    header "extern m_str* gwdepend_${2}(void);"
    echo "  plug->depend = gwdepend_${2};"
  }
}

handle_lib() {
  cat << EOF >> embed/embed_body
ANN static void embed_${name}(const Gwion gwion) {
  Plug plug = new_plug(gwion->mp);
$(modini "$1" "$2")$(modend "$1" "$2")$(plugin "$1" "$2")$(driver "$1" "$2")$(depend "$1" "$2")
  map_set(&gwion->data->plugs->map, (vtype)strdup("${2}"), (vtype)plug);
}

EOF
}

script_helper() {
cat << EOF >> embed/embed_head
ANN static void compile_script(const Gwion gwion, const m_str filename,
                              const m_str content, const size_t sz)  {
  const m_str str = mp_malloc2(gwion->mp, sz + 1);
  memcpy(str, content, sz);
  str[sz] = 0;
  compile_string(gwion, filename, str);
  mp_free2(gwion->mp, sz + 1, str);
}

EOF
}

not_null() {
 [ "$1" != "null" ] && return 0
 return 1
}

lenght() {
  jq -rc "length" <<< "$1"
}
array_is_ok() {
  not_null "$1" || return 1
  length=$(jq -rc "length" <<< "$1")
  [ "$length" = "0" ] && return 1
  return 0
}

libraries=$(jq -rc '.libraries' <<< "$json")

handle_libs() {
  array_is_ok "$libraries" || return 1
jq -rc '.[]' <<< "$libraries" |
  while read -r lib
  do
    path=$(jq -c '.path' <<< "$lib" | sed -e 's/^"//' -e 's/"$//')
    names=$(jq -c '.names' <<< "$lib")
    cflags=$(jq -c '.cflags' <<< "$lib")
    if [ "$cflags" != "null" ]
    then cflag "$cflags"
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
    ldflags=$(jq -c '.ldflags' <<< "$lib" | sed -e 's/^"//' -e 's/"$//')
    if [ "$ldflags" != "null" ]
    then config "LDFLAGS += $ldflags"
    fi
  done
}

file2hex() {
  cp "$1" "$2"
  xxd -i "$2" > "embed/${2}.h"
  rm "$2"
}

handle_script() {
  name="script$2"
  file2hex "$1" "$name"
  header "#include \"${name}.h\""
  echo "  compile_script(gwion, \"$name\", (m_str)${name}, ${name}_len);"
}

scripts=$(jq -r '.scripts' <<< "$json")

handle_scripts() {
  script_helper
  i=0
  jq -r '.[]' <<< "$scripts"  |
    while read -r name;
    do handle_script "$name" "$i"; i=$((i+1));
    done
}
embed() {
  array_is_ok "$libraries" && {
    echo "ANN void gwion_embed_libs(const Gwion gwion) {" >> embed/embed_foot
    handle_libs
    echo "}" >> embed/embed_foot
    cflag "-DGWION_EMBED_LIBS"
  }
  array_is_ok "$scripts" && {
    echo "ANN void gwion_embed_scripts(const Gwion gwion) {" >> embed/embed_foot
    handle_scripts >> embed/embed_foot
    echo "}" >> embed/embed_foot
    cflag "-DGWION_EMBED_SCRIPTS"
  }
}
embed
touch embed/embed_body
cat embed/embed_head embed/embed_body embed/embed_foot > embed/embed.c
rm embed/embed_head embed/embed_body embed/embed_foot

audio=$(jq -rc '.audio' <<< "$json")
in=$(jq -rc '.in' <<< "$audio")
out=$(jq -rc '.out' <<< "$audio")
samplerate=$(jq -rc '.samplerate' <<< "$audio")

not_null "$in" && cflag "-DGWION_DEFAULT_NIN=$in"
not_null "$out" && cflag "-DGWION_DEFAULT_NOUT=$out"
not_null "$samplerate" && cflag "-DGWION_DEFAULT_SAMPLERATE=$samplerate"

args=$(jq -rc '.args' <<< "$json")


{
  cflag "-DGWION_CONFIG_ARGS"
  echo "static const char *config_argv[] = {"
  array_is_ok "$args" && {
    jq -rc '.[]' <<< "$args" |
    while read -r arg 
    do echo "  \"$arg\", "
    done
  }
  echo "};"
  echo "static const int config_argc = $(lenght "$args");"
cat << EOF
ANN const char** gwion_config_args(int *argc, char **const argv) {
  const int nargs = config_argc + *argc;
  const char **  args = malloc(nargs * SZ_INT);
  args[0] = *argv;
  for(int i = 0; i < config_argc; i++) {
    args[i + 1] = config_argv[i];
  }
  for(int i = 1; i < *argc; i++) {
    args[i + config_argc] = argv[i];
  }
  *argc = nargs;
  return args;
}
EOF
} >> embed/embed.c

cflags=$(jq -rc '.cflags' <<< "$json")
array_is_ok "$cflags" && {
  jq -rc '.[]' <<< "$cflags" |
  while read -r cflag
  do cflag "$cflag"
  done
}
ldflags=$(jq -rc '.ldflags' <<< "$json")
array_is_ok "$ldflags" && {
  jq -rc '.[]' <<< "$ldflags" |
  while read -r ldflag
  do config "LDFLAGS += $ldflag "
  done
}

rc=$(jq -rc '.urc' <<< "$json")
[ "$rc" = "false" ] && cflag "-DGWION_HAS_URC=false"
ulib=$(jq -rc '.ulib' <<< "$json")
[ "$ulib" = "false" ] && cflag "-DGWION_HAS_ULIB=false"
uargs=$(jq -rc '.uargs' <<< "$json")
[ "$uargs" = "false" ] && cflag "-DGWION_HAS_UARGS=false"
name=$(jq -rc '.name' <<< "$json")
not_null "$name" && config "PRG := $name"
:
