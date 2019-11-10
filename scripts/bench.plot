set terminal png truecolor

#if (!exists("bench"))
#  bench = 'bench'
if (!exists("test_dir"))
  test_dir = 'tests/benchmark'

dat_name = sprintf("benchmark/results/%s.dat", bench)

stats dat_name using 0:2 noout
max = STATS_max_y+(0.1*STATS_max_y)

set title bench
set output sprintf("benchmark/results/%s.png", bench)
set xrange [-0.5:((ceil(STATS_max_x))+0.5)]
set yrange [0:max]
set boxwidth 0.50
set nokey
set xtics nomirror
set ytics nomirror

set style fill transparent solid 0.25 # partial transparency
set style fill noborder # no separate top/bottom lines

plot dat_name using 0:2:($2*($3/100.0)):xtic(2) with boxerrorbar lc "blue" notitle, \
  '' using 0:(max-(0.05*max)):1 with labels
