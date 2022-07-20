.equ MAIR_ATTR, (0x44 << 8) //Normal Memeory and Device Memory 0x44 and 0x0

.equ TCR_T0SZ,  (16)        //upper 16 bits
.equ TCR_T1SZ,  (16 << 16)
.equ TCR_TG0,   (0 << 14) //4K page granu
.equ TCR_TG1,   (0 << 30) //4K page granu
.equ TCR_VALUE, (TCR_TG0 | TCR_TG1 | TCR_T0SZ | TCR_T1SZ)

// Index filed of entry = 1 Normail Memory  = 0 Device Memory

// MMU
.section .text
.global enable_mmu
enable_mmu:
    adr x0, pgd_ttbr_el1
    msr ttbr1_el1, x0

    adr x0, pgd_ttbr_el0
    msr ttbr0_el1, x0

    // Two Memory types have been configured, index field of(pte) is used to
    // figure memory type
    ldr x0, =MAIR_ATTR
    msr mair_el1, x0

    ldr x0, =TCR_VALUE
    msr tcr_el1, x0

    // Enable Paging of MMU
    mrs x0, sctlr_el1
    orr x0, x0, #1
    msr sctlr_el1, x0

    ret

.global read_pgd_ttbr0
read_pgd_ttbr0:
    mrs x0, ttbr0_el1
    ret

.global read_pgd_ttbr1
read_pgd_ttbr1:
    mrs x0, ttbr1_el1
    ret


.global flush_tlb
flush_tlb:
    tlbi vmalle1is
    dsb ish
    isb

    ret

.global flush_ttrb0
flush_ttrb0:
    msr ttbr0_el1, x0
    tlbi vmalle1is
    dsb ish
    isb

    ret

.global flush_ttrb1
flush_ttrb1:
    msr ttbr1_el1, x0
    tlbi vmalle1is
    dsb ish
    isb

    ret