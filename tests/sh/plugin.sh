#!/bin/bash
# [test] #7
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh

: "${GWION_ADD_DIR=/usr/lib/Gwion/add}"

export GWION_ADD_DIR

test_plugin() {
	export NAME="$1"
	make install
	../../gwion "$NAME.gw" &> /dev/null
    make uninstall clean
	N=$(printf "% 4i" "$n")
	echo "ok $N test $NAME plugin"
    n=$((n+1))
}

pushd tests/test_plugins
test_plugin coverage
test_plugin array
test_plugin begin_class
test_plugin end_class
test_plugin invalid_type1
test_plugin invalid_type2
test_plugin invalid_func

# clean
rm -f *.gcda *.gcno vgcore.* *.o *.so

popd
