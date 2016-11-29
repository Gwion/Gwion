#!/bin/sh
# numbers from valgrind
#valgrind --leak-check=full --show-leak-kinds=all  ./gwion -d dummy examples/repeat.gw 2>&1 | tail -8 | head -4 | sed "s/^.*: //" |cut -d " " -f 1,4

EXAMPLE_DIR=examples
SEG_FILE=/tmp/gwion_segfault.bats
OUT_FILE=/tmp/gwion_test.bats

PASSED_ICON=/home/djay/src/git/shouldertap/lib/shouldertap/resources/dialog-warning.png
FAILED_ICON=/home/djay/src/git/shouldertap/lib/shouldertap/resources/dialog-error.png

clean() { rm -rf $OUT_FILE $SEG_FILE; }

#test wether pipe or not
#if [ -t 1 ] ; then echo terminal; fi

clean

# check for bats
which bats > /dev/null ||
{
	echo "bats not found."
	exit 1
}

# ensure in good directory (./Gwion)
test "$(basename $PWD)" = "Gwion" ||
{
	echo "wrong directory. (should be 'Gwion' root directory)."
	exit 1
}

# warn when segfault
#echo  'teardown() {  if [ "$status" -eq 139 ]; then echo segfault >&2; fi; }' > $OUT_FILE
#echo "teardown() { notify-send -i $FAILED_ICON \"\$BATS_TEST_NAME\"; }" > $OUT_FILE
#ICON=$PASSED_ICON
echo $ICON
echo "teardown() {" >> $SEG_FILE
#echo "	[ \"\$status\" -eq 0 ] || ICON=$PASSED_ICON || ICON=$FAILED_ICON" >> $SEG_FILE
echo "	killall notify-osd 2> /dev/null" >> $SEG_FILE
echo "	notify-send -i \$ICON \"\$BATS_TEST_DESCRIPTION\"" >> $SEG_FILE
#echo $ICON\n\
echo "}" >> $SEG_FILE

#list file in DIR
#ex_file=test.gw
for ex_file in $(ls $EXAMPLE_DIR)
do
	name=$(echo $ex_file | sed s/\.gw//)

#	echo -e "\
#@test \"testing $name for running ok\" {\n\
#	run ./gwion $EXAMPLE_DIR/$ex_file\n\
#	./gwion $EXAMPLE_DIR/$ex_file 2>/dev/null\n\
#	[ \"\$status\" -eq 0 ]\n}\n" >> $SEG_FILE

	echo -e "\
@test \"testing $name for segfault\" {\n\
	run ./gwion -d dummy $EXAMPLE_DIR/$ex_file\n\
#	./gwion $EXAMPLE_DIR/$ex_file 2>/dev/null\n\
	[ \"\$status\" -eq 0 ] && ICON=$PASSED_ICON || ICON=$FAILED_ICON\n\
	[ \"\$status\" -ne 139 ]\n}\n" >> $SEG_FILE
done

# run the test-suite
bats $SEG_FILE #&&
#{
#echo "la suite"
#	bats $OUT_FILE
#}

# obvious
#clean
