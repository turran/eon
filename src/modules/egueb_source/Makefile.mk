egueb_source_eon_LTLIBRARIES = src/modules/egueb_source/egueb_source_eon.la
egueb_source_eondir = @EGUEB_MODULE_SOURCE_DIR@

src_modules_egueb_source_egueb_source_eon_la_SOURCES = \
src/modules/egueb_source/egueb_source_eon.c

src_modules_egueb_source_egueb_source_eon_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib/ \
@EON_CFLAGS@

src_modules_egueb_source_egueb_source_eon_la_LIBADD = \
$(top_builddir)/src/lib/libeon.la \
@EON_LIBS@

src_modules_egueb_source_egueb_source_eon_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_egueb_source_egueb_source_eon_la_LIBTOOLFLAGS = --tag=disable-static

