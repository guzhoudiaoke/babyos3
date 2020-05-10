$(eval $(call start_directory,fs))
$(eval $(call std_header))
$(eval $(call add_to_sys_include_path))

SUB_OBJS = \
		  fs.o \
		  fd.o \
		  file_table.o \
		  inode.o \
		  file.o \


KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
