bin_PROGRAMS = client
client_SOURCES = \
	client.c
client_CPPFLAGS = 
client_LDADD = -lpthread -lzmq -ljsmn

check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

greptest.sh:
	echo './client | grep "hello"' > greptest.sh
	chmod +x greptest.sh

run: $(bin_PROGRAMS) 
	./client

CLEANFILES = greptest.sh
