SOURCES = \
src/modules/drawer/basic/eon_basic_drawer_button.c \
src/modules/drawer/basic/eon_basic_drawer_label.c \
src/modules/drawer/basic/eon_basic.c

if BUILD_STATIC_MODULE_DRAWER_BASIC

src_lib_libeon_la_SOURCES += $(SOURCES)

endif

if BUILD_MODULE_DRAWER_BASIC

eon_drawer_basic_LTLIBRARIES = src/modules/drawer/basic/eon_drawer_basic.la
eon_drawer_basicdir = $(pkglibdir)

src_modules_drawer_basic_eon_drawer_basic_la_SOURCES = $(SOURCES)
src_modules_drawer_basic_eon_drawer_basic_la_CPPFLAGS = -I$(top_srcdir)/src/lib -DEON_BUILD @EON_CFLAGS@
src_modules_drawer_basic_eon_drawer_basic_la_LIBADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@
src_modules_drawer_basic_eon_drawer_basic_la_LDFLAGS = -no-undefined -module -avoid-version
src_modules_drawer_basic_eon_drawer_basic_la_LIBTOOLFLAGS = --tag=disable-static

endif

