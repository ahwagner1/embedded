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

The setup will look similiar to the one-way-comm project

- PA9 = USART1_TX
- PA10 = USART1_RX

This time on both boards though:

- Board A PA9 -> Board B PA10
- Board B PA9 -> Board A PA10

Since the UART paths are entirely different wires, we should be able to do full-duplex communication.
In order to handle TX and RX, I am making use of the TXEIE and RXNEIE interrupts.

The general workflow looks like:
1. An EXTI interrupt is generated when the button on PA0 is pressed, signaling to the board to start a transmission.
This interrupt sets the TXEIE interrupt to *enabled*
2. TXEIE interrupt will trigger when a byte is ready to transmit. 
Once the full message is transmitted, the TXEIE interrupt is disabled to prevent locking.
3. At the same time, RXNEIE interrupt will trigger whenever there is incoming data.

Both UART interrupts are handled by the function `void USART1_IRQHandler(void);`

Reading from the status register lets us determine which interrupt to handle (or maybe both).

The external buttons will be wired into a RC filter circuit in an **Active Low** configuration.
This RC filter should let us solve the button debouncing without the need to implement a software solution.

This setup should support a robust, two-way communication between the boards.

## Project 3 
still tbd
