#include "types.h"
#include "print.h"

extern void __lock_acq_mem(u64 mem_addr);
extern void __lock_rel_mem(u64 mem_addr);

u64 console_lock_mem = 0;
u64 core_0_lock_mem = 0;
u64 core_1_lock_mem = 0;
u64 core_2_lock_mem = 0;
u64 core_3_lock_mem = 0;

static void acquire_lock(u64 addr)
{
    __lock_acq_mem(addr);

}

static void release_lock(u64 addr)
{
    __lock_rel_mem(addr);
}

void acquire_console_lock(void)
{
    acquire_lock((u64)&console_lock_mem);
}

void release_console_lock(void)
{
    release_lock((u64)&console_lock_mem);
}