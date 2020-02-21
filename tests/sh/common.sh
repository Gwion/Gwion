#!/bin/bash
# [test] #0

set -m
: "${GWOPT:=}"
: "${DRIVER:=dummy}"

run() {
    local ok
    n=$(printf "% 4i" "$1")
    ./gwion "$GWOPT" -d "$DRIVER" "$3" &> "$4" && ok=1
    if [ $ok ]
    then echo "ok $n $2"
    else echo "not ok $n $2"
    fi
    rm "$4"
}
