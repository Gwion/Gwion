" open when ready
let g:bats_timer_id = -1

func! BatsOpenWhenReady()
  if filereadable('/tmp/gwion_bats.lock')
    "tabnew
    tabedit 'Gwion Test'
" clean
	set modifiable
:.,$d
"    delete line(1)
"	execute ':dg<CR>'
"    tabnew '/tmp/gwion_bats.log'
"    tabnew
	call append(line(1), split(system('cat /tmp/gwion_bats.log'), '\n'))
    source ~/.config/nvim/syntax/tap.vim
    call timer_stop(g:bats_timer_id)
  endif
endf

let g:bats_timer_id = timer_start(1000, 'BatsOpenWhenReady', {'repeat':-1})

call system('rm /tmp/gwion_bats.lock; sh util/test.sh&')
