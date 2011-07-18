MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libeon_ecore_sdl.la

libeon_ecore_sdl_la_CFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EON_CFLAGS@ \
@ECORE_SDL_CFLAGS@ @SDL_CFLAGS@

libeon_ecore_sdl_la_SOURCES = \
eon_ecore_sdl.c

libeon_ecore_sdl_la_LIBADD = \
@ECORE_SDL_LIBS@ @SDL_LIBS@

libeon_ecore_sdl_la_DEPENDENCIES =

libeon_ecore_sdl_la_LDFLAGS =
