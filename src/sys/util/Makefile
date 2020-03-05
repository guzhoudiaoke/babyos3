$(eval $(call start_directory,util))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

util_OBJS = \
		  cxx.o \
		  delay.o \
		  math.o \
		  dlist.o \
		  string.o \
		  elf.o \

KOBJS += $(call mkobjs,$(util_OBJS))

$(eval $(call std_footer))
