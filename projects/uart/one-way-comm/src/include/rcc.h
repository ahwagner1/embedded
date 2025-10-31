#ifndef RCC_H
#define RCC_H

#include <stdint.h>

// holy memory
typedef struct {
    volatile uint32_t CR;           
    volatile uint32_t PLLCFGR;      
    volatile uint32_t CFGR;         
    volatile uint32_t CIR;
    volatile uint32_t AHB1RSTR;
    volatile uint32_t AHB2RSTR;
    volatile uint32_t RES[2];
    volatile uint32_t APB1RSTR;
    volatile uint32_t APB2RSTR;
    volatile uint32_t RES2[2];
    volatile uint32_t AHB1ENR;
    volatile uint32_t AHB2ENR;
    volatile uint32_t RES3[2];
    volatile uint32_t APB1ENR;
    volatile uint32_t APB2ENR;
    volatile uint32_t RES4[2];
    volatile uint32_t AHB1LPENR;
    volatile uint32_t AHB2LPENR;
    volatile uint32_t RES5[2];
    volatile uint32_t APB1LPENR;
    volatile uint32_t APB2LPENR;
    volatile uint32_t RES6[2];
    volatile uint32_t BDCR;
    volatile uint32_t CSR;
    volatile uint32_t RES7[2];
    volatile uint32_t SSCGR;
    volatile uint32_t PLLI2SCFGR;
    volatile uint32_t RES8;
    volatile uint32_t DCKCFGR;
} RCC_Def;

#define RCC ((RCC_Def*)0x40023800)

#define GPIOA_EN (0x1 << 0)
#define GPIOB_EN (0x1 << 1)
#define GPIOC_EN (0x1 << 2)
#define GPIOD_EN (0x1 << 3)
#define GPIOE_EN (0x1 << 4)
#define GPIOH_EN (0x1 << 7)

#endif /* RCC_H */
