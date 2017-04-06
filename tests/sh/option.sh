#!/bin/bash
# [test] #2
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh

run "$((n))" "remove " "- 1" "file2"&
run "$((n+1))" "add  " "+ examples/int.gw" "file1"
