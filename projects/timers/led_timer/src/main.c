#include "include/timer.h"
#include "include/rcc.h"
#include "include/gpio.h"

#include <stdint.h>

void toggle_led() {
    GPIOA->ODR ^= (0x1 << 5);
}

// interrupt handler on overflow count
void TIM3_IRQHandler() {
    if (TIM3->SR & (1)) { // checking UIF flag in SR
        toggle_led();
        
        // clear interrupt flag
        TIM3->SR &= ~(1);
    }
}

int main(void) {

    // enable gpioa clock and apb1 clock for timer3
    RCC->AHB1ENR |= 1;
    RCC->APB1ENR |= (1 << 1);

    // set pin mode to output 
    GPIOA->MODER &= ~(0x3 << 10);
    GPIOA->MODER |= (0x1 << 10);

    // setup timer
    // just gonna use TIM3 since I don't think I need >16 bit timer
    // need to figure out the clock config
    // HSI is 16MHz i think
    // we want a 1 second timer interval before an interrupt is generated
    // we need to select a value to count too, and then configure the clock based on that
    // clk / ((psc + 1) * (arr))
    // if we select 1000 for arr, we have the following equation:
    // 16MHz / ((psc + 1) * 1000) -> psc = 15999
    // this setup should give a 1hz signal
    TIM3->PSC &= ~(0xFF);
    TIM3->PSC = 15999;
    TIM3->ARR = 999; // 0 to 999
    
    // enable interrupts
    TIM3->DIER |= 1;

    // slave mode disabled, using internal clock
    TIM3->SMCR &= ~(0x7); 

    // up counting
    TIM3->CR1 &= ~(1 << 4);

    TIM3->EGR |= 1; // setting this generates an event, cleared by hardware
                    // this initializes the counter i think, which loads the prescalar (maybe)

    // clear the update interrupt flag from setting egr
    TIM3->SR &= ~(1);
    
    TIM3->CR1 |= 1; // counter enbaled

    while (1) {
        // just doing nothing for now
        // interrupt handler should handle this fine

    }

    return 0;
}
