#!/bin/bash
# [test] #27
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh


#(run "$n" "quit (remote $n)" "-l1" "file2"&
#(./gwion -l1 && echo "ok  $(printf "% 4i" "$n") remote "&
#(

(ret=$(./gwion -l1 &> /dev/null)
if [ "$ret" ]
then echo "not ok  $(printf "% 4i" "$n") remote "
else echo "ok $(printf "% 4i" "$n") remote "
fi)&
sleep 1;
killall gwion
#kill %1 &> /dev/null &>/dev/null
wait

# 2 - 3
n=$((n+1))
run "$((n+1))" "quit (remote $((n+1)))" "-l1" "file2"&
sleep .3
run "$n" "quit (short)" "-q" "file"&
wait

# 4 - 5
n=$((n+2))
run "$((n+1))" "quit (remote $((n+1)))" "-l1" "file2"&
sleep 1
run "$n" "quit (long)" "--quit" "file"&
wait

# 6 - 8
n=$((n+2))
run "$((n+2))" "add + (remote $((n+2)))" "-l1" "file2"&
sleep .3
run "$n" "add file (+)" "+ examples/int.gw" "file"&
sleep .3
run "$((n+1))" "add + (remote $((n + 1)))" "-l0" "file3"&
sleep 1
wait

# 9 - 11
n=$((n+3))
run "$((n+2))" "add (remote $((n+2)))" "-l1" "file2"&
sleep .3
run "$n" "add file " "examples/int.gw" "file"
sleep .3
run "$((n+1))" "add (remote $((n + 1)))" "-l0" "file3"
sleep 1
wait

# 12 -14
n=$((n+3))
run "$((n+2))" "add and remove (remote $((n+2)))" "-l1" "file2"&
sleep .3
run "$n" "add file" "examples/int.gw" "file"
sleep .3
run "$((n+1))" "add file" "-l0" "file"
wait

# 15 - 17
n=$((n+3))
run "$((n+2))" "remove (remote $((n+1)))" "-l1 sine.gw" "file2"&
sleep .3
run "$n" "rem file" "- 1" "file"&
sleep .3
run "$((n+1))" "rem file" "-l0" "file3"&
wait

# 18 - 20
n=$((n+3))
run "$((n+2))" "remove (remote $((n+2)))" "-d silent -l1 examples/sine.gw" "file2"&
sleep .3
run "$n" "rem file" "- 1 2 3 4" "file"&
sleep .3
run "$((n+1))" "quit" "-l0" "file3"&
wait

# 21 - 23
n=$((n+3))
run "$((n+2))" "remove (remote $((n+2)))" "-l1 sine.gw" "file"&
sleep .3
run "$n" "remove (remote $((n+1)))" "-l1 sine.gw" "file2"&
sleep .3
run "$((n+1))" "quit" "-l0" "file3"&
wait

# 24 - 27
n=$((n+3))
run "$((n+3))" "remove (remote $((n+1)))" "-d silent -l1 examples/sine.gw" "file"&
sleep .1
run "$((n))" "remove (remote $((n+1)))" "+ examples/sine.gw" "file2"&
sleep .1
run "$((n+1))" "remove (remote $((n+1)))" "- 1" "file3"&
sleep .1
run "$((n+2))" "quit" "-l0" "file4"&
wait


./gwion -d silent examples/sine.gw&
sleep .1
./gwion + examples/sine.gw
sleep .1
./gwion - 1
wait
#if [ $(uname) == "Linux" ]
#then NC_TIMEOUT="-q"
#else NC_TIMEOUT="-w"
#fi

#n=$((n+3))
#run "$((n))" "run loop" "-l1" "file" &
#./gwion -l1 &> /dev/null &
#sleep .3
#echo here
#echo '+ examples/sine.gw' | nc "$NC_TIMEOUT" 1 -u 127.0.0.1 8888
#sleep .3
#echo '- 1' | nc "$NC_TIMEOUT" 1 -u 127.0.0.1 8888
#sleep .3
#echo 'quit' | nc "$NC_TIMEOUT" 1 -u 127.0.0.1 8888
#wait

#n=$((n+1))
#./gwion -l1 &> /dev/null &
#run "$((n))" "../../gwion" "-l1" "file" &
#sleep .3
#echo 'examples/sine.gw' | nc "$NC_TIMEOUT" 1 -u 127.0.0.1 8888
#sleep .3
#echo 'loop 0' | nc "$NC_TIMEOUT" 1 -u 127.0.0.1 8888
#wait
#exit 0
