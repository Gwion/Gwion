from __future__ import print_function

def fib(n):
  if n < 2: return n
  return fib(n - 1) + fib(n - 2)

for i in range(0, 5):
  print(fib(28))
