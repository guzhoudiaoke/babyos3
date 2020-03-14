USERLIBS_LIST = \
				.../lib/libc/libc.a \
				.../lib/libc++/libc++.a \

mkobjs = $(addprefix $(do),$(1))
mklibs = $(patsubst .../%,$(OBJTOP)/%,$(foreach lib,$(1),$(filter %lib$(lib).a,$(USERLIBS_LIST))))

USERLIBS = $(call mklibs,c c++)

$(eval $(call top_level_config))

LIB_INCS 	:= 
USR_INCS 	:= 
SYS_INCS 	:= 
LIBS 		:=
OBJS 		:=

# execs
BOOT        :=
LOADER      :=
KERNEL      :=
USERAPPS    :=

SUBDIRS     := src
$(eval $(call include_subdirs))


$(eval $(call define_compiler,$(OBJTOP)/sys,$(SRCTOP)/sys,$(CPP),$(SYS_INCS)))
$(eval $(call define_compiler,$(OBJTOP),$(SRCTOP),$(CPP) $(LIB_INCS)))
#$(eval $(call define_compiler,$(OBJTOP)/lib,$(SRCTOP)/lib,$(CC) $(LIB_INCS)))
#$(eval $(call define_compiler,$(OBJTOP)/bin,$(SRCTOP)/bin,$(CC) $(LIB_INCS)))
$(eval $(call define_linklibrary,$(OBJTOP),$(AR)))


MKDIRS := $(sort $(dir $(OBJS) $(LIBS) $(BOOT) $(LOADER) $(KERNEL) $(USERAPPS)))
$(OBJS) $(LIBS) $(BOOT) $(LOADER) $(KERNEL) $(USERAPPS) : | mkdirs


.PHONY: mkdirs
mkdirs:
	@mkdir -p $(MKDIRS)
