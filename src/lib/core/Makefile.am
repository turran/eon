MAINTAINERCLEANFILES = Makefile.in

AM_CPPFLAGS = \
-I$(top_srcdir)/src/include \
-I$(top_builddir)/src/include \
-I$(top_srcdir)/src/include/generated \
-I$(top_builddir)/src/include/generated \
-I. \
-DPACKAGE_DATA_DIR=\"$(pkgdatadir)\" \
@EON_BUILD@ \
@EON_CFLAGS@

noinst_LTLIBRARIES = libcore.la

#disabled for now until we do the refactor
# eon_color.c \
# eon_entry.c \
# eon_frame.c \
# eon_progressbar.c \
# eon_spin.c \
# eon_splitter.c \
# eon_tab.c \
# eon_toggle.c \
#
# eon_compound.c \
#

classes = \
eon_button.c \
eon_canvas.c \
eon_checkbox.c \
eon_grid.c \
eon_image.c \
eon_label.c \
eon_list.c \
eon_radio.c \
eon_stack.c \
eon_scrollbar.c \
eon_scrollview.c \
eon_surface.c \
eon_wrapper.c

abstracts = \
eon_bin.c \
eon_button_base.c \
eon_container.c \
eon_element.c \
eon_widget.c

libcore_la_SOURCES = $(classes) $(abstracts) \
eon_backend.c \
eon_input.c \
eon_input_proxy.c \
eon_keyboard_proxy.c \
eon_keyboard_proxy_navigation.c \
eon_keyboard_proxy_passthrough.c \
eon_keyboard_proxy_focus.c \
eon_layout_frame.c \
eon_layout_grid.c \
eon_layout_list.c \
eon_layout_stack.c \
eon_main.c \
eon_misc.c \
eon_mouse_proxy.c \
eon_theme.c \
eon_window.c

libcore_la_LIBADD = \
$(top_builddir)/src/lib/theme/libeon_theme.la

libcore_la_LDFLAGS =
