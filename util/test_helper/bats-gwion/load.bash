#!/bin/sh

setup() {
	if [ $STOP_ON_ERROR -eq 1 ]
	then
	    [ ! -f ${BATS_PARENT_TMPNAME}.skip ] || skip "skip remaining tests"
	fi
}

teardown() {
	rm -f /usr/lib/Gwion/plug/invalid.so
	if [ $STOP_ON_ERROR -eq 1 ]
    then
		[ -n "$BATS_TEST_COMPLETED" ] || touch ${BATS_PARENT_TMPNAME}.skip
	fi
}

gbt() {
  run valgrind --suppressions=util/gwion.supp ./gwion -d dummy "$1"
  [ $status -ne 139 ]
  assert_success
  assert_output --partial "All heap blocks were freed -- no leaks are possible"  &2> /dev/null && return 0
  assert_output --partial "definitely lost: 0 bytes in 0 blocks"
  assert_output --partial "indirectly lost: 0 bytes in 0 blocks"
  assert_output --partial "possibly lost: 0 bytes in 0 blocks"
  refute_output --partial "Invalid write of size "
  refute_output --partial "Invalid read of size "
  refute_output --partial "Invalid free() / delete / delete[] / realloc()"
}

