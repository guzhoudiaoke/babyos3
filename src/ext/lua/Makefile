$(eval $(call start_directory,lua))
$(eval $(call std_header))

SUB_OBJS = \
		lcorolib.o \
		lcode.o \
		lbitlib.o \
		lbaselib.o \
		lauxlib.o \
		lapi.o \
		lstate.o \
		lparser.o \
		loslib.o \
		lopcodes.o \
		lobject.o \
		loadlib.o \
		lmem.o \
		lmathlib.o \
		llex.o \
		liolib.o \
		linit.o \
		lgc.o \
		lfunc.o \
		ldump.o \
		ldo.o \
		ldebug.o \
		ldblib.o \
		lctype.o \
		lzio.o \
		lvm.o \
		lutf8lib.o \
		lundump.o \
		luac.o \
		lua.o \
		ltm.o \
		ltablib.o \
		ltable.o \
		lstrlib.o \
		lstring.o \


OBJS += $(call mkobjs,$(SUB_OBJS))
USERAPPS += $(call mkobjs,lua)

$(do)lua: $(call mkobjs, $(SUB_OBJS)) $(USERLIBS)
	@$(LD) $(LDFLAGS) $(USERFLAGS) -o $@ $^

$(eval $(call std_footer))

