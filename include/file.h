#pragma once

#ifndef __ASSEMBLER__
typedef struct BPB {
    u8 jump[3];
    u8 oem[8];
    u16 bytes_per_sector;
    u8 sectors_per_cluster;
    u16 reserved_sector_count;
    u8 fat_count;
    u16 root_entry_count;
    u16 sector_count;
    u8 media_type;
    u16 sectors_per_fat;
    u16 sectors_per_track;
    u16 head_count;
    u32 hidden_sector_count;
    u32 large_sector_count;
    u8 drive_number;
    u8 flags;
    u8 signature;
    u32 volume_id;
    u8 volume_label[11];
    u8 file_system[8];
}__attribute__((packed)) BPB_t;

typedef struct DirEntry {
    u8 name[8];
    u8 ext[3];
    u8 attributes;
    u8 reserved;
    u8 create_ms;
    u16 create_time;
    u16 create_date;
    u16 access_date;
    u16 attr_index;
    u16 m_time;
    u16 m_date;
    u16 cluster_index;
    u32 file_size;
}__attribute__((packed)) DirEntry_t;
#endif

#ifndef __ASSEMBLER__

#define FS_BASE         P2V(0x30000000)
#else
#define FS_BASE         0x000030000000
#endif

#define ENTRY_EMPTY     0
#define ENTRY_DELETED   0xe5

#define PARTITION_ENTRY_OFFSET      0x1be
#define LBA_FIELD_OFFSET            0x8
#define SECTOR_SIZE_IN_BYTES        512
#define FS_SIZE                     101 * 16 * 63 * 512

#ifndef __ASSEMBLER__
void init_fs(void);
int load_file(char *path, uint64_t addr);
#endif