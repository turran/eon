MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libeon_ecore_remote.la

libeon_ecore_remote_la_CFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EON_CFLAGS@ \
@ECORE_IPC_CFLAGS@

libeon_ecore_remote_la_SOURCES = \
eon_ecore_remote.c

libeon_ecore_remote_la_LIBADD = \
@ECORE_IPC_LIBS@

libeon_ecore_remote_la_DEPENDENCIES =

libeon_ecore_remote_la_LDFLAGS =

