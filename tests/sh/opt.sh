#!/bin/bash
# [test] #34
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
run "$n" "host invalid (long)" "--host non_existant_host" "file"

# port
n=$((n+1))
run "$n" "port invalid (short)" "-p 1" "file"
n=$((n+1))
run "$n" "port invalid (long)" "--port 1" "file"

# card default
n=$((n+1))
run "$n" "card default (short)" "-c default" "file"
n=$((n+1))
run "$n" "card default (long)" "--card default" "file"

# card invalid
n=$((n+1))
run "$n" "card invalid (short)" "-c invalid" "file"
n=$((n+1))
run "$n" "card invalid (long)" "--card invalid" "file"

# channels
n=$((n+1))
run "$n" "channels (short)" "-g 2" "file"
n=$((n+1))
run "$n" "channels (long)" "--chan 2" "file"

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

# bufnum
n=$((n+1))
run "$n" "bufnum (short)" "-n 2" "file"
n=$((n+1))
run "$n" "bufnum (long)" "--bufnum 3" "file"

# bufsize
n=$((n+1))
run "$n" "bufsize (short)" "-b 256" "file"
n=$((n+1))
run "$n" "bufsize (long)" "--bufsize 256" "file"

# raw
n=$((n+1))
run "$n" "raw (short)" "-r" "file"
n=$((n+1))
run "$n" "raw (long)" "--raw" "file"

# samplerate
n=$((n+1))
run "$n" "samplerate (short)" "-s 44100" "file"
n=$((n+1))
run "$n" "samplerate (long)" "--sr 44100" "file"

# standalone
n=$((n+1))
run "$n" "standalone (short)" "-a" "file"
n=$((n+1))
run "$n" "standalone (long)" "--alone" "file"

# format
n=$((n+1))
run "$n" "format (short)" "-f test_format" "file"
n=$((n+1))
run "$n" "format (long)" "--format test_format" "file"

# backend
n=$((n+1))
run "$n" "backend (short)" "-e test_backend" "file"
n=$((n+1))
run "$n" "backend (long)" "--backend test_backend" "file"

# wrong file
n=$((n+1))
run "$n" "wrong file" "non_existant_file" "file"

