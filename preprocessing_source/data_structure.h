#ifndef __DATA_STRUCTURE_H__
#define __DATA_STRUCTURE_H__

#include "common.h"

/* system call unit */
struct systemcall_unit {
	char sn[MAX_SYSTEM_CALL_NAME_LENGTH];	// system call name
	char *fn;	// file name

	double at;	// access time

	struct systemcall_unit *next;	// using data structure for management (current: double list)
	struct systemcall_unit *prev;   // using data structure for management (current: double list)
};

/* statistics of system call */
struct stat_systemcall {
	char sn[MAX_SYSTEM_CALL_NAME_LENGTH];	// system call name
	uint32_t cnt;	//	system call count

	struct stat_systemcall *next;	// using data structure for management (current: double list)
	struct stat_systemcall *prev;   // using data structure for management (current: double list)
};

/* file unit */
struct file_unit {
	char *fn;	// file name;

	uint32_t acnt;	// access count (all of access cf. open, access, read, write etc..)
	uint32_t rcnt;	// read system call count
	uint32_t wcnt;	// write system call count
	uint64_t rsize;	// amount of read
	uint64_t wsize;	// amount of write

	struct file_unit *next;	// using data structure for management (current: double list)
	struct file_unit *prev; // using data structure for management (current: double list)
};

/* open file manager */
struct open_file_manager {
	int fd;	// file descriptor
	struct file_unit *fu;
};

/* file_manager */
struct access_file_information {
	char fn[MAX_FILE_NAME_LENGTH];
	uint32_t acnt;
	uint32_t rcnt;
	uint32_t wcnt;
	uint64_t rsize;
	uint64_t wsize;
};

/* process unit */
struct process_unit {
	uint32_t pid;	// process id
	struct open_file_manager ofm[MAX_FILE_OPEN];

	struct systemcall_unit *su;	// system calls
	struct systemcall_unit *su_tail;	// system calls
	struct stat_systemcall *ssc;	// statistics of system calls
	struct file_unit *fu;	// access files

	struct process_unit *next;	// using data structure for management (current: double list)
	struct process_unit *prev;      // using data structure for management (current: double list)
};

/* process header */
struct process_header {
	struct process_unit *pu;	// process unit
};

/* unfinished line manager */
struct unfinish_manager {
	uint32_t pid;	// process id
	char sn[MAX_SYSTEM_CALL_NAME_LENGTH];	// system call name
	char fn[MAX_FILE_NAME_LENGTH];  // file name
	int fd;	// file descriptor

	struct unfinish_manager *next;
	struct unfinish_manager *prev;
};

/* head of unfinish manager */
struct head_unfinish_manager {
	struct unfinish_manager *front;
	struct unfinish_manager *tail;
};


#endif
