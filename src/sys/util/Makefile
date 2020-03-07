$(eval $(call start_directory,util))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

SUB_OBJS = \
		  cxx.o \
		  delay.o \
		  math.o \
		  dlist.o \
		  kstring.o \
		  elf.o \

KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
