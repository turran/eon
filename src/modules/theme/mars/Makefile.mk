EON_THEME_MARS_SOURCES = \
src/modules/theme/mars/eon_theme_mars_button.c \
src/modules/theme/mars/eon_theme_mars_entry.c \
src/modules/theme/mars/eon_theme_mars_eon.c \
src/modules/theme/mars/eon_theme_mars_expander.c \
src/modules/theme/mars/eon_theme_mars_frame.c \
src/modules/theme/mars/eon_theme_mars_image.c \
src/modules/theme/mars/eon_theme_mars_main.c \
src/modules/theme/mars/eon_theme_mars_paned.c \
src/modules/theme/mars/eon_theme_mars_progress_bar.c \
src/modules/theme/mars/eon_theme_mars_radio.c \
src/modules/theme/mars/eon_theme_mars_separator.c \
src/modules/theme/mars/eon_theme_mars_switch.c

if BUILD_STATIC_MODULE_THEME_MARS

src_lib_libeon_la_SOURCES += $(EON_THEME_MARS_SOURCES)

endif

if BUILD_MODULE_THEME_MARS

eon_theme_mars_LTLIBRARIES = src/modules/theme/mars/eon_theme_mars.la
eon_theme_marsdir = $(pkglibdir)

src_modules_theme_mars_eon_theme_mars_la_SOURCES = $(EON_THEME_MARS_SOURCES)
src_modules_theme_mars_eon_theme_mars_la_CPPFLAGS = -I$(top_srcdir)/src/lib -I$(top_srcdir)/src/lib/theme -DEON_BUILD @EON_CFLAGS@
src_modules_theme_mars_eon_theme_mars_la_LIBADD = $(top_builddir)/src/lib/libeon.la @EON_LIBS@
src_modules_theme_mars_eon_theme_mars_la_LDFLAGS = -no-undefined -module -avoid-version
src_modules_theme_mars_eon_theme_mars_la_LIBTOOLFLAGS = --tag=disable-static

endif

