$(eval $(call start_directory,driver))
$(eval $(call std_header))
$(eval $(call add_to_sys_include_path))

SUB_OBJS = \
		  uart.o \
		  i8254.o \
		  i8259a.o \
		  keyboard.o \
		  ide.o \
		  rtc.o \

KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
