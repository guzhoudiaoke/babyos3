$(eval $(call start_directory,libm))
$(eval $(call std_header))
$(eval $(call add_to_lib_include_path))

LIBM_OBJS :=
SUBDIRS := x86_64
$(eval $(call include_subdirs))

SUB_OBJS = \
	s_roundf.o \
	s_absf.o \
	s_log2.o \
	s_copysign.o \
	s_copysignf.o \
	s_scalbn.o \
	s_scalbnf.o \
	e_powf.o \
	e_logf.o \
	e_expf.o \
	s_sinf.o \
	k_rem_pio2.o \
	s_cosf.o \
	s_trunc.o \
	s_tanh.o \
	s_expm1.o \
	s_frexp.o \
	s_rint.o \
	e_fmod.o \
	floor.o \
	math.o \
	

LIBS += $(call mkobjs,libm.a)
OBJS += $(call mkobjs,$(SUB_OBJS))
LIBM_OBJS += $(call mkobjs,$(SUB_OBJS))

$(do)libm.a: $(LIBM_OBJS)

$(eval $(call std_footer))
