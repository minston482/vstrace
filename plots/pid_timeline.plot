#!/usr/bin/gnuplot
dir_path = "result/".filename
file_path = dir_path.'/'.filename.".timeline.pid#".pid_num
set terminal wxt persist
set xlab "Time (ns)" font ", 15"
set yrange[0:5]
set ytics("open"1, "read"2, "write"3, "close"4) font ", 15"
set term postscript enhanced eps
set style fill solid
set size 2.0, 1.0
set output dir_path."/graphs/".filename.".timeline.pid#".pid_num.".eps"
plot file_path u 1:(stringcolumn(2) eq "open"? 1:1/0) t "" lt rgb 'black', "" u 1:(stringcolumn(2) eq "read"? 2:1/0) t "" lt rgb 'black', "" u 1:(stringcolumn(2) eq "write"? 3:1/0) t "" lt rgb 'black', "" u 1:(stringcolumn(2) eq "close"? 4:1/0) t "" lt rgb 'black'
