#ifndef UART_H
#define UART_H

#include <stdint.h>
#include "nhns_status_codes.h"

// --- Definitions ---

typedef enum uart_instance
{
    UART_INSTANCE_INVALID = -1,
    UART_INSTANCE_DEBUG,
    UART_INSTANCE_MAX,
} uart_instance_t;

// --- Functions ---

/**
 * @brief Initialize UART instance
 * @param nID - UART instance to initialize
 * @retval Status code indicating operation success or reason for failure
 */
nhns_status_t UART_Init(uart_instance_t nID);

/**
 * @brief Deinitialize UART instance
 * @param nID - UART instance to deinitialize
 * @retval Status code indicating operation success or reason for failure
 */
nhns_status_t UART_DeInit(uart_instance_t nID);

/**
 * @brief Transmit data over the UART interface
 * @param nID - UART instance to transmit data over
 * @param pTxData - Data to transmit
 * @param bLength - Length of data to transmit
 * @retval Status code indicating operation success or reason for failure
 */
nhns_status_t UART_Transmit(uart_instance_t nID, uint8_t *pTxData, uint16_t bLength);

/**
 * @brief Receive data from the UART interface
 * @param nID - UART instance to receive data from
 * @param pRxData - Buffer to store received data
 * @param bLength - Length of pRxData Buffer
 * @retval Status code indicating operation success or reason for failure
 */
nhns_status_t UART_Receive(uart_instance_t nID, uint8_t *pRxData, uint16_t bLength);

#endif    // UART_H
