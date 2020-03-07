$(eval $(call start_directory,arch))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

SUB_OBJS = \
		  cpu.o \
		  entry.o \
		  io_apic.o \
		  local_apic.o \
		  syscall.o \
		  sys_socket.o \

KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
