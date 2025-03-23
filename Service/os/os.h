#ifndef SRC_OS_H_
#define SRC_OS_H_

#include <stdint.h>
#include "nhns_status_codes.h"
#include "FreeRTOS.h"
#include "queue.h"

// --- Defines ---

// --- Types ---
typedef QueueHandle_t os_queue_t;

typedef enum task_id
{
    TASK_ID_INVALID = -1,
    TASK_ID_DEFAULT,
    TASK_ID_LOGGER,
    TASK_ID_MAX,
} task_id_t;

// --- Function pointers ---
typedef void (*fp_task_entry_t)(void *params);

// --- Functions ---
// TODO: Documentation

// Scheduler Functions
nhns_status_t OS_SchedulerInit(void);

// Task Functions
nhns_status_t OS_TaskCreate(task_id_t nID, fp_task_entry_t TaskEntryFunc);
nhns_status_t OS_TaskDelete(task_id_t nID);

// Queue Functions
nhns_status_t OS_QueueCreate(os_queue_t *pQueueHandle, uint32_t bQueueLength, uint32_t bItemSize);
nhns_status_t OS_QueueDelete(os_queue_t pQueueHandle);
nhns_status_t OS_QueueSend(os_queue_t pQueueHandle, const void *pQueueItem, uint32_t bTimeoutMs);
nhns_status_t OS_QueueReceive(os_queue_t pQueueHandle, void *pQueueItem, uint32_t bTimeoutMs);

// Delay Functions
void OS_DelayMs(uint32_t bMilliseconds);

#endif    // SRC_OS_H_
