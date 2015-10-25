#include "mem_pool.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct Chunk
{
    int a;
    double b;
    double c;
} Chunk;

int main()
{
    printf("Chunk size: %zu\n", sizeof(Chunk));
    MP_handle *handle = MP_init(sizeof(Chunk), 10);
    if (handle == NULL)
    {
        printf("Error: handle allocation failed.\n");
        return 1;
    }
    Chunk *chunk_array[5] = {NULL, NULL, NULL, NULL, NULL};
    int i = 0;
    for (i = 0; i < 5; ++i)
    {
        chunk_array[i] = MP_alloc(handle);
        if (chunk_array[i] == NULL)
        {
            printf("Error: MP_alloc falied 1. i: %d\n", i);
            MP_finalize(handle);
            return 2;
        }
        chunk_array[i]->c = i + 10;
    }
    for (i = 0; i < 4; ++i) 
    {
        if (MP_free(handle, chunk_array[i]) != 0)
        {
            printf("Error: MP_free failed. i: %d\n", i);
            MP_finalize(handle);
        }
    }
    for (i = 0; i < 9; ++i)
    {
        chunk_array[0] = MP_alloc(handle);
        if (chunk_array[0] == NULL)
        {
            printf("Error: MP_alloc falied 2. i: %d\n", i);
            MP_finalize(handle);
            return 2;
        }
        chunk_array[0]->b = i+100;
//        printf("i: %d, c: %lf\n", i, chunk_array[0]->c);
    }
    for (i = 0; i < 100000; ++i)
    {
        chunk_array[0] = MP_alloc(handle);
        if (chunk_array[0] == NULL)
        {
            printf("Error: MP_alloc falied 3. i: %d\n", i);
            MP_finalize(handle);
            return 2;
        }
        chunk_array[0]->b = i-500000;
    }

    if (MP_free(handle, chunk_array[0]) != 0)
    {
        printf("Error: MP_free failed 2.\n");
        MP_finalize(handle);
    }

    void *tmp_ptr = calloc(1, sizeof(Chunk) + sizeof(void*));
    if (MP_free(handle, tmp_ptr) != 0)
        printf("Failure test passed.\n");
    else
        printf("Failure test failed!\n");
    free(tmp_ptr);

    int tmp_int = 10;
    if (MP_free(handle, &tmp_int) != 0)
        printf("Failure test 2 passed.\n");
    else
        printf("Failure test 2 failed!\n");
    chunk_array[1] = MP_alloc(handle);
    if (chunk_array[1] == NULL)
    {
        printf("Error: MP_alloc failed 4.\n");
        MP_finalize(handle);
    }
    chunk_array[1]->a = chunk_array[1]->b = chunk_array[1]->c = 123;
    printf("tmp_int: %d\n", tmp_int);
    chunk_array[2] = chunk_array[1] + 1;
    chunk_array[2]->a = 0;
    if (MP_free(handle, chunk_array[1]) != 0)
        printf("Failure test3 passed.\n");
    else
        printf("Failure test3 failed.\n");
    
    MP_finalize(handle);
//    printf("b: %lf, c: %lf\n", chunk_array[4]->b, chunk_array[4]->c);
    return 0;
}

