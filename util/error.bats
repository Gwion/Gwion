load test_helper/bats-support/load
load test_helper/bats-assert/load
load notify

function gwion_basic_test() {
  run ./gwion -d dummy "$1"
  assert_segfault
  assert_success
  assert_output --partial "$2"
  set_icon
}

@test "non exitant file :test/error/no_file.gw:" {
  gwion_basic_test test/error/no_file.gw   "error while opening file"
}

@test "multiple public class :test/error/assultiple_public.gw:" {
  gwion_basic_test test/error/class_multiple_public.gw   "more than one 'public' class defined"
}

@test "unknown type in declaration :test/error/unknown_type.gw:" {
  gwion_basic_test test/error/unknown_type.gw   "unknown in declaration"
}

@test "postfix on non-mutable data :test/error/postfix.gw:" {
  gwion_basic_test test/error/postfix.gw   "cannot be used on non-mutable data-type"
}

@test "already defined class :test/error/defined_class.gw:" {
  gwion_basic_test test/error/defined_class.gw   "is already defined in namespace"
}

@test "class name reserved :test/error/class_res.gw:" {
  gwion_basic_test test/error/class_res.gw   "is reserved"
}

@test "template unlnow type :test/error/template_unknown.gw:" {
  gwion_basic_test test/error/template_unknown.gw   "unknown in template call"
}

@test "enum already defined :test/error/enum_defined.gw:" {
  gwion_basic_test test/error/enum_defined.gw   "already declared"
}

@test "enum defined :test/error/enum_defined_var.gw:" {
  gwion_basic_test test/error/enum_defined_var.gw   "already declared as variable"
}


@test "enum already defined as var:test/error/enum_defined2.gw:" {
  gwion_basic_test test/error/enum_defined2.gw   "already declared"
}

@test "enum already defined as var:test/error/enum_defined3.gw:" {
  gwion_basic_test test/error/enum_defined3.gw   "already declared"
}

@test "union unknown type :test/error/union_unknown.gw:" {
  gwion_basic_test test/error/union_unknown.gw   "unknown type"
}

@test "union array must be empty :test/error/union_array_empty:" {
  gwion_basic_test test/error/union_array_empty.gw   "array declaration must be empty in union"
}

@test "function pointer unknown ret type:test/error/pointer_unknown.gw:" {
  gwion_basic_test test/error/pointer_unknown.gw   "unknown type"
}

@test "function pointer static outside class :test/error/pointer_outside_class.gw:" {
  gwion_basic_test test/error/pointer_outside_class.gw   "can't declare func pointer static outside of a class"
}

@test "function pointer unknown argument type:test/error/pointer_unknown_arg.gw:" {
  gwion_basic_test test/error/pointer_unknown_arg.gw   "unknown type in argument"
}


@test "destructor outside class :test/error/dtor_outside_class.gw:" {
  gwion_basic_test test/error/dtor_outside_class.gw   "dtor must be in class def"
}

@test "function unknown ret type :test/error/func_unknown_ret.gw:" {
  gwion_basic_test test/error/func_unknown_ret.gw   "unknown return type"
}

@test "function arg unknown :test/error/func_arg_unkown.gw:" {
  gwion_basic_test test/error/func_arg_unknown.gw   "unknown type in argument"
}

@test "operator argument number :test/error/operator_narg.gw:" {
  gwion_basic_test test/error/operator_narg.gw   "operators can only have one or two arguments"
}

@test "operator valid :test/error/operator_valid.gw:" {
  gwion_basic_test test/error/operator_valid.gw   "is not a valid operator name"
}

###########
#         #
#  Scan 2 #
#         #
###########

@test "variable of size zero :test/error/0_size_variable.gw:" {
  gwion_basic_test test/error/0_size_variable.gw   "cannot declare variables of size '0'"
}

@test "class declared inside itself :test/error/class_inside.gw:" {
  gwion_basic_test test/error/class_inside.gw   "declared inside itself"
}

@test "primitive reference :test/error/prim_ref.gw:" {
  gwion_basic_test test/error/prim_ref.gw   "cannot declare references (@) of primitive type"
}

@test "decl reserved :test/error/decl_res.gw:" {
  gwion_basic_test test/error/decl_res.gw   "... in variable declaration"
}

@test "variable already defined :test/error/var_defined.gw.gw:" {
  gwion_basic_test test/error/var_defined.gw   "has already been defined in the same scope..."
}

@test "func pointer already defined :test/error/ptr_defined.gw:" {
  gwion_basic_test test/error/ptr_defined.gw   "has already been defined in the same scope"
}

@test "func pointer 0 size argument :test/error/ptr_arg_0_size.gw:" {
  gwion_basic_test test/error/ptr_arg_0_size.gw   "cannot declare variables of size '0'"
}

@test "func pointer reference primitive :test/error/ptr_ref_prim.gw:" {
  gwion_basic_test test/error/ptr_ref_prim.gw   "cannot declare references (@) of primitive type"
}

@test "function pointer argument array  :test/error/ptr_arg_array.gw:" {
  gwion_basic_test test/error/ptr_arg_array.gw   "must be defined with empty []'s"
}

@test "template inexistant :test/error/template_inexistant.gw:" {
  gwion_basic_test test/error/template_inexistant.gw   "template call of non-existant function"
}

@test "template call non template :test/error/template_non.gw:" {
  gwion_basic_test test/error/template_non.gw   "template call of non-template function"
}

@test "label already defined :test/error/label_defined.gw:" {
  gwion_basic_test test/error/label_defined.gw   "already defined"
}

@test "enum already declared :test/error/enum_declared.gw:" {
  gwion_basic_test test/error/enum_declared.gw   "already declared as variable"
}

@test "nested function :test/error/function_nested.gw:" {
skip "this fail at compiler level"
  gwion_basic_test test/error/function_nested.gw   "nested function definitions are not (yet) allowed;"
}

@test "function already used :test/error/function used.gw:" {
  gwion_basic_test test/error/function_used.gw   "is already used by another value"
}

@test "function primitive reference return type :test/error/function_ret_ref_prim.gw:" {
skip "this fail at compiler level"
  gwion_basic_test test/error/function_ret_ref_prim.gw   "cannot declare (@) of primitive type"
}

@test "function argument of no size :test/error/function_arg_no_size.gw:" {
skip "this migth be tested in scan1"
  gwion_basic_test test/error/function_arg_no_size.gw   "cannot declare variable of size"
}

@test "function argument reserved :test/error/function_arg_res.gw:" {
  gwion_basic_test test/error/function_arg_res.gw   "in function"
}

@test "function argument array empty :test/error/func_arg_array_empty.gw:" {
skip "this fail at compiler level"
  gwion_basic_test test/error/func_arg_array_empty.gw   "must be defined with empty []'s"
}

@test "function different signature :test/error/func_sig_differ.gw:" {
  gwion_basic_test test/error/func_sig_differ.gw   "function signatures differ in return type"
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
  gwion_basic_test "NOFILE" "no import function"
  rm /usr/lib/Gwion/plug/invalid.so
}

@test "incompatible array subscript :test/error/no_compat_sub.gw:" {
  gwion_basic_test test/error/no_compat_sub.gw   "must be of type 'int'"
}

@test "incompatible array subscript :test/error/no_compat_sub2.gw:" {
  gwion_basic_test test/error/no_compat_sub2.gw   "incompatible array subscript type"
}

@test "undefined parent class :test/error/undef_parent.gw:" {
  gwion_basic_test test/error/undef_parent.gw   "undefined parent class"
}

@test "undefined parent class :test/error/prim_parent.gw:" {
  gwion_basic_test test/error/prim_parent.gw   "cannot extend primitive"
}

@test "incomplete parent class :test/error/incomplete_parent.gw:" {
  gwion_basic_test test/error/incomplete_parent.gw   "cannot extend incomplete type"
}

@test "already define in  parent class :test/error/already_parent.gw:" {
  gwion_basic_test test/error/already_parent.gw   "has already been defined in parent class"
}

@test "declare in GACK  :test/error/gack_decl.gw:" {
  gwion_basic_test test/error/gack_decl.gw   "cannot use <<< >>> on variable declarations"
}

@test "declare in GACK  :test/error/gack_decl2.gw:" {
  gwion_basic_test test/error/gack_decl2.gw   "can't declare in GACK"
}

@test "static variable outside class :test/error/static_out.gw:" {
  gwion_basic_test test/error/static_out.gw   "static variables must be declared at class scope"
}

@test "provide array :test/error/provide_array.gw:" {
  gwion_basic_test test/error/provide_array.gw   "must provide values/expressions for array [...]"
}

@test "provide array :test/error/array_incompatible.gw:" {
  gwion_basic_test test/error/array_incompatible.gw   "array init [...] contains incompatible types..."
}

@test "vector too big :test/error/vector_too_big.gw:" {
  gwion_basic_test test/error/vector_too_big.gw   "vector dimensions not supported > 4..."
}

@test "invalid type in vector :test/error/vector_invalid.gw:" {
  gwion_basic_test test/error/vector_invalid.gw   "in vector value"
}

@test "this outside of class :test/error/this.gw:" {
  gwion_basic_test test/error/this.gw   "keyword 'this' can be used only inside class definition..."
}

@test "this in static function :test/error/this_static.gw:" {
  gwion_basic_test test/error/this_static.gw   "keyword 'this' cannot be used inside static functions..."
}

@test "complex too short :test/error/missing im.gw:" {
  gwion_basic_test test/error/missing_im.gw   "missing imaginary component of complex value..."
}

@test "complex too big :test/error/complex_too_big.gw:" {
  gwion_basic_test test/error/complex_too_big.gw   "extraneous component of complex value..."
}

@test "complex invalid real :test/error/complex_invalid_re.gw:" {
  gwion_basic_test test/error/complex_invalid_re.gw   "in real component of complex value"
}

@test "complex invalid real :test/error/complex_invalid_im.gw:" {
  gwion_basic_test test/error/complex_invalid_im.gw   "in imaginary component of complex value"
}

@test "polar too short :test/error/missing phase.gw:" {
  gwion_basic_test test/error/missing_phase.gw   "missing phase component of polar value..."
}

@test "polar too big :test/error/polar_too_big.gw:" {
  gwion_basic_test test/error/polar_too_big.gw   "extraneous component of polar value..."
}

@test "polar invalid modulus :test/error/polar_invalid_mod.gw:" {
  gwion_basic_test test/error/polar_invalid_mod.gw   "in modulus component of polar value..."
}

@test "polar invalid phase :test/error/polar_invalid_ph.gw:" {
  gwion_basic_test test/error/polar_invalid_ph.gw   "in phase component of polar value..."
}

@test "array exceed dimension :test/error/array_excess.gw:" {
  gwion_basic_test test/error/array_excess.gw   "exceeds defined dimension"
}

@test "array invalid type :test/error/array_invalid.gw:" {
  gwion_basic_test test/error/array_invalid.gw   "must be of type 'int'"
}

@test "assign function ptr :test/error/ptr_assign.gw:" {
  gwion_basic_test test/error/ptr_assign.gw   "can't assign function pointer to function pointer for the moment. sorry."
}

@test "assign function ptr :test/error/ptr_assign_class.gw:" {
  gwion_basic_test test/error/ptr_assign_class.gw   "can't assign function pointer in class for the moment. sorry."
}

@test "assign function ptr from other class :test/error/ptr_assign_other.gw:" {
  gwion_basic_test test/error/ptr_assign_other.gw   "can't assign member function to member function pointer of an other class"
}

@test "assign non member function ptr :test/error/ptr_assign_member.gw:" {
  gwion_basic_test test/error/ptr_assign_member.gw   "can't assign member function to non member function pointer"
}

@test "assign non member function ptr :test/error/ptr_assign_global.gw:" {
  gwion_basic_test test/error/ptr_assign_global.gw   "can't assign non member function to member function pointer"
}

@test "not match found for function :test/error/ptr_match.gw:" {
  gwion_basic_test test/error/ptr_match.gw   "not match found for function"
}

@test "not match found for operator :test/error/op_match.gw:" {
  gwion_basic_test test/error/op_match.gw   "not match found for operator"
}

@test "left-side is not mutable :test/error/left_mutable.gw:" {
  gwion_basic_test test/error/left_mutable.gw   "...(reason: --- left-side operand is not mutable)"
}

@test "right-side is not mutable :test/error/right_mutable.gw:" {
  gwion_basic_test test/error/right_mutable.gw   "...(reason: --- right-side operand is not mutable)"
}

@test "unknown type in cast expression :test/error/cast_unknown.gw:" {
  gwion_basic_test test/error/cast_unknown.gw   "in cast expression"
}

@test "postfix no match :test/error/postfix_no_match.gw:" {
  gwion_basic_test test/error/postfix_no_match.gw   "no suitable resolutation for postfix operator"
}

@test "no suitable resolutation for postfix operator :test/error/dur_prefix.gw:" {
  gwion_basic_test test/error/dur_prefix.gw   "in prefix of dur expression"
}

@test "no suitable resolutation for postfix operator :test/error/dur_postfix.gw:" {
  gwion_basic_test test/error/dur_postfix.gw   "in postfix of dur expression..."
}

@test "function call using a non-existing function :test/error/func_exist.gw:" {
  gwion_basic_test test/error/func_exist.gw   "function call using a non-existing function"
}

@test "function call using a non-existing function :test/error/func_non.gw:" {
  gwion_basic_test test/error/func_non.gw   "function call using a non-function value"
}

