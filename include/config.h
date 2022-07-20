#pragma once

#ifndef __ASSEMBLER__
#define EL_MASK 0b1100
#define EL0     0b0001
#define EL1     0b0010
#define EL2     0b0100
#define EL3     0b1000
#else
#define EL_MASK #0b1100
#define EL0     #0b0000
#define EL1     #0b0100
#define EL2     #0b1000
#define EL3     #0b1100
#endif

#define SCLTR_EL3_AARCH64_MODE_EL2   1 << 10
#define SCLTR_EL3_AARCH32_MODE_EL2   0 << 10
#define SCLTR_EL3_NS                 1
#define SCLTR_EL3_S                  0
#define SCLTR_EL3_IRQ_ENABLE         1 << 1
#define SCLTR_EL3_IRQ_DISABLE        0 << 1
#define SCLTR_EL3_FIQ_ENABLE         1 << 2
#define SCLTR_EL3_FIQ_DISABLE        0 << 2
#define SCLTR_EL3_EA_ENABLE          1 << 3
#define SCLTR_EL3_EA_DISABLE         0 << 3
#define SCLTR_EL3_RES_BIT_4_5        1 << 4
#define SCLTR_EL3_RES_BIT_6          0 << 6
#define SCLTR_EL3_SMD_ENABLE         0 << 7
#define SCLTR_EL3_SMD_DISABLE        1 << 7
#define SCLTR_EL3_HCE_ENABLE         1 << 8
#define SCLTR_EL3_HCE_DISABLE        0 << 8
#define SCLTR_EL3_SIF_P              0 << 9
#define SCLTR_EL3_SIF_NP             1 << 9

#ifndef __ASSEMBLER__
#define SCTLR_EL3                    SCLTR_EL3_AARCH64_MODE_EL2 | \
                                     SCLTR_EL3_NS | \
                                     SCLTR_EL3_IRQ_DISABLE | \
                                     SCLTR_EL3_FIQ_DISABLE | \
                                     SCLTR_EL3_EA_DISABLE | \
                                     SCLTR_EL3_RES_BIT_4_5 | \
                                     SCLTR_EL3_RES_BIT_6 | \
                                     SCLTR_EL3_SMD_DISABLE | \
                                     SCLTR_EL3_HCE_ENABLE | \
                                     SCLTR_EL3_SIF_P
#else
#define SCTLR_EL3                    #0b10110110001          
#endif

#define SPSR_EL3_M                      0b1100
#define SPSR_EL3_AARCH64_ENABLE         0 << 4
#define SPSR_EL3_AARCH64_DISABLE        1 << 4 //Might not be correct
#define SPSR_EL3_RES                    0 << 5
#define SPSR_EL3_DISABLE_EL2_EXCEPTIONS 0b111 << 6

#ifndef __ASSEMBLER__
#define SPSR_EL3                     SPSR_EL3_M | \
                                     SPSR_EL3_AARCH64_ENABLE | \
                                     SPSR_EL3_RES | \
                                     SPSR_EL3_DISABLE_EL2_EXCEPTIONS
#else
#define SPSR_EL3                     #0b01111001100
#endif

#define HCR_EL2_AARCH64_ENABEL_EL1     1 << 31
#define HCR_EL2_SWIO_ENABLE_EL1        1 << 1

#ifndef __ASSEMBLER__
#define HCR_EL2                        HCR_EL2_AARCH64_ENABEL_EL1 | \
                                       HCR_EL2_SWIO_ENABLE_EL1
#else
#define HCR_EL2                        #0b10000000000000000000000000000010
#endif



#define SPSR_EL2_M                     0b0101
#define SPSR_EL2_AARCH64_ENABLE        0 << 4
#define SPSR_EL2_AARCH64_DISABLE       1 << 4
#define SPSR_EL2_RES                   0 << 5 //reserved
#define SPSR_EL2_ENABLE_EL1_EXCEPTIONS 0b1111 << 6

#ifdef __ASSEMBLER__
#define SPSR_EL2                       SPSR_EL2_M | \
                                       SPSR_EL2_AARCH64_ENABLE | \
                                       SPSR_EL2_ENABLE_EL1_EXCEPTIONS
#else
#define SPSR_EL2                       #0b1111000101
#endif

#define SPSR_EL1_ENABLE_EL0_EXCEPTIONS 0b1111 << 6

#ifdef __ASSEMBLER__
#define SPSR_EL1                       SPSR_EL1_ENABLE_EL0_EXCEPTIONS
#else
#define SPSR_EL1                       #0b1111000000
#endif

#ifndef __ASSEMBLER__


    void read_sys_reg_config(u32* pValue, u32 regIndex);
#endif
