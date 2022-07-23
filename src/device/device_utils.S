.section .text

.global enable_timer
enable_timer:
    stp x29, x30, [sp, #-16]!

    bl read_system_clk_feq
    mov x1, #100
    udiv x0, x0, x1
    bl set_timer_interval

    mov x0, #1 
    msr CNTP_CTL_EL0, x0 //Control register for the EL1 physical timer = Enable Timer

    ldp x29, x30, [sp], #16

    ret

.global read_system_clk_feq
read_system_clk_feq:
    mrs x0, CNTFRQ_EL0
    ret

.global set_timer_interval
set_timer_interval:
    msr CNTP_TVAL_EL0, x0
    ret

.equ INTR_CNTL_REG_CORE_0_ADDR,   0x000040000040
.equ INTR_STATUS_REG_CORE_0_ADDR, 0x000040000060 //interrupt source register

.global enable_timer_irq
enable_timer_irq:
    ldr x0, =INTR_CNTL_REG_CORE_0_ADDR //pc-relative addressing pseduo-instruction
    mov x1, #(1 << 1)
    
    str x1, [x0]
    
    ret

.global read_intr_status
read_intr_status:
    ldr x1, =INTR_STATUS_REG_CORE_0_ADDR
    ldr x0, [x1]

    ret

.global enable_irq
enable_irq:
    msr daifclr, #2
    ret

.global read_timer_status
read_timer_status:
    mrs x0, CNTP_CTL_EL0 //Counter-Timer Physical Timer Control Registers
    // Control register for the el1 physical timer
    ret


