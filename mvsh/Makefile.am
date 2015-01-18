bin_PROGRAMS = mvsh
mvsh_SOURCES = \
	command.c \
	repl.c \
	main.c

STRICT_CHECK_C = \
	-pedantic-errors -ansi -Wextra -Wall \
	-Wuninitialized -Wmissing-declarations -Werror

STRICT_CHECK_CPP = \
	-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall \
	-Wold-style-cast -Wuninitialized -Wmissing-declarations -Werror

mvsh_CPPFLAGS = -I$(top_srcdir)/include

mvsh_CFLAGS =

mvsh_LDADD = ../libmv/libmv.a -lzmq -lpthread -ljsmn



check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

run: $(bin_PROGRAMS)
	./mvsh

greptest.sh:
	echo './mvsh | grep "hello"' > greptest.sh
	chmod +x greptest.sh

CLEANFILES = greptest.sh
