# get rid of that stupid cache mechanism
rm -f config.cache

m4_define([v_maj], [0])
m4_define([v_min], [0])
m4_define([v_mic], [15])
m4_define([v_ver], [v_maj.v_min.v_mic])

m4_define([lt_cur], m4_eval(v_maj + v_min))
m4_define([lt_rev], v_mic)
m4_define([lt_age], v_min)

AC_INIT([eon], [v_ver], [enesim-devel@googlegroups.com])
AC_PREREQ([2.60])
AC_CONFIG_SRCDIR([configure.ac])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.6 dist-bzip2])
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_PREREQ([2.2])
LT_INIT([win32-dll])

version_info="lt_cur:lt_rev:lt_age"
AC_SUBST([version_info])
AC_DEFINE_UNQUOTED([VERSION_MAJOR], [v_maj], [Major version])
AC_DEFINE_UNQUOTED([VERSION_MINOR], [v_min], [Minor version])
AC_DEFINE_UNQUOTED([VERSION_MICRO], [v_mic], [Micro version])

VMAJ=v_maj
AC_SUBST([VMAJ])

## Add parameters for aclocal (keep in sync with Makefile.am)
AC_SUBST(ACLOCAL_AMFLAGS, "-I m4 -I common/m4")

### Needed information

AC_CANONICAL_HOST

ENESIM_BUILD=""
case "$host_os" in
   mingw*)
      EON_BUILD="-DEON_BUILD"
   ;;
esac
AC_SUBST([EON_BUILD])


### Additional options to configure


### Checks for programs

AC_PROG_CC

# pkg-config
PKG_PROG_PKG_CONFIG

# Check whether pkg-config supports Requires.private
if $PKG_CONFIG --atleast-pkgconfig-version 0.22; then
   pkgconfig_requires_private="Requires.private"
else
   pkgconfig_requires_private="Requires"
fi
AC_SUBST([pkgconfig_requires_private])

### Checks for libraries

EINA_VERSION="1.0.99"
ENDER_VERSION="0.0.10"
ENESIM_VERSION="0.0.20"
EGUEB_VERSION="0.0.23"

requirements_pc="egueb-dom >= ${EGUEB_VERSION}"
requirements_private_pc="eina >= ${EINA_VERSION} enesim >= ${ENESIM_VERSION} ender >= ${ENDER_VERSION} egueb-smil >= ${EGUEB_VERSION}"
PKG_CHECK_MODULES([EON], [${requirements_pc} ${requirements_private_pc}])

EON_CFLAGS="${EON_CFLAGS} -DENESIM_EXTENSION"

AC_SUBST([requirements_pc])
AC_SUBST([requirements_private_pc])

## Esvg

build_extension_esvg="no"
PKG_CHECK_MODULES(ESVG, [esvg >= 0.0.18], [have_esvg="yes"], [have_esvg="no"])
if test "x$have_esvg" = "xyes" ; then
	build_extension_esvg="yes"
	AC_DEFINE([BUILD_EXTENSION_ESVG], [1], [Build support for Esvg extension])
fi

AM_CONDITIONAL([BUILD_EXTENSION_ESVG], [test "x${build_extension_esvg}" = "xyes"])

## Ender

ENDER_DATADIR=$(${PKG_CONFIG} --variable=datadir ender)
AC_SUBST([ENDER_DATADIR])

## Backends

# SDL

build_backend_sdl="no"
PKG_CHECK_MODULES([ECORE_SDL], [ecore-sdl >= 1.0.0 sdl], [have_ecore_sdl="yes"], [have_ecore_sdl="no"])
if test "x${have_ecore_sdl}" = "xyes" ; then
   build_backend_sdl="yes"
   AC_DEFINE([BUILD_BACKEND_SDL], [1], [Build support for SDL backend])
fi

AM_CONDITIONAL([BUILD_BACKEND_SDL], [test "x${build_backend_sdl}" = "xyes"])

# Eix

build_backend_remote="no"
PKG_CHECK_MODULES([EIX], [eix >= 0.0.1], [have_eix="yes"], [have_eix="no"])
if test "x${have_eix}" = "xyes"; then
   build_backend_remote="yes"
   AC_DEFINE([BUILD_BACKEND_REMOTE], [1], [Build support for remote backend])
fi

AM_CONDITIONAL([BUILD_BACKEND_REMOTE], [test "x${build_backend_remote}" = "xyes"])

ENESIM_MODULE_IMAGE_DIR=`${PKG_CONFIG} --variable=moduleimagedir enesim`
AC_SUBST([ENESIM_MODULE_IMAGE_DIR])

EGUEB_MODULE_SOURCE_DIR=`${PKG_CONFIG} --variable=modulesourcedir egueb-dom`
AC_SUBST([EGUEB_MODULE_SOURCE_DIR])

### Checks for header files


### Checks for types


### Checks for structures


### Checks for compiler characteristics

AC_C_BIGENDIAN
AM_PROG_CC_C_O
EFL_ATTRIBUTE_UNUSED
EFL_CHECK_COMPILER_FLAGS([EON], [-Wno-override-init -Wno-initializer-overrides])


### Checks for linker characteristics

lt_enable_auto_import=""
case "$host_os" in
   mingw*)
      lt_enable_auto_import="-Wl,--enable-auto-import"
      ;;
esac
AC_SUBST([lt_enable_auto_import])


### Checks for library functions

ENS_MODULE([Drawer_basic], [static], [yes])

## Make the debug preprocessor configurable

AC_CONFIG_FILES([
Makefile
])

AC_OUTPUT

#####################################################################
## Info

echo
echo
echo
echo "------------------------------------------------------------------------"
echo "$PACKAGE $VERSION"
echo "------------------------------------------------------------------------"
echo
echo
echo "Configuration Options Summary:"
echo
echo "Backends:"
echo "SDL: .....................: $build_backend_sdl"
echo "Remote: ..................: $build_backend_remote"
echo
echo "Drawers:"
echo "Basic: ...................: ${build_module_drawer_basic}"
echo
echo "Installation Path.........: $prefix"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE,"
echo "and then afterwards as root (or the user who will install this), type"
echo "'make install'. Change users with 'su' or 'sudo' appropriately."
echo
