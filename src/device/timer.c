#include "types.h"
#include "irq.h"
#include "print.h"

extern void enable_timer(void);
extern void enable_timer_irq(void);
extern void set_timer_interval(u64 value);
extern u64 read_intr_status(void);
extern u64 read_timer_status(void);
extern u64 read_system_clk_feq(void);

static u64 ticks = 0;
static u64 timer_interval = 0;

void timer_init(void)
{
    timer_interval = read_system_clk_feq() / 100;

    enable_timer(); //Timer Configuration
    enable_timer_irq();
}

void timer_irq_handler(void)
{
    if(read_intr_status() & (1 << 1))
    {
        if(read_timer_status() & (1 << 2))
        {
            if(ticks % 100 == 0)
            {
                printk("Timer Interrupt\n");
            }
            
            set_timer_interval(timer_interval);
            ticks++;
        }
    }
}