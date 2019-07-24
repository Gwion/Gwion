#!/bin/sh
: "${PRG:=./gwion}"

pahole -s ${PRG} | while read -r name size gap
do [ "$size" -gt 64 ] && echo "$name $size"
   [ "$gap" -gt 0 ] && echo "$name has a gap"
done | grep -v IO_FILE | grep -v __jmp_buf_tag | grep -v yyguts_t
