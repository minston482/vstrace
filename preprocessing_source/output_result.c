#include "output_result.h"

extern struct access_file_information afi[MAX_FILE_OPEN];
extern int afi_size;

/* output file functions */
int output_syscall (struct process_header *ph, char *infn, int select) {
	int retval = 0;
	struct stat st = {0};
	char res_dir[MAX_FILE_NAME_LENGTH], graph_dir[MAX_FILE_NAME_LENGTH];

	if (stat ("result", &st) == -1)
		mkdir ("result", 0777);

	sprintf (res_dir, "result/%s", infn);
	if (stat (res_dir, &st) == -1)
		mkdir (res_dir, 0777);

	sprintf (graph_dir, "%s/graphs", res_dir);
	if (stat (graph_dir, &st) == -1)
		mkdir (graph_dir, 0777);

	switch (select) {
		case OUTPUT_SYSCALL_FOOTPRINT_PID:
			output_syscall_timeline_pid (ph, res_dir, infn);
			break;
		case OUTPUT_SYSCALL_CNT_PID:
			output_syscall_cnt_pid (ph, res_dir, infn);
			break;
		case OUTPUT_SYSCALL_RWSIZE_PID:
			output_syscall_rwsize_pid (ph, res_dir, infn);
			break;
		case OUTPUT_SYSCALL_CNT_FILE:
			output_syscall_cnt_file (ph, res_dir, infn);
			break;
		case OUTPUT_SYSCALL_RWSIZE_FILE:
			output_syscall_rwsize_file (ph, res_dir, infn);
			break;
		default:
			break;
	}

	return retval;
}

int output_syscall_timeline_pid (struct process_header *ph, char *res_path, char *infn) {
	int retval = 0;	
	struct process_unit *cur_pu = ph->pu;
	struct systemcall_unit *cur_su = NULL;
	FILE *fp = NULL;
	char outfn[MAX_FILE_NAME_LENGTH];

	printf ("\n ----- start creating pid system call timeline file -----\n");
	while (cur_pu != NULL) {
		cur_su = cur_pu->su_tail;
		sprintf (outfn, "%s/%s.timeline.pid#%d", res_path, infn, cur_pu->pid);
		fp = fopen (outfn, "w");
		if (fp == NULL) {
			printf ("file open error!\n");
			retval = 1;
			return retval;
		}
		printf (" create %s file\n", outfn);
		fprintf (fp, "#pid system call timeline\n");
		fprintf (fp, "#clock systemcall_name\n");
		while (cur_su != NULL) {
			fprintf(fp, "%lf %s\n", cur_su->at, cur_su->sn);
			cur_su = cur_su->prev;
		}
		fclose (fp);
		printf (" create %s file (done - success) \n", outfn);
		cur_pu = cur_pu->next;
	}
	printf (" ----- end creating pid system call timeline file -----\n");

	return retval;
}

int output_syscall_cnt_pid (struct process_header *ph, char *res_path, char *infn) {
	int retval = 0, i = 0;	
	struct process_unit *cur_pu = ph->pu;
	struct stat_systemcall *cur_ssc = NULL;
	FILE *fp = NULL;
	char outfn[MAX_FILE_NAME_LENGTH];

	printf ("\n ----- start creating pid system call cnt file -----\n");
	sprintf (outfn, "%s/%s.cnt.pid", res_path, infn);
	fp = fopen (outfn, "w");
	if (fp == NULL) {
		printf ("file open error!\n");
		retval = 1;
		return retval;
	}
	printf (" create %s file\n", outfn);
	fprintf (fp, "#pid system call cnt\n");
	fprintf (fp, "#pid open read write close\n");

	while (cur_pu != NULL) {
		cur_ssc = cur_pu->ssc;
		fprintf (fp, "%d ", cur_pu->pid);
		for (i = 1; i <= SYSCALL_TYPE_SIZE; i++) {
			cur_ssc = cur_pu->ssc;
			while (cur_ssc != NULL) {
				if (identy_systemcall_type (cur_ssc->sn) == -(i)) {
					fprintf (fp, "%d", cur_ssc->cnt);
					if (i != SYSCALL_TYPE_SIZE)
						fprintf(fp, " ");
					break;
				}
				cur_ssc = cur_ssc->next;
			}
			if (cur_ssc == NULL) {
				fprintf (fp, "0");
				if (i != SYSCALL_TYPE_SIZE)
					fprintf(fp, " ");

			}
		}
		fprintf (fp, "\n");
		cur_pu = cur_pu->next;
	}

	fclose (fp);
	printf (" create %s file (done - success) \n", outfn);
	printf (" ----- end creating pid system call cnt file -----\n");

	return retval;
}

int output_syscall_rwsize_pid (struct process_header *ph, char *res_path, char *infn) {
	int retval = 0;
	struct process_unit *cur_pu = ph->pu;
	struct file_unit *cur_fu = NULL;
	FILE *fp = NULL;
	char outfn[MAX_FILE_NAME_LENGTH];
	uint64_t rsize = 0, wsize = 0;

	printf ("\n ----- start creating pid system call rwsize file -----\n");
	sprintf (outfn, "%s/%s.rwsize.pid", res_path, infn);
	fp = fopen (outfn, "w");
	if (fp == NULL) {
		printf ("file open error!\n");
		retval = 1;
		return retval;
	}
	printf (" create %s file\n", outfn);
	fprintf (fp, "#pid amount of read and write\n");
	fprintf (fp, "#pid read_size write_size\n");

	while (cur_pu != NULL) {
		cur_fu = cur_pu->fu;
		while (cur_fu != NULL) {
			rsize += cur_fu->rsize;
			wsize += cur_fu->wsize;
			cur_fu = cur_fu->next;
		}
		fprintf (fp, "%d %ld %ld\n", cur_pu->pid, rsize, wsize);
		rsize = 0;
		wsize = 0;
		cur_pu = cur_pu->next;
	}
	fclose (fp);
	printf (" create %s file (done - success) \n", outfn);
	printf (" ----- end creating pid system call rwsize file -----\n");

	return retval;
}

int output_syscall_cnt_file (struct process_header *ph, char *res_path, char *infn) {
	int retval = 0, i = 0;
	FILE *fp = NULL;
	char outfn[MAX_FILE_NAME_LENGTH];

	printf ("\n ----- start creating file name system call cnt file -----\n");
	sprintf (outfn, "%s/%s.cnt.file", res_path, infn);
	fp = fopen (outfn, "w");
	if (fp == NULL) {
		printf ("file open error!\n");
		retval = 1;
		return retval;
	}
	printf (" create %s file\n", outfn);
	fprintf (fp, "#file name system call cnt\n");
	fprintf (fp, "#file_name access read write\n");

	for (i = 0; i < afi_size; i++)
		fprintf (fp, "%s %d %d %d\n", afi[i].fn, afi[i].acnt, afi[i].rcnt, afi[i].wcnt);
	
	fclose (fp);
	printf (" create %s file (done - success) \n", outfn);
	printf (" ----- end creating file name system call cnt file -----\n");

	return retval;
}

int output_syscall_rwsize_file (struct process_header *ph, char *res_path, char *infn) {
	int retval = 0, i = 0;
	FILE *fp = NULL;
	char outfn[MAX_FILE_NAME_LENGTH];

	printf ("\n ----- start creating file name system call rwsize file -----\n");
	sprintf (outfn, "%s/%s.rwsize.file", res_path, infn);
	fp = fopen (outfn, "w");
	if (fp == NULL) {
		printf ("file open error!\n");
		retval = 1;
		return retval;
	}
	printf (" create %s file\n", outfn);
	fprintf (fp, "#file name amount of read and write\n");
	fprintf (fp, "#file_name read_size write_size\n");

	for (i = 0; i < afi_size; i++)
		fprintf (fp, "%s %ld %ld\n", afi[i].fn, afi[i].rsize, afi[i].wsize);
	
	fclose (fp);
	printf (" create %s file (done - success) \n", outfn);
	printf (" ----- end creating file name system call rwsize file -----\n");

	return retval;
}
