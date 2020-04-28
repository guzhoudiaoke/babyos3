$(eval $(call start_directory,bin))
$(eval $(call std_header))

SUBDIRS := shell xiangqi
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
		   ps.o \
		   test_socket.o \
		   test_fork.o \
		   test_wait.o \
		   test_pipe.o \
		   test_sig.o \
		   test_fb.o \
		   testbdl.o \
		   testdraw.o \
		   testtexture.o \
		   testmouse.o \
		   testpng.o \


OBJS += $(call mkobjs,$(SUB_OBJS))

APPS = init \
	   cat \
	   ln \
	   ls \
	   rm \
	   mv \
	   mkdir \
	   cp \
	   ps \
	   test_socket \
	   test_fork \
	   test_wait \
	   test_pipe \
	   test_sig \
	   test_fb \
	   testbdl \
	   testdraw \
	   testtexture \
	   testmouse \
	   testpng \


USERAPPS += $(call mkobjs, $(APPS))


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

$(do)ps: $(do)ps.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_socket: $(do)test_socket.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_fork: $(do)test_fork.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_wait: $(do)test_wait.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_pipe: $(do)test_pipe.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_sig: $(do)test_sig.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)test_fb: $(do)test_fb.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)testbdl: $(do)testbdl.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)testdraw: $(do)testdraw.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)testtexture: $(do)testtexture.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)testmouse: $(do)testmouse.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)testpng: $(do)testpng.o $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^


$(eval $(call std_footer))

