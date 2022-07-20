.equ CONTEXT_SIZE_IN_BYTES,  288


#include "irq.h"

.macro exception_enter swap_stack_not_taken
    sub sp, sp, #CONTEXT_SIZE_IN_BYTES
    stp x0, x1, [sp, #16 * 0]
    stp x2, x3, [sp, #16 * 1]
    stp	x4, x5, [sp, #16 * 2]
	stp	x6, x7, [sp, #16 * 3]
	stp	x8, x9, [sp, #16 * 4]
	stp	x10, x11, [sp, #16 * 5]
	stp	x12, x13, [sp, #16 * 6]
	stp	x14, x15, [sp, #16 * 7]
	stp	x16, x17, [sp, #16 * 8]
	stp	x18, x19, [sp, #16 * 9]
	stp	x20, x21, [sp, #16 * 10]
	stp	x22, x23, [sp, #16 * 11]
	stp	x24, x25, [sp, #16 * 12]
	stp	x26, x27, [sp, #16 * 13]
	stp	x28, x29, [sp, #16 * 14]
    
    mrs x0, spsr_el1
    and x0, x0, #0b1111
    cmp x0, 0
    bne \swap_stack_not_taken
    mrs x0, sp_el0
\swap_stack_not_taken:
	stp	x30, x0, [sp, #16 * 15] 
.endm


.macro exception_exit swap_stack_not_taken
    ldp	x30, x0, [sp, #16 * 15] 
    mrs x1, spsr_el1
    and x1, x1, #0b1111    
    cmp x1, 0
    bne \swap_stack_not_taken
    msr sp_el0, x0

\swap_stack_not_taken:
    ldp x0, x1, [sp, #16 * 0]
    ldp x2, x3, [sp, #16 * 1]
    ldp	x4, x5, [sp, #16 * 2]
	ldp	x6, x7, [sp, #16 * 3]
	ldp	x8, x9, [sp, #16 * 4]
	ldp	x10, x11, [sp, #16 * 5]
	ldp	x12, x13, [sp, #16 * 6]
	ldp	x14, x15, [sp, #16 * 7]
	ldp	x16, x17, [sp, #16 * 8]
	ldp	x18, x19, [sp, #16 * 9]
	ldp	x20, x21, [sp, #16 * 10]
	ldp	x22, x23, [sp, #16 * 11]
	ldp	x24, x25, [sp, #16 * 12]
	ldp	x26, x27, [sp, #16 * 13]
	ldp	x28, x29, [sp, #16 * 14]

	add	sp, sp, #CONTEXT_SIZE_IN_BYTES

    eret
.endm

.macro handler_entry
    mrs x1, esr_el1
    stp x0, x1, [sp, #(16 * 16)]

    mrs x0, elr_el1
    mrs x1, spsr_el1
    stp x0, x1, [sp, #(16 * 17)]

    mov x0, sp
.endm

.macro handler_exit
    ldp x0, x1, [sp, #(16 * 17)]
    msr elr_el1, x0
    msr spsr_el1, x1
.endm

.section .text
.global irq_vec_table

.balign 0x800 //2K aligned as required
irq_vec_table:
curr_el_sp0_sync:
    b error

.balign 0x80 // all handlers must be 32 instructions aligned
curr_el_sp0_irq:
    b error

.balign 0x80
curr_el_sp0_fiq:
    b error

.balign 0x80
curr_el_sp0_serror:
    b error

.balign 0x80
curr_el_spn_sync:
    b sync_handler

.balign 0x80 // all handlers must be 32 instructions aligned
curr_el_spn_irq:
    b irq_handler

.balign 0x80
curr_el_spn_fiq:
    b error

.balign 0x80
curr_el_spn_serror:
    b error

.balign 0x80
curr_el_aarch64_sync:
    b sync_handler

.balign 0x80 // all handlers must be 32 instructions aligned
curr_el_aarch64_irq:
    b irq_handler

.balign 0x80
curr_el_aarch64_fiq:
    b error

.balign 0x80
curr_el_aarch64_serror:
    b error

.balign 0x80
curr_el_aarch32_sync:
    b error

.balign 0x80 // all handlers must be 32 instructions aligned
curr_el_aarch32_irq:
    b error

.balign 0x80
curr_el_aarch32_fiq:
    b error

.balign 0x80
curr_el_aarch32_serror:
    b error

irq_handler:
    exception_enter irq_swap_stack_if_take
    mov x0, #ASYNC_EXCEPTION
    handler_entry
//    mrs x1, esr_el1
//    mrs x2, elr_el1 Truncated by TrapFrame
    bl el1_dispatcher
    handler_exit
    exception_exit irq_swap_exit_stack_if_take

sync_handler:
    exception_enter sync_swap_stack_if_take
//    mov x0, #SYNC_EXCEPTION
    mrs x0, esr_el1 //31 : 26  cause bits
    lsr x1, x0, #26
    cmp x1, #0b010101
    mov x2, #1
    mov x3, #3
    csel x0, x2, x3, ne

    handler_entry
//    mrs x1, esr_el1
//    mrs x2, elr_el1 Truncated by TrapFrame
    bl el1_dispatcher
    handler_exit
    exception_exit sync_exit_swap_stack_if_take


error:
    exception_enter error_swap_stack_if_take
    mov x0, #0
    bl el1_dispatcher
    exception_exit error_exit_swap_stack_if_take


.global start_process
start_process:
    mov sp, x0
    ldp x0, x1, [sp, #(16 * 17)]
    msr elr_el1, x0
    msr spsr_el1, x1
//    exception_exit start_process_swap_stack_if_take
    //ldp	x30, x0, [sp, #16 * 15] 
    exception_exit start_process_swap_stack_if_take
//    cmp x0, 0
//    bne \swap_stack_not_taken
    //msr sp_el0, x0
/*
    ldp x0, x1, [sp, #16 * 0]
    ldp x2, x3, [sp, #16 * 1]
    ldp	x4, x5, [sp, #16 * 2]
	ldp	x6, x7, [sp, #16 * 3]
	ldp	x8, x9, [sp, #16 * 4]
	ldp	x10, x11, [sp, #16 * 5]
	ldp	x12, x13, [sp, #16 * 6]
	ldp	x14, x15, [sp, #16 * 7]
	ldp	x16, x17, [sp, #16 * 8]
	ldp	x18, x19, [sp, #16 * 9]
	ldp	x20, x21, [sp, #16 * 10]
	ldp	x22, x23, [sp, #16 * 11]
	ldp	x24, x25, [sp, #16 * 12]
	ldp	x26, x27, [sp, #16 * 13]
	ldp	x28, x29, [sp, #16 * 14]

	add	sp, sp, #CONTEXT_SIZE_IN_BYTES

    eret

*/
/*
    Each handler is able to accommdate 32 instructions
    
    32-bit long instruction is 4 bytes and 32 * 4 = 128 bytes = 0x80 in hex

    If this handler is more than 32 instructions, it can jump to a C-handler

    Each prilleage level has 16 handlers

    eret instruction is used to return from interrupt handler, since there might be a prilleage
    swap during interrupt handling.

    eret = SPSR to PSTATE (state of processor)
         = ELR to PC (return address)
    
    When there is an interrupt occurs, hardware save current state of processor to SPSR_ELn and address
    to ELRn and eret instruction restore these values after return from interrupt

    When there is an interrupt, hardware will jump to corresponding handlers of handlers[el][id], so 
    handlers must be aligned with hardware design
 */