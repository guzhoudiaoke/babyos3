$(eval $(call start_directory,driver))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

driver_OBJS = \
		  uart.o \
		  i8254.o \
		  i8259a.o \
		  keyboard.o \
		  ide.o \
		  rtc.o \

KOBJS += $(call mkobjs,$(driver_OBJS))

$(eval $(call std_footer))
