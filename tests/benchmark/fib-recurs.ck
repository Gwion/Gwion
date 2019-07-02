//int count = 0;
fun int recursive_fib(int n)
{
//    ++count;
    if (n < 2)
        return n;
    else
        return recursive_fib(n - 2) + recursive_fib(n - 1);
}
<<<recursive_fib(40)>>>;
