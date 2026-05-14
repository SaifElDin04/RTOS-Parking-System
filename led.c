#include "led.h"
#include "config.h"
#include "tm4c123gh6pm.h"

void LED_Set(uint32_t mask, bool on)
{
    if (on) GPIO_PORTF_DATA_R |=  (mask & LED_MASK);
    else    GPIO_PORTF_DATA_R &= ~(mask & LED_MASK);
}

void LED_AllOff(void)
{
    GPIO_PORTF_DATA_R &= ~LED_MASK;
}
