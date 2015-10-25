#include "mem_pool.h"
#include "a.h"

int main()
{
    MP_handle *handle = MP_init(sizeof(A), 10);
    A *ptr_array[100];
    size_t i = 0, j = 0;;
    for (j = 0; j < 1000000; ++j)
    {
        for (i = 0; i < 100; ++i)
            ptr_array[i] = MP_alloc(handle);
        for (i = 0; i < 100; ++i)
            MP_free(handle, ptr_array[i]);
    }
    MP_finalize(handle);
    return 0;
}
