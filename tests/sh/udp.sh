#!/bin/bash
# [test] #15
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh


#(run "$n" "quit (remote $n)" "-l1" "file2"&
(./gwion -l1 && echo "ok  $(printf "% 4i" "$n") remote "&
sleep 1;
killall gwion
#kill %1 &> /dev/null &>/dev/null
wait)

n=$((n+1))
run "$((n+1))" "quit (remote $((n+1)))" "-l1" "file2"&
sleep .3
run "$n" "quit (short)" "-q" "file"&
wait

n=$((n+2))
run "$((n+1))" "quit (remote $((n+1)))" "-l1" "file2"&
sleep 1
run "$n" "quit (long)" "--quit" "file"&
wait

n=$((n+2))
run "$((n+1))" "add + (remote $((n+1)))" "-l1" "file2"&
sleep .3
run "$n" "add file (+)" "+ examples/int.gw" "file"&
sleep .3
run "$((n + 2))" "add + (remote $((n + 2)))" "-l0" "file3"&
#sleep 1
wait

n=$((n+3))
run "$((n+1))" "add (remote $((n+1)))" "-l1" "file2"&
sleep .3
run "$n" "add file " "examples/int.gw" "file"&
sleep .3
run "$((n + 2))" "add (remote $((n + 2)))" "-l0" "file3"&
#sleep 1
wait

n=$((n+3))
run "$((n+1))" "add and remove (remote $((n+1)))" "-l1" "file2"&
sleep .3
run "$n" "add file" "examples/int.gw -l0" "file"&
wait

n=$((n+2))
run "$((n+1))" "remove (remote $((n+1)))" "-l1 sine.gw" "file2"&
sleep .3
run "$n" "rem file" "- 1" "file"&
sleep .3
run "$n" "rem file" "-l0" "file3"&
wait

exit 0
