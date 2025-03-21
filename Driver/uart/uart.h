#include <stdint.h>
#include "stm32f2xx_hal.h"
#include "nhns_status_codes.h"

// --- Definitions ---

typedef enum uart_instance
{
    UART_INSTANCE_INVALID = -1,
    UART_INSTANCE_DEBUG,
    UART_INSTANCE_MAX,
} uart_instance_t;

// --- Functions ---
nhns_status_t UART_Init(uart_instance_t nID);
nhns_status_t UART_DeInit(uart_instance_t nID);
nhns_status_t UART_Transmit(uart_instance_t nID, uint8_t *pTxData, uint32_t bLength);
nhns_status_t UART_Receive(uart_instance_t nID, uint8_t *pRxData, uint32_t bLength);
