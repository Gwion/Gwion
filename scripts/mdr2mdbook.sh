#!/bin/sh
footer() {
  cat << EOF

<!-- added by doc2src.sh -->
<footer>
built with <a href="https://github.com/rust-lang/mdBook/">mdBook</a></br>
You'll find the source <a href="https://github.com/fennecdjay/Gwion">here</a>, Luke!
</footer>
EOF
}

mk_target() {
  sed 's/```\(.*\)gw/```\1cpp/' $1 |
    sed 's/:tada:/\&#128540;/g'    |
    sed 's/:champagne:/\&#127870;/g'    |
    sed 's#:gwion:#[Gwion](https://github.com/fennecdjay/Gwion)#g'
  footer
}

ensure_dir() {
  mkdir -p $(dirname $1)
}

doc2src() {
  echo $1
  mdr $1 || return
  mdfile=${1::-1}
#  target=$(sed 's/docs/md/' <<< $mdfile)
  target=$(echo "$mdfile" | sed 's/docs/md/')
  ensure_dir $target
  mk_target $mdfile > $target
  rm $mdfile
}

runall() {
  list=$(find docs -type f -name "*.mdr")
  for file in $list
  do doc2src "$file"
  done
}

if [[ "$1" == *".mdr" ]]
then doc2src "$1"
if [ "$(git status -s | grep docs)" ]
then sh scripts/summary.sh > md/SUMMARY.md
fi
else runall
fi
