#include "types.h"
#include "aux.h"
#include "mini_uart.h"
#include "uart.h"
#include "print.h"

void mini_uart_init(void)
{
    AUX_REGS->irq_status = 0b000; //reset
    AUX_REGS->enables = 0b1;
    AUX_REGS->mu_control = 0;
    AUX_REGS->mu_ier = 0xD;
    AUX_REGS->mu_lcr = 3;
    AUX_REGS->mu_mcr = 0;

// #if RPI_VERSION == 3
    AUX_REGS->mu_baud_rate = 270; // = 115200 @ 250 Mhz
// #endif

// #if RPI_VERSION == 4
//     AUX_REGS->mu_baud_rate = 541; // = 115200 @ 500 Mhz
// #endif
    AUX_REGS->mu_control = 3;

    // mini_uart_send('\n');
    mini_uart_send('r');
    mini_uart_send('\n');
}

void mini_uart_send(u8 c)
{
    // while(!(AUX_REGS->mu_lsr & 0x20));
    while(!(AUX_REGS->mu_lsr & 0x20));

    AUX_REGS->mu_io = c;
}

// Configure UART controller/device
void uart_init()
{
    UART_REGS->u0_cr = 0;
    UART_REGS->u0_ibrd = 26;
    UART_REGS->u0_fbrd = 0;
    // UART_REGS->u0_lcrh = (1 << 4) | (1 << 5) | ( 1 << 6);
    // FEN is fifo_enable bit
    // WLEN is set to use 8 bits
    UART_REGS->u0_lcrh = (1 << 5) | ( 1 << 6); //disable FIFO buffer
    // UART_REGS->u0_imsc = 0;
    UART_REGS->u0_imsc = 1 << 4; //enable receiving interrupt
    UART_REGS->u0_cr = ( 1 << 8) | ( 1 << 9) | 1;
    //enable uart device
    //enable transmit
    //enable receive
}

void uart_send(u8 c)
{
    // while (UART_REGS->u0_fr & ( 1 << 5));
    while (UART_REGS->u0_fr & ( 1 << 3));

    UART_REGS->u0_dr = c;
}

void uart_irq_handler()
{
    u8 c = UART_REGS->u0_dr;

    uart_send(c);

    UART_REGS->u0_icr = ( 1 << 4);
}