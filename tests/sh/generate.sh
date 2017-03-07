# [test] #1
n=0
[ $1 ] && n=$1
[ $n -eq 0 ] && n=1
source tests/sh/common.sh

./util/generate_header "/invalid"
ret=$?
if [ $ret ]
then echo "ok $n header wrong target"
then echo "not ok $n header wrong target"
fi

