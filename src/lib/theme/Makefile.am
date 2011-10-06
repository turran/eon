MAINTAINERCLEANFILES = \
Makefile.in

SUBDIRS = basic

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/include/generated \
-I. \
@EON_CFLAGS@

noinst_LTLIBRARIES = libtheme.la

libtheme_la_DEPENDENCIES = \
basic/libtheme_basic.la

libtheme_la_LIBADD = \
basic/libtheme_basic.la

libtheme_la_SOURCES = \
eon_theme_checkbox.c \
eon_theme_container.c \
eon_theme_button.c \
eon_theme_button_base.c \
eon_theme_entry.c \
eon_theme_frame.c \
eon_theme_image.c \
eon_theme_label.c \
eon_theme_layout.c \
eon_theme_progressbar.c \
eon_theme_radio.c \
eon_theme_scrollbar.c \
eon_theme_splitter.c \
eon_theme_scrollview.c \
eon_theme_widget.c

