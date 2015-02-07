#include "rpigpio.h"

#define PIN  24 /* P1-18 */
#define POUT 4  /* P1-07 */

int main()
{
  int repeat = 120;
  int prev = -1;
  int state = 0;

  do {
    int now = gpioGet(PIN);
        printf("I'm reading %d in GPIO %d\n", now, PIN);

    if (((now == 1) && (prev ==0)) 
	|| (prev == -1)) {
      gpioPut(POUT, state & 0x1);
      if (state & 0x1)
	printf("Led on\n");
      else
	printf("Led off\n");
      state = ~state;
    }

    prev = now;

    usleep(100 * 1000);
  }
  while (repeat--);

  return 0;

}
