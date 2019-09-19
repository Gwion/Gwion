class C {
  string name;
  int age;
}

C charles;
"charles" => charles.name;
21 => charles.age;

match charles {
  case >(name, age): <<< name , " is ", age, " years old." >>>;
}
