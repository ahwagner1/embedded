#include "../include/usart.h"
#include "../include/gpio.h"
#include "../include/rcc.h"

#define HSI_FRQ 16000000
#define HSE_FREQ 8000000

void led_on(void) {
    GPOIA->ODR |= (0x1 << 5);
}

void led_off() {
    GPIOA->ODR &= ~(1 << 5);
}

int main(void) {
    // clock setup
    RCC->AHB1ENR |= 0x1;
    RCC->APB2ENR |= (0x1 << 4);

    // unlike tx which is pa9 alternate, pa10 is usart rx alternate
    GPIOA->MODER &= ~(0x3 << 20);
    GPIOA->MODER |= (0x2 << 20);

    GPIOA->OSPEEDR &= ~(0x3 << 20);
    GPIOA->OSPEEDR |= (0x2 << 20);

    GPIOA->AFRH &= ~(0xF << 8);
    GPIOA->AFRH |= (0x7 << 8);

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
        return 0;
    }

    // finally setup LED to see when data is being recieved
    // gpioa clock already enabled, as well as ahb1 clock
    // just need to configure the pin which I think if PA5 for LD2 on the nucleo
    GPIOA->MODER &= ~(3 << 10);
    GPIOA->MODER |= (1 << 10);

    while (1) {
        if (USART->SR & (0x1 << 5)) {
            uint8_t rx_data = USART->DR & 0xFF;

            // somehow print out the data
            // could store in buffer and then use gdb to see the buffer contents
            // but i think blinking an led when data is recieved is better for this quick test
            // however, at 9600 baud the led might just look permanantly ON
            // could setup another pin that watches for data
            // like if execution enters this block, we recieved a transmission
            // and could just pulse another nearby pin to see on saelae
            led_on();
        }

        led_off();
    }


    return 0;
}