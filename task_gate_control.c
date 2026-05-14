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
        if (xQueueReceive(xButtonQueue, &evt, portMAX_DELAY) != pdTRUE)
            continue;

        // STEP 1 — Update held-state flags for all direction buttons.
        switch (evt) {
            case EVT_DRIVER_OPEN_PRESS:    drvOpenHeld  = true;  break;
            case EVT_DRIVER_OPEN_RELEASE:  drvOpenHeld  = false; break;
            case EVT_DRIVER_CLOSE_PRESS:   drvCloseHeld = true;  break;
            case EVT_DRIVER_CLOSE_RELEASE: drvCloseHeld = false; break;
            case EVT_SEC_OPEN_PRESS:       secOpenHeld  = true;  break;
            case EVT_SEC_OPEN_RELEASE:     secOpenHeld  = false; break;
            case EVT_SEC_CLOSE_PRESS:      secCloseHeld = true;  break;
            case EVT_SEC_CLOSE_RELEASE:    secCloseHeld = false; break;
            default: break;
        }

        // STEP 2 — Handle limit and obstacle events FIRST (FIX-2).
        if (evt == EVT_OPEN_LIMIT) {
            if (GateState_Get() == GATE_OPENING) {
                GateState_Set(GATE_IDLE_OPEN);
                vPrintString("Open limit hit -> IDLE_OPEN\n");
            } else {
                vPrintString("Open limit ignored (gate not opening)\n");
            }
            continue;
        }

        if (evt == EVT_CLOSED_LIMIT) {
            if (GateState_Get() == GATE_CLOSING) {
                GateState_Set(GATE_IDLE_CLOSED);
                vPrintString("Closed limit hit -> IDLE_CLOSED\n");
            } else {
                vPrintString("Closed limit ignored (gate not closing)\n");
            }
            continue;
        }

        if (evt == EVT_OBSTACLE) {
            /* Safety Task owns this transition; GateControl discards it */
            continue;
        }

        // STEP 3 — Conflicting-input detection 
        bool secConflict = secOpenHeld && secCloseHeld;
        bool drvConflict = drvOpenHeld && drvCloseHeld;

        if (secConflict || drvConflict) {
            GateState_t cur = GateState_Get();
            if (cur == GATE_OPENING || cur == GATE_CLOSING || cur == GATE_REVERSING) {
                GateState_Set(GATE_STOPPED_MIDWAY);
                vPrintString("Conflicting input! Gate STOPPED_MIDWAY\n");
            }
            continue;
        }

        // Resolve effective command using Security Priority.
        bool isOpenPress    = false;
        bool isOpenRelease  = false;
        bool isClosePress   = false;
        bool isCloseRelease = false;

        switch (evt) {
            case EVT_SEC_OPEN_PRESS:
                isOpenPress  = true;
                drvCloseHeld = false;
                break;
            case EVT_SEC_OPEN_RELEASE:
                isOpenRelease = true;
                break;
            case EVT_SEC_CLOSE_PRESS:
                isClosePress = true;
                drvOpenHeld  = false;
                break;
            case EVT_SEC_CLOSE_RELEASE:
                isCloseRelease = true;
                break;
            case EVT_DRIVER_OPEN_PRESS:
                if (!secCloseHeld) isOpenPress = true;
                break;
            case EVT_DRIVER_OPEN_RELEASE:
                if (!secOpenHeld)  isOpenRelease = true;
                break;
            case EVT_DRIVER_CLOSE_PRESS:
                if (!secOpenHeld)  isClosePress = true;
                break;
            case EVT_DRIVER_CLOSE_RELEASE:
                if (!secCloseHeld) isCloseRelease = true;
                break;
            default: break;
        }

         // State machine transitions for OPEN direction.
        if (isOpenPress) {
            pressStartTick   = xTaskGetTickCount();
            lastPressWasOpen = true;

            GateState_t cur = GateState_Get();
            switch (cur) {
                case GATE_IDLE_CLOSED:
                case GATE_STOPPED_MIDWAY:
                    GateState_Set(GATE_OPENING);
                    vPrintString("Gate OPENING\n");
                    break;
                case GATE_CLOSING:
                    GateState_Set(GATE_OPENING);
                    vPrintString("Gate OPENING (interrupted close)\n");
                    break;
                case GATE_REVERSING:
                    GateState_Set(GATE_OPENING);
                    vPrintString("Gate OPENING (was reversing)\n");
                    break;
                case GATE_IDLE_OPEN:
                    break;
                case GATE_OPENING:
                    vPrintString("Gate Already Opening....\n");
                    break;
            }
        }
        else if (isOpenRelease && lastPressWasOpen) {
            if (GateState_Get() == GATE_OPENING) {
                uint32_t heldMs = (uint32_t)(
                    (xTaskGetTickCount() - pressStartTick) * portTICK_PERIOD_MS);

                if (heldMs >= MANUAL_THRESHOLD_MS) {
                    GateState_Set(GATE_STOPPED_MIDWAY);
                    vPrintString("Manual OPEN released -> STOPPED_MIDWAY\n");
                } else {
                    vPrintString("Auto OPEN: running to open limit\n");
                }
            }
            lastPressWasOpen = false;
        }

        
        // State machine transitions for CLOSE direction.
        if (isClosePress) {
            pressStartTick   = xTaskGetTickCount();
            lastPressWasOpen = false;

            GateState_t cur = GateState_Get();
            switch (cur) {
                case GATE_IDLE_OPEN:
                case GATE_STOPPED_MIDWAY:
                    GateState_Set(GATE_CLOSING);
                    vPrintString("Gate CLOSING\n");
                    break;
                case GATE_OPENING:
                    GateState_Set(GATE_CLOSING);
                    vPrintString("Gate CLOSING (interrupted open)\n");
                    break;
                case GATE_REVERSING:
                    GateState_Set(GATE_CLOSING);
                    vPrintString("Gate CLOSING (was reversing)\n");
                    break;
                case GATE_IDLE_CLOSED:
                    break;
                case GATE_CLOSING:
                    vPrintString("Gate Already Closing....\n");
                    break;
            }
        }
        else if (isCloseRelease && !lastPressWasOpen) {
            if (GateState_Get() == GATE_CLOSING) {
                uint32_t heldMs = (uint32_t)(
                    (xTaskGetTickCount() - pressStartTick) * portTICK_PERIOD_MS);

                if (heldMs >= MANUAL_THRESHOLD_MS) {
                    GateState_Set(GATE_STOPPED_MIDWAY);
                    vPrintString("Manual CLOSE released -> STOPPED_MIDWAY\n");
                } else {
                    vPrintString("Auto CLOSE: running to closed limit\n");
                }
            }
        }
    }
}
