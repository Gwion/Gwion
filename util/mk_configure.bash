#!/bin/bash
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
  elif [ "$category" = "DBG" ]
  then
    DBG+="$long:$default "
    echo "# ARG_OPTIONAL_BOOLEAN([debug-${long}], , [debug $long], [$default])"
  elif [ "$category" = "USE" ]
  then
    USE+="$long:$default "
    echo "# ARG_OPTIONAL_BOOLEAN([${long}], , [debug $long], [$default])"
  elif [ "$category" = "DEF" ]
  then
    DEF+="$long:$default "
    echo "# ARG_OPTIONAL_SINGLE([${long}], , [debug $long], [$default])"
 fi
done < "$1" >> configure

cat << EOF >> configure
# ARG_HELP([The general script's help msg])
# ARGBASH_GO

EOF

argbash configure -o configure
#exit
# remove footer
head -n -1 configure > /tmp/_test.argbash
mv /tmp/_test.argbash configure
#cat configure
echo "[ -f Makefile  ] && rm Makefile" >> configure
for iter in $OPT
do
  key=$(echo "${iter}" | cut -d ":" -f 1)
#  val=$(echo "${iter}" | cut -d ":" -f 2 | sed 's#/#\\/#g')
  val="$(echo "${iter}" | cut -d ":" -f 2)"
  sed -i "s/^_arg_${key}=\(.*\)/: \"\${${key~~}:=\1}\"\n: \"\$\{_arg_${key}:=\$${key~~}\}\"/" configure
done

for iter in $LIB
do
  key=$(echo "${iter}" | cut -d ":" -f 1)
  val=$(echo "${iter}" | cut -d ":" -f 2)
  inc=$(echo "${iter}" | cut -d ":" -f 3 | sed 's#/#\\/#g')
  lib=$(echo "${iter}" | cut -d ":" -f 4)
  inc=$( echo "$inc"   | tr "," " ")
  sed -i "s/^_arg_${key}=\(.*\)/: \"\${${key~~}_D:=\1}\"\n: \"\$\{_arg_${key}:=\$${key~~}_D\}\"/" configure
#  [ -z "$inc" ] ||
    sed -i "s/^_arg_${key}_inc=(\(.*\))/: \"\${${key~~}_INC:=\1}\"\n: \"\$\{_arg_${key}_inc:=\$${key~~}_INC\}\"/" configure
#  [ -z "$lib" ] ||
    sed -i "s/^_arg_${key}_lib=\(\(.*\)\)/: \"\${${key~~}_LIB:=\1}\"\n: \"\$\{_arg_${key}_lib:=\$${key~~}_LIB\}\"/" configure
done

for dir in $DIR
do
  key=$(echo "${dir}" | cut -d ":" -f 1)
  val=$(echo "${dir}" | cut -d ":" -f 2)
  sed -i "s/^_arg_${key}=\(.*\)/: \"\${${key~~}_DIR:=\1}\"\n: \"\$\{_arg_${key}:=\$${key~~}_DIR\}\"/" configure
done

for iter in $DBG
do
  key=$(echo "${iter}" | cut -d ":" -f 1)
  val=$(echo "${iter}" | cut -d ":" -f 2)
  sed -i "s/^_arg_debug_${key}=\(.*\)/: \"\${DEBUG_${key~~}:=\1}\"\n: \"\$\{_arg_debug_${key}:=\$DEBUG_${key~~}\}\"/" configure
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
  sed -i "s/^_arg_${key}=\(.*\)/: \"\${${key~~}:=${val}_driver}\"\n: \"\$\{_arg_${key}:=${key~~}\}\"/" configure
done

############
# LibCheck #
############
#ARGS="$arg_cflags"
#for iter in $LIB
#do
  #key=$(echo "$iter" | cut -d ":" -f 1)
  #val=$(echo "$iter" | cut -d ":" -f 2)
  #inc=$(echo "$iter" | cut -d ":" -f 3 | sed 's#/#\\/#g')
  #lib=$(echo "$iter" | cut -d ":" -f 4)
  #inc=$( echo "$inc" | tr "," " ")
  #[ "$val" = "on" ] && export ARGS+="$lib"
  #[ "$val" = "1" ] && ARGS+="$lib"
  #echo "_CFLAGS+=$lib" >> configure
#done
{
	echo "set -e "
	echo "if [ \"\$_arg_double\" = \"on\" ]; then _CFLAGS+=\" -DUSE_DOUBLE -DSPFLOAT=double\";fi"
	echo "if [ \"\$_arg_double\" = \"1\"  ];then _CFLAGS+=\" -DUSE_DOUBLE -DSPFLOAT=double\";fi"
	echo "([ \"\$_arg_double\" = \"on\" ] || [ \"\$_arg_double\" = \"1\"  ]) || _CFLAGS+=\" -DSPFLOAT=float\""
	# build with coverage for now
	echo "if [ \"\$USE_COVERAGE\" = \"1\"  ]; then _CFLAGS+=\" -ftest-coverage -fprofile-arcs --coverage\";fi"
	echo "if [ \"\$USE_COVERAGE\" = \"on\" ]; then _CFLAGS+=\" -ftest-coverage -fprofile-arcs --coverage\";fi"
	echo "if [ \"\$_arg_soundpipe_inc\" ]; then _CFLAGS+=\" \$_arg_soundpipe_inc\";fi"
	echo "echo \"\$_arg_cc -Iinclude -DDEBUG \$_CFLAGS util/generate_header.c core/err_msg.c -o util/generate_header || (echo 'invalid compilation options'; exit 1;)\""
	echo "\$_arg_cc -Iinclude -DDEBUG \$_CFLAGS util/generate_header.c core/err_msg.c -o util/generate_header || (echo 'invalid compilation options'; exit 1;)"

	# generate header
	echo "echo generate header"
	echo "./util/generate_header || exit 1"

	# generate parser
#	echo "echo generate parser"
#	echo "\$_arg_yacc -o eval/parser.c -dv eval/gwion.y -x || exit 1"

	# generate lexer
#	echo "echo generate lexer"
#	echo "\$_arg_lex -o eval/lexer.c eval/gwion.l || exit 1"

	# generate soundpipe
	echo "echo generate soundpipe wrapper"
	echo "lua util/import.lua \"\$_arg_data\" > ugen/soundpipe.c || exit 1"
############
# Makefile #
############
	echo "{"
	for iter in $OPT
	do
	  key=$(echo "$iter" | cut -d ":" -f 1)
	  echo "echo \"${key~~} ?= \${_arg_${key}}\""
	done
	echo "} >> Makefile"
#cat << _EOF >> configure
cat << _EOF

cat <<-  EOF >> Makefile
# base flags
LDFLAGS += -lm -ldl -rdynamic -lpthread
CFLAGS += -Iinclude -std=c99 -O3 -mfpmath=sse -mtune=native -fno-strict-aliasing -Wall -pedantic -D_GNU_SOURCE

EOF
_EOF
} >> configure



echo "# handle boolean options" >> configure
echo 'echo "# handle boolean options" >> Makefile' >> configure
for iter in $USE
do
  key=$(echo "$iter" | cut -d ":" -f 1)
  val=$(echo "$iter" | cut -d ":" -f 2)
  echo "echo \"USE_${key~~} ?= \$_arg_${key}\" >> Makefile" >> configure
done

echo "# handle definitions" >> configure
echo 'echo "# handle definitions" >> Makefile' >> configure
for dir in $DEF
do
  key=$(echo "$dir" | cut -d ":" -f 1)
  val=$(echo "$dir" | cut -d ":" -f 2)
  echo "echo \"${key~~} ?= ${val}_driver\" >> Makefile" >> configure
done

echo "# handle directories" >> configure
echo 'echo "# handle directories" >> Makefile' >> configure
for dir in $DIR
do
  key=$(echo "$dir" | cut -d ":" -f 1)
  val=$(echo "$dir" | cut -d ":" -f 2)
  echo "echo \"GWION_${key~~}_DIR ?= \\\${PREFIX}/lib/Gwion/${val}\" >> Makefile" >> configure
done

echo "# handle libraries" >> configure
echo 'echo "# handle libraries" >> Makefile' >> configure
for lib in $LIB
do
  key=$(echo "${lib}" | cut -d ":" -f 1)
  val=$(echo "${lib}" | cut -d ":" -f 2)
  if [ "${val}" = "on" ]
  then val=1
  else val=0
  fi
  echo "echo \"${key~~}_D ?= ${val}\" >> Makefile" >> configure
done

echo "# handle debug" >> configure
echo 'echo "# handle debug" >> Makefile' >> configure
for dir in $DBG
do
  key=$(echo "$dir" | cut -d ":" -f 1)
  val=$(echo "$dir" | cut -d ":" -f 2)
  echo "echo \"DEBUG_${key~~} ?= \$_arg_debug_${key}\" >> Makefile" >> configure
done

cat << EOF >> configure
echo '' >> Makefile
{
	echo '# initialize source lists'
	for tgt in "core" "lang" "ugen" "eval"
	do echo "\${tgt}_src := \\\$(wildcard \${tgt}/*.c)"
	done
	printf "drvr_src := drvr/driver.c\n\n# add libraries\n"
} >> Makefile
EOF

for iter in $LIB
do
  key=$(echo "${iter}" | cut -d ":" -f 1)
  val=$(echo "${iter}" | cut -d ":" -f 2)
  lib=$(echo "${iter}" | cut -d ":" -f 4)
  if [ "${val}" = "on" ]
  then val=1
  else val=0
  fi
  [ -z "$lib" ] && echo "echo -e \"ifeq (\\\${${key~~}_D}, on)\nCFLAGS += -DHAVE_${key~~}\ndrvr_src += drvr/${key}.c\nelse ifeq (\\\${${key~~}_D}, 1)\nCFLAGS +=-DHAVE_${key~~}\ndrvr_src +=drvr/${key}.c\nendif\n\" >> Makefile"  >> configure
  [ -z "$lib" ] || echo "echo -e \"ifeq (\\\${${key~~}_D}, on)\nLDFLAGS += ${lib}\nCFLAGS += -DHAVE_${key~~}\ndrvr_src += drvr/${key}.c\nelse ifeq (\\\${${key~~}_D}, 1)\nLDFLAGS += ${lib}\nCFLAGS +=-DHAVE_${key~~}\ndrvr_src +=drvr/${key}.c\nendif\n\" >> Makefile"  >> configure
done

echo 'echo "# add boolean" >> Makefile' >> configure
for iter in $USE
do
  key=$(echo "$iter" | cut -d ":" -f 1)
  val=$(echo "$iter" | cut -d ":" -f 2)
  if [ "${val}" = "on" ]
  then [ "$key" = "double" ] && val=double;
  else [ "$key" = "double" ] && val=float;
  fi
  [ "$key" = "memcheck" ] && echo "echo -e \"ifeq (\\\${USE_${key~~}}, on)\nCFLAGS += -g\nendif\" >> Makefile" >> configure
  [ "$key" = "memcheck" ] && echo "echo -e \"ifeq (\\\${USE_${key~~}}, 1)\nCFLAGS += -g\nendif\" >> Makefile" >> configure
  [ "$key" = "coverage" ] && echo "echo -e \"ifeq (\\\${USE_${key~~}}, on)\nCFLAGS += -ftest-coverage -fprofile-arcs\nendif\" >> Makefile" >> configure
  [ "$key" = "coverage" ] && echo "echo -e \"ifeq (\\\${USE_${key~~}}, 1)\nCFLAGS += -ftest-coverage -fprofile-arcs\nendif\" >> Makefile" >> configure
  [ "$key" = "coverage" ] && echo "echo -e \"ifeq (\\\${USE_${key~~}}, on)\nLDFLAGS += --coverage\nendif\" >> Makefile" >> configure
  [ "$key" = "coverage" ] && echo "echo -e \"ifeq (\\\${USE_${key~~}}, 1)\nLDFLAGS += --coverage\nendif\" >> Makefile" >> configure
done
key="double"
echo "echo -e \"ifeq (\\\${USE_${key~~}}, on)\nCFLAGS += -DUSE_${key~~} -DSPFLOAT=double\nelse ifeq (\\\${USE_${key~~}}, 1)\nCFLAGS +=-DUSE_${key~~} -DSPFLOAT=double\nelse\nCFLAGS+=-DSPFLOAT=float\nendif\n\" >> Makefile"  >> configure

echo 'echo "# add definitions" >> Makefile' >> configure
for iter in $DEF
do
  key=$(echo "$iter" | cut -d ":" -f 1)
  val=$(echo "$iter" | cut -d ":" -f 2)
  echo "echo 'CFLAGS+= -D${key~~}=${val}_driver' >> Makefile" >> configure
done

echo 'echo "# add directories" >> Makefile' >> configure
for dir in $DIR
do
  key=$(echo "$dir" | cut -d ":" -f 1)
  val=$(echo "$dir" | cut -d ":" -f 2)
#  echo "echo \"CFLAGS+= -DGWION_${key~~}_DIR=\\\"\\\${GWION_${key~~}_DIR}\\\"\" >> Makefile" >> configure
  echo "printf 'CFLAGS+= -DGWION_${key~~}_DIR=' >> Makefile" >> configure
#  echo "printf\"\\\"\\\${GWION_${key~~}_DIR}\\\\" >> Makefile" >> configure
  echo "printf '\' >> Makefile" >> configure
  echo "printf '\"' >> Makefile" >> configure
  echo "printf '\${GWION_${key~~}_DIR}' >> Makefile" >> configure
  echo "printf '\' >> Makefile" >> configure
  echo "printf '\"' >> Makefile" >> configure
  echo "printf '\n' >> Makefile" >> configure
#  echo "echo \"CFLAGS+= -DGWION_${key~~}_DIR=\\\"\\\${GWION_${key~~}_DIR}\\\"\" >> Makefile" >> configure
done

echo 'echo "# add debug flags" >> Makefile' >> configure
for iter in $DBG
do
  key=$(echo "$iter" | cut -d ":" -f 1)
  val=$(echo "$iter" | cut -d ":" -f 2)
  echo "printf \"ifeq (\\\${DEBUG_${key~~}}, on)\nCFLAGS += -DDEBUG_${key~~}\nendif\n\" >> Makefile" >> configure
  echo "printf \"ifeq (\\\${DEBUG_${key~~}}, 1)\nCFLAGS += -DDEBUG_${key~~}\nendif\n\"  >> Makefile" >> configure
done

{
	echo 'echo "# add soundpipe" >> Makefile'
	echo "echo \"LDFLAGS+=\${SOUNDPIPE_LIB}\" >> Makefile"
	echo "echo \"CFLAGS+=\${SOUNDPIPE_INC}\"  >> Makefile"
	#os specific
	echo 'echo "# os specific" >> Makefile'
# pthread
} >> configure

cat << _EOF >> configure
# initialize object lists
echo '' >> Makefile
echo '# initialize object lists' >> Makefile
for tgt in "core" "lang" "ugen" "drvr" "eval"
do echo "\${tgt}_obj := \\\$(\${tgt}_src:.c=.o)" >> Makefile
done

###########
# recipes #
###########
cat << EOF >> Makefile

# if any debug flag is set, we need -DDEBUG
ifeq (\\\$(findstring DEBUG,\\\$(CFLAGS)), DEBUG)
DEBUG = 1
endif

ifeq (\\\${DEBUG}, 1)
CFLAGS+=-DDEBUG
endif

LDFLAGS+=-lsndfile

# os specific
ifeq (\\\$(shell uname), Linux)
LDFLAGS+=-lrt
endif

# recipes
all: \\\${core_obj} \\\${lang_obj} \\\${eval_obj} \\\${ugen_obj} \\\${drvr_obj}
	\\\${CC} \\\${core_obj} \\\${lang_obj} \\\${eval_obj} \\\${ugen_obj} \\\${drvr_obj} \\\${LDFLAGS} -o \\\${PRG}

clean:
	@rm -f */*.o \${PRG}

.c.o:
	\\\${CC} \\\${CFLAGS} -c \\\$< -o \\\$(<:.c=.o)

install:
	@echo "'gwion' is in pre-alpha stage, no install for now."

uninstall:
	@echo "'gwion' is in pre-alpha stage, no install for now."

test:
	@bash -c "source util/test.sh; do_test examples tests/error tests/tree tests/sh tests/bug | consummer"

parser:
	\\\${YACC} -o eval/parser.c -dv eval/gwion.y -x

lexer:
	\\\${LEX}  -o eval/lexer.c eval/gwion.l

EOF

# ] <-- needed because of Argbash
_EOF


sed -i "s/'\"\(.*\)\"'/'\1'/" configure
sed -i "s/\"\"\(.*\)\"\"/'\1'/" configure

printf "%s\n%s\n" "#!/bin/bash" "$(cat configure)" > configure
chmod +x configure
