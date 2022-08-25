#include "types.h"
#include "draw.h"
#include "mailbox.h"
#include "lock.h"
#include "command.h"
#include "print.h"

static Command_t command_ring[COMMAND_NUM_MAX];


void main_core_0(void)
{
    acquire_console_lock();
    printk("core 0\n");
    draw_rectangle(10, 285, 0x3);
    draw_string(20, 305, "Core 0 Hello World\n", 0x05);
    release_console_lock();

    while(1);
}

void main_core_1(void)
{
    acquire_console_lock();
    printk("core 1\n");
    draw_rectangle(10, 30, 0xd);
    draw_string(20, 50, "Core 1 Hello World\n", 0x05);
    release_console_lock();

    while(1);
}

void main_core_2(void)
{
    acquire_console_lock();
    printk("core 2\n");
    draw_rectangle(10, 115, 0xc);
    draw_string(20, 135, "Core 2 Hello World\n", 0x05);
    release_console_lock();

    while(1);
}

void main_core_3(void)
{
    acquire_console_lock();
    printk("core 3\n");
    draw_rectangle(10, 200, 0x2);
    draw_string(20, 220, "Core 3 Hello World\n", 0x05);
    release_console_lock();

    while(1);
}

/**
 * @brief Core
 * Qemu loads kernel as linux kernel module and follow linux loading mechanism
 * Core 0 is the bootstrap core
 * All other cores will be put in slient-mode/power-saving mode
 * 
 * Mailbox reigstes assoicted to each core are used to wake up core
 * By writng callback address to mailbox register will wake up associated core
 *
 *
 * 
 * 0xd8 core 0
 * 0xe0 core 1
 * 0xe8 core 2
 * 0xf0 core 3
 */