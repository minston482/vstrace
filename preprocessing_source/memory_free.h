#ifndef __FREE_UNIT_H__
#define __FREE_UNIT_H__

#include "common.h"
#include "data_structure.h"

/* memory free functions */
int free_systemcall_unit (struct systemcall_unit *su, struct process_unit *pu);
int free_stat_systemcall_unit (struct stat_systemcall *ssc, struct process_unit *pu);
int free_file_unit (struct file_unit *fu, struct process_unit *pu);
int free_process_unit (struct process_unit *pu, struct process_header *ph);
int free_process_header (struct process_header *ph);
int free_unfinish_manager (struct head_unfinish_manager *hum, struct unfinish_manager *um);

#endif
