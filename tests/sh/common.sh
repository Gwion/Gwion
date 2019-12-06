#!/bin/bash
# [test] #0

set -m
: "${GWOPT:=}"
: "${DRIVER:=dummy}"

run() {
    local good n ok
    good="All heap blocks were freed -- no leaks are possible"
    n=$(printf "% 4i" "$1")
    if [ "$VALGRIND" == "NO_VALGRIND" ]
    then ./gwion "$GWOPT" -d "$DRIVER" "$3" &> "$4" && ok=1
    else
      eval valgrind ./gwion "$GWOPT" -d "$DRIVER" "$3" &> "$4"
      if [ -f "$4" ]
      then grep "$good" "$4" > /dev/null && ok=1
      else ok=1
      fi
    fi
    if [ $ok ]
    then echo "ok $n $2"
    else echo "not ok $n $2"
    fi
    rm "$4"
}
