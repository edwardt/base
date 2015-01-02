noinst_LIBRARIES = libmv.a
libmv_a_SOURCES = \
	value.c \
	event.c \
	device.c \
	prop.c \
	func.c

STRICT_CHECK_C = \
	-pedantic-errors -ansi -Wextra -Wall \
	-Wuninitialized -Wmissing-declarations -Werror

STRICT_CHECK_CPP = \
	-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall \
	-Wold-style-cast -Wuninitialized -Wmissing-declarations -Werror

libmv_a_CPPFLAGS = -I$(top_srcdir)/include

libmv_a_CFLAGS =

libmv_a_LIBADD =

check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

greptest.sh:
	echo './meadowd | grep "hello"' > greptest.sh
	chmod +x greptest.sh

CLEANFILES = greptest.sh

