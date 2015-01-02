bin_PROGRAMS = mvrt
mvrt_SOURCES = \
	message.c \
	rtevent.c \
	rtprop.c \
	rtfunc.c \
	rtvalue.c \
	operator.c \
	context.c \
	code.c \
	evqueue.c \
	mdecoder.c \
	reactor.c \
	eval.c \
	rtsched.c \
	main.c

STRICT_CHECK_C = \
	-pedantic-errors -ansi -Wextra -Wall \
	-Wuninitialized -Wmissing-declarations -Werror

STRICT_CHECK_CPP = \
	-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall \
	-Wold-style-cast -Wuninitialized -Wmissing-declarations -Werror

mvrt_CPPFLAGS = -I$(top_srcdir)/include

mvrt_CFLAGS = -rdynamic

mvrt_LDADD =  ../libmq/libmq.a ../libmv/libmv.a -lpthread -lzmq -ljsmn \
	-ldl -lrt



check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

run: $(bin_PROGRAMS)
	./mvrt

greptest.sh:
	echo './mvrt | grep "hello"' > greptest.sh
	chmod +x greptest.sh

CLEANFILES = greptest.sh
