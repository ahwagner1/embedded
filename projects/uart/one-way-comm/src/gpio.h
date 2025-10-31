#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

typedef struct {
    volatile uint32_t GPIO_MODER;
    volatile uint32_t GPIO_OTYPER;
    volatile uint32_t GPIO_OSPEEDR;
    volatile uint32_t GPIO_PUPDR;
    volatile uint32_t GPIO_IDR;
    volatile uint32_t GPIO_ODR;
    volatile uint32_t GPIO_BSRR;
    volatile uint32_t GPIO_LCKR;
    volatile uint32_t GPIO_AFRL;
    volatile uint32_t GPIO_AFRH;
} GPIO_Def;

#define GPIOA ((GPIO_Def*)0x40020000)
#define GPIOB ((GPIO_Def*)0x40020400)
#define GPIOC ((GPIO_Def*)0x40020800)
#define GPIOD ((GPIO_Def*)0x40020C00)
#define GPIOE ((GPIO_Def*)0x40021000)
#define GPIOH ((GPIO_Def*)0x40021C00)

#endif /* GPIO_H */
