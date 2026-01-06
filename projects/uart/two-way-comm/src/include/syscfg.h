#ifndef SYSCFG_H
#define SYSCFG_H

#include <stdint.h>

typedef struct {
    volatile uint32_t MEMRMP;
    volatile uint32_t PMC;
    volatile uint32_t EXTICR1;
    volatile uint32_t EXTICR2;
    volatile uint32_t EXTICR3;
    volatile uint32_t EXTICR4;
    volatile uint32_t CMPCR;
} SYSCFG_Def;

#define SYSCFG ((SYSCFG_Def*)0x40013800)

#endif /* SYSCFG_H */
