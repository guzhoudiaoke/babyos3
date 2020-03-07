$(eval $(call start_directory,lib))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

libc_OBJS = \
	unistd.o \
	stdio.o \
	string.o \
	malloc.o \

LIBS += $(call mkobjs,libc.a)
OBJS += $(call mkobjs,$(libc_OBJS))

$(do)libc.a: $(call mkobjs,$(libc_OBJS))

$(eval $(call std_footer))
