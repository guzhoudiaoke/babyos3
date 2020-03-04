.DEFAULT_GOAL := all

# tools
CC 		:= g++
AR 		:= ar
LD 		:= ld
QEMU    := qemu-system-x86_64
OBJCOPY := objcopy

# flags
LDFLAGS  := -g -m elf_x86_64
CPPFLAGS := -g -Wall -Werror -fno-exceptions -m64
CPPFLAGS += -mcmodel=kernel -fno-pic -static -fno-builtin -fno-strict-aliasing
CPPFLAGS += -MD -fno-omit-frame-pointer -ffreestanding -fno-common -nostdlib
CPPFLAGS += -gdwarf-2 -DX64 -mtls-direct-seg-refs -mno-red-zone -fno-rtti
ASFLAGS  := -g -m64
ARFLAGS  := rv
NAME     := babyos3

# extra flags
BOOTFLAGS    := --oformat binary -N -Ttext 0x7c00 
LOADERFLAGS  := -m elf_x86_64 -Ttext 0x0000 
OBJCOPYFLAGS := -S -O binary -j .text
KERNELFLAGS  := -T src/sys/kernel.ld
USERFLAGS    := -e main -Ttext 0x1000 

# top dir
SRCTOP := src
OBJTOP := obj

include Makefile.inc


.PHONY: mkdirs
mkdirs:
	mkdir -p $(MKDIRS)


BOOTSIZE   = 1
LOADERSIZE = 3
KERNELSIZE = 2048
FONTSIZE   = 8
FONTLBA    = 2048
FLOPPYSIZE = 1024
DISKSIZE   = 4096
FSSIZE     = 4096


BOOT     := ${addprefix ./,${BOOT}}
LOADER   := ${addprefix ./,${LOADER}}
KERNEL   := ${addprefix ./,${KERNEL}}
USERAPPS := ${addprefix ./,${USERAPPS}}

.PHONY: all
all: $(LIBS) $(PROGRAMS)
	@echo "=== Build complete ==="

	./tool/checksize $(BOOT)   ${BOOTSIZE}
	./tool/checksize $(LOADER) ${LOADERSIZE}
	./tool/checksize $(KERNEL) ${KERNELSIZE}

	dd if=/dev/zero of=boot.img    ibs=512 seek=0 count=${FLOPPYSIZE}
	dd if=/dev/zero of=baby_hd.img ibs=512 seek=0 count=${DISKSIZE}
	dd if=/dev/zero of=fs.img      ibs=512 seek=0 count=${FSSIZE}

	dd if=$(BOOT)              of=boot.img    ibs=512 seek=0 			  	count=${BOOTSIZE}   conv=notrunc
	dd if=$(LOADER)            of=baby_hd.img ibs=512 seek=0 			  	count=${LOADERSIZE} conv=notrunc
	dd if=$(KERNEL)            of=baby_hd.img ibs=512 seek=${LOADERSIZE} 	count=${KERNELSIZE} conv=notrunc
	dd if=./res/font/ASC16 of=baby_hd.img ibs=512     seek=${FONTLBA}       count=${FONTSIZE}   conv=notrunc

	./tool/mkfs ${USERAPPS}

.PHONY: clean
clean:
	@echo "Delete objects, libraries, executables"
	rm -rf obj


