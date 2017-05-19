#!/bin/bash

#read from (special) Mardown file

# store basic ouput
exec 5<&1

#count number of parts
declare -a PARTS
#declare curr_file
#declare -a curr_test
declare to_test
#PARTS=0
to_test=0
LINE=0

log() {
  echo "[$0] $1" >&2
}

print_test_file() {
  local -a file=("$2")
  printf "\n%s\n" "\`\`\`" >&5
  for line in "${file[@]}"
  #do echo "$line" >&5; done
  #for line in "${curr_test[@]}"
  do
    local code show
    code=$(sed 's#// show$##' <<< "$line")
    show="$(grep '// show$' <<< "$line")"
    [ -z "$show" ] || echo "$code" >&5
    echo "$code"
  done
  printf "%s" "\`\`\`" >&5
  printf " [run_Results]($DOCIST_DIR/run/%s.log)" "$1" >&5
  printf " [wav_Results]($DOCIST_DIR/wav/%s.wav)" "$1" >&5
  printf " [prg_Results]($DOCIST_DIR/out/%s.out)" "$1" >&5
  printf "\n" >&5
}

back_to_normal() {
  local src="$1"
  local old="$2"
  shift
  shift
  local -a curr_test=("$@")
  echo "${#curr_test[@]}" >&5
  log "switch to normal output from '$old'"
  #PARTS=$((PARTS=1))
  print_test_file "$src" "${curr_test[@]}"
  #print_test_file "$src" "${curr_test[@]}"
  #PARTS+=("$curr_file")
  #to_test=0
  #curr_test=()
  #curr_file=
  exec >&1
}

read_file() {
  local to_test=0
  local src=$1
  local -a curr_test
  while read -r line
  do
    LINE=$((LINE+1))
    grep '^FILE=' <<< "$line" > /dev/null && {
      [ -z "$src" ] && {
      log "no filename. aborting."
      exit 1
  }
  log "switch to test file '$1' [${#PARTS}]"
  #curr_file="$1"
  #to_test=1
  exec > "$src"
      switch_to_test "${line:5}"
      curr_file=${line:5}
      to_test=1
      continue
    }
    grep '^!FILE' <<< "$line" > /dev/null && {
    back_to_normal "$src" "$curr_file" "${curr_test[@]}"
      curr_test=()
      to_test=0
      continue
    }
    if [ $to_test = 1 ]
    then curr_test+=("$line")
    else echo "$line"
    fi
  done < "$1"
}

#read_file2() {
  #while read -r line
  #do
    #LINE=$((LINE+1))
    #grep '^FILE=' <<< "$line" > /dev/null && switch_to_test "${line:5}" && continue
    #grep '^!FILE' <<< "$line" > /dev/null && back_to_normal && continue
    #if [ $to_test = 1 ]
    #then curr_test+=("$line")
    #else echo "$line"
    #fi
  #done < "$1"
#}

read_file "$1"
