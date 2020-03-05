.DEFAULT_GOAL := all

# tools
CC 		:= g++
AR 		:= ar
LD 		:= ld
QEMU    := qemu-system-x86_64
OBJCOPY := objcopy

# flags
LDFLAGS  := -g -m elf_x86_64
CPPFLAGS := -g -Wall -Werror -fno-exceptions -m64 -mcmodel=kernel -fno-pic -static -fno-builtin -fno-strict-aliasing
CPPFLAGS += -MD -fno-omit-frame-pointer -ffreestanding -fno-common -nostdlib -gdwarf-2 -DX64 -mtls-direct-seg-refs -mno-red-zone -fno-rtti
ASFLAGS  := -g -m64
ARFLAGS  := rv
NAME     := babyos3

# top dir
SRCTOP := src
OBJTOP := obj

include Makefile.inc
include Makefile.img

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
all: img

.PHONY: clean
clean:
	@echo "Delete objects, libraries, executables"
	rm -rf obj img


