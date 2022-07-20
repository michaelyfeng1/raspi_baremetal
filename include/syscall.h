#pragma once

#define MAX_NUM_SYSCALL_SUPPORT         20

#define SYS_CALL_WRITE_CONSOLE          0


#define SYS_CALL_STATUS_SUCESS          1
#define SYS_CALL_STATUS_FAIL            0

typedef u32 (*SYSCALL)(u64* argv);

void syscall_init(void);
void system_call_handler(struct TrapFrame* tf);