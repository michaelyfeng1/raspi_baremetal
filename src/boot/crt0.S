
.section .text.boot

.global _start
_start:
    mrs x0, mpidr_el1
    and x0, x0, #0b11
    cmp x0, #0
    beq _core_0
    cmp x0, #1
    beq _core_1
    cmp x0, #2
    beq _core_2
    cmp x0, #3
    beq _core_3

_invalid:
    wfe
    b _invalid

_core_1:
    wfe
    ldr x0, =kstack_core_1
    add x0, x0, #(16 * 1024)
    mov sp, x0
    b _core_1_start
_core_1_start:
    b main_core_1

_core_2:
    wfe
    ldr x0, =kstack_core_2
    add x0, x0, #(16 * 1024)
    mov sp, x0
    b _core_2_start
_core_2_start:
    b main_core_2

_core_3:
    wfe
    ldr x0, =kstack_core_3
    add x0, x0, #(16 * 1024)
    mov sp, x0
    b _core_3_start
_core_3_start:
    b main_core_3

_core_0:
    mrs x0, CurrentEL
    and x0, x0, #0b1100 // C is el3;  8 is el2;  4 is el1
    cmp x0, #0b1000

    blt el1_entry
    beq _switch_el2_to_el1
    bgt _switch_el3_to_el2
_switch_el3_to_el2:
   mov x0, #0b10110110001
    msr scr_el3, x0

    mov x0, #0b01111001100
    msr spsr_el3, x0

    adr x0, _switch_el2_to_el1
    msr elr_el3, x0

    eret

_switch_el2_to_el1:

    msr sctlr_el1, xzr
    mov x0, #(1 << 31)
    msr hcr_el2, x0

    mov x0, #0b1111000101 //mode file is 0101 (el1)
    msr spsr_el2, x0

    adr x0, el1_entry
    msr elr_el2, x0

    eret

el1_entry:
    // set up kernel stack
    ldr x0, =kstack_core_0
    add x0, x0, #(16 * 1024)
    mov sp, x0

    //bl set_up_el1_pt
    //bl set_up_el0_pt
    //bl enable_mmu

    ldr x0, =__sbss
    ldr x1, =__ebss
    sub x2, x1, x0
    mov x1, #0
    bl memset

    ldr x0, =irq_vec_table
    msr vbar_el1, x0

    //mov x0, #0xffff000000000000
    //add sp, sp, x0
    //ldr x0, =main
    //br x0

    bl main

start_cpu1:
    b start_cpu1

//.globl __start1
//__start1:
//    ldr x0,=spin_cpu1
//    mov x1, #0x40000000
//    str w0, [x1, #0x90]
//    sev
//    b __start1
    

.global kstack_core_0
kstack_core_0:
    .space 16 * 1024, 0

.global kstack_core_1
kstack_core_1:
    .space 16 * 1024, 0

.global kstack_core_2
kstack_core_2:
    .space 16 * 1024, 0

.global kstack_core_3
kstack_core_3:
    .space 16 * 1024, 0


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