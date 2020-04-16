$(eval $(call start_directory,x86_64))
$(eval $(call std_header))
$(eval $(call add_to_lib_include_path))

SUB_OBJS = \
	trunc.o \
	truncf.o \
	s_tan.o \
	s_tanh.o \
	s_sin.o \
	sqrt.o \
	sqrtf.o \
	s_log1p.o \
	s_expm1.o \
	s_cos.o \
	s_cbrt.o \
	s_atan.o \
	floorf.o \
	e_sinh.o \
	e_pow.o \
	e_log.o \
	e_log10.o \
	e_hypot.o \
	e_exp.o \
	e_cosh.o \
	e_atan2.o \
	e_asin.o \
	e_acos.o \
	ceil.o \
	ceilf.o \

OBJS += $(call mkobjs,$(SUB_OBJS))
LIBM_OBJS += $(call mkobjs, $(SUB_OBJS))

$(eval $(call std_footer))
