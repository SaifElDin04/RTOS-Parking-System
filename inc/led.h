/*
 * led.h — LED control function prototypes
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

void LED_Set(uint32_t mask, bool on);
void LED_AllOff(void);

#endif /* LED_H */
