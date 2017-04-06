#!/bin/bash
# [test] #4
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh

run "$((n+3))" "remove " "-l1" "file1"&
run "$((n))" "remove " "- 1" "file2"
run "$((n+1))" "add  " "+ examples/int.gw -l1" "file3"
run "$((n+2))" "add  " "-l0" "file4"
wait
