#ifndef __COMMON_H__
#define __COMMON_H__

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define	MAX_FILE_NAME_LENGTH	1024
#define MAX_SYSTEM_CALL_NAME_LENGTH	32
#define MAX_SYSTEM_CALL_RETVAL	128
#define MAX_FILE_OPEN	100
#define SYSCALL_TYPE_SIZE	4	// open, read, write, close
#define SYSCALL_TYPE_OTHERS	0
#define SYSCALL_TYPE_OPEN	-1
#define SYSCALL_TYPE_READ	-2
#define SYSCALL_TYPE_WRITE	-3
#define SYSCALL_TYPE_CLOSE	-4
#define SYSCALL_TYPE_READ_OR_WRITE	-5
#define TRUE	1
#define FALSE	0
#define STD_FD_0_INPUT	0
#define STD_FD_1_OUTPUT	1
#define STD_FD_2_ERROR	2
#define OUTPUT_SYSCALL_FOOTPRINT_PID	-1
#define OUTPUT_SYSCALL_CNT_PID	-2
#define OUTPUT_SYSCALL_RWSIZE_PID	-3
#define OUTPUT_SYSCALL_CNT_FILE	-4
#define OUTPUT_SYSCALL_RWSIZE_FILE	-5


/* malloc functions */
void* malloc_unit (size_t size);

#endif
