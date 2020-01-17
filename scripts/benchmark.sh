#!/bin/bash

language=("gwion" "wren" "lua")
extension=("gw" "wren" "lua")
test_dir="benchmark"
plot_script="scripts/bench.plot"
: "${repeats:=10}"

run() {
  echo "$1 $3 $2" >&2
  echo "perf stat -r$repeats $1 $test_dir/$3.$2"
  echo 1 | sudo -a tee /proc/sys/kernel/perf_event_paranoid
  perf stat -r"$repeats" "$1" "$test_dir/$3.$2" 2>&1 | grep "time elapsed" |
    sed 's/ *\([0-9]*\),\([0-9]*\) .* seconds time elapsed *( +- *\([0-9]*\),\([0-9]*\)% )/\1.\2 \3.\4/'
}

get_list() {
  for file in $test_dir/*.gw
  do basename "$file" .gw
  done
}

get_test() {
  for (( i=0; i<=$(( ${#language[@]} -1 )); i++ ))
  do
    if [ -f "$test_dir/$1.${extension[$i]}" ]
    then echo "${language[$i]} $(run "${language[$i]}" "${extension[$i]}" "$1")"
    fi
  done > "benchmark/results/$1.dat"
}

plot() {
  which perf
  echo "$1"
  cat "benchmark/results/$1.dat"
  gnuplot -e "bench='$1'" "$plot_script"
}

for bench in $(get_list)
do
  get_test "$bench"
  plot "$bench"
done
