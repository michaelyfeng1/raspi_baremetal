#include "types.h"
#include "mm.h"
#include "utils.h"
#include "file.h"
#include "print.h"
#include "bin.h"

extern u8 __kernel_start;
extern u8 __kernel_end;

static HeapPage_t heap_seg = {.next = nullptr};

static void dump_pt(u64 tbl)
{
    for(u64 i = 0; i < 512; i++)
    {
        if(((u64*)tbl)[i] & ENTRY_VALID)
        {
            printk("PGD Entry %d pa 0x%x\n", i, ((u64*)tbl)[i]);
            u64* pud = (u64*) P2V((((u64*)tbl)[i]) >> 12 << 12);
    
            for (u64 j = 0; j < 512 ; j++)
            {
                if(pud[j] & ENTRY_VALID)
                {
                    printk("PUD Entry %d pa 0x%x\n", j, ((u64*)pud)[j]);
                
                    u64* pmd = (u64*)P2V(((pud[j] >> 12) << 12));

                    for(u64 k = 0; k < 512; k++)
                    {
                        if (pmd[k] & ENTRY_VALID)
                        {
                            printk("PMD Entry %d pa 0x%x\n", k, ((u64*)pmd)[k]);
                        }
                    }
                }
            }
        }
    }
}

static void build_heap(u64 v, u64 e)
{
    for(u64 start = PA_UP(v); start + PAGE_SIZE <= e; start += PAGE_SIZE)
    {
        kfree(start);
    }
}

void heap_mem_seg_init(void)
{
    // printk("Kernel Starts = 0x%x\n", &__kernel_start);
    // printk("Kernel Ends   = 0x%x\n", &__kernel_end);
    
    build_heap((u64)&__kernel_end, HEAP_END);
}

void* kalloc(void)
{
    HeapPage_t* page = heap_seg.next;

    if(page != nullptr)
    {
        heap_seg.next = page->next;
    }

    return page;
}

void kfree(u64 v)
{
    HeapPage_t* page = (HeapPage_t*)v;

    page->next = heap_seg.next;
    heap_seg.next = page;
}

static u64* update_page_table()
{
    return nullptr;
}

static u64* find_pud_entry(u64 tbl, u64 v, u32 alloc, u64 attr)
{
    //update PGD
    u64* ptr = (u64*)tbl; //pgd pointer
    u64* addr = nullptr;

    if (ptr == nullptr)
    {
        return nullptr;
    }

    u16 index = (v >> 39) & PUD_INDEX_MASK_2MB;

    if ( ptr[index] & ENTRY_VALID)
    {
        addr = (u64*)P2V(PDE(ptr[index]));
    }
    else if (alloc == 1)
    {
        addr = kalloc();
        memset(addr, 0, PAGE_SIZE);
        ptr[index] = (V2P(addr) | attr | PDE_ENTRY);
    }

    return addr;
}

static u64* find_pmd_entry(u64 tbl, u64 v, u32 alloc, u64 attr)
{
    //update PUD
    u64* ptr = nullptr;
    u64* addr = nullptr;

    ptr = find_pud_entry(tbl, v, 1, attr);

    if (ptr == nullptr)
    {
        return nullptr;
    }

    u16 index = (v >> 30) & PUD_INDEX_MASK_2MB;

    if ( ptr[index] & ENTRY_VALID)
    {
        addr = (u64*)P2V(PDE(ptr[index]));
    }
    else if (alloc == 1)
    {
        addr = kalloc();

        memset(addr, 0, PAGE_SIZE);

        ptr[index] = (V2P(addr) | attr | PDE_ENTRY);
    }

    return addr;
}

int map_page(u64 tbl, u64 v, u64 p, u64 attr)
{
    u64 va = PA_DOWN(v); //aligned to page size
    u64* ptr = nullptr;

    // Update PMD
    ptr = find_pmd_entry(tbl, va, 1, attr);

    if (ptr == nullptr)
    {
        return false;
    }
    else
    {
        u16 index = (va >> PAGE_SHIFT_BITS_2MB) & PMD_INDEX_MASK_2MB; 
    
        if (index >= ENTRY_MAX_2MB)
        {
            //ASSERT
            return false;
        }
        else
        {
            ptr[index] = p | attr | PTE_ENTRY;
        }
    }

    return true;
}

static void free_pud_page(u64 tbl)
{
    uint64_t *ptr = (uint64_t*)tbl;

    for (int i = 0; i < ENTRY_MAX_2MB; i++) 
    {
        if (ptr[i] & ENTRY_VALID) 
        {
            kfree(P2V(PDE(ptr[i])));
            ptr[i] = 0;
        }
    }
}

static void free_pgd_page(u64 tbl)
{
    kfree(tbl);
}

void free_page(u64 tbl, u64 v)
{
    u64* ptr = nullptr;

    ptr = find_pmd_entry(tbl, v, 0, 0);

    if (ptr != nullptr)
    {
        u16 index = (v >> PAGE_SHIFT_BITS_2MB) & PMD_INDEX_MASK_2MB;

        if (ptr[index] & ENTRY_VALID)
        {
            kfree(P2V(PTE(ptr[index])));

            ptr[index] = 0;
        }
    }
}

u8 setup_process_page_table(u64 tbl, u8* file_name)
{
    u8 status = false;

    void *page = kalloc();

    if (page != nullptr) 
    {
        memset(page, 0, PAGE_SIZE);
        memcpy(page, (u8*)UCODE, UCODE_SIZE);
        memcpy((void*)((u8*)page + UCODE_SIZE), UDATA , UDATA_SIZE);
        status = map_page(tbl, 0x400000, V2P(page), ENTRY_VALID | USER_LEVEL | NORMAL_MEMORY | ACCESS_ALLOWED);

        // dump_pt(tbl);
        // if (status == true) {
            // if (load_file(file_name, (uint64_t)page) == -1) {
                // free_vm(tbl);
                // return false;
            // }
        // }
        // else {
        //     kfree((uint64_t)page);
        //     // free_vm(tbl);
        // }
    }

    return status;
}

void page_table_clone(u64 tbl_src, u64 tbl_dst)
{
    for(u64 i = 0; i < 512; i++)
    {
        if(((u64*)tbl_src)[i] & ENTRY_VALID)
        {
            if(!(((u64*)tbl_dst)[i] & ENTRY_VALID))
            {
                u64* addr = nullptr;
                addr = kalloc();

                memset(addr, 0, PAGE_SIZE);

                ((u64*)tbl_dst)[i] = V2P(addr) | ENTRY_VALID | NORMAL_MEMORY | ACCESS_ALLOWED | PDE_ENTRY;

            }

            // printk("PGD Entry %d pa 0x%x\n", i, ((u64*)tbl_src)[i]);
            u64* pud = (u64*) P2V((((u64*)tbl_src)[i]) >> 12 << 12);
            u64* pud_dst = (u64*) P2V((((u64*)tbl_dst)[i]) >> 12 << 12);;
            for (u64 j = 0; j < 512 ; j++)
            {
                if(pud[j] & ENTRY_VALID)
                {
                    if (!(pud_dst[j] & ENTRY_VALID))
                    {
                        u64* addr = nullptr;
                        addr = kalloc();

                        memset(addr, 0, PAGE_SIZE);

                        ((u64*)pud_dst)[j] = V2P(addr) | ENTRY_VALID | NORMAL_MEMORY | ACCESS_ALLOWED | PDE_ENTRY;

                    }
                    // printk("PUD Entry %d pa 0x%x\n", j, ((u64*)pud)[j]);
                
                    u64* pmd = (u64*)P2V(((pud[j] >> 12) << 12));
                    u64* pmd_dst =  (u64*)P2V(((pud_dst[j] >> 12) << 12));
                    
                    for(u64 k = 0; k < 512; k++)
                    {
                        if (pmd[k] & ENTRY_VALID)
                        {
                            ((u64*)pmd_dst)[k] = pmd[k] ;

                            // printk("PMD Entry %d pa 0x%x\n", k, ((u64*)pmd)[k]);
                        }
                    }
                }
            }
        }
    }
    printk("----------------------------------------\n");
    dump_pt(tbl_src);
    printk("----------------------------------------\n");
    dump_pt(tbl_dst);
    printk("----------------------------------------\n");
}