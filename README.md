# Smart Parking Garage Gate System

A real-time embedded system implementing a smart parking garage gate controller using FreeRTOS on the TM4C123GH6PM microcontroller. The system manages gate movement through a fully defined finite state machine (FSM), with support for dual-panel control, obstacle detection, manual/auto operating modes, and safety prioritization.

> **Course:** CSE411 / CSE323 — Spring 2026  
> **Target MCU:** Texas Instruments TM4C123GH6PM (Tiva C Series)  
> **RTOS:** FreeRTOS  
> **IDE:** Keil µVision

---

## Features

- Finite state machine with 6 gate states
- Dual-panel input: Driver panel + Security panel (security takes priority)
- Manual mode (hold ≥ 500 ms → gate stops on release) and Auto mode (tap < 500 ms → gate runs to limit)
- Obstacle detection during closing triggers automatic 500 ms reversal
- Conflicting button input detection (both OPEN + CLOSE simultaneously → safe stop)
- Thread-safe shared state using FreeRTOS mutex
- Real-time UART status logging every 2 seconds
- LED visual feedback (Green = opening/reversing, Red = closing, Off = idle/stopped)

---

## State Machine

```
IDLE_CLOSED ──[OPEN press]──────────────────► OPENING
OPENING     ──[Open Limit]──────────────────► IDLE_OPEN
OPENING     ──[release, held ≥ 500 ms]──────► STOPPED_MIDWAY  (Manual)
OPENING     ──[release, held < 500 ms]───────  stays OPENING   (Auto)

IDLE_OPEN   ──[CLOSE press]─────────────────► CLOSING
CLOSING     ──[Closed Limit]────────────────► IDLE_CLOSED
CLOSING     ──[release, held ≥ 500 ms]──────► STOPPED_MIDWAY  (Manual)
CLOSING     ──[release, held < 500 ms]───────  stays CLOSING   (Auto)
CLOSING     ──[Obstacle detected]───────────► REVERSING

REVERSING   ──[500 ms elapsed]──────────────► STOPPED_MIDWAY

STOPPED_MIDWAY ──[OPEN press]───────────────► OPENING
STOPPED_MIDWAY ──[CLOSE press]──────────────► CLOSING

ANY MOVING STATE ──[OPEN + CLOSE conflict]──► STOPPED_MIDWAY
```

---

## Task Architecture

| Task | Priority | Stack | Period | Responsibility |
|------|----------|-------|--------|----------------|
| Safety | 4 (Highest) | 256 words | Event-driven | Monitors obstacle sensor; triggers immediate reversal during closing |
| Input | 3 (High) | 256 words | 20 ms | Polls all GPIO pins, debounces, and sends `ButtonEvent_t` to queue |
| GateControl | 2 (Medium) | 512 words | Event-driven | Processes the FSM transitions from the button event queue |
| LED | 2 (Medium) | 128 words | 50 ms | Reads gate state and drives on-board RGB LEDs |
| Status | 1 (Low) | 256 words | 2000 ms | Prints current gate state to UART console |

---

## RTOS Primitives

| Primitive | Type | Purpose |
|-----------|------|---------|
| `xButtonQueue` | Queue (depth 20) | Passes `ButtonEvent_t` from Input Task → Gate Control Task |
| `xGateStateMutex` | Mutex | Protects read/write access to the shared `gateState` variable |
| `xObstacleSem` | Binary Semaphore | Wakes Safety Task immediately on obstacle edge detection |
| `xOpenLimitSem` | Binary Semaphore | Signals open limit switch activation |
| `xClosedLimitSem` | Binary Semaphore | Signals closed limit switch activation |

---

## Hardware Pin Mapping

### Port F — LEDs & On-board Switches
| Pin | Function | Direction | Config |
|-----|----------|-----------|--------|
| PF1 | Red LED | Output | — |
| PF2 | Blue LED | Output | — |
| PF3 | Green LED | Output | — |
| PF0 (SW2) | On-board switch | Input | Pull-up |
| PF4 (SW1) | On-board switch | Input | Pull-up |

### Port E — Driver & Security Buttons (PE0–PE3)
| Pin | Function | Direction | Config |
|-----|----------|-----------|--------|
| PE0 | Driver Open | Input | Pull-down |
| PE1 | Driver Close | Input | Pull-down |
| PE2 | Security Open | Input | Pull-down |
| PE3 | Security Close | Input | Pull-down |

### Port D — Limit Switches & Obstacle Sensor (PD0–PD2)
| Pin | Function | Direction | Config |
|-----|----------|-----------|--------|
| PD0 | Open Limit Switch | Input | Pull-down |
| PD1 | Closed Limit Switch | Input | Pull-down |
| PD2 | Obstacle Sensor | Input | Pull-down |

---

## Project Structure

```
Smart Gate Parking System/
│
├── inc/                          # Header files
│   ├── config.h                  # Pin #defines and MANUAL_THRESHOLD_MS (500 ms)
│   ├── gate_state.h              # GateState_t enum, ButtonEvent_t enum, RTOS externs
│   ├── hardware.h                # Hardware_Init() prototype
│   ├── led.h                     # LED_Set(), LED_AllOff() prototypes
│   ├── buttons.h                 # Inline Btn_*() GPIO read helpers
│   ├── task_input.h              # vInputTask() prototype
│   ├── task_gate_control.h       # vGateControlTask() prototype
│   ├── task_led.h                # vLEDControlTask() prototype
│   ├── task_safety.h             # vSafetyTask() prototype
│   ├── task_status.h             # vStatusTask() prototype
│   └── tm4c123gh6pm.h            # TI device register definitions
│
├── Required_Files/               # FreeRTOS kernel source files
├── RTE/                          # Keil RTE component configuration
│
├── gate_state.c                  # RTOS handles, shared gateState, mutex accessors
├── hardware.c                    # Port F / E / D GPIO initialisation
├── led.c                         # LED driver (LED_Set, LED_AllOff)
├── main.c                        # RTOS primitive creation, task spawning, hooks
├── task_gate_control.c           # Gate Control Task — full FSM implementation
├── task_input.c                  # Input Task — polling, debounce, edge detection
├── task_led.c                    # LED Control Task
├── task_safety.c                 # Safety Task — obstacle reversal
├── task_status.c                 # Status Task — UART logging
│
├── Smart Gate Parking System.uvprojx   # Keil µVision project file
├── buttons.ini                         # Keil simulator button script
└── .gitignore
```

---

## Keil Setup

1. Open `Smart Gate Parking System.uvprojx` in Keil µVision
2. Go to **Project → Options for Target → C/C++ → Include Paths** and add:
   - `.\inc`
   - `.\Required_Files` (or wherever your FreeRTOS headers live)
3. Ensure all `.c` files listed above are added to your Keil source group
4. Build and flash to the TM4C123GH6PM LaunchPad

---

## LED Behavior

| Gate State | Red LED | Green LED |
|------------|---------|-----------|
| IDLE_CLOSED | Off | Off |
| OPENING | Off | On |
| IDLE_OPEN | Off | Off |
| CLOSING | On | Off |
| STOPPED_MIDWAY | Off | Off |
| REVERSING | Off | On |

---

## Safety Behavior

- **Obstacle during CLOSING** → gate immediately reverses (`GATE_REVERSING`) for 500 ms, then stops at `GATE_STOPPED_MIDWAY`
- **Obstacle during OPENING** → ignored (safe direction, no reversal needed)
- Uses an atomic `CompareAndSet` operation to guarantee the transition only fires when the gate is actually closing
- Safety Task runs at the highest priority (4), so it preempts all other tasks

---

## Security Priority

When the security panel is active:
- A Security OPEN press overrides any held Driver CLOSE
- A Security CLOSE press overrides any held Driver OPEN
- Security events are always processed; driver events are blocked when a conflicting security command is active
