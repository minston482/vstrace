#ifndef __PRINT_H__
#define __PRINT_H__

#include "common.h"
#include "data_structure.h"

/* for debugging functions */
void print_all_process_unit (struct process_header *ph);
void print_all_systemcall_unit (struct process_header *ph);
void print_all_stat_systemcall_unit (struct process_header *ph);
void print_all_file_unit (struct process_header *ph);
void print_all_unfinish_manager (struct head_unfinish_manager *hum);

#endif
