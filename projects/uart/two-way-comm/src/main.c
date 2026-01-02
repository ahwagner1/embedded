#include "include/gpio.h"
#include "include/rcc.h"
#include "include/usart.h"

#include <stdint.h>

#define HSE_FREQ 16000000

void setup(void) {
    // clocks setup
    RCC->AHB1ENR |= 0x1;
    RCC->APB2ENR |= (0x1 << 4);

    // 0xF == 0b1111, bitshift 18 should cover both pins
    // 0x1 == 0b1010, which means 0b10 in both pins
    GPIOA->MODER &= ~(0xF << 18);
    GPIOA->MODER |= (0xA << 18);

    // electrical signal speed (set to high)
    // see one-way-comm for more notes
    GPIOA->OSPEEDR &= ~(0xF << 18);
    GPIOA->OSPEEDR |= (0xA << 18);

    // this should also cover both ports
    GPIOA->AFRH &= ~(0xFF << 4);
    GPIOA->AFRH |= (0x77 << 4);
    
    // setup the usart clocks
    uint8_t clock_source = (RCC->CFGR >> 2) & 0x3; // HSI (00), HSE (01) or PLL (11)
    uint8_t apb2_ppre2 = (RCC->CFGR >> 13) & 0x7;

    uint32_t apb2_prescalar;
    if (apb2_ppre2 < 4)
        apb2_prescalar = 1;
    else
        apb2_prescalar = 0x1 << (apb2_ppre2 - 3);

    USART1->CR1 |= (0x1 << 13);
    USART1->CR1 |= (0x1 << 2);
    USART1->CR1 &= ~(0x1 << 12); // set the word length (0 == 1 start, 8 dataa, n stop)
    USART1->CR1 &= ~(0x1 << 15); // setting oversampling to 16 (0 bit)
    USART1->CR2 &= ~(0x3 << 12); // setting bits 13:12 to 00 for 1 stop bit
    
    if (clock_source == 0 || clock_source == 1) {
        uint32_t clock_frequency = clock_source ? HSE_FREQ : HSI_FREQ;

        // a little verbose, I could simplify this to just 16 * 9600 baud but thats lame
        uint8_t over8 = (USART1->CR1 >> 15) & 0x1;
        float usartdiv = (clock_frequency / apb2_prescalar) / (8 * (2 - over8) * 9600);
        uint32_t mantissa = (uint32_t)(usartdiv);

        uint32_t fraction_multiplier = over8 ? 8 : 16;
        uint32_t fraction = (uint32_t)((usartdiv - mantissa) * fraction_multiplier + 0.5); // 0.5 for rounding
        
        // handle potential fraction overflow
        if (fraction >= fraction_multiplier) {
            mantissa++;
            fraction = 0;
        }
        
        USART1->BRR = (mantissa << 4) | (fraction & (over8 ? 0x7 : 0xF)); // reserved bits are all supposed to be 0
                                                                          // so this method is fine for setting BRR
    }
    else if (clock_source == 2) {
        // PLL clock, calc using formula from ref man
        // ignoring for now
        return;
    }
}

int main(void) {
    setup();

    return 0;
}
