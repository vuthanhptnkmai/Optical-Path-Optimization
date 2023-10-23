set terminal wxt size 1000,1000 enhanced font 'Verdana,10' persist
set view equal xyz
set xlabel 'x'
set ylabel 'y'
set zlabel 'z'
set autoscale x
set autoscale y
set autoscale z
set xyplane 0

set style line 1 linewidth 1.0 linecolor rgb "#4169E1"
set style line 2 pointtype 7 pointsize 1.0 linecolor rgb "#4169E1"
set style line 3 linewidth 2.0 linecolor rgb "#FF8C00"

splot 'ellipse_data.txt' using 1:2:3 with points 
pause -1
