AC_INIT([keyrunner], [3], [russ.adams@gmail.com],
             [keyrunner], [http://killergame.net/keyrunner])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 -Wall no-define])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
AC_CONFIG_FILES([Makefile level/Makefile img/Makefile src/Makefile])
PKG_CHECK_MODULES([SDL], [sdl >= 1.2])
PKG_CHECK_MODULES([SDL_image], [SDL_image])
PKG_CHECK_MODULES([SDL_ttf], [SDL_ttf])
PKG_CHECK_MODULES([libpng], [libpng])
AC_OUTPUT

