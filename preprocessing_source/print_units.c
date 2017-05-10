#include "print_units.h"

/* print functions (for checking and debugging) */
void print_all_process_unit (struct process_header *ph) {
	struct process_unit *cur_pu = ph->pu;
	
	printf ("\n ----- print all process unit -----\n");
	while (cur_pu != NULL) {
		printf ("pid : %d\n", cur_pu->pid);
		cur_pu = cur_pu->next;
	}
	printf (" ----- print all process unit (end) -----\n");
}

void print_all_systemcall_unit (struct process_header *ph) {
	struct process_unit *cur_pu = ph->pu;
	struct systemcall_unit *cur_su = NULL;

	printf ("\n ----- print all systemcall unit -----\n");
	while (cur_pu != NULL) {
		cur_su = cur_pu->su;
		while (cur_su != NULL) {
			printf ("process id : %d, systemcall name : %s, file name : %s, access time : %lf\n", cur_pu->pid, cur_su->sn, cur_su->fn, cur_su->at);
			cur_su = cur_su->next;
		}
		cur_pu = cur_pu->next;
	}
	printf (" ----- print all systemcall unit (end) -----\n");
}

void print_all_stat_systemcall_unit (struct process_header *ph) {
	struct process_unit *cur_pu = ph->pu;
	struct stat_systemcall *cur_ssc = NULL;

	printf ("\n ----- print all stat systemcall unit -----\n");
	while (cur_pu != NULL) {
		cur_ssc = cur_pu->ssc;
		while (cur_ssc != NULL) {
			printf ("process id : %d, systemcall name : %s, systemcall count : %d\n", cur_pu->pid, cur_ssc->sn, cur_ssc->cnt);
			cur_ssc = cur_ssc->next;
		}
		cur_pu = cur_pu->next;
	}
	printf (" ----- print all stat systemcall unit (end) -----\n");
}

void print_all_file_unit (struct process_header *ph) {
	struct process_unit *cur_pu = ph->pu;
	struct file_unit *cur_fu = NULL;

	printf ("\n ----- print all file unit -----\n");
	while (cur_pu != NULL) {
		cur_fu = cur_pu->fu;
		while (cur_fu != NULL) {
			printf ("\nprocess id : %d, file name : %s\n", cur_pu->pid, cur_fu->fn);
			printf ("access count : %d\n", cur_fu->acnt);
			printf ("read count : %d, write count : %d\n", cur_fu->rcnt, cur_fu->wcnt);
			printf ("read size : %ld, write size : %ld\n", cur_fu->rsize, cur_fu->wsize);
			cur_fu = cur_fu->next;
		}
		cur_pu = cur_pu->next;
	}
	printf (" ----- print all file unit (end) -----\n");
}

void print_all_unfinish_manager (struct head_unfinish_manager *hum) {
	struct unfinish_manager *um = hum->front;
	
	while (um != NULL) {
		printf (" \n pid: %d, sn: %s\n", um->pid, um->sn);
		um = um->next;
	}
}
