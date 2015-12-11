
lib_LTLIBRARIES = src/lib/libeon.la

installed_headersdir = $(pkgincludedir)-$(VMAJ)
dist_installed_headers_DATA = \
src/lib/Eon.h \
src/lib/eon_build.h \
src/lib/eon_document.h \
src/lib/eon_element_button.h \
src/lib/eon_element_button_stock.h \
src/lib/eon_element_button_toggle.h \
src/lib/eon_element_checkbox.h \
src/lib/eon_element_entry.h \
src/lib/eon_element_eon.h \
src/lib/eon_element_frame.h \
src/lib/eon_element_image.h \
src/lib/eon_element_label.h \
src/lib/eon_element_label_stock.h \
src/lib/eon_element_object.h \
src/lib/eon_element_radio.h \
src/lib/eon_element_separator.h \
src/lib/eon_element_stack.h \
src/lib/eon_element_switch.h \
src/lib/eon_main.h \
src/lib/eon_renderable.h \
src/lib/eon_theme.h \
src/lib/eon_stock.h

src_lib_libeon_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/theme \
-I$(top_srcdir)/src/lib/attr \
-I$(top_srcdir)/src/lib/value \
-I$(top_srcdir)/src/lib/feature \
-I$(top_srcdir)/src/lib/event \
-DPACKAGE_LIB_DIR=\"$(libdir)\" \
-DEON_BUILD \
@EON_CFLAGS@

src_lib_libeon_la_SOURCES = \
src/lib/eon_document.c \
src/lib/eon_element.c \
src/lib/eon_element_button.c \
src/lib/eon_element_button_stock.c \
src/lib/eon_element_button_toggle.c \
src/lib/eon_element_checkbox.c \
src/lib/eon_element_entry.c \
src/lib/eon_element_eon.c \
src/lib/eon_element_frame.c \
src/lib/eon_element_image.c \
src/lib/eon_element_label.c \
src/lib/eon_element_label_stock.c \
src/lib/eon_element_object.c \
src/lib/eon_element_radio.c \
src/lib/eon_element_separator.c \
src/lib/eon_element_stack.c \
src/lib/eon_element_switch.c \
src/lib/eon_layout_frame.c \
src/lib/eon_layout_stack.c \
src/lib/eon_layout_frame_private.h \
src/lib/eon_main.c \
src/lib/eon_renderable.c \
src/lib/eon_horizontal_align.c \
src/lib/eon_horizontal_align_private.h \
src/lib/eon_vertical_align.c \
src/lib/eon_vertical_align_private.h \
src/lib/eon_orientation.c \
src/lib/eon_orientation_private.h \
src/lib/eon_stock.c \
src/lib/eon_theme.c \
src/lib/eon_theme_private.h \
src/lib/eon_widget.c \
src/lib/eon_widget_private.h

src_lib_libeon_la_LIBADD = \
@EON_LIBS@

#core/libcore.la \
#backend/libbackend.la \
#extension/libextension.la

src_lib_libeon_la_LDFLAGS = -no-undefined -version-info @version_info@
