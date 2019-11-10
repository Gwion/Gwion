def fibonacci_recurs( n )
  if n < 2 then return n end
  return ( fibonacci_recurs( n - 1 ) + fibonacci_recurs( n - 2 ) )
end
puts fibonacci_recurs(40)
