#include "memory_free.h"

/* free functions */
int free_systemcall_unit (struct systemcall_unit *su, struct process_unit *pu) {
	int retval = 0;

	/* init values */
	strcpy (su->sn, "");
	if (su->fn != NULL) {
		su->fn = NULL;
		free (su->fn);
	}
	su->at = 0.0;

	if (su->prev != NULL && su->next != NULL) {
		su->prev->next = su->next;
		su->next->prev = su->prev;
	}
	else if (su->prev == NULL && su->next != NULL) {
		pu->su = su->next;
		su->next->prev = NULL;
	}
	else if (su->prev != NULL && su->next == NULL)
		su->prev->next = NULL;
	
	su->prev = NULL;
	su->next = NULL;

	free (su);

	return retval;
}

int free_stat_systemcall_unit (struct stat_systemcall *ssc, struct process_unit *pu) {
	int retval = 0;

	/* init values */
	strcpy (ssc->sn, "");
	ssc->cnt = 0;

	if (ssc->prev != NULL && ssc->next != NULL) {
		ssc->prev->next = ssc->next;
		ssc->next->prev = ssc->prev;
	}
	else if (ssc->prev == NULL && ssc->next != NULL) {
		pu->ssc = ssc->next;
		ssc->next->prev = NULL;
	}
	else if (ssc->prev != NULL && ssc->next == NULL)
		ssc->prev->next = NULL;
	
	ssc->prev = NULL;
	ssc->next = NULL;

	free (ssc);

	return retval;
}

int free_file_unit (struct file_unit *fu, struct process_unit *pu) {
	int retval = 0;

	/* init values */
	fu->fn = NULL;
	free (fu->fn);
	fu->acnt = 0;
	fu->rcnt = 0;
	fu->wcnt = 0;
	fu->rsize = 0;
	fu->wsize = 0;

	if (fu->prev != NULL && fu->next != NULL) {
		fu->prev->next = fu->next;
		fu->next->prev = fu->prev;
	}
	else if (fu->prev == NULL && fu->next != NULL) {
		pu->fu = fu->next;
		fu->next->prev = NULL;
	}
	else if (fu->prev != NULL && fu->next == NULL)
		fu->prev->next = NULL;
	
	fu->prev = NULL;
	fu->next = NULL;

	free (fu);

	return retval;
}

int free_process_unit (struct process_unit *pu, struct process_header *ph) {
	int retval = 0;
	
	pu->pid = 0;

	if (pu->prev != NULL && pu->next != NULL) {
		pu->prev->next = pu->next;
		pu->next->prev = pu->prev;
	}
	else if (pu->prev == NULL && pu->next != NULL) {
		ph->pu = pu->next;
		pu->next->prev = NULL;
	}
	else if (pu->prev != NULL && pu->next == NULL)
		pu->prev->next = NULL;
	
	pu->prev = NULL;
	pu->next = NULL;

	free (pu);

	return retval;
}

int free_process_header (struct process_header *ph) {
	int retval = 0;

	while (ph->pu != NULL) {
		while (ph->pu->su != NULL);
			free_systemcall_unit (ph->pu->su, ph->pu);
		while (ph->pu->ssc != NULL);
			free_stat_systemcall_unit (ph->pu->ssc, ph->pu);
		while (ph->pu->fu != NULL);
			free_file_unit (ph->pu->fu, ph->pu);
		free_process_unit (ph->pu, ph);
	}

	return retval;
}

int free_unfinish_manager (struct head_unfinish_manager *hum, struct unfinish_manager *um) {
	int retval = 0;
	
	if (hum->tail == um)
		hum->tail = um->prev;

	um->prev->next = um->next;
	if (um->next != NULL)
		um->next->prev = um->prev;
	um->prev = NULL;
	um->next = NULL;
	free (um);

	return retval;
}
