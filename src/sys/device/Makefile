$(eval $(call start_directory,device))
$(eval $(call std_header))
$(eval $(call add_to_sys_include_path))

SUB_OBJS = \
		  console.o \
		  block_dev.o \
		  vbe.o \
		  io_buffer.o \


KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
