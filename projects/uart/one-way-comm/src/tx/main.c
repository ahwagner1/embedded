#include "../include/usart.h"
#include "../include/gpio.h"
#include "../include/rcc.h"

int main(void) {
    // setup clocks and all that
    RCC->AHB1ENR |= GPIOA_EN;

    GPIOA->MODER |= (0x2 << 18);


    return 0;
}
