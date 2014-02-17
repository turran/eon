MAINTAINERCLEANFILES = Makefile.in

#SUBDIRS = basic

AM_CPPFLAGS = \
-I. \
-I$(top_srcdir)/src/lib \
@EON_BUILD@ \
@EON_CFLAGS@

lib_LTLIBRARIES = libeon_drawer.la

libeon_drawer_la_LIBADD = \
@EON_LIBS@

#eon_drawer_compound.c \
#eon_drawer_container.c \
#eon_drawer_button_base.c \
#eon_drawer_color.c \
#eon_drawer_entry.c \
#eon_drawer_frame.c \
#eon_drawer_layout.c \
#eon_drawer_progressbar.c \
#eon_drawer_spin.c \
#eon_drawer_splitter.c \
#eon_drawer_toggle.c


libeon_drawer_la_SOURCES = \
eon_drawer_button.c \
eon_drawer_widget.c

libeon_drawer_la_LDFLAGS = -no-undefined -version-info @version_info@
