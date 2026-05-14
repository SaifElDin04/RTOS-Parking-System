#include "FreeRTOS.h"
#include "task.h"
#include "gate_state.h"
#include "basic_io.h"

void vStatusTask(void *pvParameters)
{
    (void)pvParameters;
    vPrintString("Status Task started\n");

    for (;;) {
        switch (GateState_Get()) {
            case GATE_IDLE_CLOSED:
                vPrintString("[STATUS] IDLE_CLOSED\n");               break;
            case GATE_IDLE_OPEN:
                vPrintString("[STATUS] IDLE_OPEN\n");                 break;
            case GATE_OPENING:
                vPrintString("[STATUS] OPENING  (Green LED)\n");      break;
            case GATE_CLOSING:
                vPrintString("[STATUS] CLOSING  (Red LED)\n");        break;
            case GATE_STOPPED_MIDWAY:
                vPrintString("[STATUS] STOPPED_MIDWAY\n");            break;
            case GATE_REVERSING:
                vPrintString("[STATUS] REVERSING (Green LED, 500ms)\n"); break;
            default:
                vPrintString("[STATUS] UNKNOWN\n");                   break;
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}
