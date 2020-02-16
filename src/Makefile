.PHONY: all
all: babyos

CPP = g++
LD = ld
OBJCOPY = objcopy

LDFLAGS  = -g -m elf_x86_64
CPPFLAGS = -g -Wall -Werror -fno-exceptions -m64
CPPFLAGS += -mcmodel=kernel -fno-pic -static -fno-builtin -fno-strict-aliasing
CPPFLAGS += -MD -fno-omit-frame-pointer -ffreestanding -fno-common -nostdlib
CPPFLAGS += -gdwarf-2 -DX64 -mtls-direct-seg-refs -mno-red-zone
ASFLAGS  = -g -m64


OBJS = \
		cxx.o \
	   	main.o \
	   	babyos.o \
		uart.o \
		delay.o \
		vbe.o \
		string.o \
		console.o \
		bootmem.o \
		buddy.o \
		math.o \
		i8259a.o \
		i8254.o \
		entry.o \
	    cpu.o \
		rtc.o \
		keyboard.o \
		syscall.o \


BOOTSIZE   = 1
LOADERSIZE = 3
KERNELSIZE = 400
FONTSIZE   = 8
FONTLBA    = 2048
FLOPPYSIZE = 1024
DISKSIZE   = 4096
babyos: boot loader kernel
	../tool/checksize boot   ${BOOTSIZE}
	../tool/checksize loader ${LOADERSIZE}
	../tool/checksize kernel ${KERNELSIZE}

	dd if=/dev/zero of=boot.img    ibs=512 seek=0 count=${FLOPPYSIZE}
	dd if=/dev/zero of=baby_hd.img ibs=512 seek=0 count=${DISKSIZE}

	dd if=boot              of=boot.img    ibs=512 seek=0 			  	count=${BOOTSIZE}   conv=notrunc
	dd if=loader            of=baby_hd.img ibs=512 seek=0 			  	count=${LOADERSIZE} conv=notrunc
	dd if=kernel            of=baby_hd.img ibs=512 seek=${LOADERSIZE} 	count=${KERNELSIZE} conv=notrunc
	dd if=../res/font/ASC16 of=baby_hd.img ibs=512 seek=${FONTLBA}      count=${FONTSIZE}   conv=notrunc


boot: boot.S
	${CPP} ${CPPFLAGS} -c boot.S
	${LD} ${LDFLAGS} --oformat binary -N -Ttext 0x7c00 -o boot boot.o

loader: head.S loadmain.cc
	${CPP} -fno-builtin -fno-pic -m64 -nostdinc -c head.S
	${CPP} -fno-builtin -fno-pic -m64 -nostdinc -fno-stack-protector -O -c loadmain.cc
	${LD} -m elf_x86_64 -Ttext 0x0000 -o loader.o head.o loadmain.o
	${OBJCOPY} -S -O binary -j .text loader.o loader

kernel: ${OBJS} kernel.ld
	${LD} ${LDFLAGS} -T kernel.ld -o kernel ${OBJS}

%.o: %.c
	$(CPP) $(CPPFLAGS) -c $< -o  $@

%.o: %.S
	$(CPP) $(ASFLAGS) -c $< -o  $@


clean:
	rm -f boot loader kernel *.o *.img *.elf *.d
