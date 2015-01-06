bin_PROGRAMS = server
server_SOURCES = \
	svr_device.cc \
	svr_message.cc \
	svr_processor.cc \
	svr_main.cc

STRICT_CHECK_C = \
	-pedantic-errors -ansi -Wextra -Wall \
	-Wuninitialized -Wmissing-declarations -Werror

STRICT_CHECK_CPP = \
	-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall \
	-Wold-style-cast -Wuninitialized -Wmissing-declarations -Werror

server_CPPFLAGS = -I$(top_srcdir)/include

server_CFLAGS = 

server_LDADD =  ../libmq/libmq.a -lzmq -lpthread



check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

run: $(bin_PROGRAMS)
	./server

greptest.sh:
	echo './server | grep "hello"' > greptest.sh
	chmod +x greptest.sh

CLEANFILES = greptest.sh
