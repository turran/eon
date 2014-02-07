bin_PROGRAMS = eon_loader

eon_loader_CPPFLAGS = \
-I$(top_srcdir)/src/lib/ \
@EON_CFLAGS@

eon_loader_SOURCES =  eon_loader.c
eon_loader_LDADD = $(top_builddir)/src/lib/libeon.la \
@EON_LIBS@ -lm

