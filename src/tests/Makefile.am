MAINTAINERCLEANFILES = Makefile.in

check_PROGRAMS = \
eon_test_label \
eon_test_button \
eon_test_checkbox \
eon_test_radio

eon_test_button_SOURCES = eon_test_button.c
eon_test_button_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_button_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_button_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_radio_SOURCES = eon_test_radio.c
eon_test_radio_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_radio_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_radio_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_label_SOURCES = eon_test_label.c
eon_test_label_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_label_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_label_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_checkbox_SOURCES = eon_test_checkbox.c
eon_test_checkbox_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_checkbox_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_checkbox_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

