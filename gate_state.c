/*
 RTOS handle definitions, shared gate state variable,
 and mutex-protected state accessor implementations.
 */

#include "gate_state.h"

/* =========================================================================
 * RTOS HANDLE DEFINITIONS
 * ========================================================================= */
QueueHandle_t     xButtonQueue;
SemaphoreHandle_t xOpenLimitSem;
SemaphoreHandle_t xClosedLimitSem;
SemaphoreHandle_t xObstacleSem;
SemaphoreHandle_t xGateStateMutex;

/* =========================================================================
 * SHARED GATE STATE
 * ========================================================================= */
static volatile GateState_t gateState = GATE_IDLE_CLOSED;

/* =========================================================================
 * GATE STATE ACCESSOR IMPLEMENTATIONS
 * ========================================================================= */

void GateState_Set(GateState_t newState)
{
    xSemaphoreTake(xGateStateMutex, portMAX_DELAY);
    gateState = newState;
    xSemaphoreGive(xGateStateMutex);
}

GateState_t GateState_Get(void)
{
    GateState_t s;
    xSemaphoreTake(xGateStateMutex, portMAX_DELAY);
    s = gateState;
    xSemaphoreGive(xGateStateMutex);
    return s;
}

bool GateState_CompareAndSet(GateState_t expected, GateState_t newState)
{
    bool ok = false;
    xSemaphoreTake(xGateStateMutex, portMAX_DELAY);
    if (gateState == expected) {
        gateState = newState;
        ok = true;
    }
    xSemaphoreGive(xGateStateMutex);
    return ok;
}
