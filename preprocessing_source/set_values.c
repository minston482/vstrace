#include "set_values.h"

extern struct access_file_information afi[MAX_FILE_OPEN];
extern int afi_size;

/* set values functions */
int set_open_file_manager (struct open_file_manager *ofm, int fd, struct file_unit *fu) {
	int retval = 0;
	
	ofm[fd].fd = fd;
	ofm[fd].fu = fu;

	return retval;
}

int set_access_file_information (struct process_header *ph) {
	int retval = 0, i = 0;
	struct process_unit *cur_pu = ph->pu;
	struct file_unit *cur_fu = NULL;

	while (cur_pu != NULL) {
		cur_fu = cur_pu->fu;
		while (cur_fu != NULL) {
			for (i = 0; i < afi_size; i++) {
				if (strcmp (afi[i].fn, cur_fu->fn) == 0) {
					afi[i].acnt += cur_fu->acnt;
					afi[i].rcnt += cur_fu->rcnt;
					afi[i].wcnt += cur_fu->wcnt;
					afi[i].rsize += cur_fu->rsize;
					afi[i].wsize += cur_fu->wsize;
					break;
				}
			}

			if (i == afi_size) {
				afi_size++;
				strcpy (afi[i].fn, cur_fu->fn);
				afi[i].acnt = cur_fu->acnt;
				afi[i].rcnt = cur_fu->rcnt;
				afi[i].wcnt = cur_fu->wcnt;
				afi[i].rsize = cur_fu->rsize;
				afi[i].wsize = cur_fu->wsize;
			}

			cur_fu = cur_fu->next;
		}
		cur_pu = cur_pu->next;
	}
	
	return retval;
}

int set_close_file_manager (struct open_file_manager *ofm, int fd) {
	int retval = 0;
	
	ofm[fd].fd = -1;
	ofm[fd].fu = NULL;
	
	return retval;
}
