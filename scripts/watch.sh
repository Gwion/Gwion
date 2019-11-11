#!/bin/sh

wait() {
  inotifywait -q -r docs --format "%w%f" | tail -1
}

while true
do
  file=$(wait)
  sh scripts/mdr2mdbook.sh $file;
done
