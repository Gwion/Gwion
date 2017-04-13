#!/bin/bash
# [test] #1
n=0
[ "$1" ] && n="$1"
[ "$n" -eq 0 ] && n=1
source tests/sh/common.sh
cat << EOF > /tmp/In.gw
In in => dac;
2::samp => now;
EOF
echo -e "1\n2\n3\n" | run "$n" "test In module" "/tmp/In.gw" "file"
