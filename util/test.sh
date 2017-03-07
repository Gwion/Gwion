#!/bin/bash
#declare -a dirs
set -m
#declare n_test
#n_test=0

: "${ASYNC:=4}"
: "${async:=$ASYNC}"

: "${SEVERITY:=4}"
: "${async:=$SEVERITY}"

#trap '[ -z $(jobs -p) ] && jobs -p | xargs kill' INT
#trap 'jobs -p | xargs kill -TERM &> /dev/null; wait' INT
#trap 'jobs -p | xargs kill -TERM &> /dev/null; wait' EXIT
#trap 'jobs -p | xargs kill -TERM &> /dev/null; wait' TERM

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
  local n log
  n=$1
  file=$2
  log=$3
  if [ "$async" -eq 0 ]
  then echo "ok  $(printf "% 4i" "$n") $file"
  else  echo "ok $(printf "% 4i" "$n") $file" > "$log"
  fi
  return 0
}

do_skip() {
  local n SKIP skip
  SKIP=0
  skip=$(grep "// \[skip\]" "$1")
  [ "$skip" ] && SKIP=1
  skip=$(echo "$skip" | cut -d ']' -f2 )
  [ $SKIP -eq 1 ] || return 1
  n=$(printf "% 4i" "$2")
  echo "ok $n $3 # SKIP $skip" > "$4"
  return 0
}

test_gw(){
  local n file log ret
  n=$(($2))
  file=$1
  log=/tmp/gwt_$(printf "%04i" $n).log
  valgrind ./gwion -a -d dummy "$file" &> "$log" |:
  ret=$?
  #enable skip
  do_skip "$1" "$n" "" "$log" && return 0
  # enable todo

  # check program runs
#  [ "$severity" -lt 1 ] &&  success "$n" "$1" > "$file" && return 0
#  check_return  "$ret" || fail "$n" "$log" "$file" || return 1

  # check program output (include)
#  [ $severity -lt 2 ] && success "$n" "$1" "$log" && return 0
#  assert_contain "$2" "$1" "$log" || return 1
  # cck program output (exclude)
#  [ $severity -lt 3 ] && success "$n" "$1" "$log" && return 0
#  assert_exclude "$2" "$1" "$log" || return 1
#  [ $severity -lt 4 ] && success "$n" "$1" "$log" && return 0
#  check_invalid "$2" "$1" "$log" || return 1
#  [ $severity -lt 5 ] && echo "ok  $TEST_NUMBER" "$1" > "$log" && return 0
#  check_leak "$log" || fail "$TEST_NUMBER" "$log" "$1" || return 1

  #(check_lost "cat $1)" "[definitely|possibly|indirectly]" || check_leak "$(cat $1)") ||return 1

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
echo "$len" "$2"
		if [ "${1: -$len}" = "$2" ]
		then return 0
		fi
	done
	return 1
}

test_dir() {
#	trap 'kill "${pids[@]}"' EXIT
#	trap 'kill "${pids[@]}"' INT
#	trap "kill 0" TERM
#trap 'jobs -p | xargs kill -TERM &> /dev/null; wait' INT
#trap 'jobs -p | xargs kill -TERM &> /dev/null; wait' EXIT
#	trap 'jobs -p | xargs kill -TERM &> /dev/null; wait' TERM
#	trap 'jobs -p | xargs kill' EXIT
	#trap "kill 0" INT
	declare -a pids
	local n offset l
	l=0
    local n
	n=$2
	[ -z "$n"  ] && n=1
	local offset
	offset=$n
	[ "$async" -lt 0 ] && set -m
	found=0
	ls "$1" | grep \.gw > /dev/null && found=1
	if [ "$found" -eq 1 ]
	then
	for file in "$1"/*.gw
	do
#		[ -f /tmp/gwt_bailout ] && exit 1
		[ "$file" = "$1/*.gw" ] && continue
		if [ "$async" -ne 0 ]
		then
        	test_gw "$file" "$n"&
			pids+=($!)
		else test_gw "$file" "$n"
		fi
		n=$((n+1))
		l=$((l+1))
		[ "$async" -ne 0 ] && {
			if [ $((n % async)) -eq 0 ]
			then
				wait;
				for i in $(seq $offset $n)
				do
					read_test "/tmp/gwt_$(printf "%04i" "$i").log"
				done
				offset=$((offset + async));
			fi
		}
	done
 	[ "$async" -ne 0 ] && {
		wait
		local rest=$((n%async))
		for i in $(seq $((n-rest+1))  $((n-1)))
		do read_test "/tmp/gwt_$(printf "%04i" "$i").log"
		done
	}
fi


	found=0
	ls "$1" | grep \.sh > /dev/null && found=1
#	dir_contains "$1" ".sh" && found=1
    if [ "$found" -eq 1 ]
	then
#		[ -f /tmp/gwt_bailout ] && exit 1
		for file in "$1"/*.sh
		do
			[ "$file" = "$1/*.sh" ] && continue
			bash "$file" "$n"
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
#			if [ "${arg:0:6}" = "async=" ]
#		then async=$(echo "$arg" | cut -d '=' -f 2);
#		elif [ "${arg:0:9}" = "severity=" ]
#		then severity=$(echo "$arg" | cut -d '=' -f 2);
#		elif [ "${arg:0:9}" = "bailout=" ]
#		then bailout=$(echo "$arg" | cut -d '=' -f 2);
#		el
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
