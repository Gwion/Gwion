#!/bin/bash

get_latest_release() {
  curl --silent "https://api.github.com/repos/$1/$2/releases/latest" | # Get latest release from GitHub api
    grep '"tag_name":' |                                            # Get tag line
    sed -E 's/.*"([^"]+)".*/\1/'                                    # Pluck JSON value
}

update() {
  new_release=$(get_latest_release $1 $2)
  url=https://github.com/$1/$2/releases/download
  file=linux.tar.gz
  wget "$url/$new_release/$file"
  tar -zxvf $file
  rm $file
}

update fennecdjay Gwion
update fennecdjay mdr
