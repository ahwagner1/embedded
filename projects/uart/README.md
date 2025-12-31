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

Simple UART polling without interrupts. TX will just blast the same string over and over while RX sits there and waits to recieve it.

I setup a simple delay function on the TX side, and then the RX board lights up LD2 when recieving data so I can easily tell when data TX/RX is happening. I also will look at the TX wire using my Saleae and try to upload a schreenshot of the data transmission.

## Project 2
two-way-comm

This time both boards will be able to TX and RX. I also want to include interrupts for this. Interrupts seem pretty important in embedded work and I think this would be a good project to start learning them alongside UART.

## Project 3 
still tbd
