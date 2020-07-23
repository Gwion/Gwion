int count;
for (int i; i < 1000000; ++i) {
  if ("abc" == "abc") ++count;
  if ("a slightly longer string" ==
      "a slightly longer string") ++count;
  if ("a significantly longer string but still not overwhelmingly long string" ==
      "a significantly longer string but still not overwhelmingly long string") ++count;

  if ("" == "abc") ++count;
  if ("abc" == "abcd") ++count;
  if ("changed one character" == "changed !ne character") ++count;
  if ("a slightly longer string" ==
      "a slightly longer string!") ++count;
  if ("a slightly longer string" ==
      "a slightly longer strinh") ++count;
  if ("a significantly longer string but still not overwhelmingly long string" ==
      "another") ++count;
}

<<< count >>>;
