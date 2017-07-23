#!/bin/sh
remove_crash() {
	rm "crashes/$1.gw" && reorder
}


get_usable_results() {
  dir=$1
  count=0;
  [ -d crashes ] || mkdir crashes
  if [ "$dir" ]
  then
    for file in afl_out/*/"$dir"/*
     do
      [ "$(basename "$file")" = "README.txt" ] && continue
      mv "$file" "$dir"/${count}.gw
      count=$((count+1))
    done
  else
echo lol
    for file in afl_out/crashes/*
     do
      [ "$(basename "$file")" = "README.txt" ] && continue
      mv "$file" "crashes/${count}.gw"
      count=$((count+1))
    done
  fi
}

reorder() {
  count=0;
  for file in crashes/*
   do
    mv "$file" crashes/${count}.gw.old
    count=$((count+1))
  done
  count=0;
  for file in crashes/*
   do
    mv "$file" crashes/${count}.gw
    count=$((count+1))
  done
}

launch() {
	dvtm \
	"nice -10 afl-fuzz -M 1 -i cmin -o afl_out ./gwion -a @@ -a"\
	"nice -10 afl-fuzz -S 2 -i cmin -o afl_out ./gwion -a @@ -a"\
    "nice -10 afl-fuzz -S 3 -i cmin -o afl_out ./gwion -a @@ -a"
}
