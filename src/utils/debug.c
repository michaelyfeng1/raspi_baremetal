#include "types.h"
#include "irq.h"
#include "debug.h"
#include "print.h"

void print_trapframe(struct TrapFrame* tf)
{
    printk(" x0 : %x\n", tf->x0);
    printk(" x1 : %x\n", tf->x1);
    printk(" x2 : %x\n", tf->x2);
    printk(" x3 : %x\n", tf->x3);
    printk(" x4 : %x\n", tf->x4);
    printk(" x5 : %x\n", tf->x5);

    printk(" x6 : %x\n", tf->x6);
    printk(" x7 : %x\n", tf->x7);
    printk(" x8 : %x\n", tf->x8);
    printk(" x9 : %x\n", tf->x9);
    printk(" x10 : %x\n", tf->x10);
    printk(" x11 : %x\n", tf->x11);

    printk(" x12 : %x\n", tf->x12);
    printk(" x13 : %x\n", tf->x13);
    printk(" x14 : %x\n", tf->x14);
    printk(" x15 : %x\n", tf->x15);
    printk(" x16 : %x\n", tf->x16);
    printk(" x17 : %x\n", tf->x17);

    printk(" sp0 : %x\n", tf->sp0);
    printk(" trap_no : %x\n", tf->trap_no);
    printk(" esr : %x\n", tf->esr);
    printk(" elr : %x\n", tf->elr);
    printk(" spsr : %x\n", tf->spsr);
}

void error_check(char *file, uint64_t line)
{
    printk("\r\n-----------------------------------\r\n");
    printk("                    ERROR CHECK");
    printk("\r\n-----------------------------------\r\n");
    printk("Assertion Failed [%s: %u]\r\n", file, line);

    while (1) { }
}