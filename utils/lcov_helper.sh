#!/bin/bash

test_test_plugin() {
  [ -z "$TRAVIS_BUILD_DIR" ] && export GWION_ADD_DIR="$TRAVIS_BUILD_DIR"

  NAME=$1 make install
  valgrind ../../gwion
  ../..//gwion
  make uninstall
}

OUTFILE=lcov/lcov.info

[ -d lcov ] || mkdir lcov

[ -z "$TRAVIS_BUILD_DIR" ] || source utils/test.sh; do_test "tests/bug"
[ -z "$TRAVIS_BUILD_DIR" ] || {
  pushd tests/test_plugins
  for file in *.c
  do
    echo $(sed 's/\.c//' <<< $file)
    test_test_plugin "$(sed 's/\.c//' <<< "$file")"
  done
  popd
}
lcov --capture --directory src --output-file "$OUTFILE"

[ -z "$TRAVIS_BUILD_DIR" ] || {
  coveralls-lcov lcov/lcov.info
  exit
}

genhtml -q -s -t "Gwion: coverage" -o lcov --precision 2 $OUTFILE

cat << EOF > lcov/helper_gcov.js
var i = 1;
var cusid_ele = document.getElementsByClassName('LineNoCov');
function keyDownTextField(e) {
var keyCode = e.keyCode;
  if(keyCode==78) {
    var item = cusid_ele[i];
    cusid_ele[i].scrollIntoView();
    i++;
  } else if(keyCode==66){
    i--;
    var item = cusid_ele[i];
    cusid_ele[i].scrollIntoView();
  }
  if(i < 1) i = 1;
  var size = cusid_ele.length - 1;
  if(i > size) i = size;
}
document.addEventListener("keydown", keyDownTextField, false);
EOF

cat << EOF > lcov/helper_func.js
var i = 1;
var cusid_ele = document.getElementsByClassName('coverFnLo');
function keyDownTextField(e) {
var keyCode = e.keyCode;
  if(keyCode==78) {
    var item = cusid_ele[i];
    cusid_ele[i].scrollIntoView();
    i++;
  } else if(keyCode==66){
    i--;
    var item = cusid_ele[i];
    cusid_ele[i].scrollIntoView();
  }
  if(i < 1) i = 1;
  var size = cusid_ele.length - 1;
  if(i > size) i = size;
}
document.addEventListener("keydown", keyDownTextField, false);
EOF

for file in lcov/*/*.c.gcov.html
do sed -i 's/<body>/<body><script src="..\/helper_gcov.js"><\/script>/' "$file"
done

for file in lcov/*/*.*.func*.html
do sed -i 's/<body>/<body><script src="..\/helper_func.js"><\/script>/' "$file"
done

