/*
 * task_gate_control.h — Gate control task prototype
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 *
 * Priority: 2 (Medium)
 * Implements the full state machine FSM, manual/auto mode detection,
 * security priority, and conflicting-input detection.
 */

#ifndef TASK_GATE_CONTROL_H
#define TASK_GATE_CONTROL_H

void vGateControlTask(void *pvParameters);

#endif /* TASK_GATE_CONTROL_H */
