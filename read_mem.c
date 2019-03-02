#include <immintrin.h>
#include "read_mem.h"

extern uint64_t
read_mem_native(uint64_t* src, size_t size)
{
    size_t i = 0;
    uint64_t result0 = 0;

    for (; i < size; i += 1) {
        result0 ^= src[i+0];
    }

    return result0;
}

extern uint64_t
read_mem_native_opt(uint64_t* src, size_t size)
{
    size_t i = 0;
    uint64_t result0 = 0;
    uint64_t result1 = 0;
    uint64_t result2 = 0;
    uint64_t result3 = 0;

    for (; i < size; i += 4) {
        result0 ^= src[i+0];
        result1 ^= src[i+1];
        result2 ^= src[i+2];
        result3 ^= src[i+3];
    }

    return result0 ^ result1 ^ result2 ^ result3;
}

extern uint64_t
read_mem_avx2(uint64_t* src, size_t size)
{
    __m256i result0 = _mm256_setzero_si256();
    __m128i temp;
    __m256i* mm_src = (__m256i*) (src+0);

    for (; mm_src < (__m256i*) (src+size); mm_src += 1) {
        result0 = _mm256_xor_si256(result0, mm_src[0]);
    }

    temp = _mm_xor_si128(
        _mm256_castsi256_si128(result0),
        _mm256_extractf128_si256(result0, 1));
    return (_mm_extract_epi64(temp, 0) ^
            _mm_extract_epi64(temp, 1));
}


__m256i inline
rgbA2rgba(__m256i source)
{
    __m256i zero = _mm256_setzero_si256();
    __m256i half = _mm256_set1_epi16(128);
    __m256i maxalpha = _mm256_set_epi32(
        0xff000000, 0xff000000, 0xff000000, 0xff000000,
        0xff000000, 0xff000000, 0xff000000, 0xff000000);
    __m256i factormask = _mm256_set_epi8(
        15,15,15,15, 11,11,11,11, 7,7,7,7, 3,3,3,3,
        15,15,15,15, 11,11,11,11, 7,7,7,7, 3,3,3,3);
    __m256i factorsource, pix1, pix2, factors;

    factorsource = _mm256_shuffle_epi8(source, factormask);
    factorsource = _mm256_or_si256(factorsource, maxalpha);
    
    pix1 = _mm256_unpacklo_epi8(source, zero);
    factors = _mm256_unpacklo_epi8(factorsource, zero);
    pix1 = _mm256_add_epi16(_mm256_mullo_epi16(pix1, factors), half);
    pix1 = _mm256_add_epi16(pix1, _mm256_srli_epi16(pix1, 8));
    pix1 = _mm256_srli_epi16(pix1, 8);

    pix2 = _mm256_unpackhi_epi8(source, zero);
    factors = _mm256_unpackhi_epi8(factorsource, zero);
    pix2 = _mm256_add_epi16(_mm256_mullo_epi16(pix2, factors), half);
    pix2 = _mm256_add_epi16(pix2, _mm256_srli_epi16(pix2, 8));
    pix2 = _mm256_srli_epi16(pix2, 8);

    return _mm256_packus_epi16(pix1, pix2);
}

extern uint64_t
comppute_mem_avx2(uint64_t* src, size_t size)
{
    __m256i result0 = _mm256_setzero_si256();
    __m256i result1 = _mm256_setzero_si256();
    __m256i result2 = _mm256_setzero_si256();
    __m256i result3 = _mm256_setzero_si256();
    __m128i temp;
    __m256i* mm_src = (__m256i*) (src+0);

    for (; mm_src < (__m256i*) (src+size); mm_src += 4) {
        result0 = _mm256_xor_si256(result0, rgbA2rgba(_mm256_loadu_si256(mm_src + 0)));
        result1 = _mm256_xor_si256(result1, rgbA2rgba(_mm256_loadu_si256(mm_src + 1)));
        result2 = _mm256_xor_si256(result2, rgbA2rgba(_mm256_loadu_si256(mm_src + 2)));
        result3 = _mm256_xor_si256(result3, rgbA2rgba(_mm256_loadu_si256(mm_src + 3)));
    }

    result0 = _mm256_xor_si256(
        _mm256_xor_si256(result0, result2),
        _mm256_xor_si256(result1, result3));

    temp = _mm_xor_si128(
        _mm256_castsi256_si128(result0),
        _mm256_extractf128_si256(result0, 1));
    return (_mm_extract_epi64(temp, 0) ^
            _mm_extract_epi64(temp, 1));
}


extern uint64_t
read_mem_avx2_opt(uint64_t* src, size_t size)
{
    __m256i result0 = _mm256_setzero_si256();
    __m256i result1 = _mm256_setzero_si256();
    __m256i result2 = _mm256_setzero_si256();
    __m256i result3 = _mm256_setzero_si256();
    __m128i temp;
    __m256i* mm_src = (__m256i*) (src+0);

    for (; mm_src < (__m256i*) (src+size); mm_src += 4) {
        result0 = _mm256_xor_si256(result0, mm_src[0]);
        result1 = _mm256_xor_si256(result1, mm_src[1]);
        result2 = _mm256_xor_si256(result2, mm_src[2]);
        result3 = _mm256_xor_si256(result3, mm_src[3]);
    }

    result0 = _mm256_xor_si256(
        _mm256_xor_si256(result0, result2),
        _mm256_xor_si256(result1, result3));

    temp = _mm_xor_si128(
        _mm256_castsi256_si128(result0),
        _mm256_extractf128_si256(result0, 1));
    return (_mm_extract_epi64(temp, 0) ^
            _mm_extract_epi64(temp, 1));
}


extern void
fill_mem(void* src, size_t size)
{
    size_t i;
    srand(size);
    for (i = 0; i < size; i += sizeof(int)) {
        *(int *) (src + i) = rand();
    }
}
