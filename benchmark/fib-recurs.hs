fib :: Int -> Int
fib n
  | n < 2 = n
  | otherwise = (fib (n-2)) + (fib (n-1))

main = print $ fib 40
