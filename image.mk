# tools
CHECKSIZE  := ./tool/checksize
MKFS       := ./tool/mkfs
DD         := dd

# size
BOOTSIZE   := 1
LOADERSIZE := 3
KERNELSIZE := 4096
FLOPPYSIZE := 1024
DISKSIZE   := 4096
FSSIZE     := 40960

# images
IMGDIR     := ./img/
BOOTIMG    := $(IMGDIR)boot.img
HDIMG      := $(IMGDIR)baby_hd.img
FSIMG      := $(IMGDIR)fs.img

BOOT       := ${addprefix ./,${BOOT}}
LOADER     := ${addprefix ./,${LOADER}}
KERNEL     := ${addprefix ./,${KERNEL}}
USERAPPS   := ${addprefix ./,${USERAPPS}}


.PHONY: img
img: $(LIBS) $(BOOT) $(LOADER) $(KERNEL) $(USERAPPS)
	@rm -rf $(IMGDIR)
	@mkdir  $(IMGDIR)

	$(CHECKSIZE) $(BOOT)   $(BOOTSIZE)
	$(CHECKSIZE) $(LOADER) $(LOADERSIZE)
	$(CHECKSIZE) $(KERNEL) $(KERNELSIZE)

	$(DD) if=/dev/zero of=$(BOOTIMG) ibs=512 seek=0 count=$(FLOPPYSIZE)
	$(DD) if=/dev/zero of=$(HDIMG)   ibs=512 seek=0 count=$(DISKSIZE)
	$(DD) if=/dev/zero of=$(FSIMG)   ibs=512 seek=0 count=$(FSSIZE)

	$(DD) if=$(BOOT)           of=$(BOOTIMG) ibs=512 seek=0 			count=$(BOOTSIZE)   conv=notrunc
	$(DD) if=$(LOADER)         of=$(HDIMG)   ibs=512 seek=0 			count=$(LOADERSIZE) conv=notrunc
	$(DD) if=$(KERNEL)         of=$(HDIMG)   ibs=512 seek=$(LOADERSIZE) count=$(KERNELSIZE) conv=notrunc

	$(MKFS) ${USERAPPS} ./res/font/ASC16 ./luac.out ./test.lua ./arrow.bmp ./pngtest.png \
						./res/xq/board.bmp ./res/xq/r_bing.bmp ./res/xq/r_ju.bmp ./res/xq/r_ma.bmp \
						./res/xq/r_xiang.bmp ./res/xq/r_shi.bmp ./res/xq/r_shuai.bmp ./res/xq/r_pao.bmp \
						./res/xq/b_zu.bmp ./res/xq/b_ju.bmp ./res/xq/b_ma.bmp ./res/xq/b_shi.bmp \
						./res/xq/b_jiang.bmp ./res/xq/b_xiang.bmp ./res/xq/b_pao.bmp ./res/xq/mask.bmp\