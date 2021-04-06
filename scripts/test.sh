#!/bin/bash

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

: "${ASYNC:=4}"
: "${async:=$ASYNC}"

: "${SEVERITY:=11}"
: "${severity:=$SEVERITY}"


: "${separator:=/}"


[ "$async" -gt 0 ] && set -m

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
  grep "$contains" "$2.std.log" > /dev/null && return 0
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
  [ -f "$1" ] || return # needed for macOS, modulo does not seem to work
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
  log=${GWION_TEST_DIR}${separator}${GWION_TEST_PREFIX}$(printf "%04i" "$n")
  slog=${GWION_TEST_DIR}${separator}${GWION_TEST_PREFIX}$(printf "%04i" "$n").std.log
  elog=${GWION_TEST_DIR}${separator}${GWION_TEST_PREFIX}$(printf "%04i" "$n").err.log
  rlog=${GWION_TEST_DIR}${separator}${GWION_TEST_PREFIX}$(printf "%04i" "$n").log
  LANG=C ./"$PRG" "$GWOPT" -d "$DRIVER" "$file" > "$slog" 2>"$elog" |:
  ret=$?
  #enable skip
  do_skip "$1" "$n" "$file" "$rlog" && return 0
  # enable todo
  do_todo "$1" "$n" "$file" "$rlog" && return 0

  [ $severity -lt 1  ]           && success "$n" "$file" "$rlog" && return 0
  assert_returns  "$ret"  "$log" || fail    "$n" "$file" "$rlog" || return 1
  [ $severity -lt 2  ]           && success "$n" "$file" "$rlog" && return 0
  assert_contain  "$file" "$log" || fail    "$n" "$file" "$rlog" || return 1
  [ $severity -lt 3  ]           && success "$n" "$file" "$rlog" && return 0
  assert_exclude  "$file" "$log" || fail    "$n" "$file" "$rlog" || return 1
  [ $severity -lt 4  ]           && success "$n" "$file" "$rlog" && return 0
  success "$n" "$file" "$rlog" && return 0
}

count_tests_sh(){
  local count
  count=$(grep '\[test\] #' "$1" | cut -d '#' -f 3)
  echo "$count"
}

count_tests(){
  local ret
  ret=0
  for file in "$1"/*
  do
    if [ "${file: -3}" = ".gw" ]
    then ret=$((ret+1))
    elif [ "${file: -3}" = ".sh" ]
    then
         local _n=$(count_tests_sh "$file")
         ret=$((ret+_n));
    fi
  done
  echo "$ret"
}

dir_contains() {
  local ret
  ret=0
  for file in "$1"/*
  do
    len=${#2}
    if [ "${1: -$len}" = "$2" ]
    then return 0
    fi
  done
  return 1
}

test_dir() {
  local n offset l
  n=$2
  l=$n
  offset=$n
  found=0
  grep '\.gw' <<< "$(ls "$1")" &> /dev/null && found=1
  if [ "$found" -eq 1 ]
  then
    for file in "$1"/*.gw
    do
      if [ "$async" -ne 0 ]
      then test_gw "$file" "$n"&
      else test_gw "$file" "$n"
      fi
      [ "$async" -ne 0 ] && {
      if [ $((n % async)) -eq 0 ]
      then
        wait
        for i in $(seq "$offset" "$n")
        do
          read_test "${GWION_TEST_DIR}${separator}${GWION_TEST_PREFIX}$(printf "%04i" "$i").log"
          l=$((l+1))
        done
        offset=$((offset + async));
      fi
    }
    n=$((n+1))
  done
  [ $l != $n ] && [ "$async" -ne 0 ] && {
    wait
    local rest=$((n-l))
    if [ $rest -gt 0 ]
    then
      for i in $(seq $((l))  $((n-1)))
      do read_test "${GWION_TEST_DIR}${separator}${GWION_TEST_PREFIX}$(printf "%04i" "$i").log"
      done
    fi
  }
  fi


  found=0
  grep '\.sh' <<< "$(ls "$1")" &> /dev/null && found=1
  if [ "$found" -eq 1 ]
  then
#    local old_async=$async
#    async=0;
    #		[ -f ${GWION_TEST_DIR}/${GWION_TEST_PREFIX}bailout ] && exit 1
    for file in "$1"/*.sh
    do
      [ "$file" = "$1/*.sh" ] && continue
      local count
      count=$(grep '\[test\] #' "$file" | cut -d '#' -f 3)
      [ "$count" -gt 0 ] && echo "## $file"
      bash "$file" "$((n))"
      n=$((n+count))
    done
#    async=$old_async
  fi
}

count_test(){
  local n_test n_dir
  n_test=0
  for arg in "$@"
  do
    if [ -f "$arg" ]
    then
      if [ "${arg: -3}" = ".gw" ]
      then
        n_test=$((n_test + 1))
      elif [ "${arg: -3}" = ".sh" ]
      then
        local c
        c=$(count_tests_sh "$arg")
        n_test=$((n_test + c))
      fi
    elif [ -d "$arg" ]
    then
      n_test=$((n_test + $(count_tests "$arg") + 1))
      n_dir=$((n_dir+1))
    fi
  done
  [ $n_test -lt $async ] && async=0
  echo "1...$((n_test-n_dir))"
}

do_test() {
  local n_test res1
  n_test=1
  count_test "$@"
  for arg in "$@"
  do
    if [ "${arg:0:6}" = "async=" ]
    then
      async=$(echo "$arg" | cut -d '=' -f 2);
      [ "$async" -eq 1 ] && async=0
    elif [ "${arg:0:9}" = "severity=" ]
    then if [ "$(uname)" == "Linux" ] # only use severity on linux
    then severity=$(echo "$arg" | cut -d '=' -f 2);
    fi
  elif [ "${arg:0:9}" = "suppressions=" ]
  then suppressions=$(echo "$arg" | cut -d '=' -f 2);
    #		elif [ "${arg:0:9}" = "bailout=" ]
    #		then bailout=$(echo "$arg" | cut -d '=' -f 2);
  elif [ -f "$arg" ]
  then
    #			[ -f ${GWION_TEST_DIR}/${GWION_TEST_PREFIX}bailout ] && exit 1
    if [ "${arg: -3}" = ".gw" ]
    then test_gw "$arg" "$n_test"
      n_test=$((n_test + 1))
    elif [ "${arg: -3}" = ".sh" ]
    then
      local old_async=$async
      async=0
      local c
      c=$(count_tests_sh "$arg")
      [ "$c" -eq 0 ] && continue
      echo "## $arg"
      bash "$arg" "$n_test"
      n_test=$((n_test + c))
      async=$old_async
    fi
  elif [ -d "$arg" ]
  then
    #			[ -f ${GWION_TEST_DIR}/${GWION_TEST_PREFIX}bailout ] && exit 1
    [ "${arg: -1}" = "/" ] && arg=${arg%?}
    # make header
    for i in $(seq 1 $((${#arg}+4))); do printf "#"; done
    echo -e '\n'"# $arg #"'\t'"${ANSI_RESET}severity: ${ANSI_BOLD}$severity${ANSI_RESET}"
    for i in $(seq 1 $((${#arg}+4))); do printf "#"; done
    echo -e '\n'
    test_dir "$arg" "$n_test"
    n_test=$((n_test + $(count_tests "$arg")))
  fi
done
}

summary() {
  win=$1
  expected=$2
  skip=$3
  todo=$4
  if [ "$win" = "$expected" ] && [ "$skip" = 0 ] && [ "$todo" = 0 ]
  then echo -e '\n\t'"${ANSI_GREEN}Everything is OK!${ANSI_RESET}"'\n'
  else
    echo -e '\n\t'"${ANSI_GREEN}Success: $win/$expected${ANSI_RESET}"
    echo -e '\t'"${ANSI_RED}Failure: $failure${ANSI_RESET}"
    echo -e '\t'"Skipped: ${ANSI_BOLD}$skip${ANSI_RESET}"
    echo -e '\t'"todo   : ${ANSI_BOLD}$todo${ANSI_RESET}"'\n' >&2
  fi
}

consummer() {
  local win failure skip todo expected
  win=0
  failure=0
  skip=0
  todo=0
  while read -r line
  do
    # plan
    if [ "${line:0:4}" = "1..." ]
    then
      expected="${line:4}"
    elif [ "${line:0:1}" = "#" ]
    then
      echo -e "${ANSI_BOLD}$line${ANSI_RESET}" >&2
      continue
    elif [ "${line:0:6}" = "not ok" ]
    then
      printf "${ANSI_RED}not ok${ANSI_RESET}%s " "${line:6}">&2
      failure=$((failure+1))
    elif [ "${line:0:2}" = "ok" ]
    then
      win=$((win+1))
      base=$(echo "$line" | cut -d "#" -f 1)
      directive=$(echo "$line" | cut -d "#" -f 2)
      if [ "$directive" != "$base" ]
      then echo -e "${ANSI_GREEN}ok   ${ANSI_RESET}${base:2} ${ANSI_BLUE}# ${directive:1:4}${ANSI_RESET}${directive:5}"
      else echo -e "${ANSI_GREEN}ok   ${ANSI_RESET}${base:2}"
      fi
      [ "${directive:1:4}" = "Todo" ] && todo=$((todo+1))
      [ "${directive:1:4}" = "Skip" ] && skip=$((skip+1))
      #    elif [ "${line:0:9}" = "Bail out!" ]
      #    then
      #      base=$(echo "$line" | cut -d "#" -f 1)
      #      directive=$(echo "$line" | cut -d "#" -f 2)
      #      total=$((win+failure+1))
      #      if [ "$directive" != "$base" ]
      #      then echo -e "${ANSI_RED}${base}${ANSI_RESET} at test $total# ${ANSI_BOLD}# $directive${ANSI_RESET}"
      #      else echo -e "${ANSI_RED}${base}${ANSI_RESET} at test $total"
      #      fi
      #      break
    fi
    # ignore the rest
  done <&0
  summary "$win" "$expected" "$skip" "$todo"
  [ "$failure" -gt 0 ] && return 1
  return 0
}

clean() {
  rm -f ${GWION_TEST_DIR}${separator}{${GWION_TEST_PREFIX}{*.log,bailout},In.gw}
}

[ -f test.log ] && rm test.log
[ $# -ne 0 ] && do_test "${@}" | consummer

if [ -f test.log ] && [ -z "$NO_LOG" ]
then
  cat test.log
  for a in $(cat test.log)
  do ./gwion $a
  done
  exit 1
else
  exit 0
fi

