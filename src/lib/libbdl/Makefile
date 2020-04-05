$(eval $(call start_directory,libbdl))
$(eval $(call std_header))
$(eval $(call add_to_lib_include_path))

LIBBDL_OBJS :=
SUBDIRS := 
$(eval $(call include_subdirs))

SUB_OBJS = \
	video.o \
	video_device.o \
	window.o \
	rect.o \
	render.o \
	surface.o \
	

LIBS += $(call mkobjs,libbdl.a)
OBJS += $(call mkobjs,$(SUB_OBJS))
LIBBDL_OBJS += $(call mkobjs,$(SUB_OBJS))

$(do)libbdl.a: $(LIBBDL_OBJS) $(LIBC) $(LIBM) $(LIBCXX)

$(eval $(call std_footer))