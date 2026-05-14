/*
 * task_led.c — LED Control Task  (Priority 2 — Medium)
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 *
 * Reads gate state every 50 ms and drives the on-board LEDs:
 *   OPENING   -> Green ON,  Red OFF
 *   REVERSING -> Green ON,  Red OFF  (obstacle reverse shows Green per spec)
 *   CLOSING   -> Red   ON,  Green OFF
 *   all others -> both OFF
 */

#include "FreeRTOS.h"
#include "task.h"
#include "gate_state.h"
#include "led.h"
#include "config.h"
#include "basic_io.h"

void vLEDControlTask(void *pvParameters)
{
    (void)pvParameters;
    vPrintString("LED Control Task started\n");

    for (;;) {
        switch (GateState_Get()) {
            case GATE_OPENING:
            case GATE_REVERSING:
                LED_Set(GREEN_LED, true);
                LED_Set(RED_LED,   false);
                break;

            case GATE_CLOSING:
                LED_Set(RED_LED,   true);
                LED_Set(GREEN_LED, false);
                break;

            default:
                LED_AllOff();
                break;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
