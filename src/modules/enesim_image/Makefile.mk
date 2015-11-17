enesim_image_eon_LTLIBRARIES = src/modules/enesim_image/enesim_image_eon.la
enesim_image_eondir = @ENESIM_MODULE_IMAGE_DIR@

src_modules_enesim_image_enesim_image_eon_la_SOURCES = \
src/modules/enesim_image/enesim_image_eon.c

src_modules_enesim_image_enesim_image_eon_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib/ \
-I$(top_srcdir)/src/lib/theme \
-I$(top_srcdir)/src/lib/event \
@EON_CFLAGS@

src_modules_enesim_image_enesim_image_eon_la_LIBADD = \
$(top_builddir)/src/lib/libeon.la \
@EON_LIBS@

src_modules_enesim_image_enesim_image_eon_la_LDFLAGS = -no-undefined -module -avoid-version

src_modules_enesim_image_enesim_image_eon_la_LIBTOOLFLAGS = --tag=disable-static
