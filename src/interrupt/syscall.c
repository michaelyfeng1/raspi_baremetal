#include "types.h"
#include "irq.h"
#include "syscall.h"
#include "print.h"

static SYSCALL sys_call_tbl[MAX_NUM_SYSCALL_SUPPORT];

static u32 _sys_write(u64* argv)
{
    write_console((u8*)argv[0], argv[1]);

    return (u32)argv[1];
}

void syscall_init(void)
{
    sys_call_tbl[SYS_CALL_WRITE_CONSOLE] = _sys_write;
}

void system_call_handler(struct TrapFrame* tf)
{
    u64 sys_call_index = tf->x8; // saved in user mode
    u64 sys_call_param_count = tf->x0;
    u64 sys_call_arg_list_ptr = tf->x1;

    if (sys_call_index > MAX_NUM_SYSCALL_SUPPORT)
    {
        tf->x0 = SYS_CALL_STATUS_FAIL;

        return;
    }

    tf->x0 = sys_call_tbl[sys_call_index]((u64*)sys_call_arg_list_ptr);
}