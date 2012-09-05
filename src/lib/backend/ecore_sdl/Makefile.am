MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libeon_ecore_sdl.la

libeon_ecore_sdl_la_CFLAGS = \
-I$(top_srcdir)/src/include \
-I$(top_builddir)/src/include \
-I. \
@EON_BUILD@ \
@EON_CFLAGS@ \
@ECORE_SDL_CFLAGS@

libeon_ecore_sdl_la_SOURCES = \
eon_ecore_sdl.c

libeon_ecore_sdl_la_LIBADD = @ECORE_SDL_LIBS@
libeon_ecore_sdl_la_LDFLAGS =
