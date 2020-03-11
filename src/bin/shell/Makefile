$(eval $(call start_directory,shell))
$(eval $(call std_header))

SUB_OBJS = \
		   parser.o \
		   command.o \
		   shell.o \


OBJS += $(call mkobjs,$(SUB_OBJS))
USERAPPS += $(call mkobjs,shell)

$(do)shell: $(call mkobjs, $(SUB_OBJS)) $(USERLIBS)
	@$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(eval $(call std_footer))

