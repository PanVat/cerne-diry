set term pngcairo size 1200,1200
set output "screen.png"

set xrange [0:400]
set yrange [0:400]

unset xtics
unset ytics
unset colorbox

set palette rgbformulae 34,35,36

plot "screen.dat" u 1:2:3 w image