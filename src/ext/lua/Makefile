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


LUA_T = lua
LUA_O = lua.o

LUAC_T = luac
LUAC_O = luac.o

LUA_A = liblua.a
CORE_O = lapi.o lcode.o lctype.o ldebug.o ldo.o ldump.o lfunc.o lgc.o llex.o \
    lmem.o lobject.o lopcodes.o lparser.o lstate.o lstring.o ltable.o \
    ltm.o lundump.o lvm.o lzio.o
LIB_O =  lauxlib.o lbaselib.o lbitlib.o lcorolib.o ldblib.o liolib.o \
    lmathlib.o loslib.o lstrlib.o ltablib.o lutf8lib.o loadlib.o linit.o
BASE_O = $(CORE_O) $(LIB_O)


$(do)$(LUA_A): $(call mkobjs,$(BASE_O))
	@echo ar $^ to $@
	@$(AR) $(ARFLAGS) $@ $^


OBJS += $(call mkobjs,$(SUB_OBJS))
USERAPPS += $(call mkobjs,lua luac)

USERFLAGS := -e main -Ttext 0x1000 
$(do)$(LUA_T): $(call mkobjs, $(BASE_O) $(LUA_O)) $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(do)$(LUAC_T): $(call mkobjs, $(BASE_O) $(LUAC_O)) $(USERLIBS)
	@$(LD) $(LDFLAGS) -o $@ $^

$(eval $(call std_footer))

