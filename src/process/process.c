#include "types.h"
#include "utils.h"
#include "irq.h"
#include "mm.h"
#include "process.h"
#include "print.h"
#include "debug.h"

extern u64 read_pgd_ttbr0(void);
extern u64 read_pgd_ttbr1(void);
extern void start_process(struct TrapFrame* tf);
extern void flush_ttrb0(u64 tlb_addr);
extern void flush_ttrb1(u64 tlb_addr);
extern void flush_tlb(void);

static ProcessContext_t process_table[NUM_PROCESS] = { nullptr};
static u32 registered_pid = 1;


static ProcessContext_t* find_unused_process(void)
{
    ProcessContext_t* process = nullptr;

    for (int i = 0; i < NUM_PROCESS; i++) 
    {
        if (process_table[i].state == PROC_UNUSED) 
        {
            process = &process_table[i];
            break;
        }
    }

    return process;
}

static void init_idle_process(void)
{
    ProcessContext_t *process;

    process = find_unused_process();
    ASSERT(process == &process_table[0]);

    process->state = PROC_RUNNING;
    process->pid = 0;
    process->page_table = P2V(read_pgd_ttbr0());
}

void process_test(void); 

static ProcessContext_t* alloc_new_process(void)
{
    ProcessContext_t *process;

    process = find_unused_process();
    ASSERT(process == &process_table[1]);

    process->stack_start = (uint64_t)kalloc();
    ASSERT(process->stack_start != 0);

    memset((u8*)process->stack_start, 0, PAGE_SIZE);

    process->state = PROC_INIT;
    process->pid = registered_pid++;

    process->tf = (struct TrapFrame*)(process->stack_start + PAGE_SIZE - sizeof(struct TrapFrame));
    
    process->tf->elr = (u64)process_test;
    process->tf->spsr = 0b1111000101;

    return process;
}

static void init_user_process(void)
{
    ProcessContext_t *process;

    process = alloc_new_process();
    ASSERT(process != nullptr);

    // ASSERT(setup_process_page_table((uint64_t)process->page_table, "INIT.BIN"));
}

void launch(void)
{
    // switch_vm(process_table[1].page_table);
    // flush_ttrb0(V2P(process_table[1].page_table));

    flush_ttrb1(V2P(process_table[1].page_table));
    // flush_tlb();
    start_process(process_table[1].tf);
}

void init_process(void)
{
    init_idle_process();
    init_user_process();

    launch();
}

//test function

void store32(unsigned long address, unsigned long value)
{
    *(unsigned long *) address = value;
}

unsigned long load32(unsigned long address)
{
    return *(unsigned long *) address;
}

void print_f(int x)
{
    printk("Kernel Hello Word %d\n", x);
}

void process_test()
{
    printk("Kernel Hello Word\n");

    int x = x + 1;
    int y = x * 3;
    int z = y / x ;
    printk("Kernel Hello Word\n");
    printk("Kernel Hello Word\n");
    printk("%d\ %d %d\n", x, y , z);
    print_f(y);
}

void launch_cores()
{
extern u64 _cpu1_mailbox_notify;
extern u64 _cpu2_mailbox_notify;
extern u64 _cpu3_mailbox_notify;
extern void _start();

    *((u64*)&_cpu1_mailbox_notify) = (u64)_start;
    __asm__ volatile ("sev");

    *((u64*)&_cpu2_mailbox_notify) = (u64)_start;
    __asm__ volatile ("sev");
    
    *((u64*)&_cpu3_mailbox_notify) = (u64)_start;
    __asm__ volatile ("sev");
}