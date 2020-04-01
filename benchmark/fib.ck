class Fib {
  fun static int get(int n) {
    if (n < 2) return n;
    return get(n - 1) + get(n - 2);
  }
}
repeat(5)
  <<< Fib.get(28) >>>;
