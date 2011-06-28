MAINTAINERCLEANFILES = Makefile.in config.h.in~ config.h.in

BUILT_SOURCES = \
eon_generated_element.c \
eon_generated_widget.c \
eon_generated_layout.c \
eon_generated_container.c \
eon_generated_label.c \
eon_generated_button.c

%.c:
	
	ender_generator $(top_srcdir)/data/ender/eon.ender $(basename $(subst eon_generated_,,$@)) $@
