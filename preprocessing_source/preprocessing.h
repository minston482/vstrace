#ifndef __PREPROCESSING_H__
#define __PREPROCESSING_H__

#include "common.h"
#include "data_structure.h"

/* strace analysis functions */
int identy_systemcall_type (char *sn);
int preprocess_strace (FILE *fpstrc, struct process_header *ph);
struct process_unit * process_process_unit (struct process_header *ph, uint32_t pid);
struct systemcall_unit * process_systemcall_unit (struct process_unit *pu, char *sn, char *fn, double clock);
struct stat_systemcall * process_stat_systemcall_unit (struct process_header *ph, struct process_unit *pu, uint32_t pid, char *sn);
struct file_unit * process_file_unit (struct process_header *ph, struct process_unit *pu, uint32_t pid, char *fn);
struct process_unit * look_for_process_unit (struct process_header *ph, uint32_t pid);
struct stat_systemcall * look_for_stat_systemcall_unit (struct process_header *ph, uint32_t pid, char *sn);
struct file_unit * look_for_file_unit (struct process_header *ph, uint32_t pid, char *fn);
struct unfinish_manager * look_for_unfinish_manager (struct head_unfinish_manager *hum, uint32_t pid, char *sn);

#endif
