#pragma once

#include "mm.h"

#if RPI_VERSION == 4
#define PPL_BASE_ADDR           P2V(0xFE000000)
#elif RPI_VERSION == 3
#define PPL_BASE_ADDR           P2V(0x3F000000)
#else
#define PPL_BASE_ADDR           0x0
#endif

struct AuxRegs {
    reg32 irq_status;
    reg32 enables;
    reg32 reserved[14];
    reg32 mu_io;
    reg32 mu_ier;
    reg32 mu_iir;
    reg32 mu_lcr;
    reg32 mu_mcr;
    reg32 mu_lsr;
    reg32 mu_msr;
    reg32 mu_scratch;
    reg32 mu_control;
    reg32 mu_status;
    reg32 mu_baud_rate;
};

#define AUX_REGS ((struct AuxRegs *)(PPL_BASE_ADDR + 0x00215000))

/**
 * @brief Auxiliaries Notes
 * The device has three auxiliary peripherials
 * 1. UART
 * 2. Two SPI
 * One shared control register map to control the peripherials and they share same interrupt souruce id
    The following is control registers map
    Common Control Registers    
        0x7E21 5000 AUX_IRQ Auxiliary Interrupt status 3
        0x7E21 5004 AUX_ENABLES Auxiliary enables 3

        AUXIRQ is used to check any pending interrupts has been asserted
        AUXENB is used to enable the three modules
            bit - 2 set to enable SPI 2
            bit - 1 set to enable SPI 1
            bit - 0 set to enable mini UART
    UART AUX Registers
        0x7E21 5040 AUX_MU_IO_REG Mini Uart I/O Data 8
        0x7E21 5044 AUX_MU_IER_REG Mini Uart Interrupt Enable 8
        0x7E21 5048 AUX_MU_IIR_REG Mini Uart Interrupt Identify 8
        0x7E21 504C AUX_MU_LCR_REG Mini Uart Line Control 8
        0x7E21 5050 AUX_MU_MCR_REG Mini Uart Modem Control 8
        0x7E21 5054 AUX_MU_LSR_REG Mini Uart Line Status 8
        0x7E21 5058 AUX_MU_MSR_REG Mini Uart Modem Status 8
        0x7E21 505C AUX_MU_SCRATCH Mini Uart Scratch 8
        0x7E21 5060 AUX_MU_CNTL_REG Mini Uart Extra Control 8
        0x7E21 5064 AUX_MU_STAT_REG Mini Uart Extra Status 32
        0x7E21 5068 AUX_MU_BAUD_REG Mini Uart Baudrate 16
        
        0x7E21 5080 AUX_SPI0_CNTL0_REG SPI 1 Control register 0 32
        0x7E21 5084 AUX_SPI0_CNTL1_REG SPI 1 Control register 1 8
        0x7E21 5088 AUX_SPI0_STAT_REG SPI 1 Status 32
        0x7E21 5090 AUX_SPI0_IO_REG SPI 1 Data 32
        0x7E21 5094 AUX_SPI0_PEEK_REG SPI 1 Peek 16
        
        0x7E21 50C0 AUX_SPI1_CNTL0_REG SPI 2 Control register 0 32
        0x7E21 50C4 AUX_SPI1_CNTL1_REG SPI 2 Control register 1 8
        0x7E21 50C8 AUX_SPI1_STAT_REG SPI 2 Status 32
        0x7E21 50D0 AUX_SPI1_IO_REG SPI 2 Data 32
        0x7E21 50D4 AUX_SPI1_PEEK_REG SPI 2 Peek 16
 */
