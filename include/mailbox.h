#pragma once

// #include "aux.h"

typedef enum
{
    NULL_TAG                   = 0,
    FB_ALLOCATE_BUFFER         = 0x00040001,
    FB_RELESE_BUFFER           = 0x00048001,
    FB_GET_PHYSICAL_DIMENSIONS = 0x00040003,
    FB_SET_PHYSICAL_DIMENSIONS = 0x00048003,
    FB_GET_VIRTUAL_DIMENSIONS  = 0x00040004,
    FB_SET_VIRTUAL_DIMENSIONS  = 0x00048004,
    FB_GET_BITS_PER_PIXEL      = 0x00040005,
    FB_SET_BITS_PER_PIXEL      = 0x00048005,
    FB_SET_RGB_ORDER_PER_PIXEL = 0x00048006,
    FB_GET_BYTES_PER_ROW       = 0x00040008,
    FB_SET_VIRTUAL_OFFSET      = 0x00048009,
    FB_GET_VIRTUAL_OFFSET      = 0x00040009,
}mailbox_tag;

typedef enum
{
    MAIL_CHANNEL_POWER    = 0x0, // Mailbox Channel 0: Power Management Interface
    MAIL_CHANNEL_FB       = 0x1, // Mailbox Channel 1: Frame Buffer
    MAIL_CHANNEL_VUART    = 0x2, // Mailbox Channel 2: Virtual UART
    MAIL_CHANNEL_VCHIQ    = 0x3, // Mailbox Channel 3: VCHIQ Interface
    MAIL_CHANNEL_LEDS     = 0x4, // Mailbox Channel 4: LEDs Interface
    MAIL_CHANNEL_BUTTONS  = 0x5, // Mailbox Channel 5: Buttons Interface
    MAIL_CHANNEL_TOUCH    = 0x6, // Mailbox Channel 6: Touchscreen Interface
    MAIL_CHANNEL_COUNT    = 0x7, // Mailbox Channel 7: Counter
    MAIL_CHANNEL_TAGS     = 0x8 // Mailbox Channel 8: Tags (ARM to VC)
}mailbox_channel;

typedef enum
{
    MAILBOX_REQ         = 0x00000000,
    MAILBOX_REQ_SUCCESS = 0x80000000,
    MAILBOX_REQ_ERROR   = 0x80000001
}MailBoxResponseType;

typedef enum
{
    MAILBOX_EMPTY = 0x40000000,
    MAILBOX_FULL  = 0x80000000
}MailBoxStatus;

struct MailboxMsg
{
    u32 channel : 4;
    u32 data : 28;
};

struct MailboxStatus
{
    u32 reserved : 30;
    u32 empty : 1;
    u32 full : 1;
};

struct Mailbox
{
    reg32 mail_box_read;
    reg32 reserved0[3];
    reg32 mail_box_poll;
    reg32 mail_box_sender;
    reg32 mail_box_status;
    reg32 mail_box_config;
    reg32 mail_box_write;
};

#define MAILBOX_REGS ((struct Mailbox*) (PPL_BASE_ADDR + 0xb880))

void mailbox_read(u32 channel);
void mailbox_write(u32 channel, u64 data);
void frame_buffer_init(void);

struct PropertyBuffer
{
    u32 buffer_size;
    u32 status_code;
    u8  tags[0];  
};

struct MailboxTag
{
    u32 id;
    u32 buffer_size;
    u32 status_code;
};

struct MailboxTagFBCb
{
    struct MailboxTag tag;
    
    union 
    {   struct 
        {
            u32 screen_witdh;
            u32 screen_height;
        }fb_size;

        struct
        {
            u32 virt_screen_witdh;
            u32 virt_screen_height;
        }virt_fb_size;

        struct
        {
            u32 base;
            u32 screen_size;
        }fb_screen_info;

        struct
        {
            u32 x_offset;
            u32 y_offset;
        }fb_screen_virt_offset;
    }MailboxData;
};

struct MailboxTabFbPixel
{
    struct MailboxTag tag;
    
    union 
    {   struct 
        {
            u32 pitch;
        }pixel_pitch;

        struct
        {
            u32 bpp;
        }pixel_bpp;

        struct
        {
            u32 order;
        }rgb_order;
    }MailboxData;
};

struct MailboxFbArray
{
    struct MailboxTagFBCb fb_size_cb;
    struct MailboxTagFBCb fb_virt_size_cb;
    struct MailboxTagFBCb fb_virt_offset_cb;
    struct MailboxTabFbPixel fb_pixel_depth;
    struct MailboxTabFbPixel fb_rgb_order;
    struct MailboxTagFBCb fb_screen_info;
    struct MailboxTabFbPixel fb_pitch;
};

u8* get_fb_ptr(void);
struct Platte* get_paltte(void);
/**
 * @brief GPU Mailbox Communication (VC: Video Controller)
 * 
 */