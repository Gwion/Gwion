set -e
[ $# -lt 2 ] && {
  echo "usage: afl-helper crash_dir out_dir"
  exit 1
}

DEST=$(realpath $2)
cd $DEST
NUM=$(ls *.gw | wc -l)
TMP=0
for a in *.gw
do
  mv $a ${TMP}.gw
  TMP=$((TMP+1))
done

cd $1
for a in id*
do
  cp $a $DEST/${NUM}.gw
  NUM=$((NUM+1))
done
