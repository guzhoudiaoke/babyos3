$(eval $(call start_directory,bin))
$(eval $(call std_header))

SUB_OBJS = \
		   init.o \
		   shell.o \
		   cat.o \
		   ln.o \
		   ls.o \
		   rm.o \
		   mv.o \
		   mkdir.o \
		   cp.o \
		   test_local_socket.o \

OBJS += $(call mkobjs,$(SUB_OBJS))
USERAPPS += $(call mkobjs,init shell cat ln ls rm mv mkdir cp test_socket)


USERFLAGS := -e main -Ttext 0x1000 

$(do)init: $(do)init.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)shell: $(do)shell.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)cat: $(do)cat.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)ln: $(do)ln.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)ls: $(do)ls.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)rm: $(do)rm.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)mv: $(do)mv.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)mkdir: $(do)mkdir.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)cp: $(do)cp.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(do)test_socket: $(do)test_local_socket.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^


$(eval $(call std_footer))

