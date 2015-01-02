noinst_LIBRARIES = libsup.a
libsup_a_SOURCES = \
	error.c \
	logger.c \
	daemon.c

STRICT_CHECK_C = \
	-pedantic-errors -ansi -Wextra -Wall \
	-Wuninitialized -Wmissing-declarations -Werror

STRICT_CHECK_CPP = \
	-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall \
	-Wold-style-cast -Wuninitialized -Wmissing-declarations -Werror

libsup_a_CPPFLAGS = -I$(top_srcdir)/include

libsup_a_CFLAGS =

libsup_a_LIBADD =

check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

greptest.sh:
	echo './meadowd | grep "hello"' > greptest.sh
	chmod +x greptest.sh

CLEANFILES = greptest.sh

