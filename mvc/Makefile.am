BUILT_SOURCES = mvc_parse.hh

# flags for flex and bison

AM_LFLAGS = --header-file=mvc_scan.hh

AM_YFLAGS = -d

bin_PROGRAMS = mvc

mvc_SOURCES = \
	mvc_parse.yy \
	mvc_scan.ll \
	mvc_exp.cc \
	mvc_stm.cc \
	mvc_module.cc \
	mvc_print.cc \
	mvc_iterator.cc \
	mvc_parser_driver.cc \
	mvc_parser.cc \
	mvc_analyzer.cc \
	mvc_main.cc

STRICT_CHECK_C = \
	-pedantic-errors -ansi -Wextra -Wall \
	-Wuninitialized -Wmissing-declarations -Werror

STRICT_CHECK_CPP = \
	-fno-elide-constructors -pedantic-errors -ansi -Wextra -Wall \
	-Wold-style-cast -Wuninitialized -Wmissing-declarations -Werror

mvc_CPPFLAGS = -I$(top_srcdir)/include

mvc_CFLAGS = 

mvc_LDADD =  

check_SCRIPTS = greptest.sh
TESTS = $(check_SCRIPTS)

run: $(bin_PROGRAMS)
	./mvc

greptest.sh:
	echo './mvc | grep "hello"' > greptest.sh
	chmod +x greptest.sh

CLEANFILES = greptest.sh  mvc_parse.hh mvc_parse.cc mvc_scan.cc mvc_scan.hh \
	position.hh stack.hh location.hh *.output
