MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libeon_ecore_remote.la

libeon_ecore_remote_la_CFLAGS = \
-I$(top_srcdir)/src/include \
-I$(top_build)/src/include \
-I. \
@EON_BUILD@ \
@EON_CFLAGS@ \
@EIX_CFLAGS@

libeon_ecore_remote_la_SOURCES = \
eon_ecore_remote.c

libeon_ecore_remote_la_LIBADD = @EIX_LIBS@
libeon_ecore_remote_la_LDFLAGS =

