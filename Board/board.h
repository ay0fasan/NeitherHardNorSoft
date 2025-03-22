
#ifndef __BOARD_H__
#define __BOARD_H__

#include "stm32f2xx_hal.h"

// --- Defines ---

// UART
#define UART_DEBUG                 USART3
#define UART_DEBUG_CLOCK_ENABLE()  __HAL_RCC_USART3_CLK_ENABLE()
#define UART_DEBUG_CLOCK_DISABLE() __HAL_RCC_USART3_CLK_DISABLE()

#define UART_DEBUG_TX_PIN          GPIO_PIN_9
#define UART_DEBUG_TX_PORT         GPIOD

#define UART_DEBUG_RX_PIN          GPIO_PIN_8
#define UART_DEBUG_RX_PORT         GPIOD

#define UART_DEBUG_AF              GPIO_AF7_USART3

// --- Functions ---

/**
 * @brief Configure the system clock
 */
void SystemClock_Config(void);

#endif    // __BOARD_H__
