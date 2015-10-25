#ifndef MEM_POOL_H
#define MEM_POOL_H

#include <stddef.h>

// Main handle for memory pool.
typedef struct MP_handle MP_handle;

// Get version
long mp_version();

/* Initialization.
 * Initialize a memory pool that allocates chunks of memory of size bytes.
 * The initial pool size is count*size, so it can hold count number of 
 * memory chunk before additional allocation.
 * The size is fixed after initialization.
 */
MP_handle* MP_init(size_t size, size_t count);

/*
 * Finalization.
 */
void MP_finalize(MP_handle*);

/*
 * Get a chunk of memory.
 * It will return NULL if memory allocation failed internally.
 * The memory comes with uninitialized.
 */
void* MP_alloc(MP_handle*);

/*
 * Returning a chunk of memory back to pool.
 * Memory pointer will be set to NULL.
 */
void MP_free(MP_handle*, void*);

#endif
