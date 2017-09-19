#include "alloc.h"

#ifdef USE_MMAP

#include <pthread.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>  
#include <fcntl.h>
#include <sys/stat.h>

static int create_segments(void **p, size_t size, char **error) {
	assert(size);
	*p = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);
	if(*p == ALLOC_FAIL) {
		*error = "mmap error";
		return 0;
	}

	return 1;
}

static int detach_segment(void **p, size_t size) {
	if(size) {
		munmap(*p, size);
	}
	return 1;
}

alloc_handlers alloc_mmap_handlers = {
	create_segments,
	detach_segment
};


#endif /* USE_MMAP */
