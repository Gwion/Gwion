@test " wrong input file" {
	run ./gwion non_existent_file
	[  "$status" -eq 0 ]
}

@test " wrong input file. valgrind" {
#	valgrind ./gwion non_existent_file | tail -8 | head -4 | sed "s/^.*: //" |cut -d " " -f 1,4
	run valgrind  --log-file=util/valgring.log ./gwion -d dummy non_existent_file
	[  "$status" -eq 0 ]
echo $(cat util/valgring.log | tail -8)
}

