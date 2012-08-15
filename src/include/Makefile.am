MAINTAINERCLEANFILES = Makefile.in config.h.in~ config.h.in

SUBDIRS = generated

include_HEADERS = \
Eon.h \
Eon_Theme.h \
Eon_Svg.h

EON_COREHEADERS = \
eon_main.h \
eon_backend.h \
eon_bin.h \
eon_button.h \
eon_canvas.h \
eon_checkbox.h \
eon_color.h \
eon_container.h \
eon_element.h \
eon_entry.h \
eon_frame.h \
eon_image.h \
eon_input.h \
eon_label.h \
eon_progressbar.h \
eon_radio.h \
eon_scrollbar.h \
eon_scrollview.h \
eon_spin.h \
eon_splitter.h \
eon_stack.h \
eon_tab.h \
eon_toggle.h \
eon_widget.h \
eon_wrapper.h \
eon_ecore_remote.h \
eon_ecore_sdl.h

EON_THEMEHEADERS = \
theme/eon_theme_button_base.h \
theme/eon_theme_button.h \
theme/eon_theme_color.h \
theme/eon_theme_container.h \
theme/eon_theme_entry.h \
theme/eon_theme_frame.h \
theme/eon_theme_image.h \
theme/eon_theme_radio.h \
theme/eon_theme_checkbox.h \
theme/eon_theme_widget.h \
theme/eon_theme_label.h \
theme/eon_theme_progressbar.h \
theme/eon_theme_scrollbar.h \
theme/eon_theme_scrollview.h \
theme/eon_theme_spin.h \
theme/eon_theme_splitter.h \
theme/eon_theme_toggle.h

installed_themeheadersdir = $(prefix)/include/eon/theme
installed_themeheaders_DATA = $(EON_THEMEHEADERS)

installed_coreheadersdir = $(prefix)/include/eon
installed_coreheaders_DATA = $(EON_COREHEADERS)
