# c_mem_pool (v1.1.0)
Fixed size memory pool implemented in C

## Interfaces

### Handle type

The type of the handle for the memory pool is `MP_handle`.

### `long mp_version();`

Returns the version number. The version number is formatted as AABBCC (or
ABBCC), in which AA is the major version number for major feature, BB is the
minor version number for interfaces and CC is the micro version number for bug
fixes.

### `MP_handle* MP_init(size_t size, size_t count);`

Initialization routine that returns the handle for the memory pool. The first
parameter is the size of the fixed size memory chunk that the memory pool will
allocates and the second parameter is the number of memory chunk initially
allocated internally. When allocation failed, this routine returns NULL.

### `void MP_finalize(MP_handle*);`

Finalization routine that free all memory.

### `void* MP_alloc(MP_handle*);`

Return a chunk of memory from memory pool.

### `int MP_free(MP_handle*, void*);`

Recycle the memory that allocated by `MP_alloc` back to memory pool.
When `MP_DEBUG` is defined, the memory address is checked for its validity. If
the memory does not belong to a memory pool or out-of-range writings corrupt the
internal memory bookkeeping then this routine will return 1 and the memory is
not recycled. Otherwise it returns 0.

### Notes

* User does not have to call `MP_free` for every chunk of memory allocated by
  `MP_alloc`. But user must call `MP_finalize` to actually free the memory
  otherwise memory leak is followed. However, memory pool will increase its pool
  size when memory pool is fully occupied, so calling `MP_free` to recycle
  unused memory can save actual memory allocation counts.
* Memory address verification in `MP_free` should have checked whether the
  memory belongs to this memory pool or the other. So it still accept memory
  from other memory pool. (TODO)
