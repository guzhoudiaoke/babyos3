$(eval $(call start_directory,boot))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

SUB_OBJS = \
			boot.o \
			head.o \
			bootmain.o \

OBJS   +=$(call mkobjs,$(SUB_OBJS))
BOOT   +=$(call mkobjs,boot)
LOADER +=$(call mkobjs,loader)

BOOTFLAGS    := --oformat binary -N -Ttext 0x7c00 
LOADERFLAGS  := -m elf_x86_64 -Ttext 0x0000 
OBJCOPYFLAGS := -S -O binary -j .text

$(do)boot: $(call mkobjs,boot.o)
	$(LD) $(LDFLAGS) $(BOOTFLAGS) -o $@ $^

$(do)loader.o: $(call mkobjs,head.o loadmain.o)
	$(LD) $(LOADERFLAGS) -o $@ $^

$(do)loader: $(do)loader.o
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(eval $(call std_footer))
