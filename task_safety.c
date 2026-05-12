/*
 * Responsibilities:
 *   - Block indefinitely on xObstacleSem
 *   - On signal: atomically transition GATE_CLOSING -> GATE_REVERSING
 *   - Wait 500 ms (Green LED stays ON via LED task during this time)
 *   - Then set state to GATE_STOPPED_MIDWAY
 */

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "gate_state.h"
#include "basic_io.h"

void vSafetyTask(void *pvParameters)
{
    (void)pvParameters;
    vPrintString("Safety Task started\n");

    for (;;)
    {

    }
}
