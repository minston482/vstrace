#include "common.h"
#include "data_structure.h"

/* initiate functions */
int init_systemcall_unit (struct systemcall_unit **su);
int init_stat_systemcall_unit (struct stat_systemcall **ssc);
int init_file_unit (struct file_unit **fu);
int init_process_unit (struct process_unit **pu);
int init_process_header (struct process_header *ph, struct process_unit *pu);
int init_unfinish_manager (struct unfinish_manager **um);
int init_head_unfinish_manager (struct head_unfinish_manager *hum);
int init_open_file_manager (struct open_file_manager *ofm, int size);

