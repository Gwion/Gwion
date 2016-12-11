#!/bin/sh
# numbers from valgrind
#valgrind --leak-check=full --show-leak-kinds=all  ./gwion -d dummy examples/repeat.gw 2>&1 | tail -8 | head -4 | sed "s/^.*: //" |cut -d " " -f 1,4

EXAMPLE_DIR=examples
SEG_FILE=/tmp/gwion_segfault.bats
OUT_FILE=/tmp/gwion_test.bats
PASSED_ICON=/usr/share/hplip/data/images/16x16/ok.png
FAILED_ICON=/usr/share/hplip/data/images/16x16/error.png
#PASSED_ICON=/home/djay/src/git/shouldertap/lib/shouldertap/resources/dialog-warning.png
#FAILED_ICON=/home/djay/src/git/shouldertap/lib/shouldertap/resources/dialog-error.png

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
echo load $PWD/util/test_helper/bats-support/load.bash >> $SEG_FILE
echo load $PWD/util/test_helper/bats-assert/load.bash >> $SEG_FILE
echo load $PWD/util/notify.bash >> $SEG_FILE

for ex_file in $(ls $EXAMPLE_DIR)
do
	name=$(echo $ex_file | sed s/\.gw//)

	echo -e "\
@test \"segfault :$(ls **/$ex_file):\" {\n\
	run ./gwion -d dummy $EXAMPLE_DIR/$ex_file\n\
    set_icon\n\
	assert_segfault\n\
	assert_success\n}\n\n">> $SEG_FILE

#	echo -e "\
#@test \"testing '$name' for: valgrind definitely\" {\n\
#	run valgrind --log-file=/tmp/gwion.valgrind ./gwion -d dummy $EXAMPLE_DIR/$ex_file\n\
#  RESULT=\`cat /tmp/gwion.valgrind | grep \"definitely lost:\" | cut -d \" \" -f 7 | sed s/,//g\`\n\
#  [ \"\$RESULT\" -eq 0 ] && ICON=$PASSED_ICON || ICON=$FAILED_ICON\n\
#  [ \"\$RESULT\" -eq 0 ]\n}\n" >> $SEG_FILE

#echo -e "\
#@test \"testing '$name' for: valgrind indirectly\" {\n\
#	run valgrind --log-file=/tmp/gwion.valgrind ./gwion -d dummy $EXAMPLE_DIR/$ex_file\n\
#  RESULTS=\`cat /tmp/gwion.valgrind | grep \"indirectly lost:\" | cut -d \" \" -f 7,10\`\n\
#  RESULT=\`echo \$RESULTS | cut -d \" \" -f 1 | sed \"s/,//\" \`\n\
#  [ \"\$RESULT\" -eq 0 ] && ICON=$PASSED_ICON || ICON=$FAILED_ICON\n\
#  [ \"\$RESULT\" -eq 0 ]\n}\n" >> $SEG_FILE

#	echo -e "\
#@test \"testing '$name' for: valgrind possibly\" {\n\
#	run valgrind --log-file=/tmp/gwion.valgrind ./gwion -d dummy $EXAMPLE_DIR/$ex_file\n\
#  RESULT=\`cat /tmp/gwion.valgrind | grep \"possibly lost:\" | cut -d \" \" -f 9\`\n\
#  [ \"\$RESULT\" -eq 0 ] && ICON=$PASSED_ICON || ICON=$FAILED_ICON\n\
#  [ \"\$RESULT\" -eq 0 ]\n}\n" >> $SEG_FILE

#  echo -e "\
#@test \"testing '$name' for: valgrind reachable\" {\n\
#	run valgrind --log-file=/tmp/gwion.valgrind ./gwion -d dummy $EXAMPLE_DIR/$ex_file\n\
#  RESULT=\`cat /tmp/gwion.valgrind | grep \"still reachable:\" | cut -d \" \" -f 7 | sed s/,//g\`\n\
#  [ \"\$RESULT\" -eq 0 ] && ICON=$PASSED_ICON || ICON=$FAILED_ICON\n\
#  [ \"\$RESULT\" -eq 0 ]\n}\n" >> $SEG_FILE

#	echo -e "\
#@test \"testing '$name' for: valgrind suppressed\" {\n\
#	run valgrind --log-file=/tmp/gwion.valgrind ./gwion -d dummy $EXAMPLE_DIR/$ex_file\n\
#  RESULT=\`cat /tmp/gwion.valgrind | grep \"suppressed:\" | cut -d \" \" -f 11 | head -n 1\`\n\
#  [ \"\$RESULT\" -eq 0 ] && ICON=$PASSED_ICON || ICON=$FAILED_ICON\n\
#  [ \"\$RESULT\" -eq 0 ]\n}\n" >> $SEG_FILE

done

# run the test-suite
if [ -t 1 ] ; then
	bats $SEG_FILE
	bats -t util/error.bats
else
	bats $SEG_FILE | grep -v '#' | grep -v "\.\." > /tmp/gwion_bats.log
#	bats util/error.bats | grep -v '#' | grep -v "\.\." >> /tmp/gwion_bats.log
	bats util/error.bats | grep -v "\.\." >> /tmp/gwion_bats.log
	touch /tmp/gwion_bats.lock
fi
#{
#echo "la suite"
#	bats $OUT_FILE
#}

# obvious
#clean
