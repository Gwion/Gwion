#!/bin/bash
# [test] #1
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
export DRIVER=silent
source tests/sh/common.sh

(ret=$(./gwion -l1 &> /dev/null)
if [ "$ret" ]
then echo "not ok  $(printf "% 4i" "$n") remote "
else echo "ok $(printf "% 4i" "$n") remote "
fi)&
sleep 1;
killall gwion
wait

#run "$n" "main loop" "examples/sine.gw" "file"&
./gwion -d silent examples/sine.gw &> /dev/null &
sleep .1
./gwion + examples/sine.gw &> /dev/null
#run "$n" "add file (w/ '+')" "+ examples/sine.gw" "file1"
sleep .1
#run "$n" "remove first shred" "- 1" "file2"
./gwion - 1 &> /dev/null
wait
#run "$n" "launch loop" "-l1" "file"&
./gwion -l1 &> /dev/null &
sleep .1
./gwion -l1 &> /dev/null
sleep .1
./gwion -q &> /dev/null
#wait
