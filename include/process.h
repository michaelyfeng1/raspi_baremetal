#pragma once

typedef struct ProcessContext
{
    u32 pid;
    u32 state;
    u64 page_table;
    u64 stack_start;
    u64 context_content_kstack_start;
    struct TrapFrame* tf;
}ProcessContext_t;

// typedef struct ProcessControl
// {
//     ProcessContext_t* current_process;
//     HeadList_t ready_list;
// }ProcessControl_t;

#define PROCESS_STACK_SIZE_IN_BYTES     (2 * 1024 * 1024) // 2MB = 1 Page
#define NUM_PROCESS                     4
#define PROC_UNUSED                     0
#define PROC_INIT                       1
#define PROC_RUNNING                    2
#define PROC_READY                      3

void init_process(void);

void set_core_1(void);