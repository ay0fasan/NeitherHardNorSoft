
#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f2xx_hal.h"

// --- Defines ---

// UART
#define UART_DEBUG                 UART4
#define UART_DEBUG_CLOCK_ENABLE()  __HAL_RCC_UART4_CLK_ENABLE()
#define UART_DEBUG_CLOCK_DISABLE() __HAL_RCC_UART4_CLK_DISABLE();

#define UART_DEBUG_TX_PIN          GPIO_PIN_10
#define UART_DEBUG_TX_PORT         GPIOC

#define UART_DEBUG_RX_PIN          GPIO_PIN_11
#define UART_DEBUG_RX_PORT         GPIOC

#define UART_DEBUG_AF              GPIO_AF8_UART4
#define UART_DEBUG_IRQn            UART4_IRQn

// --- Functions ---

/**
 * @brief Configure the system clock
 */
void SystemClock_Config(void);

#endif    // __BOARD_H__
