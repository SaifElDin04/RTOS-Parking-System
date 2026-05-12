/*
 * task_led.h — LED control task prototype
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 *
 * Priority: 2 (Medium)
 * Reads gate state every 50 ms and drives Green/Red LEDs accordingly.
 */

#ifndef TASK_LED_H
#define TASK_LED_H

void vLEDControlTask(void *pvParameters);

#endif /* TASK_LED_H */
