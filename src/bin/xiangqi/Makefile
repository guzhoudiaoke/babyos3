$(eval $(call start_directory,xiangqi))
$(eval $(call std_header))

SUB_OBJS = \
		   xq.o \


OBJS += $(call mkobjs,$(SUB_OBJS))
USERAPPS += $(call mkobjs,xq)

$(do)xq: $(call mkobjs, $(SUB_OBJS)) $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(eval $(call std_footer))

