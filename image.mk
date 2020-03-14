# tools
CHECKSIZE  := ./tool/checksize
MKFS       := ./tool/mkfs
DD         := dd

# size
BOOTSIZE   := 1
LOADERSIZE := 3
KERNELSIZE := 2048
FLOPPYSIZE := 1024
DISKSIZE   := 4096
FSSIZE     := 4096

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

	$(MKFS) ${USERAPPS} ./res/font/ASC16 ./luac.out ./test.lua
