from __future__ import print_function

# Map "range" to an efficient range in both Python 2 and 3.
try:
    range = xrange
except NameError:
    pass

list = []
for i in range(0, 1000000):
  list.append(i)

sum = 0
for i in list:
  sum += i
print(sum)
