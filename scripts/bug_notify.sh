#!/bin/sh

wait() {
  inotifywait -q -r AFL/ --format "%w%f" | tail -1
}

notify() {
  dbus-launch notify-send -u critical -c bug \
    -i dialog-error \
    -h STRING:filename:'$1" '🐉 found bug !'
}
while true
do
  file=$(wait)
  [[ $file == *"crashes"* ]] || continue
  base=$(basename $file)
  [ ${base:0:3} = "id:" ] && notify "$base"
done
