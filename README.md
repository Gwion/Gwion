=====
Gwion
=====

gwion is a object-oriented prommaing language, aimed at making music

It is strongly inspired by 'chuck', and as chuck it is 'strongly timed'

	* based on soudpipe
	* plugin system

primitive type:
	int
	float
	complex

new features:
=============
	* destructors
	* 'sporking' of code segment
	* overloadable operators
	* function pointers
	* enums
	* Goto/Label
	* Switch
	* Doc
	* variadic functions
	* unamed global unions (in class and named unions to come)

linux (ALSA) only for now

Abstract Syntaxic Tree
======================
	* Stmt_List
	* Class_Def
	* Func_Def


STaTeMenT
=========
	* Expression
	*	Code
	* While
	* Until
	* Loop
	* For
	* If
	* Break
	* Continue
	* Return
	* Goto/Label
	* Switch
	* Case
	* Enum
	* Func_ptr

Expression
==========

	* Postfix
	* Primary
		+ var
		+ num
		+ fnum
		+ str
		+ chr
		+ array
		+ Expression
		+ Complex
		+ Polar
	* Decl
	* Unary
	* Binary
	* Cast
	* Func_call
	* If
	* Dot Member
	* Dur
	* Func_Ptr


