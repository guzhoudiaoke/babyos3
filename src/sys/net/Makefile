$(eval $(call start_directory,net))
$(eval $(call std_header))
$(eval $(call add_to_sys_include_path))

SUB_OBJS = \
		   socket.o \
		   socket_local.o \
	       sock_ring_buffer.o \
		   sock_addr.o \

KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
