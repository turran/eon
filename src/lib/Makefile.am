MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = backend theme

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/include/generated \
-I. \
@EON_CFLAGS@

lib_LTLIBRARIES = libeon.la

libeon_la_SOURCES = \
eon_backend.c \
eon_button_base.c \
eon_button.c \
eon_canvas.c \
eon_container.c \
eon_element.c \
eon_frame.c \
eon_image.c \
eon_input.c \
eon_label.c \
eon_layout.c \
eon_main.c \
eon_misc.c \
eon_checkbox.c \
eon_progressbar.c \
eon_radio.c \
eon_stack.c \
eon_scrollbar.c \
eon_splitter.c \
eon_scrollview.c \
eon_theme.c \
eon_toggle.c \
eon_wrapper.c \
eon_widget.c \
eon_window.c

libeon_la_LIBADD = \
theme/libtheme.la \
backend/libbackend.la \
@EON_LIBS@

libeon_la_DEPENDENCIES = \
theme/libtheme.la \
backend/libbackend.la

libeon_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
