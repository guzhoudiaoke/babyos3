$(eval $(call start_directory,sys))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

kernel_OBJS = \
			  cxx.o \
			  main.o \
			  babyos.o \
			  uart.o \
			  delay.o \
			  vbe.o \
			  string.o \
			  dlist.o \
			  console.o \
			  bootmem.o \
			  bootinfo.o \
			  mm.o \
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
		      slab.o \

OBJS += $(call mkobjs,$(kernel_OBJS))
PROGRAMS += $(call mkobjs,boot loader kernel)

BOOT   +=$(call mkobjs,boot)
LOADER +=$(call mkobjs,loader)
KERNEL +=$(call mkobjs,kernel)

$(d64)boot: $(call mkobjs,boot.o)
	$(LD) $(LDFLAGS) $(BOOTFLAGS) -o $@ $^

$(d64)loader.o: $(call mkobjs,head.o loadmain.o)
	$(LD) $(LOADERFLAGS) -o $@ $^
$(d64)loader: $(d64)loader.o
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

#boot: boot.S
#	$(CC) $(CPPFLAGS) -c boot.S
#	$(LD) $(LOADERFLAGS) --oformat binary -N -Ttext 0x7c00 -o boot boot.o

#loader: head.S loadmain.cc
#	${CC} -fno-builtin -fno-pic -m64 -nostdinc -c head.S
#	${CC} -fno-builtin -fno-pic -m64 -nostdinc -fno-stack-protector -O -c loadmain.cc
#	${LD} -m elf_x86_64 -Ttext 0x0000 -o loader.o head.o loadmain.o
#	${OBJCOPY} -S -O binary -j .text loader.o loader

$(d64)kernel: $(call mkobjs,$(kernel_OBJS))
	$(LD) $(LDFLAGS) $(KERNELFLAGS) -o $@ $^

$(eval $(call std_footer))
