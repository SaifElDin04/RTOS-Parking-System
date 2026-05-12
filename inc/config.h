/*
 * config.h — Hardware pin mapping and system-wide constants
 * Smart Parking Garage Gate System
 * TM4C123GH6PM + FreeRTOS
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* =========================================================================
 * PORT F — on-board LEDs + spare switches
 * ========================================================================= */
#define GREEN_LED       (1U << 3)   /* PF3 — OPENING / REVERSING              */
#define RED_LED         (1U << 1)   /* PF1 — CLOSING                          */
#define SW1             (1U << 4)   /* PF4 — spare, active-LOW,  pull-UP      */
#define SW2             (1U << 0)   /* PF0 — spare, active-LOW,  pull-UP (locked) */
#define LED_MASK        (GREEN_LED | RED_LED)

/* =========================================================================
 * PORT E — driver & security panels
 * pull-DOWN, active-HIGH (pin=1 when pressed)
 * ========================================================================= */
#define DRIVER_OPEN_PIN  (1U << 0)  /* PE0 */
#define DRIVER_CLOSE_PIN (1U << 1)  /* PE1 */
#define SEC_OPEN_PIN     (1U << 2)  /* PE2 */
#define SEC_CLOSE_PIN    (1U << 3)  /* PE3 */
#define PORTE_BTN_MASK   (DRIVER_OPEN_PIN | DRIVER_CLOSE_PIN | \
                          SEC_OPEN_PIN    | SEC_CLOSE_PIN)

/* =========================================================================
 * PORT D — limit switches & obstacle
 * pull-DOWN, active-HIGH (pin=1 when pressed)
 * ========================================================================= */
#define OPEN_LIMIT_PIN   (1U << 0)  /* PD0 */
#define CLOSED_LIMIT_PIN (1U << 1)  /* PD1 */
#define OBSTACLE_PIN     (1U << 2)  /* PD2 */
#define PORTD_BTN_MASK   (OPEN_LIMIT_PIN | CLOSED_LIMIT_PIN | OBSTACLE_PIN)

/* =========================================================================
 * RCGCGPIO bit positions
 * ========================================================================= */
#define RCGCGPIO_D  (1U << 3)
#define RCGCGPIO_E  (1U << 4)
#define RCGCGPIO_F  (1U << 5)

/* =========================================================================
 * MANUAL_THRESHOLD_MS — tap-vs-hold boundary (milliseconds).
 * Button released in  < this value -> one-touch auto mode (gate runs to limit).
 * Button released in >= this value -> manual mode (gate stops on release).
 * ========================================================================= */
#define MANUAL_THRESHOLD_MS  5000U

#endif /* CONFIG_H */
