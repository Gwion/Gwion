#!/bin/bash
# [test] #20

n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh

run "$n" "test arguments" "examples/shred.gw:12" "file"

n=$((n+1))
N=$(printf "% 4i" "$n")
run "$n" "simple run" "" "file"

# help
n=$((n+1))
run "$n" "help (short)" "-?" "file"

# host
n=$((n+1))
run "$n" "host invalid (short)" "-h non_existant_host" "file"

# help
n=$((n+1))
run "$n" "help (short)" "-?" "file"

# in channels
n=$((n+1))
run "$n" "in channels (short)" "-i 2" "file"

# out channels
n=$((n+1))
run "$n" "out channels (short)" "-o 2" "file"

# samplerate
n=$((n+1))
run "$n" "samplerate (short)" "-s 44100" "file"

# wrong file
n=$((n+1))
run "$n" "wrong file" "non_existant_file:with_args" "file"

# plug_dir
n=$((n+1))
run "$n" "plugin directory" "-p non_existant_dir" "file"

# config
n=$((n+1))
RC=tmp_gwionrc
cat << EOF >> "$RC"
-p.
-l0
EOF
run "$n" "config" "-c $RC" "file"
rm "$RC"
# loop
n=$((n+1))
run "$n" "loop" "-l0" "file"

# memoization
n=$((n+1))
run "$n" "memoize" "-z2 tests/new/pure*.gw" "file"

# (fake) module arg
n=$((n+1))
run "$n" "module argument" "-mfake:test" "file"

# set compilation passes
n=$((n+1))
run "$n" "just check" "-g check" "file"

# set compilation passes
n=$((n+1))
run "$n" "no pass" "-g nopass" "file"

# option needs argument
n=$((n+1))
run "$n" "option needs argument" "-p" "file"

# invalid global type
n=$((n+1))
run "$n" "invalid global type" "examples/complex/invalid_type0.gw examples/complex/invalid_type1.gw" "file"

# invalid global func
n=$((n+1))
run "$n" "invalid global func" "examples/complex/invalid_func0.gw examples/complex/invalid_func1.gw" "file"

# comma in module argument
n=$((n+1))
run "$n" "comma in module argument" "-mpp=-Dtest='\"dqs\,qs\"',-Dtsdl" "file"

# test signal
./gwion -l1 &
PID=$!
sleep .1 && kill $PID
