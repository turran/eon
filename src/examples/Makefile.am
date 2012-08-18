MAINTAINERCLEANFILES = Makefile.in

bin_PROGRAMS = \
eon_escen_viewer

if BUILD_EXTENSION_ESVG
bin_PROGRAMS += eon_svg_viewer
endif

eon_escen_viewer_SOURCES = eon_escen_viewer.c
eon_escen_viewer_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_escen_viewer_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_escen_viewer_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_svg_viewer_SOURCES = eon_svg_viewer.c
eon_svg_viewer_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_svg_viewer_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_svg_viewer_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

