#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "basic_io.h"
#include "hardware.h"
#include "gate_state.h"
#include "task_input.h"
#include "task_gate_control.h"
#include "task_led.h"
#include "task_safety.h"
#include "task_status.h"

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    vPrintString("STACK OVERFLOW in task: ");
    vPrintString(pcTaskName);
    vPrintString("\n");
    for (;;) {}
}


int main(void)
{
    Hardware_Init();
    vPrintString("\n=== Smart Parking Gate System Starting ===\n");

    /* Create RTOS primitives */
    xButtonQueue    = xQueueCreate(20, sizeof(ButtonEvent_t)); /* TC-19 */
    xOpenLimitSem   = xSemaphoreCreateBinary();                /* TC-21 */
    xClosedLimitSem = xSemaphoreCreateBinary();                /* TC-21 */
    xObstacleSem    = xSemaphoreCreateBinary();                /* TC-07, TC-21 */
    xGateStateMutex = xSemaphoreCreateMutex();                 /* TC-20 */

    configASSERT(xButtonQueue    != NULL);
    configASSERT(xOpenLimitSem   != NULL);
    configASSERT(xClosedLimitSem != NULL);
    configASSERT(xObstacleSem    != NULL);
    configASSERT(xGateStateMutex != NULL);

    vPrintString("RTOS primitives created\n");

    xTaskCreate(vSafetyTask,      "Safety",   256, NULL, 4, NULL);
    xTaskCreate(vInputTask,       "Input",    256, NULL, 3, NULL);
    xTaskCreate(vGateControlTask, "GateCtrl", 512, NULL, 2, NULL);
    xTaskCreate(vLEDControlTask,  "LED",      128, NULL, 2, NULL);
    xTaskCreate(vStatusTask,      "Status",   256, NULL, 1, NULL);

    vPrintString("All tasks created — starting scheduler\n");
    vTaskStartScheduler();

    for (;;) {}
}
