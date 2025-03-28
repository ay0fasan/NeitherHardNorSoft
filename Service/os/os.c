#include "os.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

// --- Defines ---

#define MAX_TASK_PRIORITY             configMAX_PRIORITIES
#define OS_MS_TO_TICKS(bMilliseconds) (pdMS_TO_TICKS(bMilliseconds))
#define OS_IS_INSIDE_ISR()            (xPortIsInsideInterrupt())

// --- Types ---
typedef enum os_state
{
    OS_STATE_INVALID = -1,
    OS_STATE_NOT_STARTED,
    OS_STATE_RUNNING,
    OS_STATE_MAX,
} os_state_t;

typedef struct task_config
{
    task_id_t nTaskID;        // Unique identifier for the task
    const char *pTaskName;    // Task name for debugging or identification
    uint16_t bStackSize;      // Stack size allocated for the task (in bytes)
    uint32_t bPriority;       // Task priority
} task_config_t;

// --- Global Variables

static os_state_t gnSchedulerState = OS_STATE_NOT_STARTED;

static const task_config_t gsTaskList[] = {
    //-------------------- highest priority ------------------
    {TASK_ID_LOGGER,  "logger",  512, 1},
    //--------------------- lowest priority ------------------
    {TASK_ID_DEFAULT, "default", 512, 0},
};

static TaskHandle_t gpTaskHandles[TASK_ID_MAX] = {0};

// --- Functions ---

// Scheduler functions

nhns_status_t OS_SchedulerInit(void)
{
    uint8_t bNumTasks = sizeof(gsTaskList) / sizeof(task_config_t);

    // 1) Validate task list
    if (bNumTasks != TASK_ID_MAX)
    {
        return NHNS_STATUS_DATA_MISMATCH;
    }

    // 2) Validate task list priorities
    for (uint8_t i = 0; i < TASK_ID_MAX; i++)
    {
        if (gsTaskList[i].bPriority > MAX_TASK_PRIORITY)
        {
            return NHNS_STATUS_INVALID_CONFIGURATION;
        }
    }

    // 3) Check if OS was already initialized
    if (gnSchedulerState != OS_STATE_NOT_STARTED)
    {
        return NHNS_STATUS_OK;
    }

    // 4) Update the global scheduler state
    gnSchedulerState = OS_STATE_RUNNING;

    // 5) Start the scheduler
    vTaskStartScheduler();

    return NHNS_STATUS_OK;
}

// Task Functions

nhns_status_t OS_TaskCreate(task_id_t nID, fp_task_entry_t TaskEntryFunc)
{
    BaseType_t nRetInternal          = pdTRUE;
    TaskHandle_t pTaskHandle         = NULL;
    const task_config_t *pTaskConfig = &gsTaskList[nID];

    // 1) Validate arguments
    if (nID <= TASK_ID_INVALID || nID >= TASK_ID_MAX || TaskEntryFunc == NULL)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if we're inside an ISR as task creation from ISR is not allowed in FreeRTOS
    if (OS_IS_INSIDE_ISR())
    {
        return NHNS_STATUS_FORBIDDEN;
    }

    // 3) Check if the task already exists
    if (gpTaskHandles[nID] != NULL)
    {
        return NHNS_STATUS_OK;
    }

    // 4) Create the task
    nRetInternal = xTaskCreate(TaskEntryFunc, pTaskConfig->pTaskName, pTaskConfig->bStackSize, NULL, pTaskConfig->bPriority, &pTaskHandle);
    if (nRetInternal != pdPASS)
    {
        return (NHNS_STATUS_BASE_RTOS + nRetInternal);
    }

    // 5) Store the task handle in the global list
    gpTaskHandles[nID] = pTaskHandle;

    return NHNS_STATUS_OK;
}

nhns_status_t OS_TaskDelete(task_id_t nID)
{
    // 1) Validate arguments
    if (nID <= TASK_ID_INVALID || nID >= TASK_ID_MAX)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if we're inside an ISR as task deletion from ISR is not allowed in FreeRTOS
    if (OS_IS_INSIDE_ISR())
    {
        return NHNS_STATUS_FORBIDDEN;
    }

    // 3) Check if the task exists
    if (gpTaskHandles[nID] == NULL)
    {
        return NHNS_STATUS_NOT_FOUND;
    }

    // 4) Delete the task
    vTaskDelete(gpTaskHandles[nID]);

    // 5) Clear the task handle from the global list
    gpTaskHandles[nID] = NULL;

    return NHNS_STATUS_OK;
}

// Queue Functions

nhns_status_t OS_QueueCreate(os_queue_t *psQueueHandle, uint32_t bQueueLength, uint32_t bItemSize)
{
    // 1) Check arguments
    if (psQueueHandle == NULL || bQueueLength == 0 || bItemSize == 0)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if we're inside an ISR as queue creation from ISR is not allowed in FreeRTOS
    if (OS_IS_INSIDE_ISR())
    {
        return NHNS_STATUS_FORBIDDEN;
    }

    // 3) Create the queue
    *psQueueHandle = xQueueCreate(bQueueLength, bItemSize);
    if (*psQueueHandle == NULL)
    {
        return NHNS_STATUS_FAIL;
    }

    return NHNS_STATUS_OK;
}

nhns_status_t OS_QueueDelete(os_queue_t sQueueHandle)
{
    // 1) Check if we're inside an ISR as queue deletion from ISR is not allowed in FreeRTOS
    if (OS_IS_INSIDE_ISR())
    {
        return NHNS_STATUS_FORBIDDEN;
    }

    // 2) Delete the queue
    vQueueDelete(sQueueHandle);

    return NHNS_STATUS_OK;
}

nhns_status_t OS_QueueSend(os_queue_t sQueueHandle, const void *pQueueItem, uint32_t bTimeoutMs)
{
    BaseType_t nRetInternal             = pdPASS;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    TickType_t bInitialTickCount        = 0;
    TickType_t bElapsedTickCount        = 0;

    // 1) Check arguments
    if (pQueueItem == NULL)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if we're inside an ISR and appropriately write item to back of the queue
    if (OS_IS_INSIDE_ISR())
    {
        nRetInternal = xQueueSendToBackFromISR(sQueueHandle, pQueueItem, &xHigherPriorityTaskWoken);
        if (nRetInternal != pdPASS)
        {
            return (NHNS_STATUS_BASE_RTOS + nRetInternal);
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        bInitialTickCount = xTaskGetTickCount();

        nRetInternal = xQueueSendToBack(sQueueHandle, pQueueItem, OS_MS_TO_TICKS(bTimeoutMs));
        if (nRetInternal != pdPASS)
        {
            bElapsedTickCount = xTaskGetTickCount() - bInitialTickCount;
            if (bElapsedTickCount >= OS_MS_TO_TICKS(bTimeoutMs))
            {
                return NHNS_STATUS_TIMEOUT;
            }
            else
            {
                return (NHNS_STATUS_BASE_RTOS + nRetInternal);
            }
        }
    }

    return NHNS_STATUS_OK;
}

nhns_status_t OS_QueueReceive(os_queue_t sQueueHandle, void *pQueueItem, uint32_t bTimeoutMs)
{
    BaseType_t nRetInternal             = pdPASS;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    TickType_t bInitialTickCount        = 0;
    TickType_t bElapsedTickCount        = 0;

    // 1) Check arguments
    if (pQueueItem == NULL)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if we're inside an ISR and appropriately read item from back of the queue
    if (OS_IS_INSIDE_ISR())
    {
        nRetInternal = xQueueReceiveFromISR(sQueueHandle, pQueueItem, &xHigherPriorityTaskWoken);
        if (nRetInternal != pdPASS)
        {
            return (NHNS_STATUS_BASE_RTOS + nRetInternal);
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        bInitialTickCount = xTaskGetTickCount();

        nRetInternal = xQueueReceive(sQueueHandle, pQueueItem, OS_MS_TO_TICKS(bTimeoutMs));
        if (nRetInternal != pdPASS)
        {
            bElapsedTickCount = xTaskGetTickCount() - bInitialTickCount;
            if (bElapsedTickCount >= OS_MS_TO_TICKS(bTimeoutMs))
            {
                return NHNS_STATUS_TIMEOUT;
            }
            else
            {
                return (NHNS_STATUS_BASE_RTOS + nRetInternal);
            }
        }
    }

    return NHNS_STATUS_OK;
}

// Delay Functions

void OS_DelayMs(uint32_t bMilliseconds)
{
    // 1) Delay for the specified number of ticks
    vTaskDelay(OS_MS_TO_TICKS(bMilliseconds));
}

// Semaphore Functions

nhns_status_t OS_SemaphoreCreate(os_semaphore_t *pSemaphoreHandle)
{
    // 1) Verify argument
    if (pSemaphoreHandle == NULL)
    {
        return NHNS_STATUS_INVALID_ARGUMENT;
    }

    // 2) Check if we're inside an ISR as semaphore creation from ISR is not allowed in FreeRTOS
    if (OS_IS_INSIDE_ISR())
    {
        return NHNS_STATUS_FORBIDDEN;
    }

    // 3) Create the semaphore
    *pSemaphoreHandle = xSemaphoreCreateBinary();
    if (*pSemaphoreHandle == NULL)
    {
        return NHNS_STATUS_FAIL;
    }

    return NHNS_STATUS_OK;
}

nhns_status_t OS_SemaphoreDelete(os_semaphore_t pSemaphoreHandle)
{
    // 1) Check if we're inside an ISR as semaphore deletion from ISR is not allowed in FreeRTOS
    if (OS_IS_INSIDE_ISR())
    {
        return NHNS_STATUS_FORBIDDEN;
    }

    // 2) Delete the semaphore
    vSemaphoreDelete(pSemaphoreHandle);

    return NHNS_STATUS_OK;
}

nhns_status_t OS_SemaphoreTake(os_semaphore_t pSemaphoreHandle, uint32_t bTimeoutMs)
{
    BaseType_t nRetInternal             = pdPASS;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    TickType_t bInitialTickCount        = 0;
    TickType_t bElapsedTickCount        = 0;

    // 1) Check if we're inside an ISR and appropriately take the semaphore
    if (OS_IS_INSIDE_ISR())
    {
        nRetInternal = xSemaphoreTakeFromISR(pSemaphoreHandle, &xHigherPriorityTaskWoken);
        if (nRetInternal != pdPASS)
        {
            return (NHNS_STATUS_BASE_RTOS + nRetInternal);
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        bInitialTickCount = xTaskGetTickCount();

        nRetInternal = xSemaphoreTake(pSemaphoreHandle, OS_MS_TO_TICKS(bTimeoutMs));
        if (nRetInternal != pdPASS)
        {
            bElapsedTickCount = xTaskGetTickCount() - bInitialTickCount;
            if (bElapsedTickCount >= OS_MS_TO_TICKS(bTimeoutMs))
            {
                return NHNS_STATUS_TIMEOUT;
            }
            else
            {
                return (NHNS_STATUS_BASE_RTOS + nRetInternal);
            }
        }
    }

    return NHNS_STATUS_OK;
}

nhns_status_t OS_SemaphoreGive(os_semaphore_t pSemaphoreHandle)
{
    BaseType_t nRetInternal             = pdPASS;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    // 1) Check if we're inside an ISR and appropriately give the semaphore
    if (OS_IS_INSIDE_ISR())
    {
        nRetInternal = xSemaphoreGiveFromISR(pSemaphoreHandle, &xHigherPriorityTaskWoken);
        if (nRetInternal != pdPASS)
        {
            return (NHNS_STATUS_BASE_RTOS + nRetInternal);
        }

        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
    else
    {
        nRetInternal = xSemaphoreGive(pSemaphoreHandle);
        if (nRetInternal != pdPASS)
        {
            return (NHNS_STATUS_BASE_RTOS + nRetInternal);
        }
    }

    return NHNS_STATUS_OK;
}
