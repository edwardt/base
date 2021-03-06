#include <stdio.h>
#include "rpigpio.h"

void gpioPut(int pout, int value)
{
  printf("gpioPut(%d, %d)\n", pout, value);
  GPIOExport(pout); 
  GPIODirection(pout, OUT);
  GPIOWrite(pout, value);
  GPIOUnexport(pout);
}

int gpioGet(int pin)
{
  printf("gpioGet(%d)\n", pin);
  GPIOExport(pin);
  GPIODirection(pin, IN);
  int tmp = GPIORead(pin);
  GPIOUnexport(pin);
  //tmp= 1223;
  return tmp;
}
