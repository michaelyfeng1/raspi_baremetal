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
    
    timer_init();
    syscall_init();
    // frame_buffer_init();
    // enable global interrupt
    enable_irq();
    
    // draw_string(200, 200, "Hello World\n", 0x05);
    // init_fs();
    // void* buf = kalloc();
    // switch_el0();
    // load_file("TEST.BIN", (u64)(buf)); 
    
    // printk("File data: %s\r\n", buf);
    
    // draw_rectangle();
    
    set_core_1();
    init_process();


    while(true);
}