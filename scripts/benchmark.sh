#!/bin/bash

language=("gwion" "wren" "lua" "python" "chuck" "ruby")
extension=("gw" "wren" "lua" "py" "ck" "rb")
test_dir="benchmark"
result_dir="$test_dir/results"
plot_script="scripts/bench.plot"
: "${repeats:=10}"

run() {
  perf stat -r"$repeats" "$1" "$test_dir/$3.$2" 2>&1 | grep "time elapsed" |
    sed 's/ *\([0-9]*\),\([0-9]*\) .* seconds time elapsed *( +- *\([0-9]*\),\([0-9]*\)% )/\1.\2 \3.\4/'
}

get_list() {
  for file in $test_dir/*.gw
  do basename "$file" .gw
  done
}

get_test() {
  for i in $(seq 0 ${#language[@]})
  do
    if [ -f "$test_dir/$1.${extension[$i]}" ]
    then echo "${language[$i]} $(run "${language[$i]}" "${extension[$i]}" "$1")"
    fi
  done > "$result_dir/$1.dat"
}

plot() {
  gnuplot -e "bench='$1'" "$plot_script"
}

[ -d "$result_dir/results" ] || mkdir "$result_dir"
for bench in $(get_list)
do
  get_test "$bench"
  plot "$bench"
done
