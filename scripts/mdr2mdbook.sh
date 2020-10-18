#!/bin/bash
footer() {
  cat << EOF

<!-- added by doc2src.sh -->
<footer>
built with <a href="https://github.com/rust-lang/mdBook/">mdBook</a></br>
You'll find the source <a href="https://github.com/Gwion/Gwion">here</a>, Luke!
</footer>
EOF
}

mk_target() {
  cmojify "$(cat $1)" |
    sed 's/```.*gw/```gw/' | 
    sed 's#:gwion:#[Gwion](https://github.com/Gwion/Gwion)#g'
  footer
}

ensure_dir() {
  mkdir -p $(dirname $1)
}

doc2src() {
  mdr $1 || return
  mdfile=${1::-1}
  target=$(sed 's/docs/src/' <<< $mdfile)
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
then bash scripts/summary.sh > src/SUMMARY.md
fi
else runall
fi
