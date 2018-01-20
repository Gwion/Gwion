#!/bin/bash
# [test] #19

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
for test_file in $(ls *.c)
do
  name=$(echo $test_file | sed 's/\.c//')
  test_plugin $name
done

# clean
rm -f ./*.gcda ./*.gcno vgcore.* ./*.o ./*.so

popd
