#include "include/dma.h"
#include "include/gpio.h"
#include "include/rcc.h"
#include "include/usart.h"

int main(void) {
    RCC->AHB1ENR |= (0x1 << 22); // DMA2 clock enable

    // setting up DMA2
    // first need to ensure DMA is off in case of pending transfers (there won't be any)
    DMA2->S2_CR &= (0 << 0);

    // waiting until bit 0 is 'off' so we can configure the stream
    while (~(DMA2->S2_CR & 0));

    // setup usart1
    // usart1 tx connects to DMA2 stream 7, channel 4
    // usart1 rx connects to DMA2 stream 2/5, channel 4
    DMA2->S2_CR &= ~(0x7 << 25);
    DMA2->S2_CR |= (0x4 << 25); // setting stream 2 selection to channel 4
    DMA2->S2_CR |= (0x3 << 16); // setting priority to very high
    DMA2->S2_CR &= ~(0x3 << 6); // peripheral to memory transfer direction

    return 0;
}
