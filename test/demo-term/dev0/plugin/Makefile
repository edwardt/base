all: clean lib 

MVROOT = ../../../..
INCDIR = $(MVROOT)/include
LIBDIR = $(MVROOT)/libmv

lib:
	g++ -Wall -Werror -fPIC -c reader.cc -I$(INCDIR) 
	g++ -shared -o libmvplugin.so reader.o

clean:
	$(RM) -rf *.o *.so