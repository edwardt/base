bin_PROGRAMS = mvc
mvc_SOURCES = \
	mvc_main.cpp

STRICT_CHECK_C = \
	-pedantic-errors -ansi -Wextra -Wall \
	-Wuninitialized -Wmissing-declarations -Werror

STRICT_CHECK_CPP = \
	-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall \
	-Wold-style-cast -Wuninitialized -Wmissing-declarations -Werror

mvc_CPPFLAGS = -I$(top_srcdir)/include

mvc_CFLAGS = 

mvc_LDADD =  ../libmq/libmq.a -lzmq -lpthread



check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

run: $(bin_PROGRAMS)
	./mvc

greptest.sh:
	echo './mvc | grep "hello"' > greptest.sh
	chmod +x greptest.sh

CLEANFILES = greptest.sh
