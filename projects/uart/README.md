### UART 
Simple UART projects to learn the protocol. All baremetal for learning. Will try to upload at least a photo of the project, maybe a schematic if I can find a way to get clean schematics. 

All code handwritten by me, just reading datasheets. Absolutely no AI assistance, and I will document sources I use for help (if any)

## Project 1
one-way-comm

I have two STM32F401RE boards. One will be configured for TX, and the other will be configured to RX.

*General Setup*
- Pins PA8-PA12 have alternate functions for UART1 on this board
  - PA8 = USART1_CK
  - PA9 = USART1_TX
  - PA10 = USART1_RX
  - PA11 = USART1_CTS
  - PA12 = USART1_RTS
- Need to setup clocks for GPIOA and USART

*TX Board Setup*
- Set PA9 to alternate function for TX

*RX Board Setup*
- Set PA10 to alternate function for RX


## Project 2
two-way-comm

This time both boards will be able to TX and RX

## Project 3 
still tbd
