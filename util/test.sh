#!/bin/bash
set -m

: "${ASYNC:=4}"
: "${async:=$ASYNC}"

: "${SEVERITY:=10}"
: "${severity:=$SEVERITY}"

: "${SUPPRESSIONS:=1}"
: "${suppressions:=$SUPPRESSIONS}"

assert_returns() {
	[ "$1" -eq 0   ] && return 0
	[ "$1" -eq 139 ] && echo "segfault" > "$2"
	return 1;
}

assert_contain() {
	local contains
	contains=$(grep "// \[contains\]" "$1" | cut -d "]" -f2)
	contains=${contains:1}
	[ -z "$contains" ] && return 0
	grep "$contains" "$2" > /dev/null && return 0
	echo "does not contain $contains" > "$2"
	return 1
}

assert_exclude() {
	local contains
	contains=$(grep "// \[exclude\]" "$1" | cut -d "]" -f2)
	contains=${contains:1}
	[ -z "$contains" ] && return 0
	grep "$contains" "$2" > /dev/null || return 0
	echo "does contain $contains" > "$2"
	return 1
}

assert_rw() {
	grep "Invalid \(read\|write\) of size" "$2" > /dev/null || return 0
	echo "invalid read/write" > "$2"
	return 1
}

assert_free() {
	grep "Invalid free()" "$2" > /dev/null || return 0
	echo "invalid free" > "$2"
	return 1
}

assert_initial() {
	grep "Conditional jump or move depends on uninitialised value(s)" "$2" > /dev/null || return 0
	echo "uninitialed value" > "$2"
	return 1
}

assert_syscall() {
	grep "Syscall param .* uninitialised byte(s)" "$2" > /dev/null || return 0
	echo "uninitialed value in syscall" > "$2"
	return 1
}

assert_mismatch() {
	grep "Mismatched free() / delete / delete \[\]" "$2" > /dev/null || return 0
	echo "mismatched free" > "$2"
	return 1
}

assert_overlap() {
	grep "Source and destination overlap" "$2" > /dev/null || return 0
	echo "mem overlap" > "$2"
	return 1
}
assert_fishy() {
	grep "Argument 'size' of .* has a fishy (possibly negative) value:" "$2" > /dev/null || return 0
	echo "fishy alloc" > "$2"
	return 1
}

assert_leak() {
	grep "All heap blocks were freed -- no leaks are possible" "$2" > /dev/null && return 0
	[ "$suppressions" -eq 0 ] && echo "mem leak" > "$2" && return 1
	heap=$(grep "in use at exit:" "$2" | cut -d ":" -f2)
	supp=$(grep "suppressed: .* bytes"     "$2" | cut -d ":" -f2)
	[ "$heap" = "$supp" ] && return 0
	echo "mem leak" > "$2"
	return 1
}

read_test() {
#	[ -f /tmp/gwt_bailout ] && exit 1
  while read -r line
  do
    if [ "$line" = "#*" ]
    then printf "\t%s\n" line >&2
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
	local info=$(cat "$log")
    echo "not ok $(printf "% 4i" "$n") $desc" > "$log"
    echo "# $info" >> "$log"
  fi
  return 1
}

do_skip() {
  local n SKIP skip
  SKIP=0
  skip=$(grep "// \[skip\]" "$1")
  [ "$skip" ] && SKIP=1
  [ $SKIP = 0 ] && return 1
  skip=$(echo "$skip" | cut -d ']' -f2 )
  n=$(printf "% 4i" "$2")
  if [ "$async" -eq 0 ]
  then  echo "ok  $(printf "% 4i" "$n") $3 # Skip $skip"
  else echo "ok  $(printf "% 4i" "$n") $3 # Skip $skip" > "$4"
  fi
  return 0
}

do_todo() {
  local n SKIP skip
  SKIP=0
  skip=$(grep "// \[todo\]" "$1")
  [ "$skip" ] && SKIP=1
  [ $SKIP = 0 ] && return 1
  skip=$(echo "$skip" | cut -d ']' -f2 )
  n=$(printf "% 4i" "$2")
  if [ "$async" -eq 0 ]
  then  echo "ok  $(printf "% 4i" "$n") $3 # Todo $skip"
  else echo "ok  $(printf "% 4i" "$n") $3 # Todo $skip" > "$4"
  fi
  return 0
}

test_gw(){
  local n file log ret
  n=$2
  file=$1
  log=/tmp/gwt_$(printf "%04i" $n).log
  valgrind --suppressions=util/gwion.supp ./gwion -a -d dummy "$file" &> "$log" |:
  ret=$?
  #enable skip
  do_skip "$1" "$n" "" "$log" && return 0
  # enable todo
  do_todo "$1" "$n" "" "$log" && return 0

  [ $severity -lt 1  ]           && success "$n" "$file" "$log" && return 0
  assert_returns  "$ret" "$log"  || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 2  ]           && success "$n" "$file" "$log" && return 0
  assert_contain  "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 3  ]           && success "$n" "$file" "$log" && return 0
  assert_exclude  "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 4  ]           && success "$n" "$file" "$log" && return 0
  assert_rw       "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 5  ]           && success "$n" "$file" "$log" && return 0
  assert_initial  "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 6  ]           && success "$n" "$file" "$log" && return 0
  assert_syscall  "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 7  ]           && success "$n" "$file" "$log" && return 0
  assert_free     "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 8  ]           && success "$n" "$file" "$log" && return 0
  assert_mismatch "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 9  ]           && success "$n" "$file" "$log" && return 0
  assert_overlap  "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 10 ]           && success "$n" "$file" "$log" && return 0
  assert_fishy    "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  [ $severity -lt 11 ]           && success "$n" "$file" "$log" && return 0
  assert_leak     "$file" "$log" || fail    "$n" "$file" "$log" || return 1
  success "$n" "$file" "$log" && return 0
}


count_tests_sh(){
	local count
	count=$(grep "\[test\] #" "$1" | cut -d '#' -f 3)
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
		then ret=$((ret+$(count_tests_sh "$file")));
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
	local n offset l base
	l=0
	n=$2
#	[ -z "$n"  ] && n=1
	offset=$n
	base=$((n-1))
	[ "$async" -lt 0 ] && set -m
	found=0
  grep "\.gw" <<< "$(ls "$1")" &> /dev/null && found=1
  if [ "$found" -eq 1 ]
	then
	for file in "$1"/*.gw
	do
#		[ -f /tmp/gwt_bailout ] && exit 1
		if [ "$async" -ne 0 ]
		then test_gw "$file" "$n"&
		else test_gw "$file" "$n"
		fi
#		n=$((n+1))
#		l=$((l+1))
		[ "$async" -ne 0 ] && {
			if [ $(( $((n-base)) % async)) -eq 0 ]
			then
				wait
				for i in $(seq $offset $n)
				do
					read_test "/tmp/gwt_$(printf "%04i" "$i").log"
				done
				offset=$((offset + async));
			fi
		}
		n=$((n+1))
		l=$((l+1))
	done
 	[ "$async" -ne 0 ] && {
		wait
		local rest=$(( $((n-base-1)) %async))
		for i in $(seq $((n-rest))  $((n-1)))
		do read_test "/tmp/gwt_$(printf "%04i" "$i").log"
		done
	}
  fi


	found=0
  grep "\.sh" <<< "$(ls "$1")" &> /dev/null && found=1
  if [ "$found" -eq 1 ]
	then
#		[ -f /tmp/gwt_bailout ] && exit 1
		for file in "$1"/*.sh
		do
			[ "$file" = "$1/*.sh" ] && continue
			bash "$file" "$((n))"
			local count
			count=$(grep "\[test\] #" "$file" | cut -d '#' -f 3)
			n=$((n+count))
		done
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
	echo "1...$((n_test-n_dir))"
}

do_test() {
	local n_test
	n_test=1
	count_test "$@"
	for arg in "$@"
	do
		if [ "${arg:0:6}" = "async=" ]
    then
			async=$(echo "$arg" | cut -d '=' -f 2);
			[ "$async" -eq 1 ] && async=0
		elif [ "${arg:0:9}" = "severity=" ]
		then severity=$(echo "$arg" | cut -d '=' -f 2);
		elif [ "${arg:0:9}" = "suppressions=" ]
		then suppressions=$(echo "$arg" | cut -d '=' -f 2);
#		elif [ "${arg:0:9}" = "bailout=" ]
#		then bailout=$(echo "$arg" | cut -d '=' -f 2);
		elif [ -f "$arg" ]
		then
#			[ -f /tmp/gwt_bailout ] && exit 1
			if [ "${arg: -3}" = ".gw" ]
			then test_gw "$arg" "$n_test"
				n_test=$((n_test + 1))
			elif [ "${arg: -3}" = ".sh" ]
			then
				bash "$arg" "$n_test"
				local c
				c=$(count_tests_sh "$arg")
				n_test=$((n_test + c))
			fi
		elif [ -d "$arg" ]
		then
#			[ -f /tmp/gwt_bailout ] && exit 1
			[ "${arg: -1}" = "/" ] && arg=${arg:0: -1}
			test_dir "$arg" "$n_test"
			n_test=$((n_test + $(count_tests "$arg")))
		fi
	done
}

consummer() {
  local win failure skip todo
  win=0
  failure=0
  skip=0
  todo=0
  while read -r line
  do
# plan
    if [ "${line:0:4}" = "1..." ]
    then  echo "$line"
# diagnostic
    elif [ "${line:0:1}" = "#" ]
    then
      echo "$line" >&2
      continue
# failure
    elif [ "${line:0:6}" = "not ok" ]
    then
      echo "$line" >&2
      failure=$((failure+1))
# success
    elif [ "${line:0:2}" = "ok" ]
    then
      win=$((win+1))
      if [ "$line" = "*#*" ]
      then
        base=$(echo "$line" | cut -d "#" -f 1)
        directive=$(echo "$line" | cut -d "#" -f 2)
        printf "%s" "$base"
        [ "$directive" ] && echo " # $directive"
        [ "$line" = "* Todo *" ] && todo=$((todo+1))
        [ "$line" = "* Skip *" ] && skip=$((skip+1))
      else echo "$line"
      fi
# bail out
    elif [ "${line:0:9}" = "Bail out!" ]
    then
      if [ "$line" = "*#*" ]
      then
        base=$(echo "$line" | cut -d "#" -f 1)
        directive=$(echo "$line" | cut -d "#" -f 2)
        printf "%s" "$base"
        [ "$directive" ] && echo " # $directive"
      else echo "$line"
      fi
      exit 1
    fi
# ignore others
  done <&0
  echo "# Success: $win $failure $skip $todo" >&2
  [ "$failure" -gt 0 ] && return 1
  return 0
}
