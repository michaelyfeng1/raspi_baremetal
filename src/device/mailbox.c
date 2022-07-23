#include "types.h"
#include "aux.h"
#include "utils.h"
#include "draw.h"
#include "mailbox.h"
#include "mm.h"
#include "print.h"

static u32 mailbox_buffer[36] __attribute__((aligned(16))) = {0,0,0,0,
                                                              0,0,0,0,
                                                              0,0,0,0,
                                                              0,0,0,0,
                                                              0,0,0,0,
                                                              0,0,0,0,
                                                              0,0,0,0,
                                                              0,0,0,0,
                                                              0,0,0,0};

static u8* fb = nullptr;
static struct Platte platte  = { .bpp = 0, .pitch = 0};

u8* get_fb_ptr(void)
{
    return fb;
}

struct Platte* get_paltte(void)
{
    return &platte;
}

// void mailbox_read(u32 channel)
// {
//     while(1) 
//     {
//         while(MAILBOX_REGS->mail_box_poll & MAILBOX_EMPTY) ;

//         u32 data = MAILBOX_REGS->mail_box_read;

//         u8 read_channel = (u8)(data & 0xF);

//         if (read_channel == channel) 
//         {
//             return;
//         }
//     }
// }

// void mailbox_write(u32 channel, u64 data)
// {
//     while (MAILBOX_REGS->mail_box_poll & MAILBOX_FULL);

//     MAILBOX_REGS->mail_box_write = (data & 0xFFFFFFF0 | (channel & 0xF));
// }

// void mailbox_cb_path_fb_query(void)
// {
//     u32 dword_index_count = 0;

//     struct MailboxTagFBCb mbx_fb_cb;

//     mbx_fb_cb.tag.id = MAIL_CHANNEL_TAGS;
//     mbx_fb_cb.tag.buffer_size = 8;
//     mbx_fb_cb.tag.status_code = MAILBOX_REQ;

//     mbx_fb_cb.MailboxData.fb_size.screen_witdh = 640;
//     mbx_fb_cb.MailboxData.fb_size.screen_height = 480;

//     u32 mailbox_buffer_size = 3 * sizeof(u32) + sizeof(struct MailboxTagFBCb);

//     dword_index_count += mailbox_buffer_size / sizeof(u32);

//     memcpy(&mailbox_buffer[2], &mbx_fb_cb, sizeof(struct MailboxTagFBCb));

//     struct PropertyBuffer* pBuf = (struct PropertyBuffer*)mailbox_buffer;

//     pBuf->buffer_size = mailbox_buffer_size;
//     pBuf->status_code = MAILBOX_REQ;

//     mailbox_buffer[dword_index_count - 1] = (u32)NULL_TAG;


//     mailbox_write(MAIL_CHANNEL_TAGS, (u64)((void*)mailbox_buffer));

//     mailbox_read(MAIL_CHANNEL_TAGS);

//     memcpy(&mbx_fb_cb, mailbox_buffer + 2,  sizeof(struct MailboxTagFBCb));
// }


static int mbox_call(u8 ch)
{

    printk("0x%x\n", &MAILBOX_REGS->mail_box_status);
    printk("0x%x\n", &MAILBOX_REGS->mail_box_write);
    u32 r = (((u32)((u64)&mailbox_buffer)&~0xF) | (ch&0xF));
    /* wait until we can write to the mailbox */
    do{ ; }while(MAILBOX_REGS->mail_box_status & MAILBOX_FULL);
    /* write the address of our message to the mailbox with channel identifier */
    MAILBOX_REGS->mail_box_write = r;
    /* now wait for the response */
    while(1) 
    {
        /* is there a response? */
        do{;}while(MAILBOX_REGS->mail_box_status & MAILBOX_EMPTY);
        /* is it a response to our message? */
        if(r == MAILBOX_REGS->mail_box_read)
            /* is it a valid successful response? */
            return mailbox_buffer[1] == MAILBOX_REQ_SUCCESS;
    }

    return 0;
}

void mail_box_frame_buffer_init()
{
    mailbox_buffer[0] = 35*4;
    mailbox_buffer[1] = MAILBOX_REQ;

    mailbox_buffer[2] = FB_SET_PHYSICAL_DIMENSIONS;  //set phy wh
    mailbox_buffer[3] = 8;
    mailbox_buffer[4] = 8;
    mailbox_buffer[5] = 1920;         //FrameBufferInfo.width
    mailbox_buffer[6] = 1080;          //FrameBufferInfo.height

    mailbox_buffer[7] = FB_SET_VIRTUAL_DIMENSIONS;  //set virt wh
    mailbox_buffer[8] = 8;
    mailbox_buffer[9] = 8;
    mailbox_buffer[10] = 1920;        //FrameBufferInfo.virtual_width
    mailbox_buffer[11] = 1080;         //FrameBufferInfo.virtual_height

    mailbox_buffer[12] = FB_SET_VIRTUAL_OFFSET; //set virt offset
    mailbox_buffer[13] = 8;
    mailbox_buffer[14] = 8;
    mailbox_buffer[15] = 0;           //FrameBufferInfo.x_offset
    mailbox_buffer[16] = 0;           //FrameBufferInfo.y.offset

    mailbox_buffer[17] = FB_SET_BITS_PER_PIXEL; //set depth
    mailbox_buffer[18] = 4;
    mailbox_buffer[19] = 4;
    mailbox_buffer[20] = 32;          //FrameBufferInfo.depth

    mailbox_buffer[21] = FB_SET_RGB_ORDER_PER_PIXEL; //set pixel order
    mailbox_buffer[22] = 4;
    mailbox_buffer[23] = 4;
    mailbox_buffer[24] = 1;           //RGB, not BGR preferably

    mailbox_buffer[25] = FB_ALLOCATE_BUFFER; //get framebuffer, gets alignment on request
    mailbox_buffer[26] = 8;
    mailbox_buffer[27] = 8;
    mailbox_buffer[28] = 16;        //FrameBufferInfo.pointer
    mailbox_buffer[29] = 0;           //FrameBufferInfo.size

    mailbox_buffer[30] = FB_GET_BYTES_PER_ROW; //get pitch
    mailbox_buffer[31] = 4;
    mailbox_buffer[32] = 4;
    mailbox_buffer[33] = 0;           //FrameBufferInfo.pitch

    mailbox_buffer[34] = NULL_TAG;

    if(mbox_call(MAIL_CHANNEL_TAGS) && mailbox_buffer[20]==32 && mailbox_buffer[28]!=0) 
    {
        mailbox_buffer[28] &= 0x3FFFFFFF;   //convert GPU address to ARM address
        // width=mbox[5];          //get actual physical width
        // height=mbox[6];         //get actual physical height
        platte.pitch = mailbox_buffer[33];         //get number of bytes per line
        platte.bpp = 32;
        // isrgb=mbox[24];         //get the actual channel order
        fb=(u8*)(P2V((u64)mailbox_buffer[28]));
    }
}

void frame_buffer_init(void)
{
    // mailbox_cb_path_fb_query();
    mail_box_frame_buffer_init();
}