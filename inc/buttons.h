#ifndef BUTTONS_H
#define BUTTONS_H

#include "config.h"
#include "tm4c123gh6pm.h"
#include <stdbool.h>

/* Port E — pull-DOWN, active-HIGH */
static inline bool Btn_DriverOpen(void)  { return (GPIO_PORTE_DATA_R & DRIVER_OPEN_PIN)  != 0; }
static inline bool Btn_DriverClose(void) { return (GPIO_PORTE_DATA_R & DRIVER_CLOSE_PIN) != 0; }
static inline bool Btn_SecOpen(void)     { return (GPIO_PORTE_DATA_R & SEC_OPEN_PIN)     != 0; }
static inline bool Btn_SecClose(void)    { return (GPIO_PORTE_DATA_R & SEC_CLOSE_PIN)    != 0; }

/* Port D — pull-DOWN, active-HIGH */
static inline bool Btn_OpenLimit(void)   { return (GPIO_PORTD_DATA_R & OPEN_LIMIT_PIN)   != 0; }
static inline bool Btn_ClosedLimit(void) { return (GPIO_PORTD_DATA_R & CLOSED_LIMIT_PIN) != 0; }
static inline bool Btn_Obstacle(void)    { return (GPIO_PORTD_DATA_R & OBSTACLE_PIN)     != 0; }

/* Port F — pull-UP, active-LOW */
static inline bool Btn_SW1(void)         { return (GPIO_PORTF_DATA_R & SW1) == 0; }
static inline bool Btn_SW2(void)         { return (GPIO_PORTF_DATA_R & SW2) == 0; }

#endif /* BUTTONS_H */
