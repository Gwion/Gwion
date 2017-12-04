#!/bin/bash
# [test] #3
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
n=$(printf "% 4i" "$n")
source tests/sh/common.sh

if [ -f  ./gwion ]
then echo "ok $n gwion is a file"
else echo "not ok $n gwion is not a file"
fi
n=$((n+1))

if [ -x  ./gwion ]
then echo "ok    $n gwion is executable"
else echo "not ok $n gwion is not executable"
fi
n=$((n+1))

./util/generate_header "/invalid" &> /dev/null
ret=$?
if [ $ret ]
then echo "ok    $n header wrong target"
else echo "not ok $n header wrong target"
fi

