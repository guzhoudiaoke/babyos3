$(eval $(call start_directory,libpng))
$(eval $(call std_header))
$(eval $(call add_to_lib_include_path))

LIBPNG_OBJS :=
SUBDIRS := 
$(eval $(call include_subdirs))

SUB_OBJS = \
	png.o \
	pngerror.o \
	pngget.o \
	pngmem.o \
	pngpread.o \
	pngread.o \
	pngrio.o \
	pngrtran.o \
	pngrutil.o \
	pngset.o \
	pngtrans.o \
	pngwio.o \
	pngwrite.o \
	pngwtran.o \
	pngwutil.o \
	pngtest.o \


LIBS += $(call mkobjs,libpng.a)
OBJS += $(call mkobjs,$(SUB_OBJS))
LIBPNG_OBJS += $(call mkobjs,$(SUB_OBJS))

$(do)libpng.a: $(LIBPNG_OBJS)

$(eval $(call std_footer))
