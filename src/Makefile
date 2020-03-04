$(eval $(call start_directory,src))
stack 					:= $(stack).rec
dirstackds_$$(stack) 	:= $(ds)
ds 						:= $(ds)$(DIRECTORY)/


SUBDIRS := sys lib bin
$(eval $(call include_subdirs))


ds 		:= $(dirstackds_$(stack))
stack 	:= $$(basename $(stack))
