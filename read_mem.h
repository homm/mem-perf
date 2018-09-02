#include <stdint.h>

extern uint64_t
read_mem_native(uint64_t* src, size_t size);

extern uint64_t
read_mem_native_opt(uint64_t* src, size_t size);

extern uint64_t
read_mem_avx2(uint64_t* src, size_t size);

extern uint64_t
read_mem_avx2_opt(uint64_t* src, size_t size);

extern void
fill_mem(void* src, size_t size);
