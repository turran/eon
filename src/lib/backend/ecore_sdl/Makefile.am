MAINTAINERCLEANFILES = Makefile.in

noinst_LTLIBRARIES = libeon_ecore_sdl.la

libeon_ecore_sdl_la_CFLAGS = \
-I$(top_srcdir)/src/include \
-I. \
@EON_CFLAGS@

libeon_ecore_sdl_la_SOURCES = \
eon_ecore_sdl.c

libeon_ecore_sdl_la_LIBADD = \
@EON_LIBS@

if BUILD_BACKEND_SDL
libeon_ecore_sdl_la_CFLAGS += @ECORE_SDL_CFLAGS@ @SDL_CFLAGS@
libeon_ecore_sdl_la_LIBADD += @ECORE_SDL_LIBS@ @SDL_LIBS@
endif


libeon_ecore_sdl_la_DEPENDENCIES =

libeon_ecore_sdl_la_LDFLAGS = @create_shared_lib@ -version-info @version_info@
