#!/bin/bash
# [test] #26

n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh

: "${GWION_ADD_DIR=/usr/lib/Gwion/add}"

export GWION_ADD_DIR

test_plugin() {
	export NAME=$"$1"
	make
	../../gwion -P. "$NAME.gw" &> /dev/null
    NAME="$1" make clean
	N=$(printf "% 4i" "$n")
	echo "ok $N plugin test: '$NAME'"
    n=$((n+1))
}

# empty plug file
touch "empty.so"
./gwion -P. &> /dev/null
N=$(printf "% 4i" "$n")
echo "ok $N plugin test: 'empty'"
n=$((n+1))
rm "empty.so"


BASE_DIR="$PWD"
cd tests/import || exit
for test_file in *.c
do test_plugin "${test_file:0:-2}"
done

# clean
rm -f ./*.gcda ./*.gcno vgcore.* ./*.o ./*.so
cd "$BASE_DIR" || exit
