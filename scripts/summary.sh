#!/bin/sh

# print the offset
tabulate() {
  for i in $(seq 1 $1)
  do printf "    "
  done
}

sign() {
  [ "$1" -eq 1 ] && echo "- "
}

# add an entry to symmary
entry() {
  title=$(head -n1 ${1}/${2}r)
  if [ "$2" = "README.md" ] && [ $3 -gt 0 ]
  then idx=$(($4-1))
  else idx=$4
  fi
  tabulate $idx
  if [ $1 = "docs" ]
  then dir="."
  else dir=$(sed 's#docs/##' <<< ${1})
  fi
  echo "$(sign $3)[${title:2}]($dir/$2)"
}

# handle a directory
handle() {
  while read -r line
  do
  if [ -z "$line" ]
  then echo ""
       continue
  fi
  if [ -f "$1/${line}r" ]
  then entry $1 $line $2 $(( $3 + 1 ))
  elif [ -d "$1/${line}" ]
  then handle $1/$line 1 $(( $3 + 1 ))
  fi
  done < "${1}/list"
}

handle docs 0 -1
