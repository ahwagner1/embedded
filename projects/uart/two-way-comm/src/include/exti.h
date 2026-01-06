#ifndef EXTI_H
#define EXTI_H

#include <stdint.h>

typedef struct {
    volatile uint32_t IMR;
    volatile uint32_t EMR;
    volatile uint32_t RTSR;
    volatile uint32_t FTSR;
    volatile uint32_t SWIER;
    volatile uint32_t PR;
} EXTI_Def;

#define EXTI ((EXTI_Def*)0x40013C00)

#endif /* EXTI_H */
