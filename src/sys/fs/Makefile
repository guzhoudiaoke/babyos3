$(eval $(call start_directory,fs))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

mm_OBJS = \
		  fs.o \
		  file.o \
		  file_table.o \
		  inode.o \


KOBJS += $(call mkobjs,$(mm_OBJS))

$(eval $(call std_footer))
