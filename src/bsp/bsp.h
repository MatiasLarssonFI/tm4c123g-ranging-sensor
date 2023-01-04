#ifndef __BSP_H__
#define __BSP_H__

#define SYS_CLOCK_HZ 16000000U

#define LED_RED   (1U << 1)
#define LED_BLUE  (1U << 2)
#define LED_GREEN (1U << 3)

#include "tm4c_cmsis.h"

// I2C only connected to APB, so don't use AHB
//#define USE_GPIOA_AHB 1

#define USE_GPIOF_AHB 1

#if USE_GPIOA_AHB
    #define BSP_GPIO_PORTA_BASE GPIO_PORTA_AHB_BASE
#else
    #define BSP_GPIO_PORTA_BASE GPIO_PORTA_BASE
#endif

#if USE_GPIOF_AHB
    #define BSP_GPIO_PORTF_BASE GPIO_PORTF_AHB_BASE
#else
    #define BSP_GPIO_PORTF_BASE GPIO_PORTF_BASE
#endif

#define HWREG(x) (*((volatile uint32_t *)(x)))

// I2C run mode clock gating control
#define RCGCI2C_REG HWREG(SYSCTL_BASE + 0x620U)

// I2C sw reset
#define SRI2C_REG HWREG(SYSCTL_BASE + 0x520U)

// GPIO High Speed control
#define GPIOHSCTL_REG HWREG(SYSCTL_BASE + 0x06CU)

// GPIO run mode clock gating control
#define RCGCGPIO_REG HWREG(SYSCTL_BASE + 0x608U)

// Address of alternate function select register for GPIO port A
#define GPIOAFSEL_PORTA (BSP_GPIO_PORTA_BASE + 0x420U)

// Address of open drain select register for GPIO port A
#define GPIOODR_PORTA (BSP_GPIO_PORTA_BASE + 0x50CU)

// Address of digital enable for GPIO port A
#define GPIODEN_PORTA (BSP_GPIO_PORTA_BASE + 0x51CU)

// Address of port control for GPIO port A
#define GPIOPCTL_PORTA (BSP_GPIO_PORTA_BASE + 0x52CU)

// Address of interrupt mask for GPIO port A
#define GPIOIM_PORTA (BSP_GPIO_PORTA_BASE + 0x410U)

// Address of interrupt sense for GPIO port A
#define GPIOIS_PORTA (BSP_GPIO_PORTA_BASE + 0x404U)

// Address of interrupt event for GPIO port A
#define GPIOIEV_PORTA (BSP_GPIO_PORTA_BASE + 0x40CU)

// Address of alternate function select register for GPIO port F
#define GPIOAFSEL_PORTF (BSP_GPIO_PORTF_BASE + 0x420U)

// Address of open drain select register for GPIO port F
#define GPIOODR_PORTF (BSP_GPIO_PORTF_BASE + 0x50CU)

// Address of digital enable for GPIO port F
#define GPIODEN_PORTF (BSP_GPIO_PORTF_BASE + 0x51CU)

// Address of port control for GPIO port F
#define GPIOPCTL_PORTF (BSP_GPIO_PORTF_BASE + 0x52CU)

// Address of interrupt mask for GPIO port F
#define GPIOIM_PORTF (BSP_GPIO_PORTF_BASE + 0x410U)

// Address of interrupt sense for GPIO port F
#define GPIOIS_PORTF (BSP_GPIO_PORTF_BASE + 0x404U)

// Address of interrupt event for GPIO port F
#define GPIOIEV_PORTF (BSP_GPIO_PORTF_BASE + 0x40CU)

// Address of I2C1 NVIC interrupt enable register
#define I2C1_NVIC_INT_ENABLE_R (0xE000E104) // interrupt 37

// Address of GPIO port A NVIC interrupt enable register
#define GPIOA_NVIC_INT_ENABLE_R (0xE000E100) // interrupt 0

// Address of GPIO port F NVIC interrupt enable register
#define GPIOF_NVIC_INT_ENABLE_R GPIOA_NVIC_INT_ENABLE_R // interrupt 30

// I2C1_MASTER_BASE and I2C1_SLAVE_BASE are defined in cmsis

#endif // __BSP_H__
