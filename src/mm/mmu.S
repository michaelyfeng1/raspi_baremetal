.equ PAGE_SIZE, (2 * 1024 * 1024)

.section .text

.global set_up_el1_pt
set_up_el1_pt:
    adr x0, pgd_ttbr_el1
    adr x1, pud_ttbr_el1

    orr x1, x1, #3
    str x1, [x0]

    adr x0, pud_ttbr_el1
    adr x1, pmd_ttbr_el1
    orr x1, x1, #3
    str x1, [x0]

    //map pa from 0x0 to 0x340000
    mov x2, #0x34000000
    adr x1, pmd_ttbr_el1
    mov x0, #(1 << 10 | 1 << 2 | 1 << 0)

loop1:
    str x0, [x1], #8
    add x0, x0, #PAGE_SIZE
    cmp x0, x2
    blo loop1

    mov x2, #0x3CF00000
    mov x0, #0x3C000000

    adr x3, pmd_ttbr_el1
    lsr x1, x0, #(21 - 3)
    add x1, x1, x3

    orr x0, x0, #1
    orr x0, x0, #(1 << 10) //bit 2 is set 0 => device memory

loop_fb:
    str x0, [x1], #8
    add x0, x0, #PAGE_SIZE
    cmp x0, x2
    blo loop_fb

    mov x2, #0x40000000
    mov x0, #0x3f000000

    adr x3, pmd_ttbr_el1
    lsr x1, x0, #(21 - 3)
    add x1, x1, x3

    orr x0, x0, #1
    orr x0, x0, #(1 << 10) //bit 2 is set 0 => device memory

loop2:
    str x0, [x1], #8
    add x0, x0, #PAGE_SIZE
    cmp x0, x2
    blo loop2

    // above 1G

    adr x0, pud_ttbr_el1
    add x0, x0, #8
    adr x1, pmd_ttbr2_el1
    orr x1, x1, #3
    str x1, [x0]

    mov x2, #0x41000000
    mov x0, #0x40000000

    adr x1, pmd_ttbr2_el1
    orr x0, x0, #1
    orr x0, x0, #(1 << 10)

loop3:
    str x0, [x1], #8
    add x0, x0, #PAGE_SIZE

    cmp x0, x2
    blo loop3

.global set_up_el0_pt
set_up_el0_pt:
    adr x0, pgd_ttbr_el0
    adr x1, pud_ttbr_el0

    orr x1, x1, #3
    str x1, [x0]

    adr x0, pud_ttbr_el0
    adr x1, pmd_ttbr_el0
    
    orr x1, x1, #3
    str x1, [x0]

    adr x1, pmd_ttbr_el0
    mov x0, #(1 << 10 | 1 << 2 | 1)
    str x0, [x1]

    ret 

//page table has to be 4k aligned
.balign 4096
.global pgd_ttbr_el1
pgd_ttbr_el1:
    .space 4096  //1 page size

pud_ttbr_el1:
    .space 4096

pmd_ttbr_el1:
    .space 4096

pmd_ttbr2_el1:
    .space 4096

.balign 4096
.global pgd_ttbr_el0
pgd_ttbr_el0:
    .space 4096  //1 page size

pud_ttbr_el0:
    .space 4096

pmd_ttbr_el0:
    .space 4096

/**
    page size = 2MB

    page entry = 64 bytes
    pgd, pud, and pmd have 1 page size and 512 enties in turn


    1 entry in pmd -> covers 2MB range
    1 pud entry -> covers 512 * 2 MB  = 1 GB range


    Kernel is loaded to first 2M phyiscal memory


    Virtual Memory Layout                           Physcak Memory Layour

    | --------- Kernel Space End                    | ----------- Peripheral Area End
    |
    |
    |                                               | ----------- File System End
    |
    |
    |                                               | ----------- Free Memory End
    |
    |
    |
    |
    |
    | --------- User Space End
    |
    |
    |                                               | ----------- 2M Kernel Binary
    |
    | --------- User Space Start                    | -----------      

    Kernel Space includes:
    OS Kernel
    Free Memory (2M 0 0x30000000)
    File System : 0x30000000 - 9x34000000 (Memory End)

 */