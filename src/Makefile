CC= gcc
LD = ld

LDFLAGS = -m elf_i386 -static
CFLAGS  = -g -w -trigraphs -fno-builtin -fno-exceptions -fno-stack-protector -m32 -fno-rtti -nostdlib -nodefaultlibs -Werror -fno-omit-frame-pointer

OBJS = \
	cxx.o\

all: babyos

babyos: boot loader
	../tool/checksize boot 1
	../tool/checksize loader 2

	dd if=/dev/zero of=boot.img bs=512 count=1024
	dd if=/dev/zero of=baby_hd.img bs=512 count=1024

	dd if=boot              of=boot.img    ibs=512 seek=0   count=1 conv=notrunc
	dd if=loader            of=baby_hd.img ibs=512 seek=0   count=1 conv=notrunc


boot: boot.S
	${CC} ${CFLAGS} -c boot.S
	${LD} ${LDFLAGS} --oformat binary -N -Ttext 0x7c00 -o boot boot.o

loader: load.S loadmain.c
	${CC} ${CFLAGS} -c load.S
	${CC} ${CFLAGS} -O -c loadmain.c
	${LD} ${LDFLAGS} -Ttext 0x0000 -o loader.o load.o loadmain.o
	objcopy -S -O binary -j .text loader.o loader

%.o: %.cc
	$(CC) $(CFLAGS) -c $< -o  $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o  $@


clean:
	rm -f boot loader kernel *.o *.img *.elf
