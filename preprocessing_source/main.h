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

#ifndef __MAIN_H__
#define __MAIN_H__

#include "common.h"
#include "data_structure.h"
#include "init_values.h"
#include "memory_free.h"
#include "output_result.h"
#include "print_units.h"
#include "set_values.h"
#include "preprocessing.h"

int main (int argc, char *argv[]);

#endif
