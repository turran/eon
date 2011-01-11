MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EON_CFLAGS@

lib_LTLIBRARIES = libeon.la

libeon_la_SOURCES = \
eon_main.c \
eon_canvas.c

libeon_la_LIBADD = \
@EON_LIBS@

libeon_la_DEPENDENCIES =

libeon_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
