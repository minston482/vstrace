#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include "common.h"
#include "data_structure.h"

/* output file functions */
int output_syscall (struct process_header *ph, char *infn, int select);
int output_syscall_timeline_pid (struct process_header *ph, char *res_path, char *infn);
int output_syscall_cnt_pid (struct process_header *ph, char *res_path, char *infn);
int output_syscall_rwsize_pid (struct process_header *ph, char *res_path, char *infn);
int output_syscall_cnt_file (struct process_header *ph, char *res_path, char *infn);
int output_syscall_rwsize_file (struct process_header *ph, char *res_path, char *infn);

#endif
