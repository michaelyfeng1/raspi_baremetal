
#include "types.h"
#include "timer.h"
#include "irq.h"
#include "syscall.h"
#include "uart.h"
#include "print.h"

extern u64 read_intr_status();

// void el1_dispatcher(u64 excp_id, u64 esr_val, u64 elr_val)
void el1_dispatcher(struct TrapFrame *tf)
{
    u32 irq_pending_bits = 0;

    switch (tf->trap_no)
    {
        case 0:
            uart_send('1');
            break;
        case SYNC_EXCEPTION:
            uart_send('2');
            while(1);
            break;
        case ASYNC_EXCEPTION:
        {

            if (read_intr_status() == TIMER_INTERRUPT)
            {
                timer_irq_handler();
            }
            else
            {
                if(query_irq_number() & ((u64)UART_RECEIVE_INTERRUPT))
                {
                    uart_irq_handler();
                }
           
            }

            // irq_pending_bits = query_irq_number();

            // if (irq_pending_bits & ( 1 << 19))
            // {
            //     printk("Keyboard\n");
            //     uart_irq_handler();
            // }
            // else
            // {
                // timer_irq_handler();
            // }
            break;
        }
        case SYNC_EXCEPTION_SYSTEM_CALL:
            system_call_handler(tf);
            break;
        default:
            break;
    }
}

/**
 * If exception is raised at level of el0, el must boost to el1
 * 
 * Interrupt controller will handle incoming interrupt request and send to CPU cores
 * Interrupt controller enable/disable interrupts and determine interrupt source ID
 * 
 * Enable IRQ Register Set = {IRQs1, IRQs2, Basic IRQs}
 * Disable IRQ Register Set = {IRQs1, IRQs2, Basic IRQs}
 * Pending IRQ Register Set = {IRQs1, IRQs2, Basic IRQs}
 * 
 */