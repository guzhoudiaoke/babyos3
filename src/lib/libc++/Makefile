$(eval $(call start_directory,libc++))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

SUB_OBJS = \
	cxx.o \

LIBS += $(call mkobjs,libc++.a)
OBJS += $(call mkobjs,$(SUB_OBJS))

$(do)libc++.a: $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
