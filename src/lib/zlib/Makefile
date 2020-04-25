$(eval $(call start_directory,zlib))
$(eval $(call std_header))
$(eval $(call add_to_lib_include_path))

LIBZLIB_OBJS :=
SUBDIRS := 
$(eval $(call include_subdirs))

SUB_OBJS = \
	adler32.o \
	crc32.o \
	deflate.o \
	infback.o \
	inffast.o \
	inflate.o \
	inftrees.o \
	trees.o \
	zutil.o \
	compress.o \
	uncompr.o \
	gzclose.o \
	gzlib.o \
	gzread.o \
	gzwrite.o \



LIBS += $(call mkobjs,libzlib.a)
OBJS += $(call mkobjs,$(SUB_OBJS))
LIBZLIB_OBJS += $(call mkobjs,$(SUB_OBJS))

$(do)libzlib.a: $(LIBZLIB_OBJS)

$(eval $(call std_footer))
