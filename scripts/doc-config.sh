#!/bin/bash
title() {
  TMP=$(head -n 1 "$1")
  sed "s/'/\'/" <<< "${TMP:2}"
}

list_dir() {
  [[ "$1" == */"assets" ]] && return
  for a in "$1"/*
  do
    if [ -d "$a" ]
    then [[ "$a" == */"assets" ]] ||
         echo "$2- '$(basename "$a" | sed 's/[0-9][0-9]_//' | sed 's/_/ /g' | sed 's/-/ /g')':";
         list_dir "$a" "$2"'  '
    else
      TITLE=$(title "$a")
      if [ ! -z "$TITLE" ]
      then
echo "$a" | grep "\.md$" >/dev/null  && echo "$2- '$(title "$a")': ${a:5}"
      fi
    fi
  done
}

echo "site_name: Gwion"
#echo "theme: windmill"
echo "theme: readthedocs"
echo "repo_url: https://github.com/Gwion/Gwion"
echo "site_description: a strongly timed musical programming language"
echo "site_author: Jérémie Astor"
echo "nav:"
echo "  - 'Welcome' : index.md"
mv docs/index.md .
list_dir docs "  "
mv index.md docs
