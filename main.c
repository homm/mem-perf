#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include "read_mem.h"


#define TEST_ON (4*1024*1024*1024L)
#define SIZE_FROM (16*1024)
#define SIZE_TO (16*1024*1024L)


int main(int argc, char *argv[])
{
    size_t size, i;

    for (size = SIZE_FROM; size <= SIZE_TO; size *= 2) {
        size_t passes = TEST_ON / size;
        void* src = calloc(1, size);
        
        fill_mem(src, size);
        {
            uint64_t best_value = 0;
            time_t current = clock();
            for (i = 0; i < passes; i++) {
                best_value ^= read_mem_avx2_opt(src, size / sizeof(uint64_t));
            }
            current = clock() - current;
            {
                double speed = TEST_ON/1024/1024 / ((double)current) * CLOCKS_PER_SEC;
                printf(">>> %" PRIu64 " %zu %.2f MB/s\n",
                    best_value, size, speed);
            }
        }
        free(src);
    }
    return 0;
}
