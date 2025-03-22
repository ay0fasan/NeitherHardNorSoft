#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "logger.h"
#include "uart.h"
#include "stm32f2xx_hal.h"

// --- Defines ---
#define LOGGER_CHECK_RETURN(nRet)   \
    do                              \
    {                               \
        if (nRet != NHNS_STATUS_OK) \
        {                           \
            return nRet;            \
        }                           \
    } while (0);

// --- Types ---
typedef struct logger_context
{
    bool fInitDone;
} logger_context_t;

// --- Global Variables ---
static logger_context_t gsCntxt = {0};

// --- Functions ---

nhns_status_t Logger_Init(void)
{
    nhns_status_t nRet = NHNS_STATUS_OK;

    // 1) Check if module's already initialized
    if (gsCntxt.fInitDone)
    {
        return NHNS_STATUS_OK;
    }

    // 2) Initialize UART
    nRet = UART_Init(UART_INSTANCE_DEBUG);
    LOGGER_CHECK_RETURN(nRet);
    UART_Transmit(UART_INSTANCE_DEBUG, (uint8_t *)"LoggerInitDone ", strlen("LoggerInitDone ")); 

    // 3) Mark as initialized
    gsCntxt.fInitDone = true;

    return nRet;
}

nhns_status_t Logger_DeInit(void)
{
    nhns_status_t nRet = NHNS_STATUS_OK;

    // 1) Check if module's already deinitialized
    if (!gsCntxt.fInitDone)
    {
        return NHNS_STATUS_OK;
    }

    // 2) Deinitialize UART
    nRet = UART_DeInit(UART_INSTANCE_DEBUG);
    LOGGER_CHECK_RETURN(nRet);

    // 3) Mark as deinitialized
    gsCntxt.fInitDone = false;

    return nRet;
}

void Logger_Log(const char *pFile, const char *pFunction, int bLine, const char *pFormat, ...)
{
    char pLogBuffer[256]                                            = {0};
    char pLogPrefix[64]                                             = {0};
    char pFinalMessage[sizeof(pLogBuffer) + sizeof(pLogPrefix) + 1] = {0};

    // 1) Verify module's initialized
    if (!gsCntxt.fInitDone)
    {
        return;
    }

    // 2) Create log prefix
    snprintf(pLogPrefix, sizeof(pLogPrefix), "%s:%d %s() - ", pFile, bLine, pFunction);

    // 3) Format the variable arguments
    va_list args;
    va_start(args, pFormat);
    vsnprintf(pLogBuffer, sizeof(pLogBuffer), pFormat, args);
    va_end(args);

    // 4) Combine prefix and message
    snprintf(pFinalMessage, sizeof(pFinalMessage), "%s%s\r\n", pLogPrefix, pLogBuffer);

    // 5) Transmit log over UART
    UART_Transmit(UART_INSTANCE_DEBUG, (uint8_t *)pFinalMessage, strlen(pFinalMessage));
}
