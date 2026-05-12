/*
 * Blocks on xObstacleSem; on signal, atomically switches CLOSING->REVERSING,
 * waits 500 ms, then sets STOPPED_MIDWAY.
 */

#ifndef TASK_SAFETY_H
#define TASK_SAFETY_H

void vSafetyTask(void *pvParameters);

#endif /* TASK_SAFETY_H */
