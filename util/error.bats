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
  run ./gwion -d dummy test/error/0_size_variable.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare variables of size '0'"
  set_icon
}

@test "class declared inside itself :test/error/class_inside.gw:" {
  run ./gwion -d dummy test/error/class_inside.gw
  assert_segfault
  assert_success
  assert_output --partial "declared inside itself"
  set_icon
}

@test "primitive reference :test/error/prim_ref.gw:" {
  run ./gwion -d dummy test/error/prim_ref.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare references (@) of primitive type"
  set_icon
}

@test "decl reserved :test/error/decl_res.gw:" {
  run ./gwion -d dummy test/error/decl_res.gw
  assert_segfault
  assert_success
  assert_output --partial "... in variable declaration"
  set_icon
}

@test "variable already defined :test/error/var_defined.gw.gw:" {
  run ./gwion -d dummy test/error/var_defined.gw
  assert_segfault
  assert_success
  assert_output --partial "has already been defined in the same scope..."
  set_icon
}

@test "func pointer already defined :test/error/ptr_defined.gw:" {
  run ./gwion -d dummy test/error/ptr_defined.gw
  assert_segfault
  assert_success
  assert_output --partial "has already been defined in the same scope"
  set_icon
}

@test "func pointer 0 size argument :test/error/ptr_arg_0_size.gw:" {
  run ./gwion -d dummy test/error/ptr_arg_0_size.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare variables of size '0'"
  set_icon
}

@test "func pointer reference primitive :test/error/ptr_ref_prim.gw:" {
  run ./gwion -d dummy test/error/ptr_ref_prim.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare references (@) of primitive type"
  set_icon
}

@test "function pointer argument array  :test/error/ptr_arg_array.gw:" {
  run ./gwion -d dummy test/error/ptr_arg_array.gw
  assert_segfault
  assert_success
  assert_output --partial "must be defined with empty []'s"
  set_icon
}

@test "template inexistant :test/error/template_inexistant.gw:" {
  run ./gwion -d dummy test/error/template_inexistant.gw
  assert_segfault
  assert_success
  assert_output --partial "template call of non-existant function"
  set_icon
}

@test "template call non template :test/error/template_non.gw:" {
  run ./gwion -d dummy test/error/template_non.gw
  assert_segfault
  assert_success
  assert_output --partial "template call of non-template function"
  set_icon
}

@test "label already defined :test/error/label_defined.gw:" {
  run ./gwion -d dummy test/error/label_defined.gw
  assert_segfault
  assert_success
  assert_output --partial "already defined"
  set_icon
}

@test "enum already declared :test/error/enum_declared.gw:" {
  run ./gwion -d dummy test/error/enum_declared.gw
  assert_segfault
  assert_success
  assert_output --partial "already declared as variable"
  set_icon
}

@test "nested function :test/error/function_nested.gw:" {
skip "this fail at compiler level"
  run ./gwion -d dummy test/error/function_nested.gw
  assert_segfault
  assert_success
  assert_output --partial "nested function definitions are not (yet) allowed;"
  set_icon
}

@test "function already used :test/error/function used.gw:" {
  run ./gwion -d dummy test/error/function_used.gw
  assert_segfault
  assert_success
  assert_output --partial "is already used by another value"
  set_icon
}

@test "function primitive reference return type :test/error/function_ret_ref_prim.gw:" {
skip "this fail at compiler level"
  run ./gwion -d dummy test/error/function_ret_ref_prim.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare (@) of primitive type"
  set_icon
}

@test "function argument of no size :test/error/function_arg_no_size.gw:" {
skip "this migth be tested in scan1"
  run ./gwion -d dummy test/error/function_arg_no_size.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot declare variable of size"
  set_icon
}

@test "function argument reserved :test/error/function_arg_res.gw:" {
  run ./gwion -d dummy test/error/function_arg_res.gw
  assert_segfault
  assert_success
  assert_output --partial "in function"
  set_icon
}

@test "function argument array empty :test/error/func_arg_array_empty.gw:" {
skip "this fail at compiler level"
  run ./gwion -d dummy test/error/func_arg_array_empty.gw
  assert_segfault
  assert_success
  assert_output --partial "must be defined with empty []'s"
  set_icon
}

@test "function different signature :test/error/func_sig_differ.gw:" {
  run ./gwion -d dummy test/error/func_sig_differ.gw
  assert_segfault
  assert_success
  assert_output --partial "function signatures differ in return type"
  set_icon
}

##########
# type.c #
##########

@test "invalid plugin" {
  touch /usr/lib/Gwion/plug/invalid.so
  run ./gwion -d dummy
  assert_segfault
  assert_success
  assert_output --partial "error in"
  set_icon
  rm /usr/lib/Gwion/plug/invalid.so
}

@test "invalid plugin2" {
  echo "static int i;" > /usr/lib/Gwion/plug/invalid.c
  cc -shared -fPIC /usr/lib/Gwion/plug/invalid.c -o /usr/lib/Gwion/plug/invalid.so
  rm /usr/lib/Gwion/plug/invalid.c
  run ./gwion -d dummy
  assert_segfault
  assert_success
  assert_output --partial "no import function"
  set_icon
  rm /usr/lib/Gwion/plug/invalid.so
}

@test "incompatible array subscript :test/error/no_compat_sub.gw:" {
  run ./gwion -d dummy test/error/no_compat_sub.gw
  assert_segfault
  assert_success
  assert_output --partial "must be of type 'int'"
  set_icon
}

@test "incompatible array subscript :test/error/no_compat_sub2.gw:" {
  run ./gwion -d dummy test/error/no_compat_sub2.gw
  assert_segfault
  assert_success
  assert_output --partial "incompatible array subscript type"
  set_icon
}

@test "undefined parent class :test/error/undef_parent.gw:" {
  run ./gwion -d dummy test/error/undef_parent.gw
  assert_segfault
  assert_success
  assert_output --partial "undefined parent class"
  set_icon
}

@test "undefined parent class :test/error/prim_parent.gw:" {
  run ./gwion -d dummy test/error/prim_parent.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot extend primitive"
  set_icon
}

@test "incomplete parent class :test/error/incomplete_parent.gw:" {
  run ./gwion -d dummy test/error/incomplete_parent.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot extend incomplete type"
  set_icon
}

@test "already define in  parent class :test/error/already_parent.gw:" {
  run ./gwion -d dummy test/error/already_parent.gw
  assert_segfault
  assert_success
  assert_output --partial "has already been defined in parent class"
  set_icon
}

@test "declare in GACK  :test/error/gack_decl.gw:" {
  run ./gwion -d dummy test/error/gack_decl.gw
  assert_segfault
  assert_success
  assert_output --partial "cannot use <<< >>> on variable declarations"
  set_icon
}

@test "declare in GACK  :test/error/gack_decl2.gw:" {
  run ./gwion -d dummy test/error/gack_decl2.gw
  assert_segfault
  assert_success
  assert_output --partial "can't declare in GACK"
  set_icon
}

@test "static variable outside class :test/error/static_out.gw:" {
  run ./gwion -d dummy test/error/static_out.gw
  assert_segfault
  assert_success
  assert_output --partial "static variables must be declared at class scope"
  set_icon
}

@test "provide array :test/error/provide_array.gw:" {
  run ./gwion -d dummy test/error/provide_array.gw
  assert_segfault
  assert_success
  assert_output --partial "must provide values/expressions for array [...]"
  set_icon
}

@test "provide array :test/error/array_incompatible.gw:" {
  run ./gwion -d dummy test/error/array_incompatible.gw
  assert_segfault
  assert_success
  assert_output --partial "array init [...] contains incompatible types..."
  set_icon
}

@test "vector too big :test/error/vector_too_big.gw:" {
  run ./gwion -d dummy test/error/vector_too_big.gw
  assert_segfault
  assert_success
  assert_output --partial "vector dimensions not supported > 4..."
  set_icon
}

@test "invalid type in vector :test/error/vector_invalid.gw:" {
  run ./gwion -d dummy test/error/vector_invalid.gw
  assert_segfault
  assert_success
  assert_output --partial "in vector value"
  set_icon
}

@test "this outside of class :test/error/this.gw:" {
  run ./gwion -d dummy test/error/this.gw
  assert_segfault
  assert_success
  assert_output --partial "keyword 'this' can be used only inside class definition..."
  set_icon
}

@test "this in static function :test/error/this_static.gw:" {
  run ./gwion -d dummy test/error/this_static.gw
  assert_segfault
  assert_success
  assert_output --partial "keyword 'this' cannot be used inside static functions..."
  set_icon
}

@test "complex too short :test/error/missing im.gw:" {
  run ./gwion -d dummy test/error/missing_im.gw
  assert_segfault
  assert_success
  assert_output --partial "missing imaginary component of complex value..."
  set_icon
}

@test "complex too big :test/error/complex_too_big.gw:" {
  run ./gwion -d dummy test/error/complex_too_big.gw
  assert_segfault
  assert_success
  assert_output --partial "extraneous component of complex value..."
  set_icon
}

@test "complex invalid real :test/error/complex_invalid_re.gw:" {
  run ./gwion -d dummy test/error/complex_invalid_re.gw
  assert_segfault
  assert_success
  assert_output --partial "in real component of complex value"
  set_icon
}

@test "complex invalid real :test/error/complex_invalid_im.gw:" {
  run ./gwion -d dummy test/error/complex_invalid_im.gw
  assert_segfault
  assert_success
  assert_output --partial "in imaginary component of complex value"
  set_icon
}

@test "polar too short :test/error/missing phase.gw:" {
  run ./gwion -d dummy test/error/missing_phase.gw
  assert_segfault
  assert_success
  assert_output --partial "missing phase component of polar value..."
  set_icon
}

@test "polar too big :test/error/polar_too_big.gw:" {
  run ./gwion -d dummy test/error/polar_too_big.gw
  assert_segfault
  assert_success
  assert_output --partial "extraneous component of polar value..."
  set_icon
}

@test "polar invalid modulus :test/error/polar_invalid_mod.gw:" {
  run ./gwion -d dummy test/error/polar_invalid_mod.gw
  assert_segfault
  assert_success
  assert_output --partial "in modulus component of polar value..."
  set_icon
}

@test "polar invalid phase :test/error/polar_invalid_ph.gw:" {
  run ./gwion -d dummy test/error/polar_invalid_ph.gw
  assert_segfault
  assert_success
  assert_output --partial "in phase component of polar value..."
  set_icon
}

@test "array exceed dimension :test/error/array_excess.gw:" {
  run ./gwion -d dummy test/error/array_excess.gw
  assert_segfault
  assert_success
  assert_output --partial "exceeds defined dimension"
  set_icon
}

@test "array invalid type :test/error/array_invalid.gw:" {
  run ./gwion -d dummy test/error/array_invalid.gw
  assert_segfault
  assert_success
  assert_output --partial "must be of type 'int'"
  set_icon
}

@test "assign function ptr :test/error/ptr_assign.gw:" {
  run ./gwion -d dummy test/error/ptr_assign.gw
  assert_segfault
  assert_success
  assert_output --partial "can't assign function pointer to function pointer for the moment. sorry."
  set_icon
}

@test "assign function ptr :test/error/ptr_assign_class.gw:" {
  run ./gwion -d dummy test/error/ptr_assign_class.gw
  assert_segfault
  assert_success
  assert_output --partial "can't assign function pointer in class for the moment. sorry."
  set_icon
}

@test "assign function ptr from other class :test/error/ptr_assign_other.gw:" {
  run ./gwion -d dummy test/error/ptr_assign_other.gw
  assert_segfault
  assert_success
  assert_output --partial "can't assign member function to member function pointer of an other class"
  set_icon
}

@test "assign non member function ptr :test/error/ptr_assign_member.gw:" {
  run ./gwion -d dummy test/error/ptr_assign_member.gw
  assert_segfault
  assert_success
  assert_output --partial "can't assign member function to non member function pointer"
  set_icon
}

@test "assign non member function ptr :test/error/ptr_assign_global.gw:" {
  run ./gwion -d dummy test/error/ptr_assign_global.gw
  assert_segfault
  assert_success
  assert_output --partial "can't assign non member function to member function pointer"
  set_icon
}

@test "not match found for function :test/error/ptr_match.gw:" {
  run ./gwion -d dummy test/error/ptr_match.gw
  assert_segfault
  assert_success
  assert_output --partial "not match found for function"
  set_icon
}

@test "not match found for operator :test/error/op_match.gw:" {
  run ./gwion -d dummy test/error/op_match.gw
  assert_segfault
  assert_success
  assert_output --partial "not match found for operator"
  set_icon
}

@test "left-side is not mutable :test/error/left_mutable.gw:" {
  run ./gwion -d dummy test/error/left_mutable.gw
  assert_segfault
  assert_success
  assert_output --partial "...(reason: --- left-side operand is not mutable)"
  set_icon
}

@test "right-side is not mutable :test/error/right_mutable.gw:" {
  run ./gwion -d dummy test/error/right_mutable.gw
  assert_segfault
  assert_success
  assert_output --partial "...(reason: --- right-side operand is not mutable)"
  set_icon
}

@test "unknown type in cast expression :test/error/cast_unknown.gw:" {
  run ./gwion -d dummy test/error/cast_unknown.gw
  assert_segfault
  assert_success
  assert_output --partial "in cast expression"
  set_icon
}

@test "postfix no match :test/error/postfix_no_match.gw:" {
  run ./gwion -d dummy test/error/postfix_no_match.gw
  assert_segfault
  assert_success
  assert_output --partial "no suitable resolutation for postfix operator"
  set_icon
}

@test "no suitable resolutation for postfix operator :test/error/dur_prefix.gw:" {
  run ./gwion -d dummy test/error/dur_prefix.gw
  assert_segfault
  assert_success
  assert_output --partial "in prefix of dur expression"
  set_icon
}

@test "no suitable resolutation for postfix operator :test/error/dur_postfix.gw:" {
  run ./gwion -d dummy test/error/dur_postfix.gw
  assert_segfault
  assert_success
  assert_output --partial "in postfix of dur expression..."
  set_icon
}

@test "function call using a non-existing function :test/error/func_exist.gw:" {
  run ./gwion -d dummy test/error/func_exist.gw
  assert_segfault
  assert_success
  assert_output --partial "function call using a non-existing function"
  set_icon
}

@test "function call using a non-existing function :test/error/func_non.gw:" {
  run ./gwion -d dummy test/error/func_non.gw
  assert_segfault
  assert_success
  assert_output --partial "function call using a non-function value"
  set_icon
}

