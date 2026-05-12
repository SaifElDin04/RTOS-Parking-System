/*
 * Responsibilities:
 *   STEP 1 — Update held-state flags for all direction buttons
 *   STEP 2 — Handle limit & obstacle events FIRST (FIX-2: before conflict check)
 *   STEP 3 — Detect conflicting input (OPEN + CLOSE on same panel -> stop)
 *   STEP 4 — Resolve effective command using Security Priority
 *   STEP 5 — State machine transitions for OPEN direction
 *   STEP 6 — State machine transitions for CLOSE direction
 *
 * MANUAL vs AUTO:
 *   On PRESS   -> record pressStartTick, start moving
 *   On RELEASE -> held_ms >= MANUAL_THRESHOLD_MS : stop (STOPPED_MIDWAY)
 *                 held_ms <  MANUAL_THRESHOLD_MS : keep moving (auto)
 */

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "gate_state.h"
#include "config.h"
#include "basic_io.h"
#include <stdint.h>
#include <stdbool.h>

void vGateControlTask(void *pvParameters)
{
    (void)pvParameters;

    ButtonEvent_t evt;

    /* Held-state flags — updated in STEP 1 */
    bool drvOpenHeld  = false;
    bool drvCloseHeld = false;
    bool secOpenHeld  = false;
    bool secCloseHeld = false;

    /* for manual vs auto decision */
    TickType_t pressStartTick   = 0;
    bool       lastPressWasOpen = false;

    vPrintString("Gate Control Task started\n");

    for (;;)
    {

    }
}
