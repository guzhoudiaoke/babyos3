/*
 * guzhoudiaoke@126.com
 * 2017-10-21
 */

#ifndef _TYPES_H_
#define _TYPES_H_

#define NULL				(0)

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned long long  uint64;
typedef char                int8;
typedef short               int16;
typedef int                 int32;
typedef long long           int64;


/* video info */
typedef struct video_info_s {
    uint16 video_mode;
    uint16 width;
    uint16 height;
    uint8  bits_per_pixel;
    uint8  memory_model;
    uint8* vram_base_addr;
} video_info_t;

#endif
