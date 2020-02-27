.PHONY: all
all: babyos

CPP = g++
LD = ld
OBJCOPY = objcopy

LDFLAGS  = -g -m elf_x86_64
CPPFLAGS = -g -Wall -Werror -fno-exceptions -m64
CPPFLAGS += -mcmodel=kernel -fno-pic -static -fno-builtin -fno-strict-aliasing
CPPFLAGS += -MD -fno-omit-frame-pointer -ffreestanding -fno-common -nostdlib
CPPFLAGS += -gdwarf-2 -DX64 -mtls-direct-seg-refs -mno-red-zone -fno-rtti
ASFLAGS  = -g -m64
QEMU     = qemu-system-x86_64
NAME     = babyos3


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
		spinlock.o \
		ide.o \
		pool.o \
		io_buffer.o \
		process.o \
		process_mgr.o \
		timer.o \
		timer_mgr.o \
		waitqueue.o \
		vm.o \
		elf.o \
		fs.o \
		inode.o \
		block_dev.o \
		file_table.o \
		file.o \
		sem.o \
		pipe.o \
		socket.o \
		socket_local.o \
	    sock_ring_buffer.o \
		sock_addr.o \
        sys_socket.o \
		local_apic.o \
		io_apic.o \

USERAPP = \
		init \
		shell \
		ls \
		cat \
		ln \
		mkdir \
		rm \
		cp \
		testsocket \
		mv \


USERAPP := ${addprefix ./,${USERAPP}}


BOOTSIZE   = 1
LOADERSIZE = 3
KERNELSIZE = 2048
FONTSIZE   = 8
FONTLBA    = 2048
FLOPPYSIZE = 1024
DISKSIZE   = 4096
FSSIZE     = 4096

babyos: boot loader kernel ${USERAPP}
	../tool/checksize boot   ${BOOTSIZE}
	../tool/checksize loader ${LOADERSIZE}
	../tool/checksize kernel ${KERNELSIZE}

	dd if=/dev/zero of=boot.img    ibs=512 seek=0 count=${FLOPPYSIZE}
	dd if=/dev/zero of=baby_hd.img ibs=512 seek=0 count=${DISKSIZE}
	dd if=/dev/zero of=fs.img      ibs=512 seek=0 count=${FSSIZE}

	dd if=boot              of=boot.img    ibs=512 seek=0 			  	count=${BOOTSIZE}   conv=notrunc
	dd if=loader            of=baby_hd.img ibs=512 seek=0 			  	count=${LOADERSIZE} conv=notrunc
	dd if=kernel            of=baby_hd.img ibs=512 seek=${LOADERSIZE} 	count=${KERNELSIZE} conv=notrunc
	dd if=../res/font/ASC16 of=baby_hd.img ibs=512 seek=${FONTLBA}      count=${FONTSIZE}   conv=notrunc

	../tool/mkfs ${USERAPP}


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


userlib.o: userlib.cc
	${CPP} ${CPPFLAGS} -O -c userlib.cc

init: init.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c init.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o init init.o userlib.o

shell: shell.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c shell.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o shell shell.o userlib.o

ls: ls.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c ls.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o ls ls.o userlib.o

ln: ln.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c ln.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o ln ln.o userlib.o

rm: rm.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c rm.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o rm rm.o userlib.o

cat: cat.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c cat.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o cat cat.o userlib.o

mkdir: mkdir.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c mkdir.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o mkdir mkdir.o userlib.o

cp: cp.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c cp.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o cp cp.o userlib.o

mv: mv.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c mv.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o mv mv.o userlib.o

ping: ping.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c ping.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o ping ping.o userlib.o

testsocket: test_local_socket.cc userlib.o
	${CPP} ${CPPFLAGS} -O -c test_local_socket.cc
	${LD} ${LDFLAGS} -e main -Ttext 0x1000 -o testsocket test_local_socket.o userlib.o




%.o: %.cc
	$(CPP) $(CPPFLAGS) -c $< -o  $@

%.o: %.S
	$(CPP) $(ASFLAGS) -c $< -o  $@


clean:
	rm -f boot loader kernel *.o *.img *.elf *.d ${USERAPP}

qemu: boot.img baby_hd.img fs.img
	${QEMU} -fda boot.img -hda baby_hd.img -hdb fs.img -serial mon:stdio -m 128 -name ${NAME}


qdebug: boot.img baby_hd.img fs.img
	${QEMU} -fda boot.img -hda baby_hd.img -hdb fs.img -serial mon:stdio -m 128 -name ${NAME} -s -S
