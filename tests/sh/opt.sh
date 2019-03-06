#!/bin/bash
# [test] #17
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh

run "$n" "test arguments" "examples/shred.gw:12" "file"

n=$((n+1))
good="All heap blocks were freed -- no leaks are possible"
N=$(printf "% 4i" "$n")
valgrind ./gwion -d silent  &> "file"
ok=0
grep "$good" "file" > /dev/null && ok=1
if [ $ok ]
then echo "ok $N test driver"
else echo "not ok $N test driver"
fi
rm "file"

# help
n=$((n+1))
run "$n" "help (short)" "-?" "file"
n=$((n+1))
run "$n" "help (long)" "--help" "file"

# host
n=$((n+1))
run "$n" "host invalid (short)" "-h non_existant_host" "file"
n=$((n+1))
run "$n" "host invalid (long)" "--host non_existant_host " "file"

# help
n=$((n+1))
run "$n" "help (short)" "-?" "file"
n=$((n+1))
run "$n" "help (long)" "--help" "file"

# in channels
n=$((n+1))
run "$n" "in channels (short)" "-i 2" "file"
n=$((n+1))
run "$n" "in channels (long)" "--in 2" "file"

# out channels
n=$((n+1))
run "$n" "out channels (short)" "-o 2" "file"
n=$((n+1))
run "$n" "out channels (long)" "--out 2" "file"

# samplerate
n=$((n+1))
run "$n" "samplerate (short)" "-s 44100" "file"
n=$((n+1))
run "$n" "samplerate (long)" "--sr 44100" "file"

# wrong file
n=$((n+1))
run "$n" "wrong file" "non_existant_file" "file"

# plug_dir
n=$((n+1))
run "$n" "plugin directory" "-p non_existant_dir" "file"

# config
n=$((n+1))
run "$n" "config" "-c" "file"
