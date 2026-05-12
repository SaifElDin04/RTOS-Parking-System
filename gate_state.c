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


void GateState_Set(GateState_t newState)
{
    /* TODO */
}


GateState_t GateState_Get(void)
{
    /* TODO */
    return gateState;
}


bool GateState_CompareAndSet(GateState_t expected, GateState_t newState)
{
    /* TODO */
    return false;
}
