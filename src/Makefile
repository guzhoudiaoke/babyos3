CPP= g++
LD = ld

LDFLAGS = -m elf_i386 -static
CPPFLAGS = -g -w -trigraphs -fno-builtin -fno-exceptions -fno-stack-protector -m32 -fno-rtti -nostdlib -nodefaultlibs -Werror -fno-omit-frame-pointer

all: babyos

babyos: boot
	dd if=boot              of=boot.img    ibs=512 seek=0   count=1 conv=notrunc

boot: boot.S
	${CPP} ${CPPFLAGS} -c boot.S
	${LD} ${LDFLAGS} --oformat binary -N -Ttext 0x7c00 -o boot boot.o

%.o: %.cc
	$(CPP) $(CPPFLAGS) -c $< -o  $@

%.o: %.S
	$(CPP) $(CPPFLAGS) -c $< -o  $@


clean:
	rm -f boot *.o *.img
