/*
 * Polls all buttons every 20 ms, detects edges, sends ButtonEvent_t
 * to xButtonQueue, and gives limit/obstacle semaphores.
 */

#ifndef TASK_INPUT_H
#define TASK_INPUT_H

void vInputTask(void *pvParameters);

#endif /* TASK_INPUT_H */
