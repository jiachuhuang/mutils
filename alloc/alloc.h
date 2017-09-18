
#ifndef ALLOC_H
#define ALLOC_H

#include <stdlib.h>
#include <assert.h>

#define USE_MMAP 1
#define ALLOC_FAIL ((void*)-1)

typedef int (*create_segments_t)(void **p, size_t size, char **msg);
typedef int (*detach_segment_t)(void **p, size_t size);

typedef struct {
	create_segments_t create_segments;
	detach_segment_t detach_segment;
} alloc_handlers;

#if defined(USE_MMAP)
extern alloc_handlers alloc_mmap_handlers;
#define alloc_handler alloc_mmap_handlers
#endif

#endif /* ALLOC_H */



