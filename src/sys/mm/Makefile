$(eval $(call start_directory,mm))
$(eval $(call std_header))
$(eval $(call add_to_include_path))

mm_OBJS = \
		  bootmem.o \
		  mm.o \
		  vm.o \
		  buddy.o \
		  slab.o \

KOBJS += $(call mkobjs,$(mm_OBJS))

$(eval $(call std_footer))
