ENTRY(_start)

/**
 * Memory Layout
**/

/* MEMORY */
/* { */
    /* RAM (rwx) : ORIGIN = 0x80000, LENGTH = 256M */
    /* RAM (rwx) : ORIGIN = 0xffff000000080000, LENGTH = 1024M */
/* } */

SECTIONS
{
    . = 0xe0;
    _cpu1_mailbox_notify = .;

    . = 0xe8;
    _cpu2_mailbox_notify = .;

    . = 0xf0;
    _cpu3_mailbox_notify = .;

    /* . = ORIGIN(RAM); */
    . = 0x000000080000;
    __kernel_start = .;
    /* PROVIDE (_kernel_start = .); */

    /* .text.boot (__kernel_start) : 
    {
        ./src/boot/crt0.o (.text.boot)
    }

    . += 0x1000; */

    .text : 
    {
        /* PROVIDE (_stext = .); */
        * (.text.boot)
        * (.text)
        /* PROVIDE (_etext = .); */
    }

    . = ALIGN (4096);

    .rodata : 
    {
        * (.rodata)
    }

    . = ALIGN (4096);

    .data : 
    {
        /* PROVIDE (_sdata = .); */
        * (.data)
        /* __edata = .; */
        /* PROVIDE (_edata = .); */
    }
    /* . = ALIGN (4096); */
    /* __xsbss = .; */
    .bss :
    {
        /* PROVIDE (_sbss = .); */
        __sbss = .;
        * (.bss)
        /* PROVIDE (_ebss = .); */
        __ebss = .;
    }

    . = ALIGN(4096);
    /* PROVIDE (_kernel_end = .); */
    __kernel_end = .;
}

