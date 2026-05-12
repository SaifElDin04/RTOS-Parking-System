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



/* Called if a task overflows its stack — halt so you can catch it */
void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName)
{
    (void)xTask;
    vPrintString("STACK OVERFLOW in task: ");
    vPrintString(pcTaskName);
    vPrintString("\n");
    for (;;) {}
}

/* Called by the idle task hook — leave empty if nothing to do */
void vApplicationIdleHook(void)
{
    /* nothing */
}


int main(void)
{
    /* TODO: Step 1 — Call Hardware_Init() */

    vPrintString("\n=== Smart Parking Gate System Starting ===\n");

    /* TODO: Step 2 — Create RTOS primitives and assign to global handles:
     *
     *   xButtonQueue    = xQueueCreate(20, sizeof(ButtonEvent_t));
     *   xOpenLimitSem   = xSemaphoreCreateBinary();
     *   xClosedLimitSem = xSemaphoreCreateBinary();
     *   xObstacleSem    = xSemaphoreCreateBinary();
     *   xGateStateMutex = xSemaphoreCreateMutex();
     *
     *   Then assert each is not NULL with configASSERT()
     */

    vPrintString("RTOS primitives created\n");

    /* TODO: Step 3 — Create all tasks: */

    vPrintString("All tasks created — starting scheduler\n");


    vTaskStartScheduler()

    for (;;) {}   /* never reached */
}
