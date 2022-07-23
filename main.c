#include "types.h"
#include "mini_uart.h"
#include "uart.h"
#include "mailbox.h"
#include "irq.h"
#include "timer.h"
#include "mm.h"
#include "draw.h"
#include "file.h"
#include "list.h"
#include "process.h"
#include "syscall.h"
#include "print.h"


extern void switch_el0(void);

int main(void)
{
    // mini_uart_init();
    uart_init();

    heap_mem_seg_init();
    
    intr_contoller_init();
    
    // timer_init();
    frame_buffer_init();
    // enable global interrupt
    // enable_irq();
    
    draw_string(200, 200, "Hello World\n", 0x05);
    
    draw_rectangle();
    
    launch_cores();

    while(true);
}