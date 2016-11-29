#/!usr/bin/env bats

load 'test_helper/bats-support/load'
load 'test_helper/bats-assert/load'


FOO="./gwion"

LOG=valgrind.log
CMD=valgrind
ARG="--log-file=$LOG"

teardown () {
  run $CMD $ARG $FOO -d dummy $1
  echo $BATS_TEST_NAME lol >&2
  [ 1 -eq 1 ]
}

run_valgrind () {
  run $CMD $FOO -d dummy $1
  assert_output --partial "no leak"
echo lol
}

@test 'no argument' {
  run $CMD $FOO -d dummy
  assert_output --partial "no leaks" 
}

@test "run valgrind to log" {
  run_valgrind
}

@test "examples" {
  for ex in $(ls examples)
  do
    echo "$ex" lol >&2
    run run_valgrind "$ex"
    [ "$status" -eq 0 ]
  done
  fail
}
