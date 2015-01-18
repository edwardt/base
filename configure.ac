#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([mvcode], [0.1])
AM_INIT_AUTOMAKE
AC_CONFIG_HEADERS([config.h])
# AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_RANLIB
AM_PROG_CC_C_O
AC_PROG_YACC
AC_PROG_LEX

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CONFIG_FILES([Makefile
                 libmv/Makefile
                 mvrt/Makefile
                 mvsh/Makefile
                 mvc/Makefile
                ])

AC_OUTPUT
