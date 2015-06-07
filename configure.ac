AC_INIT([keyrunner], [3], [russ.adams@gmail.com],
             [keyrunner], [http://killergame.net/keyrunner])
AC_PREREQ([2.69])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])
AC_CONFIG_HEADERS([config.h])
AC_LANG([C++])
AC_PROG_CXX([clang++ g++])
AX_CXX_COMPILE_STDCXX_11
CXXFLAGS+=" -D__STRICT_ANSI__"
AC_CONFIG_FILES([Makefile level/Makefile img/Makefile src/Makefile font/Makefile])
PKG_CHECK_MODULES([SDL], [sdl >= 1.2])
PKG_CHECK_MODULES([SDL_image], [SDL_image])
PKG_CHECK_MODULES([SDL_ttf], [SDL_ttf])
PKG_CHECK_MODULES([libpng], [libpng])
PKG_CHECK_MODULES([liblzma], [liblzma])
AC_OUTPUT

