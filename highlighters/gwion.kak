# Gwion syntax highlighting for kakoune

provide-module -override gwion %{
	add-highlighter shared/gwion regions
	add-highlighter shared/gwion/other default-region group

	# comment
	add-highlighter shared/gwion/comment region '#!' '($|!#)' group
	add-highlighter shared/gwion/comment/ fill comment

	# preprocessor
	add-highlighter shared/gwion/preproc region '#' '$' group
	add-highlighter shared/gwion/preproc/ fill module

	evaluate-commands %sh{
		keywords='
			operator extends global abstract
			final
			default late if else
			break continue return while do
			until repeat for foreach match
			where when case
			defer try perform handle retry
			fun function typedef distinct funptr
			new spork fork locale'

		attributes='const var static private public protect template samp ms second minute delay'
		types='auto int float bool dur time void Object Shred Event'
		values='true false none this now me adc dac maybe'
		builtins='__file__ __line__ __func__'
		entities='class struct trait union enum'
		operators='\\\? : \$ @ \\\+ - \\\* / % \~ \< \> \\\^ \\\| \& ! ='

		join() { sep=$2; eval set -- $1; IFS="$sep"; echo "$*"; }

		printf %s\\n "declare-option str-list gwion_static_words $(join "${keywords} ${attributes} ${types} ${values} ${decorators}" ' ')"

		printf %s "
			add-highlighter shared/gwion/other/ regex \b($(join "${keywords}"   '|'))\b             0:keyword
			add-highlighter shared/gwion/other/ regex \b($(join "${attributes} ${entities}" '|'))\b 0:attribute
			add-highlighter shared/gwion/other/ regex \b($(join "${types}"      '|'))\b             0:type
			add-highlighter shared/gwion/other/ regex \b($(join "${values}"     '|'))\b             0:value
			add-highlighter shared/gwion/other/ regex \b($(join "${builtins}"   '|'))\b             0:builtin
			add-highlighter shared/gwion/other/ regex   ($(join "${operators}"  '|'))               0:operator
		"
	}

	# user operator
	add-highlighter shared/gwion/other/ regex "@\w+" 0:operator

	# literals
	add-highlighter shared/gwion/other/ regex "\B'((\\.)|[^'\\])'\B" 0:value  # char

	add-highlighter shared/gwion/other/ regex "\B`((\\.)|[^'\\])*`\B" 0:value  # locales

	add-highlighter shared/gwion/other/ regex "\d+(u|U|l|L)?" 0:value                   # decimal
	add-highlighter shared/gwion/other/ regex "((\d+\.\d*)|(\d*\.\d+))([eE][\+-]\d+)*" 0:value           # float

	add-highlighter shared/gwion/other/ regex "0[xX][a-fA-F0-9]+(u|U|l|L)?" 0:value     # hex
	add-highlighter shared/gwion/other/ regex "0[bB][0-1]+(u|U|l|L)?" 0:value           # binary
	add-highlighter shared/gwion/other/ regex "0[cC][0-7]+(u|U|l|L)?" 0:value           # octal

	# string
	add-highlighter shared/gwion/string region '"' (?<!\\)(\\\\)*" group
	add-highlighter shared/gwion/string/ fill string
	add-highlighter shared/gwion/string/ regex \\[\\ntr'"] 0:keyword
}

hook global BufCreate .*\.(gw) %{ set-option buffer filetype gwion }
hook global WinSetOption filetype=gwion %{ require-module gwion }

hook -group wpp-highlight global WinSetOption filetype=gwion %{
	add-highlighter window/gwion ref gwion
	hook -once -always window WinSetOption filetype=.* %{ remove-highlighter window/gwion }
}

# comment token
hook global BufSetOption filetype=gwion %{
	set-option buffer comment_line '#!'
	set-option buffer comment_block_begin '#!'
	set-option buffer comment_block_end ''
}


