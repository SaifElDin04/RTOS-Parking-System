#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "gate_state.h"
#include "basic_io.h"

void vSafetyTask(void *pvParameters)
{
    (void)pvParameters;
    vPrintString("Safety Task started\n");

    for (;;) {
        xSemaphoreTake(xObstacleSem, portMAX_DELAY);

        if (GateState_CompareAndSet(GATE_CLOSING, GATE_REVERSING)) {
            vPrintString("OBSTACLE! Reversing for 500 ms\n");

            vTaskDelay(pdMS_TO_TICKS(500));     /* TC-07: 0.5 s reverse */

            GateState_Set(GATE_STOPPED_MIDWAY);
            vPrintString("Reverse complete -> STOPPED_MIDWAY\n");
        } else {
            vPrintString("Obstacle signal ignored (gate not closing)\n");
        }
    }
}
