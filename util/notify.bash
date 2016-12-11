#!/bin/sh
teardown() {
	twmnc -i $ICON --id 8888 -t "[bats]" -c "$BATS_TEST_DESCRIPTION"
}

set_icon() {
  if [ "$status" -eq 0 ]
  then ICON=/usr/share/hplip/data/images/16x16/ok.png
  else ICON=/usr/share/hplip/data/images/16x16/error.png
  fi
  return 0;
}

assert_segfault() {
  return $(test "$status" -ne 139)
}
