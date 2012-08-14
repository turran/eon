MAINTAINERCLEANFILES = Makefile.in

SUBDIRS = core backend extension theme

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/include/generated \
-I. \
@EON_CFLAGS@

lib_LTLIBRARIES = libeon.la

libeon_la_SOURCES =

libeon_la_LIBADD = \
core/libcore.la \
backend/libbackend.la \
extension/libextension.la \
@EON_LIBS@

libeon_la_DEPENDENCIES = \
core/libcore.la \
backend/libbackend.la \
extension/libextension.la

libeon_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
