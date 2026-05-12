/*
 * task_status.h — Status task prototype
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 *
 * Priority: 1 (Low)
 * Prints current gate state to the UART console every 2 seconds.
 */

#ifndef TASK_STATUS_H
#define TASK_STATUS_H

void vStatusTask(void *pvParameters);

#endif /* TASK_STATUS_H */
