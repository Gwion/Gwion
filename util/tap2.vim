
sign define tap_sign_ok   text=✓ texthl=TapSignOK
sign define tap_sign_bad  text=✗ texthl=TapSignX
sign define tap_sign_skip text=- texthl=TapSignSkip

syn match TapSignOk   /✓/
syn match TapSignX    /✗/
syn match TapSignSkip /-/

hi! TapSignOK ctermfg=green  cterm=bold gui=bold
hi! TapSignX  ctermfg=red    cterm=bold gui=bold
hi! TapSkip   ctermfg=yellow cterm=bold gui=bold

let g:tap_map = {}
let g:tap_timer_id = 0

function! tap2#open()
"	let nr = bufnr('.')
"	for i in range(line('.'), line('$')) 
"echo i
"sleep 1
"endfor
	for i in range(1, line('$'))
		let myline = getline(i)
"echo myline i
		if myline[1:4] == "test"
			let g:tap_map[split(myline, '"')[1]] = i
		endif
	endfor
"sleep 1
"	let file = '/tmp/' . expand('%:t') . '.log'
	if filereadable('/tmp/' . expand('%:t') . '.lock')
		execute 'new /tmp/' . expand('%:t') . '.log'
		call timer_stop(g:tap_timer_id)
	for i in range(2, line('$'))
"echo i
		let myline = getline(i)
"echo myline[1]
			let line = split(myline, '\v<\d+>')[1]
"		let myline = getline(i)
"echo myline
"		if myline[0:1] == "ok"
"			let line = split(myline, '\v<\d+>')[1]
:echo 'test:' line 'test:' g:tap_map[line]
"			exe 'sign place 1 line=1 name=tap_sign_ok buffer=' . nr
"			exe 'sign place ' . i . ' line=' . g:tap_map[line] . ' name=tap_sign_ok buffer=' . nr
"		endif
"		endif
	endfor
"	call system('rm  /tmp/' . expand('%:t') . '.log')
"	call system('rm  /tmp/' . expand('%:t') . '.lock')
	endif
endfunction

"function! tap2#update()
"	for i in range(1, line('$'))
"		let myline = getline(i)
"		if myline[1:4] == "test"
"			let g:tap_map[split(myline, '"')[1]] = i
"echo split(myline, '"')[1] g:tap_map[split(myline, '"')[1]]
"		endif
"	endfor
"endf

let g:tap_timer_id = timer_start(1000, 'tap2#open', { 'repeat': -1 })
"let g:tap_timer_id = timer_start(1000, 'tap2#open', { 'repeat': 1})
"call tap2#update()
call system('(bats ' . expand('%:p') . ' 1>/tmp/' . expand('%:t') . '.log; touch /tmp/' . expand('%:t') . '.lock; sleep 2; rm  /tmp/' . expand('%:t') . '.lock)&')
