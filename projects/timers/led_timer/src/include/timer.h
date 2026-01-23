#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RCR;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t BDTR;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
} TIM1_Def;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t CR2;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t CCMR2;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RES;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t CCR3;
    volatile uint32_t CCR4;
    volatile uint32_t RES2;
    volatile uint32_t DCR;
    volatile uint32_t DMAR;
    volatile uint32_t OR_2;
    volatile uint32_t OR_5;
} TIM2_5_Def;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t RES;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RES2;
    volatile uint32_t CCR1;
    volatile uint32_t CCR2;
    volatile uint32_t RES3[4]; // 0x3c to 0x4c is reserved
} TIM9_Def;

typedef struct {
    volatile uint32_t CR1;
    volatile uint32_t SMCR;
    volatile uint32_t DIER;
    volatile uint32_t SR;
    volatile uint32_t EGR;
    volatile uint32_t CCMR1;
    volatile uint32_t RES;
    volatile uint32_t CCER;
    volatile uint32_t CNT;
    volatile uint32_t PSC;
    volatile uint32_t ARR;
    volatile uint32_t RES2;
    volatile uint32_t CCR1;
    volatile uint32_t RES3[5]; // 0x38 to 0x4c is reserved
    volatile uint32_t OR;
} TIM10_11_Def;

// on apb1
#define TIM2 ((TIM2_5_Def*)0x40000000)
#define TIM3 ((TIM2_5_Def*)0x40000400)
#define TIM4 ((TIM2_5_Def*)0x40000800)
#define TIM5 ((TIM2_5_Def*)0x40000C00)

// on apb2
#define TIM1 ((TIM1_Def*)0x40010000)
#define TIM9 ((TIM9_Def*)0x40014000)
#define TIM10 ((TIM10_11_Def*)0x40014400)
#define TIM11 ((TIM10_11_Def*)0x40014800)

#endif /* TIMER_H */
