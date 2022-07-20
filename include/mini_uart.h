#pragma once

#include "types.h"

void mini_uart_init(void);
// u8 mini_uart_recv(void);
void mini_uart_send(u8 c);
// void mini_uart_send_string(u8* str);

/**
 * @brief Mini UART Notes
 * 
 * UART AUX Registers
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

        
 * 
 */