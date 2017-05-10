#include "init_values.h"

/* memory allocate and init systemcall unit values */
int init_systemcall_unit (struct systemcall_unit **su) {
	int retval = 0;
	
	/* init values */
	*su = (struct systemcall_unit *) malloc_unit (sizeof(struct systemcall_unit));
	if ((*su) == NULL) {
		retval = 1;
		return retval;
	}

	strcpy ((*su)->sn, "");
	(*su)->fn = NULL;
	(*su)->at = 0.0;
	(*su)->prev = NULL;
	(*su)->next = NULL;
	
	return retval;
}

/* memory allocate and init stat systemcall unit values */
int init_stat_systemcall_unit (struct stat_systemcall **ssc) {
	int retval = 0;
	
	/* init values */
	*ssc = (struct stat_systemcall *) malloc_unit (sizeof(struct stat_systemcall));
	if ((*ssc) == NULL) {
		retval = 1;
		return retval;
	}

	strcpy ((*ssc)->sn, "");
	(*ssc)->cnt = 0;
	(*ssc)->prev = NULL;
	(*ssc)->next = NULL;
	
	return retval;
}

/* memory allocate and init file unit values */
int init_file_unit (struct file_unit **fu) {
	int retval = 0;
	
	/* init values */
	*fu = (struct file_unit *) malloc_unit (sizeof(struct file_unit));
	if ((*fu) == NULL) {
		retval = 1;
		return retval;
	}

	(*fu)->fn = NULL;
	(*fu)->acnt = 0;
	(*fu)->rcnt = 0;
	(*fu)->wcnt = 0;
	(*fu)->rsize = 0;
	(*fu)->wsize = 0;
	(*fu)->prev = NULL;
	(*fu)->next = NULL;
	
	return retval;
}

/* memory allocate and init process unit values */
int init_process_unit (struct process_unit **pu) {
	int retval = 0, i = 0;
	
	/* init values */
	*pu = (struct process_unit *) malloc_unit (sizeof(struct process_unit));
	if ((*pu) == NULL) {
		retval = 1;
		return retval;
	}

	init_open_file_manager ((*pu)->ofm, MAX_FILE_OPEN);	

	(*pu)->pid = 0;
	(*pu)->su = NULL;
	(*pu)->ssc = NULL;
	(*pu)->fu = NULL;
	(*pu)->next = NULL;
	(*pu)->prev = NULL;

	return retval;
}

/* init process header */
int init_process_header (struct process_header *ph, struct process_unit *pu) {
	int retval = 0;

	ph->pu = pu;

	return retval;
}

int init_unfinish_manager (struct unfinish_manager **um) {
	int retval = 0;

	/* init values */
	*um = (struct unfinish_manager *) malloc_unit (sizeof(struct unfinish_manager));
	if ((*um) == NULL) {
		retval = 1;
		return retval;
	}

	(*um)->pid = 0;
	strcpy ((*um)->sn, "");
	strcpy ((*um)->fn, "");
	(*um)->fd = -1;
	(*um)->next = NULL;
	(*um)->prev = NULL;

	return retval;
}

int init_head_unfinish_manager (struct head_unfinish_manager *hum) {
	int retval = 0;
	struct unfinish_manager *head_node = NULL;

	init_unfinish_manager (&head_node);
	strcpy (head_node->sn, "start_node");

	hum->front = head_node;
	hum->tail = head_node;

	return retval;
}

int init_open_file_manager (struct open_file_manager *ofm, int size) {
	int retval = 0, i = 0;

	for (i = 0; i < size; i++) {
		ofm[i].fd = -1;
		ofm[i].fu = NULL;
	}

	return retval;
}
