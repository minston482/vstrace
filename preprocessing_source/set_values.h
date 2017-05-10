#ifndef __SET_VALUES_H__
#define __SET_VALUES_H__

#include "common.h"
#include "data_structure.h"

/* set the value functions */
int set_open_file_manager (struct open_file_manager *ofm, int fd, struct file_unit *fu);
int set_access_file_information (struct process_header *ph);
int set_close_file_manager (struct open_file_manager *ofm, int fd);

#endif
