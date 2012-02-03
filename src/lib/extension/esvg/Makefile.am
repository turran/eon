MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libeon_svg.la

libeon_svg_la_CFLAGS = \
-I$(top_srcdir)/src/include \
-I$(top_srcdir)/src/include/generated \
-I. \
@EON_CFLAGS@ \
@ESVG_CFLAGS@

libeon_svg_la_SOURCES = \
eon_svg.c

libeon_svg_la_LIBADD = \
@ESVG_LIBS@

libeon_svg_la_DEPENDENCIES =

libeon_svg_la_LDFLAGS =

