#include "stdlib.h"
#include "a.h"

int main()
{
    A *ptr_array[100];
    size_t i = 0, j = 0;
    for (j = 0; j < 1000000; ++j)
    {
        for (i = 0; i < 100; ++i)
            ptr_array[i] = malloc(sizeof(A));
        for (i = 0; i < 100; ++i)
            free(ptr_array[i]);
    }
    return 0;
}
