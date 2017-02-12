#!/bin/bash
rm -rf coverage

type_defs_global=$(cat ./*/*.c | grep "struct Type_ "| sed 's/static //' | sed 's/extern //' | grep "= {"  | grep -v '@' | cut -d ' ' -f 3 | tr '\n' ':')
type_name_global=$(cat ./*/*.c | grep "struct Type_ "| sed 's/static //' | sed 's/extern //' | grep "= {"  | grep -v '@' | cut -d '"' -f 2 | tr '\n' ':')
export type_defs_global
export type_name_global

#export type_defs_array=(${type_defs_global//:/ })
#export type_name_array=(${type_name_global//:/ })

# generate files from ./lang and ./ugen
for file in lang/*.c ugen/*.c
do
	bash util/op2sign.sh "$file" "$PWD"
done

# try file
mkdir -p coverage/{run,seg,leak,log}
cd "$PWD" || exit
for file in *.gw
do
	./gwion -d dummy "$file"
	if [ "$?" = 0 ]
	then mv "$file" coverage/run
	else mv "$file" coverage/seg
	fi
done

# remove leaks
cd coverage/run || exit
for file in *.gw
do
	valgrind --log-file="../log/$file.log" ../..//gwion -d dummy "$file"
	grep 'Invalid write of size ' "../$file.log" > /dev/null && mv "$file" ../leak
	grep 'Invalid read  of size ' "../$file.log" > /dev/null && mv "$file" ../leak
	grep 'Invalid free() / delete / delete\[\] / realloc()' "../$file.log" > /dev/null && mv "$file" ../leak
	grep 'depends on uninitialised value' "../$file.log" > /dev/null && mv "$file" ../leak
	grep 'All heap blocks were freed -- no leaks are possible' "../$file.log"> /dev/null && rm "../log/$file.log" && continue
	grep 'definitely lost: 0 bytes in 0 blocks' "../$file.log" > /dev/null && mv "$file" ../leak
	grep 'indirectly lost: 0 bytes in 0 blocks' "../$file.log" > /dev/null && mv "$file" ../leak
	grep 'possibly lost: 0 bytes in 0 blocks' "../$file.log" > /dev/null   && mv "$file" ../leak
done
