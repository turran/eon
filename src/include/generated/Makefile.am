MAINTAINERCLEANFILES = Makefile.in config.h.in~ config.h.in

BUILT_SOURCES = \
eon_generated_canvas.c \
eon_generated_element.c \
eon_generated_frame.c \
eon_generated_widget.c \
eon_generated_layout.c \
eon_generated_container.c \
eon_generated_stack.c \
eon_generated_progressbar.c \
eon_generated_radio.c \
eon_generated_checkbox.c \
eon_generated_label.c \
eon_generated_button.c \
eon_generated_button_base.c \
eon_generated_scrollview.c \
eon_generated_image.c \
eon_generated_wrapper.c

%.c:	$(top_srcdir)/data/ender/eon.ender
	ender_generator $(top_srcdir)/data/ender/eon.ender $(basename $(subst eon_generated_,,$@)) $@
