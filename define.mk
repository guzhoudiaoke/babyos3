define top_level_config
do      := $(OBJTOP)/
ds 		:= 
stack 	:=
endef


define start_directory
DIRECTORY 	:= $1
DIRECTORYO 	:= $(subst $(SRCTOP),,$1)
SUBDIRS 	:=
endef


define std_header
stack 				 := $(stack).rec
dirstackdo_$$(stack) := $(do)
dirstackds_$$(stack) := $(ds)
do  				 := $(do)$(DIRECTORYO)/
ds 					 := $(ds)$(DIRECTORY)/
endef


define std_footer
do		:= $(dirstackdo_$(stack))
ds 		:= $(dirstackds_$(stack))
stack 	:= $$(basename $(stack))
endef


define include_subdirs
include $(foreach dir,$(SUBDIRS), $(ds)$(dir)/Makefile)
endef


define add_to_lib_include_path
LIB_INCS += -I$(ds)
endef

define add_to_user_include_path
USR_INCS += -I$(ds)
endef

define add_to_sys_include_path
SYS_INCS += -I$(ds)
endef


define define_compiler
$1/%.o: $(2)/%.c
	@echo Compiling $$@ $$<
	@$3 $(CPPFLAGS) $4 -o $$@ -c $$<

$1/%.o: $(2)/%.cc
	@echo Compiling $$@ $$<
	@$3 $(CPPFLAGS) $4 -o $$@ -c $$<

$1/%.o: $(2)/%.S
	@echo Compiling $$@ $$<
	@$3 $(ASFLAGS) $4 -o $$@ -c $$<
endef


define define_linklibrary
$1/%.a:
	@echo Linking library $$@ $$^
	@$2 $(ARFLAGS) $$@ $$^
endef

