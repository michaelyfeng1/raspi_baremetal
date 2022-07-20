#pragma once

#include "types.h"

struct Uart
{
    reg32 u0_dr;
    reg32 reserved[5];
    reg32 u0_fr;
    reg32 reserved1[2];  
    reg32 u0_ibrd;
    reg32 u0_fbrd;
    reg32 u0_lcrh;
    reg32 u0_cr;
    reg32 reserved2;
    reg32 u0_imsc;
    u16   reserved3;
    reg32 u0_icr;
} __attribute__((packed));

#define UART_REGS ((struct Uart*)(PPL_BASE_ADDR + 0x201000))

void uart_init();
void uart_send(u8 c);
void uart_irq_handler(void);