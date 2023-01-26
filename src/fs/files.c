#include "types.h"
#include "mm.h"
#include "file.h"
#include "utils.h"
#include "print.h"

// #include <stdint.h>
#include <stdbool.h>

// BPB holds information
/**
 * 0        3           imp
 * 3        8           OEM name
 * 0xb      2           bytes per sector
 * 0xd      1           sectors per cluster
 * 0xe      2           reserved sectors
 * 0x10     1           number of fat table
 * 0x11     2           root dir entries
 * 0x13     2           total sectors
 * 0x15     1           media descriptor
 * 0x16     2           sectors per fat table
 * 0x18     2           sectors per track
 * 0x1a     2           heads
 * 0x1c     4           hidden sectors
 * 0x20     4           extended sectors
 * 
 * 0x2b     11          label
 * 0x30     8           system type (fat16, fat32, etc)
 * 
 */

static BPB_t* get_bpb(void)
{
    // FS is appened at the end of kernel image = 0x30000000
    // First 512 is MBR and partition entry is at offset = 0x1be
    // lbe field is at offset of 0x8
    u32 lbe_val = *(u32*)(FS_BASE + PARTITION_ENTRY_OFFSET + LBA_FIELD_OFFSET);
    printk("0x%x\n", lbe_val);
    BPB_t* bpb = (BPB_t*)(FS_BASE + lbe_val * SECTOR_SIZE_IN_BYTES);

    return bpb;
}

static u16* get_fat_table(void)
{
    // FAT represents File Allocation Table
    // Reserved sectors are space used for MBR and BPB
    // FAT starts be;ow BPB
    BPB_t* bpb = get_bpb();

    u32 fat_offset = bpb->reserved_sector_count * bpb->bytes_per_sector;

    // 16 bit represents a cluster number
    // From BPB, it call tell number of fat table and size of each fat table
    // e.g. 2 fat and each takes 64 sectors
    return (u16*)(FS_BASE + fat_offset);
}

static u16 get_cluster_number(u32 index)
{
    u16* fat = get_fat_table();

    return fat[index];
}

static u32 get_cluster_size(void)
{
    //return bytes per cluster
    BPB_t* bpb = get_bpb();

    return (u32)bpb->bytes_per_sector * bpb->sectors_per_cluster;
}

static uint32_t get_root_directory_count(void)
{
    BPB_t* bpb = get_bpb();

    return bpb->root_entry_count;
}

/**
 * 0        8       file_name
 * 8        3       extension name
 * 0xb      1       attribute
 * 0xc      10      reserved
 * 0x16     2       time
 * 0x18     2       date
 * 0x1a     2       starting cluster
 * 0x1c     4       file size
 */

static DirEntry_t* get_root_directory(void)
{
    BPB_t* bpb = get_bpb();

    //Reserved + FAT = offset of Root Directory Table from BPB
    //Each root directory entry is 32 bytes
    u32 root_dir_offset = (bpb->reserved_sector_count + 
                            (u32)bpb->fat_count * bpb->sectors_per_fat) * bpb->bytes_per_sector;


    return (DirEntry_t*) ((u8*)bpb + root_dir_offset);
}

static u32 get_cluster_offset(u32 index)//data_offset
{
    u32 adjusted_index = index - 2;

    BPB_t* bpb = get_bpb();

    return bpb->reserved_sector_count * bpb->bytes_per_sector + //fat start
           (u32)bpb->fat_count * bpb->sectors_per_fat * bpb->bytes_per_sector + //RD Start
           bpb->root_entry_count * sizeof(DirEntry_t) + 
           adjusted_index * ((u32)bpb->sectors_per_cluster * bpb->bytes_per_sector); //Data start
}

static bool split_path(u8 *path, u8 *name, u8 *ext)
{
    int i;

    for (i = 0; i < 8 && path[i] != '.' && path[i] != '\0'; i++) 
    {
        if (path[i] == '/') //does not support sub folders 
        {
            return false;
        }

        name[i] = path[i];
    }

    if (path[i] == '.') 
    {
        i++;
        
        for (int j = 0; j < 3 && path[i] != '\0'; i++, j++) 
        {
            if (path[i] == '/') {
                return false;
            }

            ext[j] = path[i];
        }
    }

    if (path[i] != '\0') 
    {        
        return false;
    }

    return true;
}

static bool is_file_name_equal(DirEntry_t *dir_entry, u8 *name, u8 *ext)
{
    bool status = false;

    if (memcmp(dir_entry->name, name, 8) == 0 &&
        memcmp(dir_entry->ext, ext, 3) == 0) status = true;

    return status;
}


static u32 search_file(u8 *path)
{
    u8 name[8] = {"        "};
    u8 ext[3] =  {"   "};

    u32 root_entry_count;

    DirEntry_t *dir_entry; 

    bool status = split_path(path, name, ext);

    if (status == true) 
    {
        root_entry_count = get_root_directory_count();
        dir_entry = get_root_directory();

        for (u32 i = 0; i < root_entry_count; i++) 
        {
            if (dir_entry[i].name[0] == ENTRY_EMPTY || dir_entry[i].name[0] == ENTRY_DELETED) continue;

            if (dir_entry[i].attributes == 0xf) continue;
            
            if (is_file_name_equal(&dir_entry[i], name, ext)) return i;
        }
    }
    // printk("hello7\n");
    return 0xffffffff;
}

static u32 read_raw_data(u32 cluster_index, u8 *buffer, u32 size)
{
    struct BPB* bpb;
    u8 *data;
    u32 read_size = 0;
    u32 cluster_size; 
    u32 index; 
    
    bpb = get_bpb();
    cluster_size = get_cluster_size();
    index = cluster_index;

    if (index < 2) 
    {
        return 0xffffffff;
    }
    
    while (read_size < size) 
    {
        data  = (u8 *)((u64)bpb + get_cluster_offset(index));

        index = get_cluster_number(index);

        if (index >= 0xfff7) {
            memcpy(buffer, data, size - read_size);
            read_size += size - read_size;
            break;
        }

        memcpy(buffer, data, cluster_size);

        buffer += cluster_size;
        read_size += cluster_size;
    }

    return read_size;
}

static u32 read_file(u32 cluster_index, void *buffer, u32 size)
{
    return read_raw_data(cluster_index, buffer, size);
}

void init_fs(void)
{
    u8 *bpb = (u8*)get_bpb();

    printk("0x%x\n", (u64)bpb);
    printk("0x%x\n", bpb[0x1fe]);
    printk("0x%x\n", bpb[0x1ff]);

    if (bpb[0x1fe] != 0x55 || bpb[0x1ff] != 0xaa) 
    {
        printk("invalid signature\n");
        // ASSERT(0);
    }
    else
    {
        printk("valid signature\n");    
    }
}

int load_file(char *path, u64 addr)
{
    u32 index;
    u32 file_size;
    u32 cluster_index;
    struct DirEntry *dir_entry;
    int ret = -1;
    

    index = search_file(path);

    if (index != 0xffffffff) 
    {
        
        dir_entry = get_root_directory();
        file_size = dir_entry[index].file_size;
        cluster_index = dir_entry[index].cluster_index;

        // if (read_file(cluster_index, (void*)addr, file_size) == file_size) 
        if (read_file(cluster_index, (void*)addr, file_size) != -1)
        {
            ret = 0;
        }
    }

    return ret;
}

/**
 * 
 * cluster -> sectors
 * 
 * file_size stored in RDEs should be in the unit of cluster
 * but defined in bytes (A.K.A file_size % cluster_size == 0)
 */
