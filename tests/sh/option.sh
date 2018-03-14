#!/bin/bash
# [test] #6
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1

source tests/sh/common.sh

# test gwion copiled with UDP
(./gwion -C | grep GWUDP) &> /dev/null || {
  echo "ok $(printf "% 4i" "$n") UDP (skip)"
  echo "ok $(printf "% 4i" "$((n+1))") UDP (skip)"
  echo "ok $(printf "% 4i" "$((n+2))") UDP (skip)"
  echo "ok $(printf "% 4i" "$((n+3))") UDP (skip)"
  echo "ok $(printf "% 4i" "$((n+4))") UDP (skip)"
  echo "ok $(printf "% 4i" "$((n+5))") UDP (skip)"
  exit;
}



./gwion -l1 &>/dev/null &
n=$(printf "% 4i" "$((n))")
echo "ok    $((n)) start loop"
sleep .3
run "$((n+1))" "add  " "+ examples/int.gw" "file3"
sleep .3
run "$((n+2))" "remove " "- 1" "file2"
sleep .3
run "$((n+3))" "end loop  " "-q" "file4"
wait
n=$(printf "% 4i" "$((n+4))")
echo "ok $n quitted"

n=$((n+1))
run "$n" "format " "-f F32" "file"
