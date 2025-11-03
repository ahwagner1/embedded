#ifndef USART_H
#define USART_H

#include <stdint.h>

typedef struct {
    volatile uint32_t SR; 
    volatile uint32_t DR; 
    volatile uint32_t BRR; 
    volatile uint32_t CR1; 
    volatile uint32_t CR2;
    volatile uint32_t CR3;
    volatile uint32_t GTPR;
} USART_Def;

#define USART1 ((USART_Def*)0x40011000)
#define USART2 ((USART_Def*)0x40004400)
#define USART6 ((USART_Def*)0x40011400)

#endif /* USART_H */
