main() {
  var list = [];

  for (var i = 0; i < 1000000; i++) list.add(i);

  var sum = 0;
  for (i in list) sum += i;

  print(sum);
}
