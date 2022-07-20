.equ CALLEE_SAVE_SIZE,      12 * 8

.global __swap
__swap:
    //Save current call stack (only callee saved register based on convention)
    sub sp, sp, CALLEE_SAVE_SIZE
    
    //Callee save reigsters
    stp x19, x20, [sp, #(16 * 0)]
    stp x21, x22, [sp, #(16 * 1)]
    stp x23, x24, [sp, #(16 * 2)]
    stp x25, x26, [sp, #(16 * 3)]
    stp x27, x28, [sp, #(16 * 4)]
    stp x29, x30, [sp, #(16 * 0)]

    //x0 = p0.context_content_on_stack_start
    // x1 = p1.context_content_on_stack_start
    mov x2, sp
    str x2, [x0]
    mov sp, x1

    ldp	x19, x20, [sp, #(16 * 0)]
	ldp	x21, x22, [sp, #(16 * 1)]
	ldp	x23, x24, [sp, #(16 * 2)]
	ldp	x25, x26, [sp, #(16 * 3)]
	ldp	x27, x28, [sp, #(16 * 4)]
	ldp	x29, x30, [sp, #(16 * 5)]
    add sp,  sp,  #(12 * 8)

    // x30 is ra register and ret will set pc = x30(ra)
    // ra is set to trap_ return

    ret



/*
    SVC intruction trap CPU to kernel mode ( = el1 on aarch64)
    or Timer interrupt traps CPU to kerne mode
    trap handler will switch to corresponding handler
    Function calls = stack saves
 */