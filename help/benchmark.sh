#!/bin/sh
#run benchmarks and make plots
: "${repeats:=10}"

fib_test() {
#  echo "\"$1\"" "$(perf stat -r"$repeats" "$1" "fib_recurs.$2" 2>&1 | grep "time elapsed" | cut -d s -f 1 | sed 's/+-//' | sed 's/,/./g')"
  printf "$(perf stat -r"$repeats" "$1" "fib_recurs.$2" 2>&1 | grep "time elapsed" | cut -d s -f 1 | sed 's/+-//' | sed 's/,/./g')"
}

cd ~/fib_recurs/ || exit 1
printf "\"fib recurs\" "
fib_test gwion gw
fib_test lua   lua
fib_test wren  wren
#fib_test chuck ck
#fib_test ruby  rb
#fib_test perl  pl
#fib_test python2 py
#fib_test python3 py
#fib_test runhaskell hs
