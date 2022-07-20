.equ FS_BASE, 0xffff000030000000
.equ FS_SIZE, 101*16*63*512

.section .text.boot

//.section ".text"

// #include "config.h"
//#include "file.h"

.global _start
_start:
    //single core: only hart 0 jump to kernel
    mrs x0, mpidr_el1
    and x0, x0, #0b11
    cbnz x0, _pause_hart

    mrs x0, CurrentEL
    and x0, x0, #0b1100
    cmp x0, #0b1000

    blt _switch_to_current
    beq _switch_el2_to_el1
    bgt _switch_el3_to_el2

    b _pause_hart

_pause_hart:
    wfe
    b _pause_hart

_switch_el3_to_el2:
    mov x0, #0b10110110001
    msr scr_el3, x0

    mov x0, #0b01111001100
    msr spsr_el3, x0

    adr x0, _switch_el2_to_el1
    msr elr_el3, x0

    eret

_switch_el2_to_el1:
//    mov x0, #(1 << 31)
//    orr x0, x0, #(1 << 1)
//    msr hcr_el2, x0

//    mov     x3, #0x0800
//    movk    x3, #0x30d0, lsl #16
//    msr     sctlr_el1, x3
    msr sctlr_el1, xzr
    mov x0, #(1 << 31)
    msr hcr_el2, x0

    mov x0, #0b1111000101 //mode file is 0101 (el1)
    msr spsr_el2, x0

    adr x0, _switch_to_current
    msr elr_el2, x0

    eret

_switch_to_current:
    //ldr x0, =_start
    //mov sp, x0
    mov sp, #0x80000

    bl set_up_el1_pt
    bl set_up_el0_pt
    bl enable_mmu

    ldr x0, =FS_BASE
    ldr x1, =__sbss
    ldr x2, =FS_SIZE
    bl memcpy

    ldr x0, =__sbss
    ldr x1, =__ebss
    sub x2, x1, x0
    mov x1, #0
    bl memset

    ldr x0, =irq_vec_table
    msr vbar_el1, x0

    mov x0, #0xffff000000000000
    add sp, sp, x0

    ldr x0, =main
    br x0
/** 
.global switch_el0
switch_el0:
    //mov x0, SPSR_EL1
    mov x0, #0b01111000000
    msr spsr_el1, x0
    //msr spsr_el1, x0

    adr x0, el0_entry
    msr elr_el1, x0
    eret

el0_entry:
    wfi
    b el0_entry
**/

/** 
    Exception:
        PSTATE -> SPSR_ELn
        Return Address -> ELR_ELn

    Exception return:
        SPSR_ELn -> PSTATE
        ELR_ELn -> Return Address
*/