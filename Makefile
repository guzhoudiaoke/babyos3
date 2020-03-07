.DEFAULT_GOAL := all

# tools
CC 		:= g++
AR 		:= ar
LD 		:= ld
OBJCOPY := objcopy
QEMU    := qemu-system-x86_64

# flags
LDFLAGS  := -g -m elf_x86_64
CPPFLAGS := -g -m64 -Wall -Werror -fno-exceptions -mcmodel=kernel -fno-pic -static -fno-builtin -fno-strict-aliasing
CPPFLAGS += -MD -fno-omit-frame-pointer -ffreestanding -fno-common -nostdlib -gdwarf-2 -DX64 -mtls-direct-seg-refs 
CPPFLAGS += -mno-red-zone -fno-rtti
ASFLAGS  := -g -m64
ARFLAGS  := rv

# top dir
SRCTOP := src
OBJTOP := obj

include Makefile.inc
include Makefile.img


.PHONY: all
all: img


NAME := babyos3
QEMUIMGS := -fda $(BOOTIMG) -hda $(HDIMG) -hdb $(FSIMG)
QEMUARGS := $(QEMUIMGS) -rtc base=localtime -serial mon:stdio -m 128 -name $(NAME)
QEMUARGS_DEBUG := $(QEMUARGS) -s -S

.PHONY: qemu
qemu: img
	@echo "Qemu run"
	$(QEMU) $(QEMUARGS)

.PHONY: qemud
qemud: img
	@echo "Qemu debug"
	$(QEMU) $(QEMUARGS_DEBUG)


.PHONY: clean
clean:
	@echo "Delete objects, libraries, executables"
	rm -rf obj img


