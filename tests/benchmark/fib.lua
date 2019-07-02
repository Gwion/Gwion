function fib(n)
  if n < 2 then return n end
  return fib(n - 2) + fib(n - 1)
end

for i = 1, 5 do
  io.write(fib(28) .. "\n")
end
