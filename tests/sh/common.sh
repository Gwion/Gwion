#!/bin/bash
# [test] #0

set -m
: "${UDP:=}"
: "${DRIVER:=dummy}"

run() {
    local good n ok
    good="All heap blocks were freed -- no leaks are possible"
    n=$(printf "% 4i" "$1")
echo "valgrind ./gwion $UDP -d $DRIVER $3 &>$4" >&2
    eval valgrind ./gwion "$UDP" -d "$DRIVER" "$3" &> "$4"
    ok=0
    grep "$good" "$4" > /dev/null && ok=1
    if [ $ok ]
    then echo "ok $n $2"
    else echo "not ok $n $2"
    fi
    rm "$4"
}
