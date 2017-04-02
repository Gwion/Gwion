#!/bin/bash
# [test] #1
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh

pushd tests/test_plugins
NAME=coverage make install
../../gwion coverage.gw
NAME=coverage make uninstall clean
popd
N=$(printf "% 4i" "$n")
echo "ok $N test coverage plugin"
