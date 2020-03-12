$(eval $(call start_directory,task))
$(eval $(call std_header))
$(eval $(call add_to_sys_include_path))

SUB_OBJS = \
		  process.o \
		  process_mgr.o \
		  timer.o \
		  timer_mgr.o \
		  waitqueue.o \

KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
