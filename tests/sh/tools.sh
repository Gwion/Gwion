#!/bin/bash
# [test] #7
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1

make gwpp && echo "ok $n make gwpp" || echo "not ok $n make gwpp"
n=$((n+1))
./gwpp examples/*.gw &>/dev/null && echo "ok $n test gwpp"
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
