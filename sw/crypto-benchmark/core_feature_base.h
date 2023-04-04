#ifndef __CORE_FEATURE_BASE__
#define __CORE_FEATURE_BASE__

#include <stdint.h>


#define __RISCV_XLEN    32
#define CSR_MCYCLE 0xb00
#define CSR_MINSTRET 0xb02
#define CSR_TIME 0xc01
#define CSR_MCYCLEH 0xb80
#define CSR_MINSTRETH 0xb82
#define CSR_TIMEH 0xc81

#define __STR(s)                #s
#define STRINGIFY(s)            __STR(s)


/** \brief Define a static function that should be always inlined by the compiler. */
#ifndef   __STATIC_FORCEINLINE
  #define __STATIC_FORCEINLINE                   __attribute__((always_inline)) static inline
#endif

/** \brief Type of Control and Status Register(CSR), depends on the XLEN defined in RISC-V */
#if __RISCV_XLEN == 32
  typedef uint32_t rv_csr_t;
#elif __RISCV_XLEN == 64
  typedef uint64_t rv_csr_t;
#else
  typedef uint32_t rv_csr_t;
#endif

/**
 * \brief CSR operation Macro for csrr instruction.
 * \details
 * Read the content of csr register to __v and return it
 * \param csr   CSR macro definition defined in
 *              \ref NMSIS_Core_CSR_Registers, eg. \ref CSR_MSTATUS
 * \return the CSR register value
 */
#define __RV_CSR_READ(csr)                                      \
    ({                                                          \
        register rv_csr_t __v;                                  \
        __asm volatile("csrr %0, " STRINGIFY(csr)               \
                     : "=r"(__v)                                \
                     :                                          \
                     : "memory");                               \
        __v;                                                    \
    })


/**
 * \brief   Read whole 64 bits value of machine instruction-retired counter
 * \details This function will read the whole 64 bits of MINSTRET register
 * \return  The whole 64 bits value of MINSTRET
 * \remarks It will work for both RV32 and RV64 to get full 64bits value of MINSTRET
 */
__STATIC_FORCEINLINE uint64_t __get_rv_instret(void)
{
#if __RISCV_XLEN == 32
    volatile uint32_t high0, low, high;
    uint64_t full;

    high0 = __RV_CSR_READ(CSR_MINSTRETH);
    low = __RV_CSR_READ(CSR_MINSTRET);
    high = __RV_CSR_READ(CSR_MINSTRETH);
    if (high0 != high) {
        low = __RV_CSR_READ(CSR_MINSTRET);
    }
    full = (((uint64_t)high) << 32) | low;
    return full;
#elif __RISCV_XLEN == 64
    return (uint64_t)__RV_CSR_READ(CSR_MINSTRET);
#else // TODO Need cover for XLEN=128 case in future
    return (uint64_t)__RV_CSR_READ(CSR_MINSTRET);
#endif
}

/**
 * \brief   Read whole 64 bits value of mcycle counter
 * \details This function will read the whole 64 bits of MCYCLE register
 * \return  The whole 64 bits value of MCYCLE
 * \remarks It will work for both RV32 and RV64 to get full 64bits value of MCYCLE
 */
__STATIC_FORCEINLINE uint64_t __get_rv_cycle(void)
{
#if __RISCV_XLEN == 32
    volatile uint32_t high0, low, high;
    uint64_t full;

    high0 = __RV_CSR_READ(CSR_MCYCLEH);
    low = __RV_CSR_READ(CSR_MCYCLE);
    high = __RV_CSR_READ(CSR_MCYCLEH);
    if (high0 != high) {
        low = __RV_CSR_READ(CSR_MCYCLE);
    }
    full = (((uint64_t)high) << 32) | low;
    return full;
#elif __RISCV_XLEN == 64
    return (uint64_t)__RV_CSR_READ(CSR_MCYCLE);
#else // TODO Need cover for XLEN=128 case in future
    return (uint64_t)__RV_CSR_READ(CSR_MCYCLE);
#endif
}


/**
 * \brief   Read whole 64 bits value of real-time clock
 * \details This function will read the whole 64 bits of TIME register
 * \return  The whole 64 bits value of TIME CSR
 * \remarks It will work for both RV32 and RV64 to get full 64bits value of TIME
 * \attention only available when user mode available
 */
__STATIC_FORCEINLINE uint64_t __get_rv_time(void)
{
#if __RISCV_XLEN == 32
    volatile uint32_t high0, low, high;
    uint64_t full;

    high0 = __RV_CSR_READ(CSR_TIMEH);
    low = __RV_CSR_READ(CSR_TIME);
    high = __RV_CSR_READ(CSR_TIMEH);
    if (high0 != high) {
        low = __RV_CSR_READ(CSR_TIME);
    }
    full = (((uint64_t)high) << 32) | low;
    return full;
#elif __RISCV_XLEN == 64
    return (uint64_t)__RV_CSR_READ(CSR_TIME);
#else // TODO Need cover for XLEN=128 case in future
    return (uint64_t)__RV_CSR_READ(CSR_TIME);
#endif
}

#endif /* __CORE_FEATURE_BASE__ */