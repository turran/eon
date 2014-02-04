MAINTAINERCLEANFILES = Makefile.in

#SUBDIRS = theme core backend extension

AM_CPPFLAGS = \
-I. \
@EON_BUILD@ \
@EON_CFLAGS@

lib_LTLIBRARIES = libeon.la

libeon_la_SOURCES = \
eon_document.c \
eon_element.c

libeon_la_LIBADD = \
@EON_LIBS@

#core/libcore.la \
#backend/libbackend.la \
#extension/libextension.la

libeon_la_LDFLAGS = -no-undefined -version-info @version_info@
