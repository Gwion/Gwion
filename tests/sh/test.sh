#!/bin/bash
# [test] #0

: "${PRG:=gwion}"
: "${GWION_TEST_DIR:=/tmp}"
: "${GWION_TEST_PREFIX:=gwt_}"

: "${ANSI_RED:=\033[31;1m}"
: "${ANSI_GREEN:=\033[32;1m}"
: "${ANSI_BLUE:=\033[34;1m}"
: "${ANSI_RESET:=\033[0m}"
: "${ANSI_CLEAR:=\033[0K}"
: "${ANSI_BOLD:=\033[33;1m}"

: "${DRIVER:=dummy}"
: "${MODULE:=dummy}"
: "${SUPP:=scripts/supp}"

: "${ASYNC:=4}"
: "${async:=$ASYNC}"

: "${SEVERITY:=11}"
: "${severity:=$SEVERITY}"

assert_returns() {
  [ "$1" -eq 0   ] && return 0
  [ "$1" -eq 139 ] && echo "segfault" > "$2"
  return 1;
}

assert_contain() {
  local contains
  contains=$(grep '#! \[contains\]' "$1" | cut -d "]" -f2)
  contains=${contains:1}
  [ -z "$contains" ] && return 0
  grep "$contains" "$2.err.log" > /dev/null && return 0
  echo "does not contain $contains" > "$2.log"
  return 1
}

assert_exclude() {
  local contains
  contains=$(grep '// \[excludes\]' "$1" | cut -d "]" -f2)
  contains=${contains:1}
  [ -z "$contains" ] && return 0
  grep "$contains" "$2.err.log" > /dev/null || return 0
  echo "does contain $contains" > "$2.log"
  return 1
}

read_test() {
  while read -r line
  do
    if [ "$line" = "#*" ]
    then printf '\t%s\n' line >&2
    else echo "$line"
    fi
  done < "$1"
}

success() {
  local n log desc
  n=$1
  desc=$2
  log=$3
  if [ "$async" -eq 0 ]
  then echo "ok  $(printf "% 4i" "$n") $desc"
  else echo "ok $(printf "% 4i" "$n") $desc" > "$log"
  fi
  return 0
}

fail() {
  local n log desc
  n=$1
  desc=$2
  log=$3
  if [ "$async" -eq 0 ]
  then
    echo "not ok $(printf "% 4i" "$n") $desc"
    echo "# $(cat "$log")"
  else
    local info
    info=$(cat "$log")
    echo "not ok $(printf "% 4i" "$n") $desc" > "$log"
    echo "# $info" >> "$log"
    echo "$desc $(cat "$4")" >> "test.log"
  fi
  return 1
}

do_skip() {
  local n SKIP skip
  SKIP=0
  skip=$(grep '// \[skip\]' "$1")
  [ "$skip" ] && SKIP=1
  [ $SKIP = 0 ] && return 1
  skip=$(echo "$skip" | cut -d ']' -f2 )
  n=$(printf "% 4i" "$2")
  if [ "$async" -eq 0 ]
  then echo "ok $(printf "% 4i" "$n") $3 # Skip $skip"
  else echo "ok $(printf "% 4i" "$n") $3 # Skip $skip" > "$4"
  fi
  return 0
}

do_todo() {
  local n SKIP skip
  SKIP=0
  skip=$(grep '// \[todo\]' "$1")
  [ "$skip" ] && SKIP=1
  [ $SKIP = 0 ] && return 1
  skip=$(echo "$skip" | cut -d ']' -f2 )
  n=$(printf "% 4i" "$2")
  if [ "$async" -eq 0 ]
  then echo "ok  $(printf "% 4i" "$n") $3 # Todo $skip"
  else echo "ok  $(printf "% 4i" "$n") $3 # Todo $skip" > "$4"
  fi
  return 0
}

test_gw(){
  local n file log ret
  n=$2
  if [ "$1" = "no_file" ]
  then file=""
  else file=$1
  fi
  log=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n")
  slog=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n").std.log
  elog=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n").err.log
  rlog=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n").log
  LANG=C ./"$PRG" "$GWOPT" -m "$MODULE" -d "$DRIVER" "$file" > "$slog" 2>"$elog" |:
  ret=$?
  #enable skip
  [ -z "$file" ] || do_skip "$1" "$n" "$file" "$rlog" && return 0
  # enable todo
  [ -z "$file" ] || do_todo "$1" "$n" "$file" "$rlog" && return 0

  [ $severity -lt 1  ]           && success "$n" "$file" "$rlog" && return 0
  assert_returns  "$ret"  "$log" || fail    "$n" "$file" "$rlog" || return 1
  [ -z "$file" ] || [ $severity -lt 2  ] && success "$n" "$file" "$rlog" && return 0
  assert_contain  "$file" "$log" || fail    "$n" "$file" "$rlog" || return 1
  [ $severity -lt 3  ]           && success "$n" "$file" "$rlog" && return 0
  [ -f "$file" ] && assert_exclude  "$file" "$log" || fail    "$n" "$file" "$rlog" || return 1
  [ $severity -lt 4  ]           && success "$n" "$file" "$rlog" && return 0
  success "$n" "$file" "$rlog" && return 0
}
