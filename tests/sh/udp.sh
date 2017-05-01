#!/bin/bash
# [test] #7
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
export DRIVER=silent
source tests/sh/common.sh
run "$((n+5))" "main loop" "-l1" "file4"&
sleep .1
run "$((n))" "add file" "examples/sine.gw" "file"
run "$((n+1))" "add file" "examples/sine.gw" "file"
sleep .1
run "$((n+2))" "remove shred" "- 1" "file2"
run "$((n+3))" "unset loop" "-l0" "file3"
run "$((n+4))" "quit" "-q" "file"
run "$((n+6))" "quit" "-h invalid" "file5"
wait
