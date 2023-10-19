
this_is_a_function = () => {
      ...
};
this_is_also_a_function = function() {
      ...
};
function also_a_function() {

}

let local_variable = 3;
if (local_variable == 3) { // if clause
      print("%v should be 3", local_variable); // local_variable should be 3
}
if local_variable == 3 { // also an if clause
      var kind_of_local_variable = 5;
}
print(kind_of_local_variable); // 5

// print all numbers from 0 to 41
for (i in 42) {
      print(i);
}

// print all numbers from 7 to 41
for i in 7..42 {
      print(i);
}

// print all numbers from 41 to 0
for i in 41..0
      print(i);

function call_some_function(arg1, callback, arg2 = -3, arg3 = "hello world", special_argument=1.41)
{
      ...
}

let arg_01 = 42;
let spec_arg = 1.61;
call_some_function(arg_01, () => {
      ... some callback ...
}, special_argument=spec_arg);

function call_some_other_function(arg1, arg2, arg3)
{
      ...
}
call_some_other_function(arg2=3, arg1=5, arg3=1);

function special_form_of_calling(arg1)
{
      ...
}

let a = 3;
a.special_form_of_calling(); // equal to special_form_of_calling(a);

const constant_variable = 4;

// specify types
let integer = 4: int;
let floating_point = 3.1415: float;
floating_point = "string"; // error

struct example_struct {
      member_01: double,
      member_02
}

let example_struct_instance = { 0.4252, "some string" }: example_struct;
let example_struct_instance_02 = { 0.4252, 42 }: example_struct;

