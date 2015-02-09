-------------------
 Button-n-LED demo
-------------------

1. Prepare two Raspberry pi devices: dev0 and dev1.

2. Preparation of dev0.

   a. cd dev0
   b. vi etc/device.dat 
      - update the IP address of dev0 and dev1
   c. cd gpio; make; cd ..
   d. connect the button to pin 2
      - pin 2 is connected to GND when button is unpressed (thru 10K resistor)
      - pin 2 is connected to 3.3V when button is pressed 

3. Preparation of dev1.

   a. cd dev0
   b. vi etc/device.dat
      - update the IP address of dev0 and dev1
   c. cd gpio; make; cd ..
   e. connect the LED to pin 2
      - pin 2 is connected to (+) leg of LED
      - GND is connected to (-) leg of LED (thru 300 ohm resistor)

4. Run the demo
   a. on dev1, run "sudo ./run.sh"
   b. on dev0, run "sudo ./run.sh"
   c. press button for a few seconds, then reactor will be activated to
      read the button status (pressed or not) and call gpioPut(2, 0 or 1)
      to dev1
      
