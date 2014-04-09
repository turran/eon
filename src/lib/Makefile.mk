
lib_LTLIBRARIES = src/lib/libeon.la

installed_headersdir = $(pkgincludedir)-$(VMAJ)
dist_installed_headers_DATA = \
src/lib/Eon.h \
src/lib/eon_document.h \
src/lib/eon_element_button.h \
src/lib/eon_element_button_stock.h \
src/lib/eon_element_eon.h \
src/lib/eon_element_label.h \
src/lib/eon_element_label_stock.h \
src/lib/eon_element_object.h \
src/lib/eon_element_stack.h \
src/lib/eon_stock.h

src_lib_libeon_la_CPPFLAGS = \
-I$(top_srcdir)/src/lib \
-I$(top_srcdir)/src/lib/drawer \
@EON_BUILD@ \
@EON_CFLAGS@

src_lib_libeon_la_SOURCES = \
src/lib/eon_document.c \
src/lib/eon_element.c \
src/lib/eon_element_button.c \
src/lib/eon_element_button_stock.c \
src/lib/eon_element_eon.c \
src/lib/eon_element_label.c \
src/lib/eon_element_label_stock.c \
src/lib/eon_element_object.c \
src/lib/eon_element_stack.c \
src/lib/eon_event_geometry.c \
src/lib/eon_event_geometry_private.h \
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
src/lib/eon_widget.c \
src/lib/eon_widget_private.h \
src/lib/eon_widget_drawer.c \
src/lib/eon_widget_drawer_private.h \
src/lib/eon_widget_proxy.c \
src/lib/eon_widget_proxy_private.h

src_lib_libeon_la_LIBADD = \
@EON_LIBS@

#core/libcore.la \
#backend/libbackend.la \
#extension/libextension.la

src_lib_libeon_la_LDFLAGS = -no-undefined -version-info @version_info@
