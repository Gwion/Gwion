[ $# -lt 2 ] && {
  echo "usage: afl-helper crash_dir out_dir"
  exit 1
}

# check false positive
FUZZER=$PWD/gwion-fuzz
FAULT=0
pushd $1
for a in id*
do
  cat $a | $FUZZER || FAULT=$((FAULT+1))
done
[ $FAULT = 0 ] && { echo "only false positive"; exit 1; }
popd

set -e

DEST=$(realpath $2)
[ -d $DEST ] || mkdir $DEST
pushd $DEST
NUM=$(ls *.gw | wc -l)
TMP=0
for a in *.gw
do
  mv $a ${TMP}.gw
  TMP=$((TMP+1))
done
popd

pushd $1
for a in id*
do
  cp $a $DEST/${NUM}.gw
  NUM=$((NUM+1))
done
popd
