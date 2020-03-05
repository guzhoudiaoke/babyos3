define top_level_config
do     := $(OBJTOP)/
ds 		:= 
stack 	:=
endef

define start_directory
DIRECTORY 	:= $1
DIRECTORY1 	:= $(subst $(SRCTOP),,$1)
SUBDIRS 	:=
endef

define std_header
stack 					:= $(stack).rec
dirstackdo_$$(stack)   := $(do)
dirstackds_$$(stack) 	:= $(ds)
do  					:= $(do)$(DIRECTORY1)/
ds 						:= $(ds)$(DIRECTORY)/
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
	$2 $3 -o $$@ -c $$<

$1/%.o: $(SRCTOP)/%.cc
	@echo Compiling $$@ dep $$< flag $(CPPFLAGS) incs $(INCS)
	$2 $3 -o $$@ -c $$<

$1/%.o: $(SRCTOP)/%.S
	@echo Compiling $$@ dep $$< flag $(ASFLAGS) incs $(INCS)
	$2 $(ASFLAGS) -o $$@ -c $$<
endef

define define_linklibrary
$1/%.a:
	@echo Linking library $$@ dep $$^
	$2 $$(ARFLAGS) $$@ $$^
endef


USERLIBS_LIST = \
				.../lib/libc.a \

mkobjs = $(addprefix $(do),$(1))
mklibs = $(patsubst .../%,$(OBJTOP)/%,$(foreach lib,$(1),$(filter %lib$(lib).a,$(USERLIBS_LIST))))

USERLIBS = $(call mklibs,c)

$(eval $(call top_level_config))

INCS 		:= 
LIBS 		:=
OBJS 		:=
PROGRAMS 	:=

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


MKDIRS := $(sort $(dir $(OBJS) $(LIBS) $(PROGRAMS)))

$(OBJS) $(LIBS) $(PROGRAMS): | mkdirs
