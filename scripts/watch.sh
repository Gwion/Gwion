#!/bin/sh

wait() {
  inotifywait -q -r docs --format "%w%f" | tail -1
}

while true
do [[ $1 == *".mdr" ]] &&
  sh scripts/mdr2mdbook.sh $(wait);
done
