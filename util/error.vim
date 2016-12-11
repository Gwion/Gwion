function! GwionTestErrorMsg()


  " set variables
  let test_name = input("name for the test? ")
  let test_file = input("name for the file? ")
  let test_outp = input("output of the file? ")
  let test_cmdl = '  run ./gwion -d -dummy 
    \test/error/' . test_file . '.gw'
  let test_desc = '@test "' . test_name . 
    \' :test/error/' . test_file . '.gw:" {'
  let test_outl = '  assert_output --partial "'
    \. test_outp . '"'
  let test_file = 'test/error/' . test_file . '.gw'

  " get the error test file
  tabedit util/error.bats

  " append the function
  call append(line('$'),    test_desc       )
  call append(line('$'),    test_cmdl       )
  call append(line('$'), '  assert_segfault')
  call append(line('$'), '  assert_success' )
  call append(line('$'),    test_outl       )
  call append(line('$'), '  set_icon'       )
  call append(line('$'),    '}'             )
  call append(line('$'), ''                 )

  " append changes
  write

  " close
  quit

  " edit the test file, if needed/wanted
  if !filereadable(test_file)
    execute 'new' test_file
    elseif confirm("edit test file? ", "&Yes\n&No") == 0
    execute 'new' test_file
  endif

  " re-open bats file if it needs editing
  if confirm("Edit bats file? ", "&Yes\n&No") == 1
    tabedit util/error.bats
  endif

  " clean variables
  unlet test_name
  unlet test_file
  unlet test_outp
  unlet test_cmdl
  unlet test_desc
  unlet test_outl

endf
