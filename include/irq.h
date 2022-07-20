#pragma once

#define INTR_MASK_ALL                    0xffffffff

#define SYNC_EXCEPTION                   0x1
#define ASYNC_EXCEPTION                  0x2
#define SYNC_EXCEPTION_SYSTEM_CALL       0x3

#define TIMER_INTERRUPT                  (1 << 1)
#define UART_RECEIVE_INTERRUPT           (1 << 19)

#ifndef __ASSEMBLER__

struct IrqController
{
    u32 irq_pending;
    u32 reserved[3];
    u32 irq_enable_reg1;
    u32 irq_enable_reg2;
    u32 irq_enable_basic;
    u32 irq_disable_reg1; //irq 0 - 31
    u32 irq_disable_reg2; //irq 32 - 63
    u32 irq_disable_basic;
};

struct TrapFrame {
    u64 x0;
    u64 x1;
    u64 x2;
    u64 x3;
    u64 x4;
    u64 x5;
    u64 x6;
    u64 x7;
    u64 x8;
    u64 x9;
    u64 x10;
    u64 x11;
    u64 x12;
    u64 x13;
    u64 x14;
    u64 x15;
    u64 x16;
    u64 x17;
    u64 x18;
    u64 x19;
    u64 x20;
    u64 x21;
    u64 x22;
    u64 x23;
    u64 x24;
    u64 x25;
    u64 x26;
    u64 x27;
    u64 x28;
    u64 x29;
    u64 x30;
    u64 sp0;
    u64 trap_no;
    u64 esr;
    u64 elr;
    u64 spsr;
};


#define IRQCLR_REGS ((struct IrqController*) (PPL_BASE_ADDR + 0xB200))

void intr_contoller_init(void);
void enable_irq(void);

u32 query_irq_number(void);

#endif
/**
 * Device Interrupt
 * 
 * Interrupt Controller
 * It needs to use interrupt controller to enable specific hardware interrupt source, or
 * interrupt willbe ignore otherwise.
 * 
 * IC is a memory-mapped device. Control registers are memory-mapped to config IC>
 * 
 * To enable interrupt
 * 1. enable interrupt on this core (globally) by setting DIAF bit of PSTATE Register
 *      DAIF register is 64-bit register and used to set DIAF bits of PSTATE
 *      MRS/MSR <number> daifclr to read/write PSTATE.DIAF bits
 *      D - Debug Assertion/exception
 *      F - Fast Interrupt
 *      I - External Interrupt
 *      A - Exception
 * 
 * 2. enable device/hardware interrupt (e.g. UART, Timer)
 * 
 * 3. After serve the interrupt, it needs to clear interrupt bits
 */