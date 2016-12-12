load test_helper/bats-support/load
load test_helper/bats-assert/load
load notify

@test "non exitant file :test/error/no_file.gw:" {
  run ./gwion -d dummy test/error/no_file.gw
  assert_segfault
  assert_success
  assert_output --partial "error while opening file"
  set_icon
}

@test "multiple public class :test/error/assultiple_public.gw:" {
  run ./gwion -d dummy test/error/class_multiple_public.gw
  assert_segfault
  assert_success
  assert_output --partial "more than one 'public' class defined"
  set_icon
}

@test "unknown type in declaration :test/error/unknown_type.gw:" {
  run ./gwion -d dummy test/error/unknown_type.gw
  assert_segfault
  assert_success
  assert_output --partial "unknown in declaration"
  set_icon
}

@test "postfix on non-mutable data :test/error/postfix.gw:" {
  run ./gwion -d dummy test/error/postfix.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot be used on non-mutable data-type"
  set_icon
}

@test "already defined class :test/error/defined_class.gw:" {
  run ./gwion -d dummy test/error/defined_class.gw
  assert_segfault
  assert_success
  assert_output --partial "is already defined in namespace"
  set_icon
}

@test "class name reserved :test/error/class_res.gw:" {
  run ./gwion -d dummy test/error/class_res.gw
  assert_segfault
  assert_success
  assert_output --partial "is reserved"
  set_icon
}

@test "template unlnow type :test/error/template_unknown.gw:" {
  run ./gwion -d dummy test/error/template_unknown.gw
  assert_segfault
  assert_success
  assert_output --partial "unknown in template call"

  set_icon
}

@test "enum already defined :test/error/enum_defined.gw:" {
  run ./gwion -d dummy test/error/enum_defined.gw
  assert_segfault
  assert_success
  assert_output --partial "already declared"
  set_icon
}

@test "enum defined :test/error/enum_defined_var.gw:" {
  run ./gwion -d dummy test/error/enum_defined_var.gw
  assert_segfault
  assert_success
  assert_output --partial "already declared as variable"
  set_icon
}


@test "enum already defined as var:test/error/enum_defined2.gw:" {
  run ./gwion -d dummy test/error/enum_defined2.gw
  assert_segfault
  assert_success
  assert_output --partial "already declared"
  set_icon
}

@test "enum already defined as var:test/error/enum_defined3.gw:" {
  run ./gwion -d dummy test/error/enum_defined3.gw
  assert_segfault
  assert_success
  assert_output --partial "already declared"
  set_icon
}

@test "union unknown type :test/error/union_unknown.gw:" {
  run ./gwion -d dummy test/error/union_unknown.gw
  assert_segfault
  assert_success
  assert_output --partial "unknown type"
  set_icon
}

@test "union array must be empty :test/error/union_array_empty:" {
  run ./gwion -d dummy test/error/union_array_empty.gw
  assert_segfault
  assert_success
  assert_output --partial "array declaration must be empty in union"
  set_icon
}

@test "function pointer unknown ret type:test/error/pointer_unknown.gw:" {
  run ./gwion -d dummy test/error/pointer_unknown.gw
  assert_segfault
  assert_success
  assert_output --partial "unknown type"
  set_icon
}

@test "function pointer static outside class :test/error/pointer_outside_class.gw:" {
  run ./gwion -d dummy test/error/pointer_outside_class.gw
  assert_segfault
  assert_success
  assert_output --partial "can't declare func pointer static outside of a class"
  set_icon
}

@test "function pointer unknown argument type:test/error/pointer_unknown_arg.gw:" {
  run ./gwion -d dummy test/error/pointer_unknown_arg.gw
  assert_segfault
  assert_success
  assert_output --partial "unknown type in argument"
  set_icon
}


@test "destructor outside class :test/error/dtor_outside_class.gw:" {
  run ./gwion -d dummy test/error/dtor_outside_class.gw
  assert_segfault
  assert_success
  assert_output --partial "dtor must be in class def"
  set_icon
}

@test "function unknown ret type :test/error/func_unknown_ret.gw:" {
  run ./gwion -d dummy test/error/func_unknown_ret.gw
  assert_segfault
  assert_success
  assert_output --partial "unknown return type"
  set_icon
}

@test "function arg unknown :test/error/func_arg_unkown.gw:" {
  run ./gwion -d dummy test/error/func_arg_unknown.gw
  assert_segfault
  assert_success
  assert_output --partial "unknown type in argument"
  set_icon
}

@test "operator argument number :test/error/operator_narg.gw:" {
  run ./gwion -d dummy test/error/operator_narg.gw
  assert_segfault
  assert_success
  assert_output --partial "operators can only have one or two arguments"
  set_icon
}

@test "operator valid :test/error/operator_valid.gw:" {
  run ./gwion -d dummy test/error/operator_valid.gw
  assert_segfault
  assert_success
  assert_output --partial "is not a valid operator name"
  set_icon
}

###########
#         #
#  Scan 2 #
#         #
###########

@test "variable of size zero :test/error/0_size_variable.gw:" {
  run ./gwion -d -dummy test/error/0_size_variable.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare variables of size '0'"
  set_icon
}

@test "class declared inside itself :test/error/class_inside.gw:" {
  run ./gwion -d -dummy test/error/class_inside.gw
  assert_segfault
  assert_success
  assert_output --partial "declared inside itself"
  set_icon
}

@test "primitive reference :test/error/prim_ref.gw:" {
  run ./gwion -d -dummy test/error/prim_ref.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare references (@) of primitive type"
  set_icon
}

@test "decl reserved :test/error/decl_res.gw:" {
  run ./gwion -d -dummy test/error/decl_res.gw
  assert_segfault
  assert_success
  assert_output --partial "... in variable declaration"
  set_icon
}

@test "variable already defined :test/error/var_defined.gw.gw:" {
  run ./gwion -d -dummy test/error/var_defined.gw
  assert_segfault
  assert_success
  assert_output --partial "has already been defined in the same scope..."
  set_icon
}

@test "func pointer already defined :test/error/ptr_defined.gw:" {
  run ./gwion -d -dummy test/error/ptr_defined.gw
  assert_segfault
  assert_success
  assert_output --partial "has already been defined in the same scope"
  set_icon
}

@test "func pointer 0 size argument :test/error/ptr_arg_0_size.gw:" {
  run ./gwion -d -dummy test/error/ptr_arg_0_size.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare variables of size '0'"
  set_icon
}

@test "func pointer reference primitive :test/error/ptr_ref_prim.gw:" {
  run ./gwion -d -dummy test/error/ptr_ref_prim.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare references (@) of primitive type"
  set_icon
}

@test "function pointer argument array  :test/error/ptr_arg_array.gw:" {
  run ./gwion -d -dummy test/error/ptr_arg_array.gw
  assert_segfault
  assert_success
  assert_output --partial "must be defined with empty []'s"
  set_icon
}

@test "template inexistant :test/error/template_inexistant.gw:" {
  run ./gwion -d -dummy test/error/template_inexistant.gw
  assert_segfault
  assert_success
  assert_output --partial "template call of non-existant function"
  set_icon
}

@test "template call non template :test/error/template_non.gw:" {
  run ./gwion -d -dummy test/error/template_non.gw
  assert_segfault
  assert_success
  assert_output --partial "template call of non-template function"
  set_icon
}

@test "label already defined :test/error/label_defined.gw:" {
  run ./gwion -d -dummy test/error/label_defined.gw
  assert_segfault
  assert_success
  assert_output --partial "already defined"
  set_icon
}

@test "enum already declared :test/error/enum_declared.gw:" {
  run ./gwion -d -dummy test/error/enum_declared.gw
  assert_segfault
  assert_success
  assert_output --partial "already declared as variable"
  set_icon
}

@test "nested function :test/error/function_nested.gw:" {
skip "this fail at compiler level"
  run ./gwion -d -dummy test/error/function_nested.gw
  assert_segfault
  assert_success
  assert_output --partial "nested function definitions are not (yet) allowed;"
  set_icon
}

@test "function already used :test/error/function used.gw:" {
  run ./gwion -d -dummy test/error/function_used.gw
  assert_segfault
  assert_success
  assert_output --partial "is already used by another value"
  set_icon
}

@test "function primitive reference return type :test/error/function_ret_ref_prim.gw:" {
skip "this fail at compiler level"
  run ./gwion -d -dummy test/error/function_ret_ref_prim.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare (@) of primitive type"
  set_icon
}

@test "function argument of no size :test/error/function_arg_no_size.gw:" {
skip "this migth be tested in scan1"
  run ./gwion -d -dummy test/error/function_arg_no_size.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare variable of size"
  set_icon
}

@test "function argument reserved :test/error/function_arg_res.gw:" {
  run ./gwion -d -dummy test/error/function_arg_res.gw
  assert_segfault
  assert_success
  assert_output --partial "in function"
  set_icon
}

@test "function argument array empty :test/error/func_arg_array_empty.gw:" {
skip "this fail at compiler level"
  run ./gwion -d -dummy test/error/func_arg_array_empty.gw
  assert_segfault
  assert_success
  assert_output --partial "must be defined with empty []'s"
  set_icon
}

@test "function different signature :test/error/func_sig_differ.gw:" {
  run ./gwion -d -dummy test/error/func_sig_differ.gw
  assert_segfault
  assert_success
  assert_output --partial "function signatures differ in return type"
  set_icon
}

