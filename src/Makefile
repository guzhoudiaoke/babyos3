.PHONY: all
all: babyos

CPP = g++
LD = ld
OBJCOPY = objcopy

LDFLAGS  = -m elf_x86_64 -nodefaultlibs
CPPFLAGS = -g -w -fno-builtin -fno-exceptions -fno-stack-protector -m64 -fno-rtti -nostdlib -nodefaultlibs -Werror -fno-omit-frame-pointer -mcmodel=small -fno-pie
ASFLAGS  = -gdwarf-2 -m64 -DX64 -mcmodel=small -mtls-direct-seg-refs -mno-red-zone -fno-pie

OBJS = \
	   main.o \


BOOTSIZE   = 1
LOADERSIZE = 3
KERNELSIZE = 10
DISKSIZE   = 1024
babyos: boot loader kernel
	../tool/checksize boot   ${BOOTSIZE}
	../tool/checksize loader ${LOADERSIZE}
	../tool/checksize kernel ${KERNELSIZE}

	dd if=/dev/zero of=boot.img    ibs=512 seek=0 				count=${DISKSIZE}
	dd if=/dev/zero of=baby_hd.img ibs=512 seek=0 				count=${DISKSIZE}

	dd if=boot      of=boot.img    ibs=512 seek=0 			  	count=${BOOTSIZE}   conv=notrunc
	dd if=loader    of=baby_hd.img ibs=512 seek=0 			  	count=${LOADERSIZE} conv=notrunc
	dd if=kernel    of=baby_hd.img ibs=512 seek=${LOADERSIZE} 	count=${KERNELSIZE} conv=notrunc


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
	rm -f boot loader kernel *.o *.img *.elf
