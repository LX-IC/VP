/*!
@defgroup test_utils Test Utils
@{
*/

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

#ifndef __SHARE_TEST_H__
#define __SHARE_TEST_H__

//! Length of test input for a hash function.
#define TEST_HASH_INPUT_LENGTH 1024

#define __riscv_xlen 32

//
// Misc
// ----------------------------------------------------------------------

/*!
@brief Prints a 64-bit input as hex to stdout.
@details Prints in LITTLE ENDIAN mode.
@param [in] in - The thing to print.
*/
void puthex64(uint64_t in);


/*!
@brief Prints a byte string as hex to std out in little endian mode.
@param [in] in - The byte string to print.
@param [in] len - Length of the string to print.
*/
void puthex(unsigned char * in, size_t len);


/*!
@brief Prints a byte string such that it can be read by a python program as
    a giant literal.
@param [in] in - The byte string to print.
@param [in] len - Length of the string to print.
*/
void puthex_py(unsigned char * in, size_t len);


/*!
@brief Read len random bytes into dest.
*/
size_t test_rdrandom(unsigned char * dest, size_t len);



//
// Low level register access.
// ------------------------------------------------------------------
#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif
#if __riscv_xlen == 32
  typedef uint32_t rv_csr_t;
#elif __riscv_xlen == 64
  typedef uint64_t rv_csr_t;
#else
  typedef uint32_t rv_csr_t;
#endif

#define __STR(s)                #s
#define STRINGIFY(s)            __STR(s)
#define __RV_CSR_READ(csr)                                      \
    ({                                                          \
        register rv_csr_t __v;                                  \
        __asm volatile("csrr %0, " STRINGIFY(csr)               \
                     : "=r"(__v)                                \
                     :                                          \
                     : "memory");                               \
        __v;                                                    \
    })
#define CSR_MCYCLE 0xb00
#define CSR_MCYCLEH 0xb80


__STATIC_FORCEINLINE uint64_t test_rdinstret() {
    uint64_t result = 0;

#if ( __riscv_xlen == 32 )

    uint32_t hi1,hi2,lo;

    do {
        asm volatile (
            "rdinstreth %0;"
            "rdinstret  %1;" 
            "rdinstreth %2;" 
            : "=r"(hi1), "=r"(lo), "=r"(hi2)
        );
    } while(hi1 != hi2);

    result = (((uint64_t)hi1) << 32) | lo;

#elif ( __riscv_xlen == 64 )
        
    asm volatile (
        "rdinstret  %0;" 
        : "=r"(result)
    );

#else
    #error "Unsupported RISC-V XLEN: __riscv_xlen, expected 32 or 64"
#endif

    return result;
}


__STATIC_FORCEINLINE uint64_t test_rdcycle() {
    uint64_t result = 0;

#if ( __riscv_xlen == 32 )

    volatile uint32_t high0, low, high;
    uint64_t full;

    high0 = __RV_CSR_READ(CSR_MCYCLEH);
    low = __RV_CSR_READ(CSR_MCYCLE);
    high = __RV_CSR_READ(CSR_MCYCLEH);
    if (high0 != high) {
        low = __RV_CSR_READ(CSR_MCYCLE);
    }
    result = (((uint64_t)high) << 32) | low;

#elif ( __riscv_xlen == 64 )
        
    __asm__ volatile (
        "rdcycle  %0;" 
        : "=r"(result)
        : 
    );

#else
    #error "Unsupported RISC-V XLEN: __riscv_xlen, expected 32 or 64"
#endif

    return result;
}

__STATIC_FORCEINLINE uint64_t test_rdtime() {
    uint64_t result = 0;

#if ( __riscv_xlen == 32 )

    uint32_t hi1,hi2,lo;

    do {
        __asm__ volatile (
            "rdtimeh %0;"
            "rdtime  %1;" 
            "rdtimeh %2;" 
            : "=r"(hi1), "=r"(lo), "=r"(hi2)
            : 
        );
    } while(hi1 != hi2);

    result = (((uint64_t)hi1) << 32) | lo;

#elif ( __riscv_xlen == 64 )
        
    __asm__ volatile (
        "rdtime  %0;" 
        : "=r"(result)
        : 
    );

#else
    #error "Unsupported RISC-V XLEN: __riscv_xlen, expected 32 or 64"
#endif

    return result;
}

#endif

//! @}