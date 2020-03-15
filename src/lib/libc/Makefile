$(eval $(call start_directory,libc))
$(eval $(call std_header))
$(eval $(call add_to_lib_include_path))

SUB_OBJS = \
	crt0.o \
	init.o \
	unistd.o \
	stdio.o \
	string.o \
	malloc.o \
	stdlib.o \
	errno.o \
	math.o \
	time.o \
	locale.o \
	signal.o \
	sprintf.o \
	fileop.o \
	printf.o \
	gets.o \
	ctype.o \
	flags.o \

LIBS += $(call mkobjs,libc.a)
OBJS += $(call mkobjs,$(SUB_OBJS))

$(do)libc.a: $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
