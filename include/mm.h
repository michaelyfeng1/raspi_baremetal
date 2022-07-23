#pragma once

// #define KERNEL_BASE         0xffff000000000000
#define KERNEL_BASE         0x000000000000

#define P2V(p) ((u64)(p) + KERNEL_BASE)
#define V2P(p) ((u64)(p) - KERNEL_BASE)

#ifndef __ASSEMBLER__

#define PAGE_SIZE       (2 * 1024 * 1024)
#define HEAP_END        P2V(0x30000000)

#define PA_UP(v)    ((((u64)v + PAGE_SIZE - 1) >> 21) << 21)
#define PA_DOWN(v)  (((u64)v >> 21) << 21) 

#define PDE(p)      ((u64)p & 0xfffffffff000) //Global Directory and upper directory has attributes with lower 12 bits
#define PTE(p)      ((u64)p & 0xffffffe00000) //Middle Directory as lower 21 bits of attributes

#define PAGE_SHIFT_BITS_2MB         21
#define PT_INDEX_SHIFT              9
#define PGD_INDEX_MASK_2MB          0x1ff
#define PUD_INDEX_MASK_2MB          0x1ff
#define PMD_INDEX_MASK_2MB          0x1ff

#define ENTRY_VALID                 1
#define PDE_ENTRY                   1 << 1
#define PTE_ENTRY                   0 << 1
#define NORMAL_MEMORY               1 << 2
#define DEVICE_MEMORY               0 << 2
#define ACCESS_ALLOWED              1 << 10
#define USER_LEVEL                  1 << 6
#define KERNEL_LEVEL                0 << 6

#define ENTRY_MAX_2MB               512

typedef struct HeapPage
{
    struct HeapPage* next;
}HeapPage_t;

void heap_mem_seg_init(void);
void* kalloc(void);
void kfree(u64 v);

u8 setup_process_page_table(u64 tbl, u8* file_name);
void page_table_clone(u64 tbl_src, u64 tbl_dst);
#endif

/**
 * Memory Segment
 * 
 * Physical Memroy (Heal Segment)
 * Qemu
 * 
 * Kernel End to the start of device memory mapping
 * 
 * Pi3
 * 
 * Pi4
 * 
 * 
 * Memory Map
 * Kernel Space maps
 * 1. Kerne Space
 * 2. Free Memory
 * 3. File System
 * 
 * 
 * Kernel is loaded to 0x80000
 * Project uses 1G 
 * 
 * 1G Memory Map
 * 1. Memoy-mapped devices (video core)
 * 2. Free Memory
 * 3, Kernel
 * 
 * Virtual Address
 * 
 * Sign     PGD     PUD     PMD     Page Offset
 *          9-bits  9-bits  9-bits  21-bits   (2MB)
 * sign = all 0 or all 1
 * 
 * ttbr0    upper bits all 0 -> use ttbr0 to do translation
 * ttbr1    upper bits all 1 -> use ttbr1 to do translation
 * where ttbr = translation table base register
 * 
 * 
 * PGD entry  (stored in ttbr0/ttbr1)
 * bit 0 = valid bit
 * bit 1 = next level or physical page
 * 
 * PUD entry
 * bit 0 = valid bit
 * bit 1 = next level or physical page
 * 
 * PMD entry - points to 2MB phyiscal page
 * bit 0 = valid bit
 * bit 1 = next level or physical page
 * 
 * PTE entry
 * bit 4-2 = index in memory atrribute indrection register
 * bit 6 = program in el0 or el1. 1 means el0 can access it
 * bit 7 = read only bit. 1 means read-only
 * bit 10 = access bit and needs to set to 1
 * 
 * 
 * Kernel is loaded to first 2MB = 1 page
 * 
 * kernel space map 0 to 0x3400000
 * user space maps 0 to 2M
 * 
 * upper 16-bit are used to select registers
 */