/*
 * Responsibilities:
 *   - Read gate state every 50 ms
 *   - Drive LEDs based on state:
 *       OPENING / REVERSING -> Green ON,  Red OFF
 *       CLOSING             -> Red   ON,  Green OFF
 *       all others          -> both OFF
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

    for (;;)
    {

    }
}
