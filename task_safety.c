/*
 * task_safety.c — Safety Task  (Priority 4 — Highest)
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 *
 * Blocks indefinitely on xObstacleSem (given by Input Task on obstacle edge).
 *
 * On signal:
 *   1. GateState_CompareAndSet(GATE_CLOSING, GATE_REVERSING)
 *      — if gate is NOT closing, this fails silently (TC-09: obstacle
 *        during opening is correctly ignored).
 *      — if gate IS closing, state atomically becomes GATE_REVERSING.
 *   2. Wait 500 ms  (Green LED ON via LED task during this period).
 *   3. Set state to GATE_STOPPED_MIDWAY — gate stops completely.
 *
 * TC-07: primary obstacle + reverse.
 * TC-08: overrides even a held CLOSE button (Safety priority = 4 wins).
 * TC-09: ignored during opening (CompareAndSet returns false).
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
