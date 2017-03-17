#!/bin/bash
# [test] #1
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
n=$(printf "% 4i" "$n")
source tests/sh/common.sh

./util/generate_header "/invalid" &> /dev/null
ret=$?
if [ $ret ]
then echo "ok $n header wrong target"
else echo "not ok $n header wrong target"
fi

