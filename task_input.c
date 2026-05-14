/*
 * task_input.c — Input Task  (Priority 3 — High)
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 *
 * Polls all buttons every 20 ms (debounce window).
 * Detects rising edges (press) and falling edges (release).
 * Sends ButtonEvent_t to xButtonQueue for GateControl to process.
 * Also gives binary semaphores for limit/obstacle so Safety Task and
 * GateControl can react to them immediately (TC-21).
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

    /*allow pull resistors to settle before snapshotting idle state */
    vTaskDelay(pdMS_TO_TICKS(10));

    /*seed prev* from real pin state — polarity handled by Btn_*() */
    bool prevDrvOpen   = Btn_DriverOpen();
    bool prevDrvClose  = Btn_DriverClose();
    bool prevSecOpen   = Btn_SecOpen();
    bool prevSecClose  = Btn_SecClose();
    bool prevOpenLim   = Btn_OpenLimit();
    bool prevClosedLim = Btn_ClosedLimit();
    bool prevObstacle  = Btn_Obstacle();

    vPrintString("Input Task started\n");

    for (;;) {
        bool curDrvOpen   = Btn_DriverOpen();
        bool curDrvClose  = Btn_DriverClose();
        bool curSecOpen   = Btn_SecOpen();
        bool curSecClose  = Btn_SecClose();
        bool curOpenLim   = Btn_OpenLimit();
        bool curClosedLim = Btn_ClosedLimit();
        bool curObstacle  = Btn_Obstacle();

        ButtonEvent_t evt;

#define SEND_EDGE(cur, prev, pressEvt, relEvt)          \
        if ((cur) && !(prev)) {                          \
            evt = (pressEvt);                            \
            xQueueSend(xButtonQueue, &evt, 0);           \
        } else if (!(cur) && (prev)) {                   \
            evt = (relEvt);                              \
            xQueueSend(xButtonQueue, &evt, 0);           \
        }

        SEND_EDGE(curDrvOpen,  prevDrvOpen,  EVT_DRIVER_OPEN_PRESS,  EVT_DRIVER_OPEN_RELEASE)
        SEND_EDGE(curDrvClose, prevDrvClose, EVT_DRIVER_CLOSE_PRESS, EVT_DRIVER_CLOSE_RELEASE)
        SEND_EDGE(curSecOpen,  prevSecOpen,  EVT_SEC_OPEN_PRESS,     EVT_SEC_OPEN_RELEASE)
        SEND_EDGE(curSecClose, prevSecClose, EVT_SEC_CLOSE_PRESS,    EVT_SEC_CLOSE_RELEASE)

#undef SEND_EDGE

        /* Limit and obstacle: send queue event + give semaphore */
        if (curOpenLim && !prevOpenLim) {
            evt = EVT_OPEN_LIMIT;
            xQueueSend(xButtonQueue, &evt, 0);
            xSemaphoreGive(xOpenLimitSem);      /* TC-21 */
        }
        if (curClosedLim && !prevClosedLim) {
            evt = EVT_CLOSED_LIMIT;
            xQueueSend(xButtonQueue, &evt, 0);
            xSemaphoreGive(xClosedLimitSem);    /* TC-21 */
        }
        if (curObstacle && !prevObstacle) {
            evt = EVT_OBSTACLE;
            xQueueSend(xButtonQueue, &evt, 0);
            xSemaphoreGive(xObstacleSem);       /* wakes Safety Task immediately */
        }

        prevDrvOpen   = curDrvOpen;
        prevDrvClose  = curDrvClose;
        prevSecOpen   = curSecOpen;
        prevSecClose  = curSecClose;
        prevOpenLim   = curOpenLim;
        prevClosedLim = curClosedLim;
        prevObstacle  = curObstacle;

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
