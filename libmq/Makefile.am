noinst_LIBRARIES = libmq.a
libmq_a_SOURCES = \
	mqueue.c \
	callback.c \
	mqutil.c

STRICT_CHECK_C = \
	-pedantic-errors -ansi -Wextra -Wall \
	-Wuninitialized -Wmissing-declarations -Werror

STRICT_CHECK_CPP = \
	-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall \
	-Wold-style-cast -Wuninitialized -Wmissing-declarations -Werror

libmq_a_CPPFLAGS = -I$(top_srcdir)/include

libmq_a_CFLAGS =

#libmq_a_LIBADD = /usr/local/lib/libzmq.a

check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

greptest.sh:
	echo './meadowd | grep "hello"' > greptest.sh
	chmod +x greptest.sh

CLEANFILES = greptest.sh

