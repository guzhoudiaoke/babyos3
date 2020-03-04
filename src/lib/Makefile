$(eval $(call start_directory,lib))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

libc_OBJS = \
	userlib.o \

LIBS += $(call mkobjs,libc.a)
OBJS += $(call mkobjs,$(libc_OBJS))

$(d64)libc.a: $(call mkobjs,$(libc_OBJS))

$(eval $(call std_footer))
