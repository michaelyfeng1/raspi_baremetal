#pragma once

//memory

void memcpy(void *dst, void *src, u32 size);
void memset(void* dst, u32 value, u32 size);
int memcmp(void* dst, void* src, u32 size);

//cpu
void delay_cpu(u64 value);
u8 get_cur_el(void);