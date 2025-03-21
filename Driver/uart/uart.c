#include <stdbool.h>
#include "uart.h"
#include "board.h"
#include "stm32f2xx_hal.h"

// --- Definitions ---

#define UART_RX_TX_TIMEOUT 5000

#define UART_CHECK_RETURN(nRet)     \
    do                              \
    {                               \
        if (nRet != NHNS_STATUS_OK) \
        {                           \
            return nRet;            \
        }                           \
    } while (0)

#define UART_CHECK_HAL_RETURN(nHALRet)               \
    do                                               \
    {                                                \
        if (nHALRet != HAL_OK)                       \
        {                                            \
            return (NHNS_STATUS_BASE_STM + nHALRet); \
        }                                            \
    } while (0)

// --- Types ---

typedef struct uart_context
{
    bool fInitDone;
    UART_HandleTypeDef sUARTHandle;
} uart_context_t;

// --- Global Variables ---

uart_context_t gsCntxt[UART_INSTANCE_MAX] = {0};

// --- Functions ---

nhns_status_t UART_Init(uart_instance_t nID)
{
    nhns_status_t nRet        = NHNS_STATUS_OK;
    HAL_StatusTypeDef nHalRet = HAL_OK;

    // 1) Verify argument
    if (nID <= UART_INSTANCE_INVALID || nID >= UART_INSTANCE_MAX)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if module has been previously initialized
    if (gsCntxt[nID].fInitDone)
    {
        return NHNS_STATUS_OK;
    }

    // 3) Configure UART handle
    if (nID == UART_INSTANCE_DEBUG)
    {
        gsCntxt[nID].sUARTHandle.Instance          = UART_DEBUG;
        gsCntxt[nID].sUARTHandle.Init.BaudRate     = 115200;
        gsCntxt[nID].sUARTHandle.Init.WordLength   = UART_WORDLENGTH_8B;
        gsCntxt[nID].sUARTHandle.Init.StopBits     = UART_STOPBITS_1;
        gsCntxt[nID].sUARTHandle.Init.Parity       = UART_PARITY_NONE;
        gsCntxt[nID].sUARTHandle.Init.Mode         = UART_MODE_TX_RX;
        gsCntxt[nID].sUARTHandle.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
        gsCntxt[nID].sUARTHandle.Init.OverSampling = UART_OVERSAMPLING_16;
    }

    // 4) Initialize UART
    nHalRet = HAL_UART_Init(&gsCntxt[nID].sUARTHandle);
    UART_CHECK_HAL_RETURN(nHalRet);

    // 5) Mark as initialized
    gsCntxt[nID].fInitDone = true;

    return nRet;
}

nhns_status_t UART_DeInit(uart_instance_t nID)
{
    nhns_status_t nRet        = NHNS_STATUS_OK;
    HAL_StatusTypeDef nHalRet = HAL_OK;

    // 1) Verify argument
    if (nID <= UART_INSTANCE_INVALID || nID >= UART_INSTANCE_MAX)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if already deinitialized
    if (!gsCntxt[nID].fInitDone)
    {
        return NHNS_STATUS_OK;
    }

    // 3) Deinitialize UART
    nHalRet = HAL_UART_DeInit(&gsCntxt[nID].sUARTHandle);
    UART_CHECK_HAL_RETURN(nHalRet);

    // 4) Mark as deinitialized
    gsCntxt[nID].fInitDone = false;

    return nRet;
}

nhns_status_t UART_Transmit(uart_instance_t nID, uint8_t *pTxData, uint16_t bLength)
{
    nhns_status_t nRet        = NHNS_STATUS_OK;
    HAL_StatusTypeDef nHalRet = HAL_OK;

    // 1) Verify arguments
    if (nID <= UART_INSTANCE_INVALID || nID >= UART_INSTANCE_MAX || pTxData == NULL || bLength == 0)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if module is initialized
    if (gsCntxt[nID].fInitDone)
    {
        return NHNS_STATUS_MODULE_NOT_INIT;
    }

    // 3) Tranmit data
    nHalRet = HAL_UART_Transmit(&gsCntxt[nID].sUARTHandle, pTxData, bLength, UART_RX_TX_TIMEOUT);
    UART_CHECK_HAL_RETURN(nHalRet);

    return nRet;
}

nhns_status_t UART_Receive(uart_instance_t nID, uint8_t *pRxData, uint16_t bLength)
{
    nhns_status_t nRet        = NHNS_STATUS_OK;
    HAL_StatusTypeDef nHalRet = HAL_OK;

    // 1) Verify arguments
    if (nID <= UART_INSTANCE_INVALID || nID >= UART_INSTANCE_MAX || pRxData == NULL || bLength == 0)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if module is initialized
    if (gsCntxt[nID].fInitDone)
    {
        return NHNS_STATUS_MODULE_NOT_INIT;
    }

    // 3) Receive data
    nHalRet = HAL_UART_Receive(&gsCntxt[nID].sUARTHandle, pRxData, bLength, UART_RX_TX_TIMEOUT);
    UART_CHECK_HAL_RETURN(nHalRet);

    return nRet;
}
