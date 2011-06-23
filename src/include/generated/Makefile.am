MAINTAINERCLEANFILES = Makefile.in config.h.in~ config.h.in

BUILT_SOURCES = \
eon_generated_element.c \
eon_generated_widget.c \
eon_generated_layout.c

%.c:
	
	ender_generator $(top_srcdir)/data/ender/eon.ender $(basename $(subst eon_generated_,,$@)) $@
