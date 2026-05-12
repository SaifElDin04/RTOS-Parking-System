/*
LED control function prototypes
 */

#ifndef LED_H
#define LED_H

#include <stdint.h>
#include <stdbool.h>

void LED_Set(uint32_t mask, bool on);
void LED_AllOff(void);

#endif /* LED_H */
