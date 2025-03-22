#include <stdbool.h>
#include <string.h>
#include "nhns_status_codes.h"
#include "board.h"
#include "uart.h"

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

    while (1)
    {
        /* code */
    }
}
