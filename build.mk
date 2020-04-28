USERLIBS_LIST = \
				.../lib/libc/libc.a \
				.../lib/libm/libm.a \
				.../lib/libc++/libc++.a \
				.../lib/libbdl/libbdl.a \
				.../lib/libpng/libpng.a \
				.../lib/libzlib/libzlib.a \

mkobjs = $(addprefix $(do),$(1))
mklibs = $(patsubst .../%,$(OBJTOP)/%,$(foreach lib,$(1),$(filter %lib$(lib).a,$(USERLIBS_LIST))))

USERLIBS = $(call mklibs,bdl png c c++ zlib m)
LIBC = $(call mklibs, c)
LIBM = $(call mklibs, m)
LIBCXX = $(call mklibs, c++)
LIBZLIB = $(call mklibs, zlib)
LIBPNG = $(call mklibs, png)

$(eval $(call top_level_config))

LIB_INCS 	:= -I src/include -I src/include/babyos
USR_INCS 	:= -I src/include -I src/include/babyos
SYS_INCS 	:= -I src/include/
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
$(eval $(call define_linklibrary,$(OBJTOP),$(AR)))


MKDIRS := $(sort $(dir $(OBJS) $(LIBS) $(BOOT) $(LOADER) $(KERNEL) $(USERAPPS)))
$(OBJS) $(LIBS) $(BOOT) $(LOADER) $(KERNEL) $(USERAPPS) : | mkdirs


.PHONY: mkdirs
mkdirs:
	@mkdir -p $(MKDIRS)
