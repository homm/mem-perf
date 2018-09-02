#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "read_mem.h"


#define TEST_ON (16*1024*1024*1024L)
#define SIZE_FROM (16*1024)
#define SIZE_TO (16*1024*1024L)


double
run_passes(size_t passes, read_mem_func* func, void* src, size_t size)
{
    size_t i, j;
    time_t best = 0;
    for (j = 0; j < 8; j++) {
        time_t current = clock();
        for (i = 0; i < passes; i++) {
            (void) func(src, size / sizeof(uint64_t));
        }
        current = clock() - current;
        if (best == 0 || current < best) {
            best = current;
        }
    }
    return passes * size / 1024 / 1024 / ((double)best) * CLOCKS_PER_SEC;
}


int main(int argc, char *argv[])
{
    size_t size;

    for (size = SIZE_FROM; size <= SIZE_TO; size *= 2) {
        size_t passes = TEST_ON / size / 8;
        void* src = calloc(1, size);
        
        fill_mem(src, size);
        {
            double speed = run_passes(passes, read_mem_native, src, size);
            printf(">>> %zu %.2f MB/s\n", size, speed);
        }
        {
            double speed = run_passes(passes, read_mem_avx2_opt, src, size);
            printf(">>> %zu %.2f MB/s\n", size, speed);
        }
        printf("\n");
        free(src);
    }
    return 0;
}
