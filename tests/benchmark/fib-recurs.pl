sub fibonacci_recurs {
   my ($number) = @_;
   if ($number < 2) { # base case
      return $number;
   }
   return fibonacci_recurs($number-1) + fibonacci_recurs($number-2);
}

print fibonacci_recurs(40) , "\n";
