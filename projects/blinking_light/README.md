# Blinking light

I will be making the onboard LED light up when connected to power, and power off when not connected.
This is sort of like the "Hello, World!" program for MCU's.

Using the NUCLEO-STM32F401RE, this program works by:
- Connecting a jumper wire to pin 51 (PC10)
- Connecting the other end of the jumper to power/ground

The on board led will light up when connected to power, and turn off when not
Since I'm using a pull down register, the led automatically turns off when not connected to power

