#include "types.h"
#include "list.h"
#include "process.h"
#include "mm.h"

extern void __swap(u64* curr_proc_ptr, u64 next_proc_ptr);
extern void flush_ttrb0(u64 tlb_addr);

void swap_process(ProcessContext_t* curr_proc, ProcessContext_t* next_proc)
{
    flush_ttrb0(V2P(next_proc->page_table));
    __swap((u64*)curr_proc->context_content_kstack_start, (u64)next_proc->context_content_kstack_start);
}

void schedule(void)
{
    ProcessContext_t* curr_proc = nullptr;
    ProcessContext_t* next_proc = nullptr;

    // HeadList_t*
    
    next_proc->state = PROC_RUNNING;


    swap_process(curr_proc, next_proc);
}