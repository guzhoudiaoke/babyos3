define top_level_config
do     := $(OBJTOP)/
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

define add_to_include_path
INCS += -I$(ds)
endef

define define_compiler
$1/%.o: $(SRCTOP)/%.c
	@$2 $3 -o $$@ -c $$<

$1/%.o: $(SRCTOP)/%.cc
	@echo Compiling $$@ $$<
	@$2 $3 -o $$@ -c $$<

$1/%.o: $(SRCTOP)/%.S
	@echo Compiling $$@ $$<
	@$2 $(ASFLAGS) -o $$@ -c $$<
endef

define define_linklibrary
$1/%.a:
	@echo Linking library $$@ $$^
	@$2 $$(ARFLAGS) $$@ $$^
endef


USERLIBS_LIST = \
				.../lib/libc/libc.a \
				.../lib/libc++/libc++.a \

mkobjs = $(addprefix $(do),$(1))
mklibs = $(patsubst .../%,$(OBJTOP)/%,$(foreach lib,$(1),$(filter %lib$(lib).a,$(USERLIBS_LIST))))

USERLIBS = $(call mklibs,c c++)

$(eval $(call top_level_config))

INCS 		:= 
LIBS 		:=
OBJS 		:=

# execs
BOOT     :=
LOADER   :=
KERNEL   :=
USERAPPS :=

SUBDIRS := src
$(eval $(call include_subdirs))

CPPFLAGS += $(INCS)

$(eval $(call define_compiler,$(OBJTOP),$(CC) $(CPPFLAGS)))
$(eval $(call define_linklibrary,$(OBJTOP),$(AR)))


MKDIRS := $(sort $(dir $(OBJS) $(LIBS) $(BOOT) $(LOADER) $(KERNEL) $(USERAPPS)))
$(OBJS) $(LIBS) $(BOOT) $(LOADER) $(KERNEL) $(USERAPPS) : | mkdirs



.PHONY: mkdirs
mkdirs:
	mkdir -p $(MKDIRS)
