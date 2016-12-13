load test_helper/bats-support/load
load test_helper/bats-assert/load
load notify

function gbt() {
  run ./gwion -d dummy "$1"
  assert_segfault
  assert_success
  assert_output --partial "$2"
  set_icon
}

@test "non exitant file" {  gbt test/error/no_file.gw   "error while opening file"; }
@test "multiple public class" {  gbt test/error/class_multiple_public.gw   "more than one 'public' class defined"; }
@test "unknown type in declaration" {  gbt test/error/unknown_type.gw   "unknown in declaration"; }
@test "postfix on non-mutable data" {  gbt test/error/postfix.gw   "cannot be used on non-mutable data-type"; }
@test "already defined class" {  gbt test/error/defined_class.gw   "is already defined in namespace"; }
@test "class name reserved" {  gbt test/error/class_res.gw   "is reserved"; }
@test "template unlnow type" {  gbt test/error/template_unknown.gw   "unknown in template call"; }
@test "enum already defined" {  gbt test/error/enum_defined.gw   "already declared"; }
@test "enum defined" {  gbt test/error/enum_defined_var.gw   "already declared as variable"; }
@test "enum already defined as var" {  gbt test/error/enum_defined2.gw   "already declared"; }
@test "enum already defined as var" {  gbt test/error/enum_defined3.gw   "already declared"; }
@test "union unknown type" {  gbt test/error/union_unknown.gw   "unknown type"; }
@test "union array must be empty" {  gbt test/error/union_array_empty.gw   "array declaration must be empty in union"; }
@test "function pointer unknown ret type" {  gbt test/error/pointer_unknown.gw   "unknown type"; }
@test "function pointer static outside class" {  gbt test/error/pointer_outside_class.gw   "can't declare func pointer static outside of a class"; }
@test "function pointer unknown argument type" {  gbt test/error/pointer_unknown_arg.gw   "unknown type in argument"; }
@test "destructor outside class" {  gbt test/error/dtor_outside_class.gw   "dtor must be in class def"; }
@test "function unknown ret type" {  gbt test/error/func_unknown_ret.gw   "unknown return type"; }
@test "function arg unknown" {  gbt test/error/func_arg_unknown.gw   "unknown type in argument"; }
@test "operator argument number" {  gbt test/error/operator_narg.gw   "operators can only have one or two arguments"; }
@test "operator valid" {  gbt test/error/operator_valid.gw   "is not a valid operator name"; }

###########
#  Scan 2 #
###########
@test "variable of size zero" {  gbt test/error/0_size_variable.gw   "cannot declare variables of size '0'"; }
@test "class declared inside itself" {  gbt test/error/class_inside.gw   "declared inside itself"; }
@test "primitive reference" {  gbt test/error/prim_ref.gw   "cannot declare references (@) of primitive type"; }
@test "decl reserved" {  gbt test/error/decl_res.gw   "... in variable declaration"; }
@test "variable already defined" {  gbt test/error/var_defined.gw   "has already been defined in the same scope..."; }
@test "func pointer already defined" {  gbt test/error/ptr_defined.gw   "has already been defined in the same scope"; }
@test "func pointer 0 size argument" {  gbt test/error/ptr_arg_0_size.gw   "cannot declare variables of size '0'"; }
@test "func pointer reference primitive" {  gbt test/error/ptr_ref_prim.gw   "cannot declare references (@) of primitive type"; }
@test "function pointer argument array " {  gbt test/error/ptr_arg_array.gw   "must be defined with empty []'s"; }
@test "template inexistant" {  gbt test/error/template_inexistant.gw   "template call of non-existant function"; }
@test "template call non template" {  gbt test/error/template_non.gw   "template call of non-template function"; }
@test "label already defined" {  gbt test/error/label_defined.gw   "already defined"; }
@test "enum already declared" {  gbt test/error/enum_declared.gw   "already declared as variable"; }
@test "nested function" {skip "this fail at compiler level"  gbt test/error/function_nested.gw   "nested function definitions are not (yet) allowed;"; }
@test "function already used" {  gbt test/error/function_used.gw   "is already used by another value"; }
@test "function primitive reference return type" {skip "this fail at compiler level"  gbt test/error/function_ret_ref_prim.gw   "cannot declare (@) of primitive type"; }
@test "function argument of no size" { gbt test/error/function_arg_no_size.gw   "cannot declare variable of size"; }
@test "function argument reserved" {  gbt test/error/function_arg_res.gw   "in function"; }
@test "function argument array empty" { gbt test/error/func_arg_array_empty.gw   "must be defined with empty []'s"; }
@test "function different signature" {  gbt test/error/func_sig_differ.gw   "function signatures differ in return type"; }

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
  gbt "NOFILE" "no import function"
  rm /usr/lib/Gwion/plug/invalid.so
}

@test "incompatible array subscript" {  gbt test/error/no_compat_sub.gw   "must be of type 'int'"; }
@test "incompatible array subscript" {  gbt test/error/no_compat_sub2.gw   "incompatible array subscript type"; }
@test "undefined parent class" {  gbt test/error/undef_parent.gw   "undefined parent class"; }
@test "undefined parent class" {  gbt test/error/prim_parent.gw   "cannot extend primitive"; }
@test "incomplete parent class" {  gbt test/error/incomplete_parent.gw   "cannot extend incomplete type"; }
@test "already define in  parent class" {  gbt test/error/already_parent.gw   "has already been defined in parent class"; }
@test "declare in GACK " {  gbt test/error/gack_decl.gw   "cannot use <<< >>> on variable declarations"; }
@test "declare in GACK " {  gbt test/error/gack_decl2.gw   "can't declare in GACK"; }
@test "static variable outside class" {  gbt test/error/static_out.gw   "static variables must be declared at class scope"; }
@test "provide array" {  gbt test/error/provide_array.gw   "must provide values/expressions for array [...]"; }
@test "provide array" {  gbt test/error/array_incompatible.gw   "array init [...] contains incompatible types..."; }
@test "vector too big" {  gbt test/error/vector_too_big.gw   "vector dimensions not supported > 4..."; }
@test "invalid type in vector" {  gbt test/error/vector_invalid.gw   "in vector value"; }
@test "this outside of class" {  gbt test/error/this.gw   "keyword 'this' can be used only inside class definition..."; }
@test "this in static function" {  gbt test/error/this_static.gw   "keyword 'this' cannot be used inside static functions..."; }
@test "complex too short" {  gbt test/error/missing_im.gw   "missing imaginary component of complex value..."; }
@test "complex too big" {  gbt test/error/complex_too_big.gw   "extraneous component of complex value..."; }
@test "complex invalid real" {  gbt test/error/complex_invalid_re.gw   "in real component of complex value"; }
@test "complex invalid real" {  gbt test/error/complex_invalid_im.gw   "in imaginary component of complex value"; }
@test "polar too short" {  gbt test/error/missing_phase.gw   "missing phase component of polar value..."; }
@test "polar too big" {  gbt test/error/polar_too_big.gw   "extraneous component of polar value..."; }
@test "polar invalid modulus" {  gbt test/error/polar_invalid_mod.gw   "in modulus component of polar value..."; }
@test "polar invalid phase" {  gbt test/error/polar_invalid_ph.gw   "in phase component of polar value..."; }
@test "array exceed dimension" {  gbt test/error/array_excess.gw   "exceeds defined dimension"; }
@test "array invalid type" {  gbt test/error/array_invalid.gw   "must be of type 'int'"; }
@test "assign function ptr" {  gbt test/error/ptr_assign.gw   "can't assign function pointer to function pointer for the moment. sorry."; }
@test "assign function ptr" {  gbt test/error/ptr_assign_class.gw   "can't assign function pointer in class for the moment. sorry."; }
@test "assign function ptr from other class" {  gbt test/error/ptr_assign_other.gw   "can't assign member function to member function pointer of an other class"; }
@test "assign non member function ptr" {  gbt test/error/ptr_assign_member.gw   "can't assign member function to non member function pointer"; }
@test "assign non member function ptr" {  gbt test/error/ptr_assign_global.gw   "can't assign non member function to member function pointer"; }
@test "not match found for function" {  gbt test/error/ptr_match.gw   "not match found for function"; }
@test "not match found for operator" {  gbt test/error/op_match.gw   "not match found for operator"; }
@test "left-side is not mutable" { gbt test/error/left_mutable.gw "left-side operand is not mutable"; }
@test "right-side is not mutable" { gbt test/error/right_mutable.gw "right-side operand is not mutable"; }
@test "unknown type in cast expression" {  gbt test/error/cast_unknown.gw   "in cast expression"; }
@test "postfix no match" {  gbt test/error/postfix_no_match.gw   "no suitable resolutation for postfix operator"; }
@test "no suitable resolutation for postfix operator" {  gbt test/error/dur_prefix.gw   "in prefix of dur expression"; }
@test "no suitable resolutation for postfix operator" {  gbt test/error/dur_postfix.gw   "in postfix of dur expression..."; }
@test "non-existing function" {  gbt test/error/func_exist.gw   "function call using a non-existing function"; }
@test "non function" {  gbt test/error/func_non.gw   "function call using a non-function value"; }
