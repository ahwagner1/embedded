#ifndef USART_H
#define USART_H

#include <stdint.h>

typedef struct {
    volatile uint32_t USART_SR; 
    volatile uint32_t USART_DR; 
    volatile uint32_t USART_BRR; 
    volatile uint32_t USART_CR1; 
    volatile uint32_t USART_CR2;
    volatile uint32_t USART_CR3;
    volatile uint32_t USART_GTPR;
} USART_Def;

#define USART1 ((USART_DEF*)0x40011000)
#define USART2 ((USART_DEF*)0x40004400)
#define USART6 ((USART_DEF*)0x40011400)

#endif /* USART_H */
