$(eval $(call start_directory,device))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

device_OBJS = \
		  console.o \
		  block_dev.o \
		  vbe.o \
		  io_buffer.o \


KOBJS += $(call mkobjs,$(device_OBJS))

$(eval $(call std_footer))
