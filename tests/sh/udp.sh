#!/bin/bash
# [test] #7
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1

# needs to run real time
export DRIVER=silent
source tests/sh/common.sh

run "$((n+5))" "main loop" "-l1" "file1"&
sleep .1
run "$((n))" "add file" "examples/sine.gw" "file2"
run "$((n+1))" "add file" "examples/sine.gw" "file3"
run "$((n+2))" "remove shred" '- 1' "file4"
run "$((n+3))" "unset loop" "-l0" "file5"
run "$((n+4))" "quit" "-q" "file6"
run "$((n+6))" "quit" "-h invalid" "file7"
wait
