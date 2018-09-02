#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

#define TEST_ON (16*1024*1024*1024L)
#define SIZE_FROM (16*1024)
#define SIZE_TO (16*1024*1024L)


uint32_t
read_mem_int32(uint32_t* src, size_t size)
{
    size_t i;
    uint32_t result = 0;

    for (i = 0; i < size; i++) {
        result ^= src[i];
    }

    return result;
}

uint64_t
read_mem_int64(uint64_t* src, size_t size)
{
    size_t i;
    uint64_t result = 0;

    for (i = 0; i < size; i++) {
        result ^= src[i];
    }

    return result;
}

void
fill_mem(void* src, size_t size)
{
    size_t i;
    for (i = 0; i < size; i += sizeof(int)) {
        *(int *) (src + i) = rand();
    }
}


int main(int argc, char *argv[])
{
    void* src;
    size_t size, i;

    for (size = SIZE_FROM; size <= SIZE_TO; size *= 2) {
        size_t passes = TEST_ON / size;
        time_t current;
        uint64_t best_value = 0;

        src = calloc(1, size);
        fill_mem(src, size);

        current = clock();
        for (i = 0; i < passes; i++) {
            uint64_t value = read_mem_int64(src, size / sizeof(uint64_t));

            best_value ^= value;
        }
        current = clock() - current;

        free(src);
        {
            double speed = TEST_ON/1024/1024 / ((double)current) * CLOCKS_PER_SEC;
            printf(">>> %" PRIu64 " %zu %.2f MB/s\n",
                best_value, size, speed);
        }
    }
    return 0;
}
