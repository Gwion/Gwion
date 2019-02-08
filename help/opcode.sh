echo "#ifndef __GWION_OPCODES__"
echo "#define __GWION_OPCODES__"

list=$(grep -v "#" opcode.txt)
COUNT=0
echo "enum {"
for a in ${list}
do
  [ -z "$a" ] || {
    echo "  $a,"
    COUNT=$((COUNT+1))
  }
done
echo "};"
echo ""

for a in ${list}
do
  [ -z "$a" ] || echo "#define  $a (f_instr)$a"
done | column -t

echo "#endif"

echo "generated" "$COUNT" "opcodes" >&2
