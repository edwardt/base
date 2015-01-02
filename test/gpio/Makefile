all: clean sharedLib test

clean: 
	rm -f *.o *.so ./test

sharedLib:
	gcc -c -Wall -Werror -fpic rpigpio.c rgpioCtrl.c 
	gcc -shared -o libgpio.so rpigpio.o rgpioCtrl.o

test:
	gcc -L. -Wl,-rpath=. -o test test.c -lgpio
