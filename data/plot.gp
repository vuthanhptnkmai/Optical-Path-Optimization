set terminal wxt size 1000,1000 enhanced font 'Verdana,10' persist
set view equal xyz
set xlabel 'x'
set ylabel 'y'
set zlabel 'z'
set xrange [0:15]
set yrange [0:15]
set zrange [0:15]
set autoscale x
set autoscale y
set autoscale z
set grid xtics mxtics
set mxtics 3
set grid ytics mytics
set mytics 3
set grid ztics mztics
set mztics 3
set grid
set xyplane 0
set style line 1 pointtype 7 pointsize 0.8
splot 'rays.dat' using 1:2:3:4:5:6 with vectors head size 0,0 filled lc rgb 'red'
pause -1
