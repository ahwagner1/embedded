#include "../include/usart.h"
#include "../include/gpio.h"
#include "../include/rcc.h"

// this board is using PA9 for USART1 TX
// USART1 lies on APB2, max of 84 MHz
// - reading more on clocks, I think HSI is 16 MHz
// - HSE is 4-26 MHz, looking at the nucleo schematic, it looks like my board has 8MHz HSE
// - PLL can be up to 84 MHz, need 

#define HSI_FREQ 16000000
#define HSE_FREQ 8000000

int main(void) {
    // setup clocks and all that
    RCC->AHB1ENR |= 0x1; // gpioa clock enable
    RCC->APB2ENR |= (0x1 << 4); // usart clock enable
    
    // reset pin 9, then set to alternate function
    GPIOA->MODER &= ~(0x3 << 18);
    GPIOA->MODER |= (0x2 << 18);

    // set to high speed
    // this doesn't mean clock speed
    // this means how fast the electrical signal transitions
    // for 9600 baud -> 9600 bits/s -> 9.6kHz toggle rate
    // HSI == 16 MHz which is more than capable of handling this
    // it seems like medium and high are the standard from examples I've seen
    GPIOA->OSPEEDR &= ~(0x3 << 18);
    GPIOA->OSPEEDR |= (0x2 << 18);

    // the alternate function is mapped to AF07 according to the datasheets
    // not sure if I need to set AFRL or AFRH, maybe both
    // but 0b0111 maps to AF7 in both, just not sure which register
    // ah I think low translates to pins 0-7, and high translates to pins 8-15
    // so in my case, PA9, I would need to set AFRH9[7:4] to 0x7 (i think)
    GPIOA->AFRH &= ~(0xF << 4);
    GPIOA->AFRH |= (0x7 << 4);

    // setup uart
    uint8_t clock_source = (RCC->CFGR >> 2) & 0x3; // HSI (00), HSE (01) or PLL (11)
    uint8_t apb2_ppre2 = (RCC->CFGR >> 13) & 0x7;

    uint32_t apb2_prescalar;
    if (apb2_ppre2 < 4)
        apb2_prescalar = 1;
    else
        apb2_prescalar = 0x1 << (apb2_ppre2 - 3); // ppre2 can be either 4, 5, 6, 7
                                                  // subtracting 3 gives us the number of bits
                                                  // to shift by for setting the prescalar
                                                  // which is either 2, 4, 8, 16
    
    USART1->CR1 |= (0x1 << 13); // set usart to enable
    USART1->CR1 &= ~(0x1 << 12); // set the word length (0 == 1 start, 8 dataa, n stop)
    USART1->CR1 &= ~(0x1 << 15); // setting oversampling to 16 (0 bit)
    USART1->CR2 &= ~(0x3 << 12); // setting bits 13:12 to 00 for 1 stop bit

    // so HSI should be default, but in the future I shoudl explore the others
    if (clock_source == 0) {
        // HSI Clock, calc BRR using HSI_FREQ

        // a little verbose, I could simplify this to just 16 * 9600 baud but thats lame
        uint8_t over8 = (USART1->CR1 >> 15) & 0x1;
        float usartdiv = (HSI_FREQ / apb2_prescalar) / (8 * (2 - over8) * 9600);
        uint32_t mantissa = (uint32_t)(usartdiv);

        uint32_t fraction_multiplier = over8 ? 8 : 16;
        uint32_t fraction = (uint32_t)((usartdiv - mantissa) * fraction_multiplier + 0.5); // 0.5 for rounding
        
        // handle potential fraction overflow
        if (fraction >= fraction_multiplier) {
            mantissa++;
            fraction = 0;
        }
        
        USART1->BRR = (mantissa << 4) | (fraction & (over8 ? 0x7 : 0xF));
    }
    else if (clock_source == 1) {
        // HSE clock, calc BRR using HSE_FREQ
    }
    else if (clock_source == 2) {
        // PLL clock, calc using formula from ref man
    }
    USART1->BRR; // need to find the clock freq to calculate what to put in here
                 // to configure the baud rate. im shooting for 9600 baud, 16 oversampling
                 // https://www.nichecalcs.com/stm32_uart_brr

    return 0;
}
