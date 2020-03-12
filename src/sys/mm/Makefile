$(eval $(call start_directory,mm))
$(eval $(call std_header))
$(eval $(call add_to_sys_include_path))

SUB_OBJS = \
		  bootmem.o \
		  mm.o \
		  vm.o \
		  buddy.o \
		  slab.o \

KOBJS += $(call mkobjs,$(SUB_OBJS))

$(eval $(call std_footer))
