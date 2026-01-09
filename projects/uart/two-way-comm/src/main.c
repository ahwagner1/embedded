#include "include/gpio.h"
#include "include/rcc.h"
#include "include/usart.h"

#include <stdint.h>

#define HSI_FREQ 16000000
#define HSE_FREQ 8000000

enum InterruptFlags {
    FLAG_NORMAL     = 0x1 << 0,
    FLAG_WAITING_TX = 0x1 << 1, // waiting to TX, currently RX
    FLAG_TX         = 0x1 << 2, // TX allowed, no current RX
};

uint32_t flags = 0; // all flags off

// gpio interrupt handler
// this will be used to handle button interrupt for sending data
void EXTI0_IRQHandler(void) {
    // we have triggered whe button interrupt
    // could transmit from here
    // but I think that setting a flag is a better approach incase we need to wait to transmit
    
    if ((USART1->SR >> 4) & 0x1) {
        // bit 4 of SR translates to idle line detected
        flags = 0;
        flags |= FLAG_TX;
        return;
    }

}

/*
*   Setting up all the UART essentials for TX/RX
*   Might move things into other functions to further divide
*/

void clock_setup(void) {

}

void gpio_setup(void) {

}

void uart_setup(void) {
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
    USART1->CR1 |= (0x1 << 2); // rx enable
    USART1->CR1 |= (0x1 << 3); // tx enable
    USART1->CR1 &= ~(0x1 << 12); // set the word length (0 == 1 start, 8 dataa, n stop)
    USART1->CR1 &= ~(0x1 << 15); // setting oversampling to 16 (0 bit)
    USART1->CR2 &= ~(0x3 << 12); // setting bits 13:12 to 00 for 1 stop bit
    
    // interrupts that I could make use of
    // bit 4 in CR1 = IDLEIE
    // - when this bit is set to 1, an interrupt is generated when the IDLE = 1 in the SR
    // - could be useful for determining when a transmission is allowed to be sent if RX-ing
    // - IDLE in the SR is set to 1 by hardware, and cleared by the software sequence:
    //      1. Read to SR
    //      2. Read to DR
    // - not sure why i would read the SR or DR if the idle line is detected?
    // - or is this saying that when a TX comes in, I would already do a read to SR and DR
    //
    // bit 5 in CR1 = RXNEIE
    // - when this bit is set to 1, an interrupt is generated when RXNE/ORE = 1 in SR
    // - this will be useful, RXNE -> read data register not empty
    // - this allows us to f off and do other things until a transmission comes in
    // - then an interrupt notifies us of the incoming message, MUST USE
    //
    // bit 6 in CR1 = TCIE
    // - this bit generates an interrupt when a frame transmit has been completed
    // - and TXE is set which means the TDR register has been transferred to shift register
    // - not sure if super useful for what I want to do
    // - could be nice if I had a more complex transmission setup I think
    // - like if I was transmitting from multiple channels in "parrallel"
    // - when this generates, I could reliable decide the next channel thats allowed to TX
    //
    //
    // I think for two way communication setup like 
    // - Board1_PA9_TX -> Board2_PA10_RX
    // - Board2_PA9_TX -> Board1_PA10_RX
    //
    // I would want the idle line interrupt and the RXNEIE interrupt
    // idle line lets a board know it's allowed to transmit. useful when:
    // - board is doing other things, but now know it can transmit if needed
    // - board is waiting to transmit
    // RXNEIE lets a board know that it's receiving a transmit. useful when:
    // - board is doing other things but still needs to be aware of transmits
    
    if (clock_source == 0 || clock_source == 1) {
        uint32_t clock_frequency = clock_source ? HSE_FREQ : HSI_FREQ;

        uint32_t periphclk = clock_frequency / apb2_prescalar;
        USART1->BRR = ((periphclk + (9600/2)) / 9600);
    }
    else if (clock_source == 2) {
        // PLL clock, calc using formula from ref man
        // ignoring for now
        return;
    }
}

int main(void) {
    uart_setup();

    // super loop go brrrrr
    while (1) {
        // need a way to check if the line is currently transmitting, or if it's receiving

    }

    return 0;
}
