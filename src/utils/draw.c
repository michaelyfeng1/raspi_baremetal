#include "types.h"
#include "color.h"
#include "font.h"
#include "draw.h"
#include "mailbox.h"
#include "print.h"


static void drawPixel(int x, int y, unsigned char attr)
{
    int offs = (y * get_paltte()->pitch) + (x * (get_paltte()->bpp >> 3));
    *((unsigned int*)(get_fb_ptr() + offs)) = vgapal[attr & 0x0f];
}

void draw_char(u8 ch, int x, int y, u8 attr)
{
    unsigned char *glyph = (unsigned char *)&font + (ch < FONT_NUMGLYPHS ? ch : 0) * FONT_BPG;

    for (int i=0;i<FONT_HEIGHT;i++) 
    {
        for (int j=0;j<FONT_WIDTH;j++) 
        {
            unsigned char mask = 1 << (j);
            unsigned char col = (*glyph & mask) ? attr & 0x0f : (attr & 0xf0) >> 4;

            drawPixel(x+j, y+i, col);
        }
	    glyph += FONT_BPL;
    }
}

void draw_rectangle(int x, int y, unsigned char attr)
{
	int _y = y;
    while (_y <= y + 45) 
    {
       int _x = x;
        while (_x <= x + 400) 
        {
            if ((x == _x || _x == x + 400) || (y == _y || _y == y + 45)) 
            {
                int offs = (_y * get_paltte()->pitch) + (_x * (get_paltte()->bpp >> 3 ));
                ((unsigned int*)get_fb_ptr())[offs >> 2] = vgapal[attr];
            }
            
            _x++;
       }

       _y++;
    }
}

void draw_string(u32 x, u32 y, u8* s, u8 attr)
{
    while (*s) 
    {
        if (*s == '\r') 
        {
          x = 0;
        } 
        else if(*s == '\n') 
        {
          x = 0; y += FONT_HEIGHT;
        } 
        else 
        {
	        draw_char(*s, x, y, attr);
            x += FONT_WIDTH;
        }
        s++;
    }
}