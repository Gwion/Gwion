#!/bin/sh
: "${AFL_PREFIX:=afl-}"
: "${AFL_CC:=afl-gcc}"
: "${RESULT_DIR:=${AFL_PREFIX}result}"

clean_lib() {
  echo "🛀 cleaning $1"
  cd "$1" && make clean all && cd ..
}

compile() {
  export CC="$AFL_CC"
  clean_lib util
  clean_lib ast
  clean_lib "$PWD"
}

prepare() {
  CRASH_DIR=${AFL_PREFIX}crash
  if [ -d "$CRASH_DIR" ]
  then echo "✋ $CRASH_DIR already contains at risk data"
  else mkdir "$CRASH_DIR"
  fi
  for file in "$RESULT_DIR"/*/crashes/id*
  do
    NEW_NAME="$(basename "$file" | sed 's/id:0*\([0-9]*\),.*/\1.gw/')"
    cp "$file" "$NEW_NAME"
  done
}

#compile
prepare
