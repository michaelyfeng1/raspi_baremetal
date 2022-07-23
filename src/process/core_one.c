#include "types.h"
#include "print.h"

void main_core_1(void)
{
    printk("core 1\n");
    while(1);
}

void main_core_2(void)
{
    printk("core 2\n");
    while(1);
}

void main_core_3(void)
{
    printk("core 3\n");
    while(1);
}