$(eval $(call start_directory,bin))
$(eval $(call std_header))

bin_OBJS = \
		   init.o \
		   shell.o \
		   cat.o \
		   ln.o \
		   ls.o \
		   rm.o \
		   test_local_socket.o \

OBJS += $(call mkobjs,$(bin_OBJS))
PROGRAMS += $(call mkobjs,init shell cat ln ls rm test_socket)
USERAPPS += $(call mkobjs,init shell cat ln ls rm test_socket)

$(d64)init: $(d64)init.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^
$(d64)shell: $(d64)shell.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^
$(d64)cat: $(d64)cat.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^
$(d64)ln: $(d64)ln.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^
$(d64)ls: $(d64)ls.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^
$(d64)rm: $(d64)rm.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^
$(d64)test_socket: $(d64)test_local_socket.o $(USERLIBS)
	$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(eval $(call std_footer))

