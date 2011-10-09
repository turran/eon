# get rid of that stupid cache mechanism
rm -f config.cache

AC_INIT(eon, 0.0.3, enesim-devel@googlegroups.com)
AC_PREREQ(2.52)
AC_CONFIG_SRCDIR(configure.ac)
AC_CANONICAL_BUILD
AC_CANONICAL_HOST
AC_ISC_POSIX

AM_INIT_AUTOMAKE(1.6 dist-bzip2)
AM_CONFIG_HEADER(src/include/config.h)
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

AC_PROG_CC
AM_PROG_CC_STDC
AC_HEADER_STDC
AC_C_CONST
AC_C_BIGENDIAN

AC_LIBTOOL_WIN32_DLL
define([AC_LIBTOOL_LANG_CXX_CONFIG], [:])dnl
define([AC_LIBTOOL_LANG_F77_CONFIG], [:])dnl
AC_PROG_LIBTOOL

dnl Set PACKAGE_DATA_DIR in config.h.
if test "x${prefix}" = "xNONE"; then
  PACKAGE_DATA_DIR="${ac_default_prefix}/share/${PACKAGE}"
else
  PACKAGE_DATA_DIR="${prefix}/share/${PACKAGE}"
fi
AC_SUBST(PACKAGE_DATA_DIR)
AC_DEFINE_UNQUOTED(PACKAGE_DATA_DIR, "${PACKAGE_DATA_DIR}", [Shared Data Directory])

VMAJ=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $1);}'`
VMIN=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $2);}'`
VMIC=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $3);}'`
SNAP=`echo $PACKAGE_VERSION | awk -F. '{printf("%s", $4);}'`
version_info=`expr $VMAJ + $VMIN`":$VMIC:$VMIN"
AC_SUBST(version_info)
AC_DEFINE_UNQUOTED(VERSION_MAJOR, ${VMAJ}, [Major version])
AC_DEFINE_UNQUOTED(VERSION_MINOR, ${VMIN}, [Minor version])
AC_DEFINE_UNQUOTED(VERSION_MICRO, ${VMIC}, [Micro version])

create_shared_lib=""
case "$host_os" in
    mingw|mingw32)
	create_shared_lib="-no-undefined"
    ;;
esac
AC_SUBST(create_shared_lib)

# Common
PKG_CHECK_MODULES(ENESIM, [enesim >= 0.0.9])
PKG_CHECK_MODULES(ENESIM, [emage >= 0.0.1])
PKG_CHECK_MODULES(ENDER, [ender >= 0.0.1])
PKG_CHECK_MODULES(ESCEN, [escen >= 0.0.1])
PKG_CHECK_MODULES(ETEX, [etex >= 0.0.3])
ENDER_DATADIR=$(pkg-config --variable=datadir ender)
AC_SUBST(ENDER_DATADIR)
PKG_CHECK_MODULES(EINA, [eina >= 1.0.0])
PKG_CHECK_MODULES(ECORE, [ecore >= 1.0.0])

EON_CFLAGS="$EINA_CFLAGS $ENESIM_CFLAGS $EMAGE_CFLAGS $ENDER_CFLAGS $ESCEN_CFLAGS $ETEX_CFLAGS $ECORE_CFLAGS -Wall"
EON_LIBS="$EINA_LIBS $ENESIM_LIBS $EMAGE_LIBS $ENDER_LIBS $ESCEN_LIBS $ETEX_LIBS $ECORE_LIBS"
AC_SUBST(EON_CFLAGS)
AC_SUBST(EON_LIBS)

# Backends
PKG_CHECK_MODULES(ECORE_SDL, [ecore-sdl >= 0.9.9], [have_ecore_sdl=yes], [have_ecore_sdl=no])
PKG_CHECK_MODULES(SDL, [sdl], [have_sdl=yes], [have_sdl=no])
if test "x$have_ecore_sdl" = "xyes" && test "x$have_sdl" = "xyes"; then
	build_backend_sdl=yes
	AC_DEFINE(BUILD_BACKEND_SDL, [1], [Build support for SDL backend])
else
	build_backend_sdl=no
fi
AM_CONDITIONAL(BUILD_BACKEND_SDL, test "x$build_backend_sdl" = "xyes")

PKG_CHECK_MODULES(EIX, [eix >= 0.0.1], [have_eix=yes], [have_eix=no])
if test "x$have_eix" = "xyes"; then
	build_backend_remote=yes
	AC_DEFINE(BUILD_BACKEND_REMOTE, [1], [Build support for remote backend])
else
	build_backend_remote=no
fi
AM_CONDITIONAL(BUILD_BACKEND_REMOTE, test "x$build_backend_remote" = "xyes")

## Make the debug preprocessor configurable

AC_OUTPUT([
Makefile
eon.pc
data/Makefile
data/ender/Makefile
data/theme/Makefile
src/Makefile
src/lib/Makefile
src/lib/backend/Makefile
src/lib/backend/ecore_common/Makefile
src/lib/backend/ecore_sdl/Makefile
src/lib/backend/ecore_remote/Makefile
src/lib/theme/Makefile
src/lib/theme/basic/Makefile
src/include/Makefile
src/include/generated/Makefile
src/tests/Makefile
],[
])


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
echo "Installation Path.........: $prefix"
echo
echo "Now type 'make' ('gmake' on some systems) to compile $PACKAGE,"
echo "and then afterwards as root (or the user who will install this), type"
echo "'make install'. Change users with 'su' or 'sudo' appropriately."
echo
