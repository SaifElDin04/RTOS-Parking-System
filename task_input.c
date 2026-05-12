/*
 * Responsibilities:
 *   - Poll all buttons every 20 ms (debounce window)
 *   - Detect rising edges (press) and falling edges (release)
 *   - Send ButtonEvent_t to xButtonQueue for GateControl
 *   - Give binary semaphores for limit switches and obstacle sensor
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "gate_state.h"
#include "buttons.h"
#include "basic_io.h"
#include <stdbool.h>

void vInputTask(void *pvParameters)
{
    (void)pvParameters;

    /* TODO: Step 1 — Add a 10 ms delay so pull resistors settle (FIX-1) */

    /* TODO: Step 2 — Seed all prev* booleans from real pin state using  */

    vPrintString("Input Task started\n");

    for (;;)
    {

    }
}
