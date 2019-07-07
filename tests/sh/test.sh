#!/bin/bash
# [test] #0

: "${PRG:=gwion}"
: "${VALGRIND:=valgrind}"
: "${GWION_TEST_DIR:=/tmp}"
: "${GWION_TEST_PREFIX:=gwt_}"

: "${ANSI_RED:=\033[31;1m}"
: "${ANSI_GREEN:=\033[32;1m}"
: "${ANSI_BLUE:=\033[34;1m}"
: "${ANSI_RESET:=\033[0m}"
: "${ANSI_CLEAR:=\033[0K}"
: "${ANSI_BOLD:=\033[33;1m}"

: "${DRIVER:=dummy}"
: "${SUPP:=help/supp}"

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
  contains=$(grep '// \[contains\]' "$1" | cut -d "]" -f2)
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

assert_rw() {
  grep 'Invalid \(read\|write\) of size' "$2.valgrind.log" > /dev/null || return 0
  echo "invalid read/write" > "$2.log"
  return 1
}

assert_free() {
  grep 'Invalid free()' "$2.valgrind.log" > /dev/null || return 0
  echo "invalid free" > "$2.log"
  return 1
}

assert_initial() {
  grep 'Conditional jump or move depends on uninitialised value(s)' "$2.valgrind.log" > /dev/null || return 0
  echo "uninitialed value" > "$2.log"
  return 1
}

assert_syscall() {
  grep 'Syscall param .* uninitialised byte(s)' "$2.valgrind.log" > /dev/null || return 0
  echo "uninitialed value in syscall" > "$2.log"
  return 1
}

assert_mismatch() {
  grep 'Mismatched free() / delete / delete \[\]' "$2.valgrind.log" > /dev/null || return 0
  echo "mismatched free" > "$2.log"
  return 1
}

assert_overlap() {
  grep 'Source and destination overlap' "$2.valgrind.log" > /dev/null || return 0
  echo "mem overlap" > "$2.log"
  return 1
}

assert_fishy() {
  grep "Argument 'size' of .* has a fishy (possibly negative) value:" "$2.valgrind.log" > /dev/null || return 0
  echo "fishy alloc" > "$2.log"
  return 1
}

assert_leak() {
  grep "All heap blocks were freed -- no leaks are possible" "$2.valgrind.log" > /dev/null && return 0
  #[ "$suppressions" -eq 0 ] && echo "mem leak" > "$2.log" && return 1
  #[ -z "$suppressions" ] && echo "mem leak" > "$2.log" && return 1
  heap=$(grep "in use at exit:" "$2.valgrind.log" | cut -d ":" -f2)
  supp=$(grep "suppressed: .* bytes"     "$2.valgrind.log" | cut -d ":" -f2)
  #[ -z "$supp" ] && echo "mem leak" > "$2.log" && return 1
  [ "$heap" = "$supp" ] && return 0
  echo "mem leak" > "$2.log"
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
  file=$1
  log=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n")
  slog=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n").std.log
  elog=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n").err.log
  vlog=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n").valgrind.log
  rlog=${GWION_TEST_DIR}/${GWION_TEST_PREFIX}$(printf "%04i" "$n").log
  if [ "$VALGRIND" == "NO_VALGRIND" ]
  then LANG=C ./"$PRG" "$GWOPT" -d "$DRIVER" "$file" > "$slog" 2>"$elog" |:
  else
    LANG=C "$VALGRIND" --suppressions="$SUPP" --log-file="$vlog" \
    ./"$PRG" "$GWOPT" -d "$DRIVER" "$file" > "$slog" 2>"$elog" |:
  fi
  ret=$?
  #enable skip
  do_skip "$1" "$n" "$file" "$rlog" && return 0
  # enable todo
  do_todo "$1" "$n" "$file" "$rlog" && return 0

  [ $severity -lt 1  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_returns  "$ret"  "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 2  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_contain  "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 3  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_exclude  "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 4  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_rw       "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 5  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_initial  "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 6  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_syscall  "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 7  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_free     "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 8  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_mismatch "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 9  ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_overlap  "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 10 ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_fishy    "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  [ $severity -lt 11 ]           && success "$n" "$file" "$rlog" "$vlog" && return 0
  assert_leak     "$file" "$log" || fail    "$n" "$file" "$rlog" "$vlog" || return 1
  success "$n" "$file" "$rlog" && return 0
}
