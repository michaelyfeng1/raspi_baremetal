#pragma once

struct Platte
{
    u32 pitch;
    u32 bpp;
};

void draw_string(u32 x, u32 y, u8* s, u8 attr);
void draw_char(u8 ch, int x, int y, u8 attr);
void draw_rectangle(void);