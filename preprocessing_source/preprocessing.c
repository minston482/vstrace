#include "preprocessing.h"

int identy_systemcall_type (char *sn) {
	int retval = SYSCALL_TYPE_OTHERS;

	if (strcmp (sn, "open") == 0)
		retval = SYSCALL_TYPE_OPEN;
	else if (strcmp (sn, "close") == 0)
		retval = SYSCALL_TYPE_CLOSE;
	else if (strcmp (sn, "write") == 0)
		retval = SYSCALL_TYPE_WRITE;
	else if (strcmp (sn, "read") == 0)
		retval = SYSCALL_TYPE_READ;

	return retval;
}

int preprocess_strace (FILE *fpstrc, struct process_header *ph) {
	int fd = 0, retval = 0, isresume = FALSE, isunfinish = FALSE;
	double base_clock = 0.0, clock = 0.0;
	uint32_t pid = 0;
	char sn[MAX_SYSTEM_CALL_NAME_LENGTH], strcretval[MAX_SYSTEM_CALL_RETVAL], fn[(MAX_FILE_NAME_LENGTH)*2], *pfn = NULL, etc_sn[MAX_FILE_NAME_LENGTH], after_equal[MAX_SYSTEM_CALL_RETVAL];
	uint64_t rsize = 0, wsize = 0; // amount of read, write
	char sepresume, seppoint[6], sepunfinish[(MAX_FILE_NAME_LENGTH)*2];
	struct process_unit *pu = NULL;
	struct file_unit *fu = NULL;
	struct stat_systemcall *ssc = NULL;
	struct systemcall_unit *su = NULL;
	struct unfinish_manager *um = NULL;
	struct head_unfinish_manager hum = {NULL, NULL};

	/* create and init headers */
	init_process_unit (&pu);
	init_process_header (ph, pu);
	init_head_unfinish_manager (&hum);

	/* read first line in strace file */
	fscanf (fpstrc, "%d %lf %[^(] %[^=] %[^\n]\n", &pid, &clock, sn, fn, strcretval);
	base_clock = clock;	// set base_clock
	pu->pid = pid;

	while (1) {
		if (feof (fpstrc))
			break;
		
		/* read file */
		fscanf (fpstrc, "%d %lf ", &pid, &clock);
		clock -= base_clock;
		fscanf (fpstrc, "%c", &sepresume);
		fseek (fpstrc, -1, SEEK_CUR);
		
		if (sepresume == '-' || sepresume == '+' || sepresume == '_') { // this line is SIGALRM, exit
			fscanf (fpstrc, "%[^\n]\n", etc_sn);
			continue;
		}

		if (sepresume == '<') {	// this line is resume
			isresume = TRUE;

			/* read file */
			fscanf (fpstrc, "%s %s", seppoint, sn);
			fscanf (fpstrc, "%[^\n]\n", sepunfinish);

			if (identy_systemcall_type (sn) == SYSCALL_TYPE_OTHERS)	// not necessary value is skip
				continue;

			/* unfinished line */
			if (sepunfinish[strlen(sepunfinish)-1] == '>') 
				isunfinish = TRUE;
			else {	// ordinary routine
				isunfinish = FALSE;

				fseek (fpstrc, -(strlen(sepunfinish)+1), SEEK_CUR);
				fscanf (fpstrc, "%[^=] %[^\n]\n", fn, strcretval);
			}
		} else {	// systemcall
			isresume = FALSE;

			fscanf (fpstrc, "%[^(]", sn);
			fscanf (fpstrc, "%[^\n]\n", sepunfinish);

			if (identy_systemcall_type (sn) == SYSCALL_TYPE_OTHERS)	// not necessary value is skip
				continue;

			/* unfinished line */
			if (sepunfinish[strlen(sepunfinish)-1] == '>') 
				isunfinish = TRUE;
			else {	// ordinary routine
				isunfinish = FALSE;

				fseek (fpstrc, -(strlen(sepunfinish)+1), SEEK_CUR);
				fscanf (fpstrc, "%[^=] %[^\n]\n", fn, strcretval);		
			}
		}
		/* read file (end) */

		/* process unfinish line */
		if (isunfinish == TRUE) {
			/* add the node into unfinish list */			
			init_unfinish_manager (&um);
			um->pid = pid;
			strcpy (um->sn, sn);
			if (identy_systemcall_type (sn) == SYSCALL_TYPE_OPEN) {
				strcpy (sepunfinish, sepunfinish+2);	// reposition path pos
				pfn = strtok (sepunfinish, "\"");	// path identify
				strcpy (um->fn, pfn);
			} else if (identy_systemcall_type (sn) == SYSCALL_TYPE_READ || identy_systemcall_type (sn) == SYSCALL_TYPE_WRITE) { // the read or write line
				strcpy (sepunfinish, sepunfinish+1);	// reposition fd pos
				pfn = strtok (sepunfinish, ",");	// fd identify
				um->fd = atoi (pfn);
			} else if (identy_systemcall_type (sn) == SYSCALL_TYPE_CLOSE) {
				strcpy (sepunfinish, sepunfinish+1);	// reposition fd pos
				pfn = strtok (sepunfinish, " ");	// fd identify
				um->fd = atoi (pfn);
			}

			if (hum.front == hum.tail) // unfinish list is empty 
				hum.front->next = um;
			else // unfinish list is not empty 
				hum.tail->next = um;
			
			um->prev = hum.tail;
			hum.tail = um;
			continue;
		}
		
		if (identy_systemcall_type (sn) == SYSCALL_TYPE_OTHERS)
			continue;

		/* skip open error line */
		strcpy (after_equal, strcretval);
		if (after_equal[2] == '-')
			continue;
		strcpy (after_equal, after_equal+2);

		/* process resume */
		if (isresume == TRUE) {
			um = look_for_unfinish_manager (&hum, pid, sn);	// find unfinish unit by pid, sn

			/* for debug*/
			if (um == NULL) {
				printf("clock:%lf, pid:%d, sn:%s, um == NULL\n", clock, pid, sn);
				print_all_unfinish_manager (&hum);
			}
			strcpy (fn, um->fn);
			pfn = fn;
			fd = um->fd;
			free_unfinish_manager (&hum, um);	// delete unfinish unit
		} else if (identy_systemcall_type (sn) == SYSCALL_TYPE_OPEN) {
			strcpy (fn, fn+2);
			pfn = strtok (fn, "\"");
			fd = atoi (after_equal);	// after_equal is fd
		} else if (identy_systemcall_type (sn) == SYSCALL_TYPE_READ || identy_systemcall_type (sn) == SYSCALL_TYPE_WRITE) { // the read or write line
			strcpy (fn, fn+1);
			pfn = strtok (fn, ",");
			fd = atoi (pfn);	// pfn is fd
		} else if (identy_systemcall_type (sn) == SYSCALL_TYPE_CLOSE) {
			strcpy (fn, fn+1);
			pfn = strtok (fn, ")");
			fd = atoi (pfn);	// pfn is fd
		}
				
		/* set values of process_unit */
		pu = process_process_unit (ph, pid);
		/* set values of process_unit (end) */

		/* set values of stat_systemcall */
		ssc = process_stat_systemcall_unit (ph, pu, pid, sn);
		/* set values of stat_systemcall (end) */

		/* pre-process each system call */
		if (identy_systemcall_type (sn) == SYSCALL_TYPE_OPEN) {

			/* set values of systemcall_unit */
			su = process_systemcall_unit (pu, sn, pfn, clock);
			/* set values of systemcall_unit (end) */

			/* set values of file_unit */
			fu = process_file_unit (ph, pu, pid, pfn);
			/* set values of file_unit (end) */

			/* set open file manager */
			fd = atoi (after_equal);
			set_open_file_manager (pu->ofm, fd, fu);
			/* set open file manager (end) */

		} else if (identy_systemcall_type (sn) == SYSCALL_TYPE_CLOSE) {	// the close line

			/* set values of systemcall_unit */
			fu = pu->ofm[fd].fu;
			if (fu != NULL) 
				su = process_systemcall_unit (pu, sn, fu->fn, clock);
			/* set values of systemcall_unit (end) */

			/* set values of file_unit */
			if (fu != NULL)
				fu = process_file_unit (ph, pu, pid, fu->fn);
			/* set values of file_unit (end) */

			set_close_file_manager (pu->ofm, fd);	

		} else if (identy_systemcall_type (sn) == SYSCALL_TYPE_READ || identy_systemcall_type (sn) == SYSCALL_TYPE_WRITE) { // the read or write line

			/* set values of file_unit and open_file_manager */
			if (pu->ofm[fd].fu == NULL) {
				/* set values of file_unit */
				init_file_unit (&fu);
				fu->fn = (char *) malloc (sizeof (char) * (strlen ("existing file fd:")+5));
				sprintf (fu->fn, "existing_file_fd:%d", fd);
//				sprintf (fu->fn, "unknown");

				if (pu->fu != NULL) {
					fu->next = pu->fu;
					pu->fu->prev = fu;
				}
				pu->fu = fu;

				set_open_file_manager (pu->ofm, fd, fu);

				pu->ofm[fd].fd = fd;
				pu->ofm[fd].fu = fu;
			}
			/* set values of file_unit and open_file_manager (end) */
				
			fu = pu->ofm[fd].fu;

			/* set values of systemcall_unit */
			su = process_systemcall_unit (pu, sn, fu->fn, clock);
			/* set values of systemcall_unit (end) */

			if (identy_systemcall_type (sn) == SYSCALL_TYPE_READ) {	// read
				fu->acnt++;
				fu->rcnt++;
				fu->rsize += atoi (after_equal);
			} else if (identy_systemcall_type (sn) == SYSCALL_TYPE_WRITE) {	// write line
				fu->acnt++;
				fu->wcnt++;
				fu->wsize += atoi (after_equal);
			} else {	// error..
				printf(" system call naming error\n");
			}
			
		}
		/* pre-process each system call (end) */

	} // end of file read (while)

	return retval;
}

/* process funtions */
struct process_unit * process_process_unit (struct process_header *ph, uint32_t pid) {
	struct process_unit *pu = NULL;

	pu = look_for_process_unit (ph, pid);
	if (pu == NULL) {
		init_process_unit (&pu);
		pu->pid = pid;
		
		if (ph->pu != NULL) {
			pu->next = ph->pu;
			ph->pu->prev = pu;
		}
		ph->pu = pu;
	}
	
	return pu;
}

struct systemcall_unit * process_systemcall_unit (struct process_unit *pu, char *sn, char *fn, double clock) {
	struct systemcall_unit *su = NULL;

	init_systemcall_unit (&su);
	strcpy (su->sn, sn);
	su->fn = (char *) malloc (sizeof (char) * (strlen (fn)+1));
	strcpy (su->fn, fn);
	su->at = clock;

	if (pu->su == NULL) 
		pu->su_tail = su;
	else {
		su->next = pu->su;
		pu->su->prev = su;
	}
	pu->su = su;

	return su;
}

struct stat_systemcall * process_stat_systemcall_unit (struct process_header *ph, struct process_unit *pu, uint32_t pid, char *sn) {
	struct stat_systemcall *ssc = NULL;

	ssc = look_for_stat_systemcall_unit (ph, pid, sn);
	if (ssc == NULL) {
		init_stat_systemcall_unit (&ssc);
		strcpy (ssc->sn, sn);
	
		if (pu->ssc != NULL) {
			ssc->next = pu->ssc;
			pu->ssc->prev = ssc;
		}
		pu->ssc = ssc;
	}

	ssc->cnt++;

	return ssc;
}

struct file_unit * process_file_unit (struct process_header *ph, struct process_unit *pu, uint32_t pid, char *fn) {
	struct file_unit *fu = NULL;

	fu = look_for_file_unit (ph, pid, fn);
	if (fu == NULL) {
		init_file_unit (&fu);
		fu->fn = (char *) malloc (sizeof (char) * (strlen (fn)+1));
		strcpy (fu->fn, fn);
	
		if (pu->fu != NULL) {
			fu->next = pu->fu;
			pu->fu->prev = fu;
		}
		pu->fu = fu;
	}

	fu->acnt++;

	return fu;
}


/* find process unit by pid */
struct process_unit * look_for_process_unit (struct process_header *ph, uint32_t pid) {
	struct process_unit *retval = ph->pu;
	
	while (retval != NULL) {
		if (retval->pid == pid)
			break;
		retval = retval->next;
	}

	return retval;
}

/* find stat systemcall unit by pid, systemcall name */
struct stat_systemcall * look_for_stat_systemcall_unit (struct process_header *ph, uint32_t pid, char *sn) {
	struct process_unit *cur_pu = look_for_process_unit (ph, pid);
	struct stat_systemcall *retval = cur_pu->ssc;

	while (retval != NULL) {
		if (strcmp (retval->sn, sn) == 0)
			break;
		retval = retval->next;
	}

	return retval;
}

/* find file unit by pid, file name */
struct file_unit * look_for_file_unit (struct process_header *ph, uint32_t pid, char *fn) {
	struct process_unit *cur_pu = look_for_process_unit (ph, pid);
	struct file_unit *retval = cur_pu->fu;

	while (retval != NULL) {
		if (strcmp (retval->fn, fn) == 0)
			break;
		retval = retval->next;
	}

	return retval;
}

/* find unfinish unit by pid, systemcall name */
struct unfinish_manager * look_for_unfinish_manager (struct head_unfinish_manager *hum, uint32_t pid, char *sn) {
	struct unfinish_manager *retval = hum->front;

	while (retval != NULL) {
		if (retval->pid == pid && strcmp (retval->sn, sn) == 0)
			break;
		retval = retval->next;
	}

	return retval;
}
