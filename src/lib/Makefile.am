MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = drawer 
#theme core backend extension

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib/drawer \
@EON_BUILD@ \
@EON_CFLAGS@

lib_LTLIBRARIES = libeon.la

libeon_la_SOURCES = \
eon_document.c \
eon_element.c \
eon_element_button.c \
eon_element_eon.c \
eon_element_label.c \
eon_event_geometry.c \
eon_main.c \
eon_renderable.c \
eon_horizontal_align.c \
eon_vertical_align.c \
eon_widget.c

libeon_la_LIBADD = \
$(top_builddir)/src/lib/drawer/libeon_drawer.la \
@EON_LIBS@

#core/libcore.la \
#backend/libbackend.la \
#extension/libextension.la

if BUILD_STATIC_MODULE_DRAWER_BASIC
libeon_la_LIBADD += $(top_builddir)/src/modules/basic/libbasic.la
endif


libeon_la_LDFLAGS = -no-undefined -version-info @version_info@
