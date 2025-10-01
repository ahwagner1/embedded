#include <stdint.h>
#include "include/led.h"

/*
* This project will turn the led on/off depending on if pin 51 (pc10)
* is connected to power/ground
*/

int main(void) {
    ld2 led;
    led_init(&led);
    led_off(&led);

    // enabling clock reg for port c 
    *(led.rcc_reg) |= (1 << 2);

    volatile uint32_t *gpioc_moder = (volatile uint32_t*)0x40020800;
    *gpioc_moder &= ~(3 << 20); // setting the 18th and 19th bits to 0 which means input

    volatile uint32_t *gpioc_pupdr = (volatile uint32_t*)0x4002080C;
    *gpioc_pupdr &= ~(3 << 20); // reset the out bits
    *gpioc_pupdr |= (2 << 20); // set the bits to 10 which should mean pull down
    
    // reading for high/low 
    volatile uint32_t *gpioc_idr = (volatile uint32_t*)0x40020810;;

    while (1) {
        if (*gpioc_idr & (1 << 10))
            led_on(&led);
        else
            led_off(&led);
    }

    return 0;
}
