#include "include/gpio.h"
#include "include/rcc.h"
#include "include/usart.h"

#include <stdint.h>

#define HSI_FREQ 16000000
#define HSE_FREQ 8000000

const char* msg = "TWO WAY COMMUNICATION FTW!";
uint8_t tx_idx = 0;
uint8_t tx_msg = 0;

void USART1_IRQHandler(void) {
    if ((USART1->SR >> 5) & 0x1) {
        // read data register not empty
        // reading from DR will clear this bit
        uint8_t byte = USART1->SR & 0xFF;
        (void)byte; // no unused warning from compiler
                    // for now I'm just sending this to the void
    }

    // button has been pressed and TXE = 1
    if ((USART1->SR >> 7) & 0x1) {
        if (tx_idx < strlen(msg)) {
            USART1->DR = msg[tx_idx]; // writing this byte clears TXE to 0
                                      // but once it's shifted, it should trigger again
            tx_idx++;
        }
        else {
            // transmit the message, turn off interrupt
            // if we didn't turn it off, this would turn into an inf loop
            USART1->CR1 &= ~(0x1 << 7);
            tx_idx = 0;
        }
    }
}

// gpio interrupt handler
// this will be used to handle button interrupt for sending data
void EXTI0_IRQHandler(void) {
    // we have triggered whe button interrupt
    // could transmit from here
    // but I think that setting a flag is a better approach incase we need to wait to transmit
    tx_msg = 1;
    USART1->CR1 |= (0x1 << 7); // TXEIE enabled

    // also need to clear the EXTI pending register by writing '1' to the bit for our exti line
    EXTI->PR |= 0x1;
}

/*
*   Setting up all the UART essentials for TX/RX
*   Might move things into other functions to further divide
*/

void clock_setup(void) {
    RCC->AHB1ENR |= 0x1;
    RCC->APB2ENR |= (0x1 << 4);
}

void gpio_setup(void) {
    // setting to alternate function mode on pins PA9 and PA10
    // 0xF == 0b1111, bitshift 18 should cover both pins
    // 0x1 == 0b1010, which means 0b10 in both pins
    GPIOA->MODER &= ~(0xF << 18);
    GPIOA->MODER |= (0xA << 18);

    // electrical signal speed (set to high)
    // see one-way-comm for more notes
    GPIOA->OSPEEDR &= ~(0xF << 18);
    GPIOA->OSPEEDR |= (0xA << 18);

    // setting pins PA9 and PA10 to USART1 TX and RX
    // this should also cover both pins
    GPIOA->AFRH &= ~(0xFF << 4);
    GPIOA->AFRH |= (0x77 << 4);

    // need to setup pin for external button
    // not setting pull up/pull down since the hardware config should solve that
    GPIOA->MODER &= ~(0x3); // setting up PA0 for now
    GPIOA->OSPEEDR |= 0x2;

}

void interrupt_setup(void) {
    // need to configure syscfg register I think for the actual muxing
    // cause according to the diagram, PAx, PBx, PCx, ..., PHx are inputs to EXTIx
    // and I think syscfg is the selector for the multiplexer to configure the pin

    // the bits in the register "select the source input for the EXTIx external interrupt"
    SYSCFG->EXTICR1 &= ~(0xF); // sets EXTI0[3:0] to 0b0000 which is PA[x] pin
                                    // and the button will be connected to PA0 which inputs to EXTI0

    // reset entire register
    EXTI->IMR &= 0x00000000;
    EXTI->IMR |= 0x1; // setting MR0 to not masked (aka generate interrupt)
    EXTI->RTSR &= 0x00000000;
    EXTI->RTSR |= 0x1; // using rising edge since active low configuratin
                       // this will trigger when button is RELEASED
}

void uart_setup(void) {
        // setup the usart clocks
    uint8_t clock_source = (RCC->CFGR >> 2) & 0x3; // HSI (00), HSE (01) or PLL (11)
    uint8_t apb2_ppre2 = (RCC->CFGR >> 13) & 0x7;

    uint32_t apb2_prescalar;
    if (apb2_ppre2 < 4)
        apb2_prescalar = 1;
    else
        apb2_prescalar = 0x1 << (apb2_ppre2 - 3);

    USART1->CR1 |= (0x1 << 13); // usart enable
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
    // bit 7 in CR1 = TXEIE
    // - when this bit is set, an interrupt is generated when TXE in the SR is set to 1
    // - this lets us know when we can TX our next byte
    // - this will be useful
    //
    //
    // I think for two way communication setup like
    // - Board1_PA9_TX -> Board2_PA10_RX
    // - Board2_PA9_TX -> Board1_PA10_RX
    //
    // ~~I would want the idle line interrupt and the RXNEIE interrupt
    // idle line lets a board know it's allowed to transmit. useful when:
    // - board is doing other things, but now know it can transmit if needed
    // - board is waiting to transmit
    // RXNEIE lets a board know that it's receiving a transmit. useful when:
    // - board is doing other things but still needs to be aware of transmits~~
    //
    // Since thr UART paths above are separate wires, the electrical signals wont interfere
    // this made me do some reading and i realised that TX/RX does not need to be blocking
    // e.g I don't need to wait to transmit if I'm currently receiving a message
    // uart comm in full-duplex meaning both can happen at the same time
    // this got me wondering, how is that possible when TX and RX share the same DR
    // after some reading, it looks like DR is an alias to different registers
    // the chip is smart enough to tell read (_ = USART1->DR;) vs write (USART1->DR = _;)
    // it then routes things accordingly through the one interface, allowing for simpler programming
    // so I don't really need to worry about TX/RX blocking each other
    // but I still want to use interrupts to handle reading/writing data
    //
    // basic workflow will look like:
    // 1. GPIOA interrupt for button will trigger a flag saying telling us to start transmit
    // 2. TXEIE interrupt will trigger when a byte has been transmit
    // 3. At the same time, RXNEIE interrupt will trigger when there are incoming data
    //
    // while researching button debouncing techniques I found this post:
    // https://www.allpcb.com/allelectrohub/the-engineers-guide-to-switch-contact-debounce-techniques
    //
    // I was originally gonna go for a software debouncing implementation
    // but i need practive with hardware so i plan on using an RC filter
    // I should already have the parts needed and it's pretty simple to setup and use

    //USART1->CR |= (0x1 << 7); // TXEIE
    USART1->CR |= (0x1 << 5); // TXEIE

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
    clock_setup();
    gpio_setup();
    interrupt_setup();
    uart_setup();

    // super loop go brrrrr
    while (1) {
        // simulate working in here
        // not actually gonna do anything
    }

    return 0;
}