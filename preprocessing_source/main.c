/*	
*	program name : strace viewer
*	what is it? pre-process the strace's result to make the graphs.
*	 
*	history : 2017. 04. 26 - create data structure
		  2017. 04. 27 - create pre-processing function
		  2017. 04. 28 - create reading type of input file
		  2017. 05. 01 - create open process in pre-processing function
		  2017. 05. 02 - create close process in pre-processing function
		  2017. 05. 03 - create read and write process in pre-processing function
		  			   - describe functions
		  2017. 05. 04 - create output file function (output_syscall(), output_syscall_timeline_pid(), output_syscall_cnt_pid(), output_syscall_rwsize_pid(), output_syscall_cnt_file(), output_syscall_rwsize_file())
*
*	last update : 2017. 05. 04 (Thur)
*	create by Minsong Jeong (https://github.com/minston482/vstrace, minseong482@gmail.com)
*/

#include "main.h"

/* global variable */
struct access_file_information afi[MAX_FILE_OPEN];
int afi_size = 0;

int main (int argc, char *argv[]) {
	char strcfn[MAX_FILE_NAME_LENGTH];
	FILE *fpstrc = NULL;
	struct process_header ph;

	if (argc < 2) {
		printf (" input error : the number of input is too low\n");
		printf (" Usage : [%s] [strace file name]\n", argv[0]);
		return 1;
	}

	strcpy (strcfn, argv[1]);	// copy strace file name from argv[1]

	fpstrc = fopen (strcfn, "r");

	preprocess_strace (fpstrc, &ph);

	/* print all */
	//print_all_process_unit (&ph);
	//print_all_systemcall_unit (&ph);
	//print_all_stat_systemcall_unit (&ph);
	//print_all_file_unit (&ph);

	fclose (fpstrc);
	set_access_file_information (&ph);
	output_syscall (&ph, strcfn, OUTPUT_SYSCALL_FOOTPRINT_PID);
	output_syscall (&ph, strcfn, OUTPUT_SYSCALL_CNT_PID);
	output_syscall (&ph, strcfn, OUTPUT_SYSCALL_RWSIZE_PID);
	output_syscall (&ph, strcfn, OUTPUT_SYSCALL_CNT_FILE);
	output_syscall (&ph, strcfn, OUTPUT_SYSCALL_RWSIZE_FILE);

	return 0;
}
