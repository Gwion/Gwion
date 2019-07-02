def fibonacci_recurs(n):
  if n < 2:
    return n;
  else:
    return (fibonacci_recurs(n-1) + fibonacci_recurs(n-2))

print(fibonacci_recurs(40))
