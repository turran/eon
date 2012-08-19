MAINTAINERCLEANFILES = Makefile.in

# old programs
# eon_test_canvas \
# eon_test_color \
# eon_test_entry \
# eon_test_frame \
# eon_test_label \
# eon_test_loader \
# eon_test_progressbar \
# eon_test_splitter \
# eon_test_spin \
# eon_test_toggle \
# eon_test_wrapper

check_PROGRAMS = \
eon_test_button01 \
eon_test_button02 \
eon_test_button03 \
eon_test_button_image \
eon_test_checkbox \
eon_test_image \
eon_test_radio \
eon_test_scrollbar \
eon_test_scrollview

if BUILD_EXTENSION_ESVG
check_PROGRAMS += eon_test_svg
endif

eon_test_loader_SOURCES = eon_test_loader.c
eon_test_loader_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_loader_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_loader_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_svg_SOURCES = eon_test_svg.c
eon_test_svg_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_svg_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_svg_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_button01_SOURCES = eon_test_button01.c
eon_test_button01_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_button01_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_button01_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_button02_SOURCES = eon_test_button02.c
eon_test_button02_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_button02_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_button02_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_button03_SOURCES = eon_test_button03.c
eon_test_button03_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_button03_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_button03_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_toggle_SOURCES = eon_test_toggle.c
eon_test_toggle_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_toggle_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_toggle_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_button_image_SOURCES = eon_test_button_image.c
eon_test_button_image_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_button_image_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_button_image_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_entry_SOURCES = eon_test_entry.c
eon_test_entry_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_entry_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_entry_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

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

eon_test_color_SOURCES = eon_test_color.c
eon_test_color_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_color_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_color_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_scrollview_SOURCES = eon_test_scrollview.c
eon_test_scrollview_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_scrollview_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_scrollview_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_image_SOURCES = eon_test_image.c
eon_test_image_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_image_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_image_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_progressbar_SOURCES = eon_test_progressbar.c
eon_test_progressbar_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_progressbar_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_progressbar_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_canvas_SOURCES = eon_test_canvas.c
eon_test_canvas_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_canvas_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_canvas_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_wrapper_SOURCES = eon_test_wrapper.c
eon_test_wrapper_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_wrapper_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_wrapper_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_frame_SOURCES = eon_test_frame.c
eon_test_frame_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_frame_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_frame_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_scrollbar_SOURCES = eon_test_scrollbar.c
eon_test_scrollbar_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_scrollbar_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_scrollbar_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_spin_SOURCES = eon_test_spin.c
eon_test_spin_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_spin_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_spin_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

eon_test_splitter_SOURCES = eon_test_splitter.c
eon_test_splitter_CFLAGS = -I. -I$(top_srcdir)/src/include @EON_CFLAGS@
eon_test_splitter_DEPENDENCIES = $(top_builddir)/src/lib/libeon.la
eon_test_splitter_LDADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@

