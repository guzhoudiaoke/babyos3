$(eval $(call start_directory,ipc))
$(eval $(call std_header))
$(eval $(call add_to_sys_include_path))

SUB_OBJS = \
		  spinlock.o \
		  sem.o \
		  pipe.o \
		  signal.o \

KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
