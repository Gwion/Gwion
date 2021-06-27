#!/bin/bash
echo "#ifndef __GWION_OPCODES__"
echo "#define __GWION_OPCODES__"

list="$(grep -v "#" opcode.txt)"
COUNT=0
echo "enum {"
for info in ${list}
do
  a=$(echo "$info" | cut -f1 -d"~")
  [ -z "$a" ] || {
    echo "  e$a,"
    COUNT=$((COUNT+1))
  }
done
echo "};"
echo ""

print_arg1() {
  if [ "$1" = "p" ]
  then echo "        gw_out(\" {-R}%-14p{0}\", instr->m_val);"
  elif [ "$1" = "u" ]
  then echo "        gw_out(\" {-R}%-14\"UINT_F\"{0}\", instr->m_val);"
  elif [ "$1" = "i" ]
  then echo "        gw_out(\" {-R}%-14\"INT_F\"{0}\", instr->m_val);"
  elif [ "$1" = "f" ]
  then echo "        gw_out(\" {-R}%-14f\", instr->f);"
  elif [ "$1" = "pc" ]
  then echo "        gw_out(\" {-B}=>%-12\"UINT_F\"{0}\", instr->m_val);"
  elif [ "$1" = "t" ]
  then echo "        gw_out(\" {-B}%-14s\"UINT_F\"{0}\", ((Type)instr->m_val)->name);"
  fi
}

print_arg2() {
  if [ "$1" = "p" ]
  then  echo "       gw_out(\" {-M}%-14p{0}\", instr->m_val2);"
  elif [ "$1" = "u" ]
  then echo "        gw_out(\" {-M}%-14\"UINT_F\"{0}\", instr->m_val2);"
  elif [ "$1" = "i" ]
  then echo "        gw_out(\" {-M}%-14\"INT_F\"{0}\", instr->m_val2);"
  elif [ "$1" = "t" ]
  then echo "        gw_out(\" {-B}%-14s\"UINT_F\"{0}\", ((Type)instr->m_val2)->name);"
  fi
}

for info in ${list}
do
  a=$(echo "$info" | cut -f1 -d"~")
  [ -z "$a" ] || echo "#define  $a (f_instr)e$a"
done | column -t

echo "ANN static inline void dump_opcodes(const VM_Code code) {"
echo "  gw_out(\"{Y}┏━━━━┓{0}{-Y} {+}%s{0}\n{Y}┃{0}\n\", code->name);"
echo "  m_uint j = 0;"
echo "  for(m_uint i = 0; i < vector_size(&code->instr); i++) {"
echo "    const Instr instr = (Instr)vector_at(&code->instr, i);"
echo "    if (instr->opcode == eNoOp) continue;"
echo "    switch(instr->opcode) {"
for info in ${list}
do
  a=$(echo "$info" | cut -f1 -d"~")
#  [ -z "$a" ] && continue
  val1=$(echo "$info" | cut -f2 -d"~")
  [ "$val1" = "$a" ] && val1=""
  val2=$(echo "$info" | cut -f3 -d"~")
  [ "$val2" = "$a" ] && val2=""
  formatted="$(printf '%-12s' $a)"
  #printf "    case e$a:\n     gw_out(\"{Y}┃{0}{-}%% 4lu{0}: $formatted\", j);\n"
  printf '      case e%s:\n        gw_out(\"{Y}┃{0}{-}%% 4lu{0}: %s\", j);\n' "$a" "$formatted"
  [ -z "$val1" ] || print_arg1 "$val1"
  [ -z "$val2" ] || print_arg2 "$val2"
  echo "        gw_out(\"\n\");"
  printf '        break;\n'
done
echo "    }"
echo "    j++;"
echo "  }"
echo "  gw_out(\"{Y}┃\n┗━━━━┛{0}\n\");"
echo "}"

echo "#endif"

echo "generated" "$COUNT" "opcodes" >&2
