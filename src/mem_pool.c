#include "mem_pool.h"
#include <stdlib.h>
#include <inttypes.h>

typedef struct Pool_node Pool_node;
typedef struct Mem_node Mem_node;
int add_mem_pool(MP_handle*, size_t);

struct MP_handle
{
    Pool_node *pool_node_ptr;
    size_t mem_size;
    size_t mem_node_count;
    Mem_node *free_node_ptr;
#ifdef MP_DEBUG
    size_t id;
#endif
};

struct Pool_node
{
    Pool_node *next_pool_node;
    void *pool_mem;
};

struct Mem_node
{
    Mem_node *next_mem_node;
};

long mp_version()
{
    return 10200L;
}

MP_handle* MP_init(size_t size, size_t count)
{
    MP_handle *handle = malloc(sizeof(MP_handle));
    if (handle == NULL)
        return NULL;
    const size_t mem_node_size = sizeof(Mem_node);
    handle->mem_size = size > mem_node_size ? size : mem_node_size;
    handle->mem_node_count = handle->mem_size / mem_node_size;
    if (handle->mem_size % mem_node_size != 0)
        ++handle->mem_node_count;
    handle->mem_size = mem_node_size * handle->mem_node_count;
#ifdef MP_DEBUG
    handle->mem_size += mem_node_size;
    static size_t id = 0;
    handle->id = id++;
#endif
    handle->free_node_ptr = NULL;
    handle->pool_node_ptr = NULL;
    int error_code = add_mem_pool(handle, count);
    if (error_code == 1)
    {
        free(handle->pool_node_ptr);
        free(handle);
        return NULL;
    }
    else if (error_code == 2)
    {
        free(handle);
        return NULL;
    }
    return handle;
}

void MP_finalize(MP_handle *handle)
{
    Pool_node *scanner = handle->pool_node_ptr;
    Pool_node *tmp = scanner;
    while (scanner != NULL)
    {
        tmp = scanner;
        scanner = scanner->next_pool_node;
        free(tmp->pool_mem);
        free(tmp);
    }
    free(handle);
    handle = NULL;
}

void* MP_alloc(MP_handle *handle)
{
    if (handle->free_node_ptr == NULL)
        if (add_mem_pool(handle, 0) != 0)
            return NULL;
    Mem_node *new_mem_ptr = handle->free_node_ptr;
    handle->free_node_ptr = handle->free_node_ptr->next_mem_node;
    new_mem_ptr->next_mem_node = NULL;
#ifdef MP_DEBUG
    Mem_node *tmp_node_ptr = new_mem_ptr + handle->mem_node_count;
    tmp_node_ptr->next_mem_node =
        (Mem_node*)((uintptr_t)(new_mem_ptr) ^ handle->id);
#endif
    return new_mem_ptr;
}


int MP_free(MP_handle *handle, void *ptr)
{
    if (ptr == NULL)
        return 0;
#ifdef MP_DEBUG
    Mem_node *node_ptr = ptr;
    node_ptr += handle->mem_node_count;
    if ((void*)((uintptr_t)(node_ptr->next_mem_node) ^ handle->id) != ptr)
        return 1;
    else
        node_ptr->next_mem_node = NULL;
#endif
    Mem_node *new_mem_node_ptr = ptr;
    new_mem_node_ptr->next_mem_node = handle->free_node_ptr;
    handle->free_node_ptr = new_mem_node_ptr;
    return 0;
}

/*****************************************************************************
 *
 * private functions
 *
 ****************************************************************************/

int add_mem_pool(MP_handle *handle, size_t count)
{
    static size_t alloc_count = 1;
    if (count != 0)
        alloc_count = count;
    if (alloc_count == 0)
        return 0;
    // add raw memory.
    Pool_node *scanner = handle->pool_node_ptr;
    if (scanner == NULL)
    {
        handle->pool_node_ptr = malloc(sizeof(Pool_node));
        if (handle->pool_node_ptr == NULL)
            return 2;
        handle->pool_node_ptr->next_pool_node = NULL;
        scanner = handle->pool_node_ptr;
    }
    else
    {
        while (scanner->next_pool_node != NULL)
            scanner = scanner->next_pool_node;
        scanner->next_pool_node = malloc(sizeof(Pool_node));
        if (scanner->next_pool_node == NULL)
            return 2;
        scanner = scanner->next_pool_node;
        scanner->next_pool_node = NULL;
    }
    scanner->pool_mem = malloc(alloc_count * handle->mem_size);
    if (scanner->pool_mem == NULL)
        return 1;

    // format the raw memory.
    Mem_node *node_builder_ptr = scanner->pool_mem;
    Mem_node *mem_node_ptr = scanner->pool_mem;
    size_t i = 0;
    for (; i < alloc_count - 1; ++i)
    {
#ifdef MP_DEBUG
        node_builder_ptr += handle->mem_node_count + 1;
#else
        node_builder_ptr += handle->mem_node_count;
#endif
        mem_node_ptr->next_mem_node = node_builder_ptr;
        mem_node_ptr = node_builder_ptr;
    }
    mem_node_ptr->next_mem_node = NULL;

    // append new free memory to free_node_ptr.
    if (handle->free_node_ptr == NULL)
        handle->free_node_ptr = scanner->pool_mem;
    else
    {
        mem_node_ptr = handle->free_node_ptr;
        while (mem_node_ptr->next_mem_node != NULL)
            mem_node_ptr = mem_node_ptr->next_mem_node;
        mem_node_ptr->next_mem_node = scanner->pool_mem;
    }

    alloc_count *= 2;
    return 0;
}
