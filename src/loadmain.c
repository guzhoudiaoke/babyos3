#include "types.h"
#include "kernel.h"

int is_pixel_valid(int32 x, int32 y)
{
    video_info_t* video_info = (video_info_t *)VIDEO_INFO_ADDR;
    uint32 width = video_info->width;
    uint32 height = video_info->height;

    if (x < 0 || y < 0 || (uint32)x >= width || (uint32)y >= height) {
        return 0;
    }

    return 1;
}


void set_pixel(int32 x, int32 y, uint8 r, uint8 g, uint8 b)
{
    video_info_t* video_info = (video_info_t *)VIDEO_INFO_ADDR;
    uint8* vram_base_addr = video_info->vram_base_addr;
    uint32 width = video_info->width;
    uint32 bytes_per_pixel = video_info->bits_per_pixel/8;

    uint8* pvram = NULL;
    if (!is_pixel_valid(x, y)) {
        return;
    }

    pvram = vram_base_addr + bytes_per_pixel*(y*width + x);
    pvram[0] = b;
    pvram[1] = g;
    pvram[2] = r;
}

void test()
{
    for (int i = 100; i < 1024-100; i++) {
        set_pixel(i, 200, 0x00, 0xff, 0x00);
    }
}

void loadmain(void)
{
    test();
}
