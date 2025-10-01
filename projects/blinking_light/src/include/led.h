#ifndef _LED_H
#define _LED_H

#include <stdint.h>

#define RCC_AHB1ENR ((volatile uint32_t*)0x40023830)
#define GPIOA_MODER ((volatile uint32_t*)0x40020000)
#define GPIOA_ODR   ((volatile uint32_t*)0x40020014)

typedef struct _ld2 {
	volatile uint32_t *rcc_reg;
	volatile uint32_t *pin_mode_reg;
	volatile uint32_t *pin_output_reg;
} ld2;

void led_init(ld2 *led) {
	led->rcc_reg = RCC_AHB1ENR; // peripheal clock register
	led->pin_mode_reg = GPIOA_MODER; // mode register
	led->pin_output_reg = GPIOA_ODR; // output data register

	// enable the gpioa clock
	*(led->rcc_reg) |= 0x1;

	// set the pin mode to output (01 in the MODER reg)
	*(led->pin_mode_reg) &= ~(3 << 10);
	*(led->pin_mode_reg) |= (1 << 10);
}

void led_on(ld2 *led) {
	*(led->pin_output_reg) |= (1 << 5);
}

void led_off(ld2 *led) {
	*(led->pin_output_reg) &= ~(1 << 5);
}

void led_deinit(ld2 *led) {
	// reset the pin to input mode
	*(led->pin_mode_reg) &= ~(3 << 10);
}

void my_sleep(int secs) {
  #define STEPS_PER_SEC 500000
  unsigned int i,s;
  for (s=0; s < secs; s++) {
    for (i=0; i < STEPS_PER_SEC; i++) {
       // skip CPU cycle or any other statement(s) for making loop
       // untouched by C compiler code optimizations
       asm("nop");
    }
  }
}

#endif /* _LED_H */
