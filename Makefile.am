AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = -I m4 -I common/m4

MAINTAINERCLEANFILES = \
Makefile.in \
aclocal.m4 \
config.guess \
config.sub \
configure \
install-sh \
ltconfig \
ltmain.sh \
missing mkinstalldirs \
stamp-h.in \
build-stamp \
configure-stamp \
depcomp \
eon.pc \
m4/libtool.m4 \
m4/lt~obsolete.m4 \
m4/ltoptions.m4 \
m4/ltsugar.m4 \
m4/ltversion.m4

EXTRA_DIST = 
CLEAN_LOCAL = eon.pc

### Library
include src/lib/Makefile.mk
include src/lib/drawer/Makefile.mk

### Modules
include src/modules/drawer/basic/Makefile.mk
include src/modules/enesim_image/Makefile.mk
include src/modules/egueb_source/Makefile.mk

### Themes
include data/theme/Makefile.mk

EXTRA_DIST += \
AUTHORS \
COPYING \
gendoc \
Doxyfile \
README \
eon.pc.in \
m4/efl_attributes \
m4/efl_compiler.m4

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = eon.pc
