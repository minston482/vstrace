#include "common.h"

/* malloc functions */
void* malloc_unit (size_t size) {
	void *retval = (void *) malloc (size);
	return retval;
}

