/*
 * Responsibilities:
 *   - Every 2 seconds, read the current gate state
 *   - Print a human-readable status string to the UART console
 */

#include "FreeRTOS.h"
#include "task.h"
#include "gate_state.h"
#include "basic_io.h"

void vStatusTask(void *pvParameters)
{
    (void)pvParameters;
    vPrintString("Status Task started\n");

    for (;;)
    {
    }
}
