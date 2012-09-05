MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = theme core backend extension

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I$(top_builddir)/src/include \
-I$(top_srcdir)/src/include/generated \
-I$(top_builddir)/src/include/generated \
-I. \
@EON_CFLAGS@

lib_LTLIBRARIES = libeon.la

libeon_la_SOURCES =

libeon_la_LIBADD = \
core/libcore.la \
backend/libbackend.la \
extension/libextension.la \
@EON_LIBS@

libeon_la_LDFLAGS = -no-undefined -version-info @version_info@
