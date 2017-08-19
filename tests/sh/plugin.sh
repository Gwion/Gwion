#!/bin/bash
# [test] #13

n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh

: "${GWION_ADD_DIR=/usr/lib/Gwion/add}"

export GWION_ADD_DIR

test_plugin() {
	export NAME=$"$1"
	make install
	../../gwion "$NAME.gw" &> /dev/null
    NAME="$1" make uninstall clean
	N=$(printf "% 4i" "$n")
	echo "ok $N plugin test: '$NAME'"
    n=$((n+1))
}

# empty plug file
touch "${GWION_ADD_DIR}/empty.so"
./gwion &> /dev/null
N=$(printf "% 4i" "$n")
echo "ok $N plugin test: 'empty'"
n=$((n+1))
rm "${GWION_ADD_DIR}/empty.so"


pushd tests/test_plugins
test_plugin coverage
test_plugin array
test_plugin begin_class
test_plugin end_class
test_plugin invalid_type1
test_plugin invalid_type2
test_plugin invalid_type3
test_plugin invalid_func
test_plugin invalid_array
test_plugin invalid_arg
test_plugin no_import
test_plugin op_err

# clean
rm -f ./*.gcda ./*.gcno vgcore.* ./*.o ./*.so

popd
