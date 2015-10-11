egueb_implementation_eon_LTLIBRARIES = src/modules/egueb_implementation/egueb_implementation_eon.la
egueb_implementation_eondir = @EGUEB_MODULE_IMPLEMENTATION_DIR@

src_modules_egueb_implementation_egueb_implementation_eon_la_SOURCES = \
src/modules/egueb_implementation/egueb_implementation_eon.c

src_modules_egueb_implementation_egueb_implementation_eon_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib/ \
@EON_CFLAGS@

src_modules_egueb_implementation_egueb_implementation_eon_la_LIBADD = \
$(top_builddir)/src/lib/libeon.la \
@EON_LIBS@

src_modules_egueb_implementation_egueb_implementation_eon_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_egueb_implementation_egueb_implementation_eon_la_LIBTOOLFLAGS = --tag=disable-static

