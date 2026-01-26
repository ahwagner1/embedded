#ifndef DMA_H
#define DMA_H

#include <stdint.h>

typedef struct {
    volatile uint32_t LISR;
    volatile uint32_t HISR;
    volatile uint32_t LIFCR;
    volatile uint32_t HIFCR;
    volatile uint32_t S0_CR;
    volatile uint32_t S0_NDTR;
    volatile uint32_t S0_PAR;
    volatile uint32_t S0_M0_AR;
    volatile uint32_t S0_M1_AR;
    volatile uint32_t S0_FCR;
    volatile uint32_t S1_CR;
    volatile uint32_t S1_NDTR;
    volatile uint32_t S1_PAR;
    volatile uint32_t S1_M0_AR;
    volatile uint32_t S1_M1_AR;
    volatile uint32_t S1_FCR;
    volatile uint32_t S2_CR;
    volatile uint32_t S2_NDTR;
    volatile uint32_t S2_PAR;
    volatile uint32_t S2_M0_AR;
    volatile uint32_t S2_M1_AR;
    volatile uint32_t S2_FCR;
    volatile uint32_t S3_CR;
    volatile uint32_t S3_NDTR;
    volatile uint32_t S3_PAR;
    volatile uint32_t S3_M0_AR;
    volatile uint32_t S3_M1_AR;
    volatile uint32_t S3_FCR;
    volatile uint32_t S4_CR;
    volatile uint32_t S4_NDTR;
    volatile uint32_t S4_PAR;
    volatile uint32_t S4_M0_AR;
    volatile uint32_t S4_M1_AR;
    volatile uint32_t S4_FCR;
    volatile uint32_t S5_CR;
    volatile uint32_t S5_NDTR;
    volatile uint32_t S5_PAR;
    volatile uint32_t S5_M0_AR;
    volatile uint32_t S5_M1_AR;
    volatile uint32_t S5_FCR;
    volatile uint32_t S6_CR;
    volatile uint32_t S6_NDTR;
    volatile uint32_t S6_PAR;
    volatile uint32_t S6_M0_AR;
    volatile uint32_t S6_M1_AR;
    volatile uint32_t S6_FCR;
    volatile uint32_t S7_CR;
    volatile uint32_t S7_NDTR;
    volatile uint32_t S7_PAR;
    volatile uint32_t S7_M0_AR;
    volatile uint32_t S7_M1_AR;
    volatile uint32_t S7_FCR;
} DMADef;

// ahb1
#define DMA1 ((DMADef*)0x40026000) // 0x4002 6000 - 0x4002 63ff
#define DMA2 ((DMADef*)0x40026400) // 0x4002 6400 - 0x4002 67ff

#endif /* DMA_H */