/*
 * Reads gate state every 50 ms and drives Green/Red LEDs accordingly.
 */

#ifndef TASK_LED_H
#define TASK_LED_H

void vLEDControlTask(void *pvParameters);

#endif /* TASK_LED_H */
