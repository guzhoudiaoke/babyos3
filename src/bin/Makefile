$(eval $(call start_directory,bin))
$(eval $(call std_header))

SUBDIRS := shell
$(eval $(call include_subdirs))

SUB_OBJS = \
		   init.o \
		   cat.o \
		   ln.o \
		   ls.o \
		   rm.o \
		   mv.o \
		   mkdir.o \
		   cp.o \
		   test_socket.o \
		   test_fork.o \
		   test_wait.o \
		   test_pipe.o \


OBJS += $(call mkobjs,$(SUB_OBJS))
USERAPPS += $(call mkobjs,init cat ln ls rm mv mkdir \
						  cp test_socket test_fork \
						  test_wait test_pipe)


#USERFLAGS := -e main -Ttext 0x1000 
#USERFLAGS := -Ttext 0x1000 

$(do)init: $(do)init.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)cat: $(do)cat.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)ln: $(do)ln.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)ls: $(do)ls.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)rm: $(do)rm.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)mv: $(do)mv.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)mkdir: $(do)mkdir.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)cp: $(do)cp.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_socket: $(do)test_socket.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_fork: $(do)test_fork.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_wait: $(do)test_wait.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_pipe: $(do)test_pipe.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^


$(eval $(call std_footer))

