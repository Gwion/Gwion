def fib(n)
  if n < 2 then
    n
  else
    fib(n - 1) + fib(n - 2)
  end
end

for i in 0...5
  puts fib(28)
end
