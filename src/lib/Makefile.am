MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = theme

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EON_CFLAGS@

lib_LTLIBRARIES = libeon.la

libeon_la_SOURCES = \
eon_button.c \
eon_canvas.c \
eon_element.c \
eon_label.c \
eon_layout.c \
eon_main.c \
eon_radio.c \
eon_stack.c \
eon_theme.c \
eon_theme_widget.c \
eon_widget.c

libeon_la_LIBADD = \
theme/libtheme_basic.la \
@EON_LIBS@

libeon_la_DEPENDENCIES = \
theme/libtheme_basic.la

libeon_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
