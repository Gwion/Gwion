#!/bin/bash

get_latest_release() {
  curl --silent "https://api.github.com/repos/$1/$2/releases/latest" | # Get latest release from GitHub api
    grep '"browser_download_url"' | grep 'nightly-ubuntu' |
    sed -E 's/.*"([^"]+)".*/\1/'                                    # Pluck JSON value
}

update() {
  new_release=$(get_latest_release $1 $2)
  file=${2}*.zip
  wget "$new_release"
  7z e $file
  rm $file
}

update Gwion      gwion
update Gwion      gwion-plug
update fennecdjay mdr
update fennecdjay cmojify
