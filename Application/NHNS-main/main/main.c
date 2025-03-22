#include <stdbool.h>
#include <string.h>
#include "nhns_status_codes.h"
#include "board.h"
#include "logger.h"

// --- Defines ---

// --- Types ---

// --- Global Variables ---

// --- Functions ---

int main(void)
{
    // 1) STM32 HAL library initialization
    HAL_Init();

    // 2) Configure the system clock
    SystemClock_Config();

    // 3) Initialize the logger
    Logger_Init();

    // 4) Main loop - we should not get here as OS has taken over
    while (1)
    {
        // Do nothing
    }
}
