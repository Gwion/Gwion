#!/bin/bash
# [test] #1
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh
cat << EOF > /tmp/In.gw
In in => dac;
2::samp => now;
./gwion /tmp/In.gw &&
echo "$1 +
