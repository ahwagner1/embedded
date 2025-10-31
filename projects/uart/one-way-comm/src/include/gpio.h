#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

typedef struct {
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_Def;

#define GPIOA ((GPIO_Def*)0x40020000)
#define GPIOB ((GPIO_Def*)0x40020400)
#define GPIOC ((GPIO_Def*)0x40020800)
#define GPIOD ((GPIO_Def*)0x40020C00)
#define GPIOE ((GPIO_Def*)0x40021000)
#define GPIOH ((GPIO_Def*)0x40021C00)

#endif /* GPIO_H */
