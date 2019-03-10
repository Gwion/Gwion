#!/bin/zsh
#run benchmarks and make plots
n=0
function fib_test() {
  echo $n $1 $(perf stat -r10 $1 fib_recurs.$2 2>&1 | grep "time elapsed" | cut -d s -f 1 | sed 's/+-//' | sed 's/,/./g')
  n=$((n+1))
}

GWION_DIR=$PWD
cd ~/fib_recurs/

fib_test gwion gw
fib_test lua   lua
fib_test wren  wren
fib_test chuck ck
fib_test ruby  rb
fib_test perl  pl
fib_test python2 py
fib_test python3 py
fib_test runhaskell hs
