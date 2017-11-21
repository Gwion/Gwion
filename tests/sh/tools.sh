#!/bin/bash
# [test] #4
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1

make gwlint && echo "ok $n make gwlint" || echo "not ok $n make gwlint"
n=$((n+1))
./gwlint examples/*.gw && echo "ok $n test gwlint"
n=$((n+1))
make gwtag && echo "ok $n make gwtag" || echo "not ok $n make gwlint"
n=$((n+1))
./gwtag examples/*.gw && echo "ok $n test gwtag"
n=$((n+1))

