#include <stdbool.h>
#include "uart.h"
#include "board.h"
#include "logger.h"
#include "os.h"
#include "stm32f2xx_hal.h"

// --- Definitions ---

#define UART_RX_TX_TIMEOUT 5000

#define UART_CHECK_RETURN(nRet)                                   \
    do                                                            \
    {                                                             \
        if (nRet != NHNS_STATUS_OK)                               \
        {                                                         \
            LOGGER_LOG("%s %d %d", __FUNCTION__, __LINE__, nRet); \
            return nRet;                                          \
        }                                                         \
    } while (0)

#define UART_CHECK_HAL_RETURN(nHALRet)                            \
    do                                                            \
    {                                                             \
        if (nHALRet != HAL_OK)                                    \
        {                                                         \
            LOGGER_LOG("%s %d %d", __FUNCTION__, __LINE__, nRet); \
            return (NHNS_STATUS_BASE_STM + nHALRet);              \
        }                                                         \
    } while (0)

// --- Types ---

typedef struct uart_context
{
    bool fInitDone;
    UART_HandleTypeDef sUARTHandle;
    os_semaphore_t sUARTTxCompleteSemaphore;
    os_semaphore_t sUARTRxCompleteSemaphore;
} uart_context_t;

// --- Global Variables ---

static uart_context_t gsCntxt[UART_INSTANCE_MAX] = {0};

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

    // 5) Create semaphores
    nRet = OS_SemaphoreCreate(&gsCntxt[nID].sUARTRxCompleteSemaphore);
    UART_CHECK_RETURN(nRet);

    nRet = OS_SemaphoreCreate(&gsCntxt[nID].sUARTTxCompleteSemaphore);
    UART_CHECK_RETURN(nRet);

    // 6) Mark as initialized
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

    // 3) Delete semaphores
    nRet = OS_SemaphoreDelete(gsCntxt[nID].sUARTRxCompleteSemaphore);
    UART_CHECK_RETURN(nRet);

    nRet = OS_SemaphoreDelete(gsCntxt[nID].sUARTTxCompleteSemaphore);
    UART_CHECK_RETURN(nRet);

    // 4) Deinitialize UART
    nHalRet = HAL_UART_DeInit(&gsCntxt[nID].sUARTHandle);
    UART_CHECK_HAL_RETURN(nHalRet);

    // 5) Mark as deinitialized
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
    if (!gsCntxt[nID].fInitDone)
    {
        return NHNS_STATUS_MODULE_NOT_INIT;
    }

    // 3) Tranmit data
    nHalRet = HAL_UART_Transmit_IT(&gsCntxt[nID].sUARTHandle, pTxData, bLength);
    UART_CHECK_HAL_RETURN(nHalRet);

    // 4) Take UART transmit semaphore
    nRet = OS_SemaphoreTake(gsCntxt[nID].sUARTTxCompleteSemaphore, UART_RX_TX_TIMEOUT);
    UART_CHECK_RETURN(nRet);

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
    if (!gsCntxt[nID].fInitDone)
    {
        return NHNS_STATUS_MODULE_NOT_INIT;
    }

    // 3) Receive data
    nHalRet = HAL_UART_Receive_IT(&gsCntxt[nID].sUARTHandle, pRxData, bLength);
    UART_CHECK_HAL_RETURN(nHalRet);

    // 4) Take UART receive semaphore
    nRet = OS_SemaphoreTake(gsCntxt[nID].sUARTRxCompleteSemaphore, UART_RX_TX_TIMEOUT);
    UART_CHECK_RETURN(nRet);

    return nRet;
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    // 1) Give UART transmit semaphore
    if (huart->Instance == UART_DEBUG)
    {
        OS_SemaphoreGive(gsCntxt[UART_INSTANCE_DEBUG].sUARTTxCompleteSemaphore);
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    // 1) Give UART receive semaphore
    if (huart->Instance == UART_DEBUG)
    {
        OS_SemaphoreGive(gsCntxt[UART_INSTANCE_DEBUG].sUARTRxCompleteSemaphore);
    }
}
