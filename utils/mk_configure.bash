#!/bin/bash
mk_header() {
  echo "# $1" >&5
  printf "\n# %s\nprintf \"\\\n# %s\\\n\"\n" "$1" "$1"
}

do_expand() {
  if [ -z "$(echo "$1" | cut -d ' ' -f2)" ]
  then
    key=$(echo "$1" | cut -d ":" -f 1)
    printf "%s=%s\nfor iter in \$%s\n" "$key" "$key" "$key"
  else
    printf "for iter in"
    for iter in $1
    do
      key=$(echo "$iter" | cut -d ":" -f 1)
      printf " %s" "$key"
    done
  fi
  printf "\ndo\n\t"
}

do_expand2() {
  if [ -z "$(echo "$1" | cut -d ' ' -f2)" ]
  then
    key=$(echo "$1" | cut -d ":" -f 1)
    printf "%s=%s\nfor iter in \$%s\n" "$key" "$key" "$key"
  else
    printf "for iter in"
    for iter in $1
    do printf " %s" "$iter"
    done
  fi
  printf "\ndo\n\tkey=\$(echo \"\$iter\" | cut -d ':' -f 1)\n\tval=\$(echo \"\$iter\" | cut -d ':' -f 2)\n\tlib=\$(echo \"\$iter\" | cut -d ':' -f 4)\n\t"
}

config_init() {
  set -e
  [ -f configure ] && rm configure
  [ -f Makefile  ] && rm Makefile
  while read -r line
  do
    line=$(echo "$line" | cut -d"#" -f1)
    [ -z "$line" ] && continue
    category=$(echo "$line" | cut -d ":" -f1 | xargs)
    long=$(    echo "$line" | cut -d ":" -f2 | xargs)
    #  short=$(   echo "$line" | cut -d ":" -f3 | xargs)
    #  help=$(    echo "$line" | cut -d ":" -f4 | xargs)
    default=$( echo "$line" | cut -d ":" -f5)

    inc=$( echo "$line" | cut -d ":" -f6)
    lib=$( echo "$line" | cut -d ":" -f7)
    if [ "$category" = "LIB"  ]
    then
      echo "# ARG_OPTIONAL_BOOLEAN([$long],, [enable $long], [$default])"
      echo "# ARG_OPTIONAL_REPEATED([${long}-inc], , [Directories where to look for include files for $long], [$inc])"
      echo "# ARG_OPTIONAL_SINGLE([$long-lib], , [${long} library], [$lib])"
      inc=$( echo "$inc" | tr " " ",")
      LIB+="$long:$default:$inc:$lib "
    elif [ "$category" = "OPT" ]
    then
      #    default=$(echo "$default" | sed 's/ /\\ /g')
      default=${default/ /\\ /}
      OPT+="$long:\"${default}\" "
      echo "# ARG_OPTIONAL_SINGLE([$long], , [value for ${long~~}], [$default])"
    elif [ "$category" = "DIR" ]
    then
      echo "# ARG_OPTIONAL_SINGLE([$long], , [$long path], [$default])"
      DIR+="${long}:$default "
    elif [ "$category" = "USE" ]
    then
      USE+="$long:$default "
      echo "# ARG_OPTIONAL_BOOLEAN([${long}], , [debug $long], [$default])"
    elif [ "$category" = "DEF" ]
    then
      DEF+="$long:$default "
      echo "# ARG_OPTIONAL_SINGLE([${long}], , [debug $long], [$default])"
    fi
  done < "$1"
  cat << EOF
  # ARG_HELP([The general script's help msg])
  # ARGBASH_GO

EOF
}

config_prep() {
  argbash configure.tmp -o configure.argbash
  rm configure.tmp
  head -n -1 configure.argbash > configure.tmp
  mv configure.tmp configure
  rm configure.argbash

  for iter in $OPT
  do
    key=$(echo "${iter}" | cut -d ":" -f 1)
    sed -i "s/^_arg_${key}=\(.*\)/: \"\${${key~~}:=\1}\"\n: \"\$\{_arg_${key}:=\$${key~~}\}\"/" configure
  done

  for iter in $LIB
  do
    key=$(echo "${iter}" | cut -d ":" -f 1)
    sed -i "s/^_arg_${key}=\(.*\)/: \"\${${key~~}_D:=\1}\"\n: \"\$\{_arg_${key}:=\$${key~~}_D\}\"/" configure
    sed -i "s/^_arg_${key}_inc=(\(.*\))/: \"\${${key~~}_INC:=\1}\"\n: \"\$\{_arg_${key}_inc:=\$${key~~}_INC\}\"/" configure
    sed -i "s/^_arg_${key}_lib=\(\(.*\)\)/: \"\${${key~~}_LIB:=\1}\"\n: \"\$\{_arg_${key}_lib:=\$${key~~}_LIB\}\"/" configure
  done

  for iter in $DIR
  do
    key=$(echo "${iter}" | cut -d ":" -f 1)
    sed -i "s/^_arg_${key}=\(.*\)/: \"\${${key~~}_DIR:=\1}\"\n: \"\$\{_arg_${key}:=\$${key~~}_DIR\}\"/" configure
  done

  for iter in $USE
  do
    key=$(echo "${iter}" | cut -d ":" -f 1)
    val=$(echo "${iter}" | cut -d ":" -f 2)
    sed -i "s/^_arg_${key}=\(.*\)/: \"\${USE_${key~~}:=${val}}\"\n: \"\$\{_arg_${key}:=\$USE_${key~~}\}\"/" configure
  done

  for iter in $DEF
  do
    key=$(echo "${iter}" | cut -d ":" -f 1)
    val=$(echo "${iter}" | cut -d ":" -f 2)
    sed -i "s/^_arg_${key}=\(.*\)/: \"\${${key~~}:=${val}}\"\n: \"\$\{_arg_${key}:=\$${key~~}\}\"/" configure
  done
}

config_check() {
  echo "set -e"
  echo -e "\n# remove Makefile\n[ -f Makefile  ] && rm Makefile"
  printf "to_upper(){\n\techo \"\$1\" | tr '[:lower:]' '[:upper:]'\n}\n\n"
  echo "config_check() {"
  # check default driver
  echo -e "\n# check default driver"
  printf "VALID_DRIVER=\""
  for iter in $LIB
  do
    key=$(echo "${iter}" | cut -d ":" -f 1)
    printf " %s" "$key"
  done
  echo -e "\"\ngrep \"\$_arg_d_func\" <<<  \"\$VALID_DRIVER\" > /dev/null || { echo \"invalid default driver\";exit 1; }\n"

  echo "if [ \"\$_arg_double\" = \"on\" ]; then _CFLAGS+=\" -DUSE_DOUBLE -DSPFLOAT=double\";fi"
  echo "if [ \"\$_arg_double\" = \"1\"  ];then _CFLAGS+=\" -DUSE_DOUBLE -DSPFLOAT=double\";fi"
  echo "([ \"\$_arg_double\" = \"on\" ] || [ \"\$_arg_double\" = \"1\"  ]) || _CFLAGS+=\" -DSPFLOAT=float\""
  # build with coverage for now
  echo "if [ \"\$USE_COVERAGE\" = \"1\"  ]; then _CFLAGS+=\" -ftest-coverage -fprofile-arcs --coverage\";fi"
  echo "if [ \"\$USE_COVERAGE\" = \"on\" ]; then _CFLAGS+=\" -ftest-coverage -fprofile-arcs --coverage\";fi"
  echo "if [ \"\$_arg_soundpipe_inc\" ]; then _CFLAGS+=\" \$_arg_soundpipe_inc\";fi"
  #	echo "\$_arg_cc -Iinclude \$_CFLAGS utils/generate_header.c -o utils/generate_header || (echo 'invalid compilation options'; exit 1;)"
  echo "cmd=\"\$_arg_cc -I/usr/local/include -Iinclude \$_CFLAGS utils/generate_header.c -o utils/generate_header\""
  echo "eval \"\$cmd\" || (echo 'invalid compilation options'; exit 1;)"

  # generate header
  echo "./utils/generate_header || exit 1"
  echo "}"
}

make_handle() {
  echo "exec >> Makefile"
  echo "echo \"# generated by ./configure\""
  echo "make_handle() {"
  mk_header "handle base options"
  do_expand "$OPT"
  printf "echo \"\$(to_upper \$iter) ?=\$(eval echo \\\$_arg_\$iter)\"\n\tdone\n"
  #  printf "arg=\"_arg_\${iter}\"\n\techo \"\$(to_upper iter) ?= \${!arg}\"\ndone\n"
  printf " printf \"LDFLAGS += -lm -ldl -rdynamic -lpthread\\\nCFLAGS += -Iinclude -std=c99 -O3 -mfpmath=sse -mtune=native -fno-strict-aliasing -Wall -pedantic -D_GNU_SOURCE\\\n\"\n"

  mk_header "handle boolean options"
  do_expand "$USE"
  printf "if [ \"\$iter\" = \"double\" ]\n\tthen echo \"USE_\$(to_upper \$iter) = \$(eval echo \\\$_arg_\$iter)\"\n"
  printf "\telse echo \"USE_\$(to_upper \$iter) ?= \$(eval echo \\\$_arg_\$iter)\"\n\tfi\ndone\n"
  #  printf "arg=\"_arg_\$iter\"\n\tif [ \"\$iter\" = \"double\" ]\n\tthen echo \"USE_\${iter~~}  = \${!arg}\"\n"
  #  printf "\telse echo \"USE_\${iter~~} ?= \${!arg}\"\n\tfi\ndone\n"

  mk_header "handle definitions"
  do_expand "$DEF"
  printf "echo \"\$(to_upper \$iter) ?= \$(eval echo \\\$_arg_\${iter})_driver\"\ndone\n"
  #  printf "arg=\"_arg_\${iter}\"\n\techo \"\${iter~~} ?= \${!arg}_driver\"\ndone\n"

  mk_header "handle directories"
  do_expand2 "$DIR"
  printf "echo \"GWION_\$(to_upper \"\$key\")_DIR ?= \\\${PREFIX}/lib/Gwion/\${val}\"\ndone"

  mk_header "handle libraries"
  do_expand "$LIB"
  printf "echo \"\$(to_upper \$iter)_D ?= \$(eval echo \\\$_arg_\$iter)\"\ndone\n"
  #  printf "arg=\"_arg_\$iter\"\n\techo \"\${iter~~}_D ?= \${!arg}\"\ndone\n"

  mk_header "initialize source lists"
  do_expand "src"
  printf "echo \"\${iter}_src := \\\$(wildcard \${iter}/*.c)\"\ndone\necho \"lib_src := \\\$(wildcard src/lib/*.c)\"\necho \"oo_src := \\\$(wildcard src/oo/*.c)\"\necho \"vm_src := \\\$(wildcard src/vm/*.c)\"\necho \"ast_src := \\\$(wildcard src/ast/*.c)\"\necho \"parse_src := \\\$(wildcard src/parse/*.c)\"\necho \"ugen_src := \\\$(wildcard src/ugen/*.c)\"\necho \"util_src := \\\$(wildcard src/util/*.c)\"\necho \"emit_src := \\\$(wildcard src/emit/*.c)\"\necho \"drvr_src := src/drvr/driver.c\"\n"
  echo "}"
}

make_add() {
  echo "make_add() {"
  mk_header "add libraries"
  do_expand2 "$LIB"
  printf "if [ \"\${val}\" = \"on\" ]\n\tthen val=1\n\telse val=0\n\tfi\n"
  cat << EOF
  [ -z "\$lib" ] && printf "ifeq (\\\${%s_D}, on)\\\nCFLAGS += -DHAVE_%s\\\ndrvr_src += src/drvr/%s.c\\\nelse ifeq (\\\${%s_D}, 1)\\\nCFLAGS +=-DHAVE_%s\\\ndrvr_src +=src/drvr/%s.c\\\nendif\\\n" \
    "\$(to_upper "\$key")" "\$(to_upper "\$key")" "\$key" "\$(to_upper "\$key")" "\$(to_upper "\$key")" "\${key}"
  [ -z "\$lib" ] || printf "ifeq (\\\${%s_D}, on)\\\nLDFLAGS += %s\\\nCFLAGS += -DHAVE_%s\\\ndrvr_src += src/drvr/%s.c\\\nelse ifeq (\\\${%s_D}, 1)\\\nLDFLAGS += %s\\\nCFLAGS +=-DHAVE_%s\\\ndrvr_src +=src/drvr/%s.c\\\nendif\\\n" \
    "\$(to_upper "\$key")" "\${lib}" "\$(to_upper "\$key")" "\${key}" "\$(to_upper "\$key")" "\${lib}" "\$(to_upper "\$key")" "\${key}"
done
EOF

mk_header "add boolean"
do_expand2 "$USE"
cat << EOF
if [ "\${val}" = "on" ]
then [ "\$key" = "double" ] && val=double;
else [ "\$key" = "double" ] && val=float;
fi
[ "\$key" = "memcheck" ] && printf "ifeq (\\\${USE_%s}, on)\\\nCFLAGS += -g\\\nelse " "\$(to_upper "\$key")"
[ "\$key" = "memcheck" ] && printf "ifeq (\\\${USE_%s}, 1)\\\nCFLAGS += -g\\\nendif\n" "\$(to_upper "\$key")"
[ "\$key" = "coverage" ] && printf "ifeq (\\\${USE_%s}, on)\\\nCFLAGS += -ftest-coverage -fprofile-arcs\\\nelse " "\$(to_upper "\$key")"
[ "\$key" = "coverage" ] && printf "ifeq (\\\${USE_%s}, 1)\\\nCFLAGS += -ftest-coverage -fprofile-arcs\\\\nendif\n" "\$(to_upper "\$key")"
[ "\$key" = "coverage" ] && printf "ifeq (\\\${USE_%s}, on)\\\nLDFLAGS += --coverage\nelse " "\$(to_upper "\$key")"
[ "\$key" = "coverage" ] && printf "ifeq (\\\${USE_%s}, 1)\\\nLDFLAGS += --coverage\nendif\n" "\$(to_upper "\$key")"
[ "\$key" = "gwcov"    ] && printf "ifeq (\\\${USE_%s}, 1)\\\nCFLAGS += -DGWCOV\nendif\n" "\$(to_upper "\$key")"
[ "\$key" = "gwcov"    ] && printf "ifeq (\\\${USE_%s}, on)\\\nCFLAGS += -DGWCOV\nendif\n" "\$(to_upper "\$key")"
  done
  key="double"
  printf "ifeq (\\\${USE_%s}, on)\\\nCFLAGS += -DUSE_%s -DSPFLOAT=double\\\nelse ifeq (\\\${USE_%s}, 1)\\\nCFLAGS +=-DUSE_%s -DSPFLOAT=double\\\nelse\\\nCFLAGS+=-DSPFLOAT=float\\\nendif\\\n" "\$(to_upper \$key)" "\$(to_upper \$key)" "\$(to_upper \$key)" "\$(to_upper \$key)"
EOF

  mk_header "add definitions"
  do_expand2 "$DEF"
  printf "echo \"CFLAGS+= -D\$(to_upper \"\$key\")=\\\${\$(to_upper \"\$key\")}\"\ndone\n"

  mk_header "add directories"
  do_expand "$DIR"
  printf "printf \"CFLAGS+=-DGWION_%%s_DIR=%q\\\${GWION_%%s_DIR}%q\\\n\" \"\$(to_upper \$iter)\" \"\$(to_upper \$iter)\"\ndone\n" '\\\"' '\\\"'

  mk_header "add soundpipe"
  echo "echo \"LDFLAGS += \\\${SOUNDPIPE_LIB}\""
  echo "echo \"CFLAGS  += \\\${SOUNDPIPE_INC}\""

  mk_header "initialize object lists"
  do_expand "src lib ast parse emit oo vm ugen util drvr"
  printf "echo \"\${iter}_obj := \\\$(\${iter}_src:.c=.o)\"\ndone\n"
  echo "}"
}

make_recipe() {
  cat << _EOF
make_recipe() {
cat << EOF
TOOL_OBJ=src/util/map.o src/util/vector.o src/util/symbol.o src/util/err_msg.o src/util/absyn.c src/ast/lexer.o src/ast/parser.o src/parse/op_utils.o
EOF

###########
# recipes #
###########
cat << EOF

# if any debug flag is set, we need -DDEBUG
ifeq (\\\$(findstring DEBUG,\\\$(CFLAGS)), DEBUG)
DEBUG = 1
endif

ifeq (\\\${DEBUG}, 1)
CFLAGS+=-DDEBUG
endif

# config flags
CCFG="\\\${CFLAGS}"
LDCFG="\\\${LDFLAGS}"
# os specific
ifeq (\\\$(shell uname), Linux)
LDFLAGS+=-lrt
endif

# recipes
all: options \\\${src_obj} \\\${lib_obj} \\\${ast_obj} \\\${parse_obj} \\\${emit_obj} \\\${oo_obj} \\\${vm_obj} \\\${util_obj} \\\${ugen_obj} \\\${drvr_obj}
	\\\$(info link \\\${PRG})
	@\\\${CC} \\\${src_obj} \\\${lib_obj} \\\${ast_obj} \\\${parse_obj} \\\${emit_obj} \\\${oo_obj} \\\${vm_obj} \\\${ugen_obj} \\\${util_obj} \\\${drvr_obj} \\\${LDFLAGS} -o \\\${PRG}

options:
	\\\$(info CFLAGS  : \\\${CFLAGS})
	\\\$(info LDFLAGS  : \\\${LDFLAGS})

clean:
	\\\$(info cleaning ...)
	@rm -f */*.o */*/*.o */*.gw.* */*/*.gw.* */*/*.gcda */*/*.gcno \${PRG}

src/arg.o:
	\\\$(info compile \\\$(<:.c=) (with arguments defines))
	@\\\${CC} \\\${CFLAGS} -c src/arg.c -o src/arg.o -DLDFLAGS='\\\${LDCFG}' -DCFLAGS='\\\${CCFG}'

.c.o:
	\\\$(info compile \\\$(<:.c=))
	@\\\${CC} \\\${CFLAGS} -c \\\$< -o \\\$(<:.c=.o)

install: directories
	cp \\\${PRG} \\\${PREFIX}

uninstall:
	rm \\\${PREFIX}/\\\${PRG}

test:
	@bash utils/test.sh tests/sh severity=11 test/sh examples severity=10 tests/error tests/tree tests/ugen_coverage test/bug

parser:
	\\\$(info generating parser)
	@\\\${YACC} -o src/ast/parser.c --defines=include/parser.h utils/gwion.y

lexer:
	\\\$(info generating lexer)
	@\\\${LEX}  -o src/ast/lexer.c utils/gwion.l

gwcov: utils/gwcov.o
	\\\$(info compiling gwcov)
	@\\\${CC} \\\${CFLAGS} utils/gwcov.o -o gwcov \\\${LDFLAGS}

gwlint: \\\${TOOL_OBJ} utils/gwlint.o
	\\\$(info compiling gwlint)
	@\\\${CC} \\\${CFLAGS} -DGWLINT \\\${TOOL_OBJ} utils/gwlint.o \\\${LDFLAGS} -o gwlint

gwtag: \\\${TOOL_OBJ} utils/gwtag.o
	\\\$(info compiling gwtag)
	@\\\${CC} \\\${CFLAGS} -DGWLINT \\\${TOOL_OBJ} utils/gwtag.o \\\${LDFLAGS} -o gwtag

directories:
	mkdir -p \\\${PREFIX} \\\${GWION_ADD_DIR}
EOF

_EOF
  echo "}"
  }

config_post() {
  printf "config_check\nmake_handle\nmake_add\nmake_recipe\n# ] <-- needed because of Argbash"
  sed -i "s/'\"\(.*\)\"'/'\1'/" configure
  sed -i "s/\"\"\(.*\)\"\"/'\1'/" configure
  grep -v '^\#' configure > configure.tmp
  grep -v '^  \#' configure.tmp > configure
  printf "%s\n%s\n" "#!/bin/bash" "$(cat configure)" > configure
  chmod +x configure
  rm configure.tmp
}
# ] <-- needed because of Argbash

config_init "$1" > configure.tmp
config_prep
exec 5<&1
exec >> configure
config_check
make_handle
make_add
make_recipe
config_post

