#include <stdint.h>

typedef uint64_t (read_mem_func)(uint64_t*, size_t);

extern read_mem_func read_mem_native;
extern read_mem_func read_mem_native_opt;
extern read_mem_func read_mem_avx2;
extern read_mem_func read_mem_avx2_opt;

extern void
fill_mem(void* src, size_t size);
