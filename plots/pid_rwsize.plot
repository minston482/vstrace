#!/usr/bin/gnuplot
dir_path = "result/".filename
file_path = dir_path.'/'.filename.".rwsize.pid"
set terminal wxt persist
set ylab "Amount of data (B)" font ", 25"
set xlab "Process ID" font ", 15"
set term postscript enhanced eps
set style fill pattern 
set size 2.0, 1.0
set output dir_path."/graphs/".filename.".rwsize.pid.eps"
plot file_path u 2:xticlabels(1) t "Read" w histograms lt 1, ""u 3:xticlabels(1) t "Write" w histograms lt 2
