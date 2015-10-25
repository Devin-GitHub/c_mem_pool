#include "mem_pool.h"
#include <stdio.h>

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
        MP_free(handle, chunk_array[i]);
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
    MP_finalize(handle);
//    printf("b: %lf, c: %lf\n", chunk_array[4]->b, chunk_array[4]->c);
    return 0;
}

