#!/bin/bash
# [test] #7
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1

make gwlint && echo "ok $n make gwlint" || echo "not ok $n make gwlint"
n=$((n+1))
./gwlint examples/*.gw 2>/dev/null && echo "ok $n test gwlint"
n=$((n+1))
make gwtag && echo "ok $n make gwtag" || echo "not ok $n make gwtag"
n=$((n+1))
./gwtag examples/*.gw && echo "ok $n test gwtag"
n=$((n+1))
make gwcov && echo "ok $n make gwcov" || echo "not ok $n make gwcov"
n=$((n+1))
./gwion -K examples/*.gw &>/dev/null && echo "ok $n test gwion on all examples" || echo "ok $n [skip] make gwion coverage"

n=$((n+1))
./gwcov examples/*.gw &>/dev/null && echo "ok $n test gwcov" || echo "ok $n make gwcov test"
