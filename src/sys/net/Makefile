$(eval $(call start_directory,net))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

net_OBJS = \
		   socket.o \
		   socket_local.o \
	       sock_ring_buffer.o \
		   sock_addr.o \

KOBJS += $(call mkobjs,$(net_OBJS))

$(eval $(call std_footer))
