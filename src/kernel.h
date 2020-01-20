/*
 * guzhoudiaoke@126.com
 * 2017-10-21
 */

#ifndef _KERNEL_H_
#define _KENERL_H_

/* segs */
#define SEG_NULL			(0)
#define SEG_KCODE           (1)
#define SEG_KDATA           (2)
#define SEG_UCODE           (3)
#define SEG_UDATA           (4)
#define SEG_TSS				(5)
#define GDT_LEN			    (6)
#define IDT_LEN				(256)

#define	SECT_SIZE			(512)
#define KSTACK_SIZE         (8192)
#define STACK_BOOT			(0x1000)
#define STACK_PM_BOTTOM     (0x10000)

/* boot information */
#define BOOT_INFO_ADDR		(0x8000)    /* 0x8000, 32k */
#define BOOT_INFO_SEG		(BOOT_INFO_ADDR >> 4)

/* 1. video info */
#define VIDEO_INFO_ADDR	    (BOOT_INFO_ADDR)
#define VIDEO_INFO_OFFSET	(VIDEO_INFO_ADDR - BOOT_INFO_ADDR)
#define VIDEO_INFO_SIZE	    (12)

/* for cr0 */
#define CR0_PE              0x00000001
#define CR0_PG              0x80000000
#define CR0_WP              0x00010000

#endif
