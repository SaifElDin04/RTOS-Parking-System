/*
 * gate_state.h — Gate state machine types, RTOS handle declarations,
 *                and mutex-protected state accessor prototypes.
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 */

#ifndef GATE_STATE_H
#define GATE_STATE_H

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include <stdbool.h>

/* =========================================================================
 * GATE STATE ENUM
 * ========================================================================= */
typedef enum {
    GATE_IDLE_CLOSED,    /* fully closed, stationary                    */
    GATE_IDLE_OPEN,      /* fully open,   stationary                    */
    GATE_OPENING,        /* moving up   — Green LED ON                  */
    GATE_CLOSING,        /* moving down — Red   LED ON                  */
    GATE_STOPPED_MIDWAY, /* stopped between limits                      */
    GATE_REVERSING       /* obstacle reverse: open 500 ms then stop     */
} GateState_t;

/* =========================================================================
 * BUTTON EVENT ENUM  (queue message type)
 * Press AND release events are queued so GateControl can measure hold time.
 * ========================================================================= */
typedef enum {
    EVT_DRIVER_OPEN_PRESS,
    EVT_DRIVER_OPEN_RELEASE,
    EVT_DRIVER_CLOSE_PRESS,
    EVT_DRIVER_CLOSE_RELEASE,
    EVT_SEC_OPEN_PRESS,
    EVT_SEC_OPEN_RELEASE,
    EVT_SEC_CLOSE_PRESS,
    EVT_SEC_CLOSE_RELEASE,
    EVT_OPEN_LIMIT,
    EVT_CLOSED_LIMIT,
    EVT_OBSTACLE
} ButtonEvent_t;

/* =========================================================================
 * RTOS HANDLE DECLARATIONS (defined in gate_state.c)
 * ========================================================================= */
extern QueueHandle_t     xButtonQueue;     /* Input -> GateControl         */
extern SemaphoreHandle_t xOpenLimitSem;    /* open  limit edge signal      */
extern SemaphoreHandle_t xClosedLimitSem;  /* closed limit edge signal     */
extern SemaphoreHandle_t xObstacleSem;     /* obstacle edge -> Safety Task */
extern SemaphoreHandle_t xGateStateMutex;  /* guards gateState             */

/* =========================================================================
 * GATE STATE ACCESSOR PROTOTYPES
 * All functions are mutex-protected and safe to call from any task.
 * ========================================================================= */
void        GateState_Set(GateState_t newState);
GateState_t GateState_Get(void);
bool        GateState_CompareAndSet(GateState_t expected, GateState_t newState);

#endif /* GATE_STATE_H */
