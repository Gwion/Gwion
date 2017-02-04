#!/bin/sh
EXAMPLE_DIR=examples
AST_DIR=test/tree
SEG_FILE=/tmp/gwion_segfault.bats
AST_FILE=/tmp/gwion_ast.bats
ADD_FILE="util/error.bats"

# check for bats
which bats > /dev/null ||
{
	echo "bats not found."
	exit 1
}

# ensure in good directory (./Gwion)
test "$(basename "$PWD")" = "Gwion" ||
{
	echo "wrong directory. (should be 'Gwion' root directory)."
	exit 1
}

rm -f $SEG_FILE $AST_FILE
echo "load $PWD/util/test_helper/bats-support/load.bash
load $PWD/util/test_helper/bats-assert/load.bash
load $PWD/util/test_helper/bats-gwion/load.bash"   >> $SEG_FILE

echo "load $PWD/util/test_helper/bats-support/load.bash
load $PWD/util/test_helper/bats-assert/load.bash
load $PWD/util/test_helper/bats-gwion/load.bash"   >> $AST_FILE

for ex_file in "$AST_DIR"/*.gw
do
	echo "@test \"$(ls "$ex_file")\" { gbt $ex_file; }" >> $AST_FILE
done

for ex_file in "$EXAMPLE_DIR"/*.gw
do
	echo "@test \"$(ls "$ex_file")\" { gbt $ex_file; }" >> $SEG_FILE
done

[ "$CI" = "true" ] && FORMAT=-t

[ -z "${GWION_PLUG_DIR}" ] || export GWION_PLUG_DIR="/usr/lib/Gwion/plug"

bats $FORMAT $SEG_FILE $ADD_FILE $AST_FILE
