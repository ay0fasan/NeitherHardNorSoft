#include <stdbool.h>
#include <string.h>
#include "nhns_status_codes.h"
#include "board.h"
#include "logger.h"
#include "os.h"

// --- Defines ---

// --- Types ---

// --- Global Variables ---

// --- Functions ---

void StartDefaultTask(void *argument)
{
    while (1)
    {
        OS_DelayMs(2000);
    }
}

int main(void)
{
    nhns_status_t nRet = NHNS_STATUS_OK;

    // 1) STM32 HAL library initialization
    HAL_Init();

    // 2) Configure the system clock
    SystemClock_Config();

    // 3) Initialize the logger
    nRet = Logger_Init();
    if (nRet != NHNS_STATUS_OK)
    {
        return nRet;
    }

    // 4) Create the default thread
    nRet = OS_TaskCreate(TASK_ID_DEFAULT, StartDefaultTask);
    if (nRet != NHNS_STATUS_OK)
    {
        LOGGER_LOG("%d", nRet);
        return nRet;
    }

    // 5) Start scheduler
    nRet = OS_SchedulerInit();
    if (nRet != NHNS_STATUS_OK)
    {
        LOGGER_LOG("%d", nRet);
        return nRet;
    }

    // 6) Main loop - we should not get here as OS has taken over
    while (1)
    {
        // Do nothing
    }
}
