#! /bin/bash

if [ $# -lt 1 ];then
	echo "[ERROR!] Usage: $0 strace_file"
	echo "[EXAMPLE] $0 tpcc.strc"
	exit
fi

STRACE_FILE_NAME=$1
PLOT_FILE_DIR="plots"

./preprocessing $STRACE_FILE_NAME

echo ""
echo "start pid read and write size plot"
gnuplot -e "filename='$STRACE_FILE_NAME'" $PLOT_FILE_DIR/pid_rwsize.plot
echo "end pid read and write size plot (success)"
echo ""

echo "start pid system call count plot"
gnuplot -e "filename='$STRACE_FILE_NAME'" $PLOT_FILE_DIR/pid_cnt.plot
echo "end pid system call count plot (success)"
echo ""

echo "start file read and write size plot"
gnuplot -e "filename='$STRACE_FILE_NAME'" $PLOT_FILE_DIR/file_rwsize.plot
echo "end file read and write size plot (success)"
echo ""

echo "start file system call count plot"
gnuplot -e "filename='$STRACE_FILE_NAME'" $PLOT_FILE_DIR/file_cnt.plot
echo "end file system call count plot (success)"
echo ""

for timelinefile in result/$STRACE_FILE_NAME/$STRACE_FILE_NAME.timeline.pid\#*;
do
	file_name=$(echo $timelinefile | tr "#" "\n")
	cnt=0
	for pid_nums in $file_name;
	do
		cnt=$(expr $cnt + 1)
		if [ $cnt -eq 2 ];
		then
			pid_num=$(echo $pid_nums | tr -d " ")
			echo "start pid $pid_num timeline plot"
			gnuplot -e "filename='$STRACE_FILE_NAME';pid_num='$pid_num'" $PLOT_FILE_DIR/pid_timeline.plot
			echo "end pid $pid_num timeline plot (success)"
			echo ""
		fi
	done
done
