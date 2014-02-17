SOURCES = \
eon_basic_drawer_button.c \
eon_basic.c

if BUILD_STATIC_MODULE_DRAWER_BASIC

lib_LTLIBRARIES = libbasic.la
libbasic_la_SOURCES = $(SOURCES)
libbasic_la_CPPFLAGS = -I. -I$(top_srcdir)/src/lib -I$(top_srcdir)/src/lib/drawer @EON_CFLAGS@

endif

if BUILD_MODULE_DRAWER_BASIC

basic_LTLIBRARIES = basic.la
basicdir = $(pkglibdir)

basic_la_SOURCES = $(SOURCES)
basic_la_CPPFLAGS = -I$(top_srcdir)/src/lib -DEON_BUILD @EON_CFLAGS@
basic_la_LIBADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@
basic_la_LDFLAGS = -no-undefined -module -avoid-version
basic_la_LIBTOOLFLAGS = --tag=disable-static

endif

