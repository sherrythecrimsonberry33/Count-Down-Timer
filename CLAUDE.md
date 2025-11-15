# CLAUDE.md - AI Assistant Development Guide

## Project Overview

**Count-Down-Timer** is an embedded C application for a countdown timer running on a Microchip PIC24F16KA101 microcontroller. The project implements a configurable countdown timer with terminal display output via UART, LED indicators, and three push-button controls.

### Hardware Platform
- **Microcontroller**: PIC24F16KA101 (16-bit)
- **Clock**: 8 MHz (Fast RC Oscillator - FRC)
- **Communication**: UART2 for terminal display
- **I/O**: 3 push buttons (PB1, PB2, PB3) and 1 LED

### Authors
- Sheharyar
- Aryan Mogera
- Gerardo Garcia de Leon

---

## Codebase Structure

### Architecture Pattern
The project follows a **modular architecture** with clear separation of concerns:

```
Count-Down-Timer/
├── main.c                  # Application entry point with MCU configuration
├── TimerOperations.c/h     # Core timer logic and state management
├── TimerState.h            # Timer state enum definitions
├── IOs.c/h                 # Hardware I/O control and interrupt handling
├── TerminalDisplay.c/h     # Display abstraction layer
├── TimeInterval.c/h        # Timing utilities (delays)
├── UART2.c/h               # UART communication driver
├── clkChange.c/h           # Clock configuration utilities
└── README.md
```

### Module Descriptions

#### 1. **main.c** - Application Entry Point
- **Purpose**: MCU initialization and main control loop
- **Key Features**:
  - PIC24F configuration pragmas (oscillator, watchdog, brown-out, etc.)
  - System initialization sequence
  - Infinite main loop calling `IOControl_check()`
- **Dependencies**: All other modules
- **Critical Details**:
  - Uses 8 MHz clock via `newClk(8)`
  - All configuration is done via `#pragma config` statements
  - Main loop is intentionally simple - all logic delegated to modules

#### 2. **TimerOperations.c/h** - Timer Logic Core
- **Purpose**: Core countdown timer operations and state management
- **Key Data Structure**:
  ```c
  typedef struct {
      int minutes;
      int seconds;
      TimerState state;
  } Timer;
  ```
- **Key Functions**:
  - `Timer_init()` - Initialize timer to idle state
  - `Timer_incrementMinutes()` - Increment minutes with wraparound at 60
  - `Timer_incrementSeconds()` - Increment seconds by variable amount
  - `Timer_start()`, `Timer_pause()`, `Timer_reset()` - State transitions
  - `Timer_tick()` - Decrement timer every second when running
  - `Timer_isFinished()`, `Timer_isRunning()` - State queries
- **Pattern**: Simple state machine with clean API

#### 3. **TimerState.h** - State Definitions
- **Purpose**: Define timer states as enumeration
- **States**:
  - `STATE_IDLE` - Initial/reset state
  - `STATE_SET_MINUTES` - Setting minutes (not currently used)
  - `STATE_SET_SECONDS` - Setting seconds (not currently used)
  - `STATE_RUNNING` - Timer actively counting down
  - `STATE_PAUSED` - Timer paused
  - `STATE_FINISHED` - Countdown completed
- **Note**: SET states defined but not used in current implementation

#### 4. **IOs.c/h** - Hardware I/O Control
- **Purpose**: Manage hardware inputs/outputs and user interaction
- **Hardware Mapping**:
  - `PB1` (RA2) - Increment minutes
  - `PB2` (RA4) - Increment seconds (1s normally, 5s when held >3s)
  - `PB3` (RB4) - Start/Pause (short press) or Reset (long press >3s)
  - `LED` (RB8) - Blinks when running, solid when finished
- **Key Functions**:
  - `IOControl_init()` - Configure GPIO, pull-ups, and change notifications
  - `IOControl_check()` - Main I/O polling loop (called continuously)
- **Implementation Notes**:
  - Uses polling, not interrupt-driven (despite CN interrupt setup)
  - Long press detection for PB2 (3+ seconds) and PB3 (3+ seconds)
  - LED blinks every second during countdown
  - Includes 300ms debounce delays for PB1/PB2

#### 5. **TerminalDisplay.c/h** - Display Abstraction
- **Purpose**: Provide clean API for terminal output via UART
- **Key Functions**:
  - `Display_init()` - Initialize UART2
  - `Display_update()` - Show "SET XXm : XXs"
  - `Display_count()` - Show "CNT XXm : XXs" (during countdown)
  - `Display_alarm()` - Show "FIN 00m : 00s -- ALARM"
  - `Display_reset()` - Show "CLR 00m : 00s"
  - `Display_clear()` - Clear current line with carriage return
- **Pattern**: Wrapper around UART2 functions for semantic display operations

#### 6. **UART2.c/h** - UART Driver
- **Purpose**: Low-level UART2 communication driver
- **Key Functions**:
  - `InitUART2()` - Configure UART2 peripheral
  - `XmitUART2()` - Transmit single character
  - `Disp2String()` - Transmit null-terminated string
  - `Disp2Dec()` - Display unsigned integer in decimal
  - `Disp2Hex()`, `Disp2Hex32()` - Display in hexadecimal
  - Interrupt handlers: `_U2RXInterrupt`, `_U2TXInterrupt`
- **Author**: rvyas (November 2016)

#### 7. **TimeInterval.c/h** - Timing Utilities
- **Purpose**: Provide millisecond-level delay functions
- **Key Function**:
  - `Delay_ms(uint16_t time_ms)` - Blocking delay
- **Usage**: Used for debouncing (300ms) and timer ticks (1000ms)

#### 8. **clkChange.c/h** - Clock Configuration
- **Purpose**: Configure MCU clock frequency
- **Key Function**:
  - `newClk(unsigned int clkval)` - Set clock to specified MHz
- **Usage**: Called in main with `newClk(8)` for 8 MHz operation

---

## Coding Conventions

### Naming Conventions
1. **Modules**: PascalCase for multi-word names (e.g., `TimerOperations`, `TerminalDisplay`)
2. **Functions**:
   - Module prefix with underscore: `Timer_init()`, `Display_update()`
   - Action-oriented names: `Timer_tick()`, `Display_clear()`
3. **Types**: PascalCase (e.g., `Timer`, `TimerState`)
4. **Enums**: `STATE_` prefix with SCREAMING_SNAKE_CASE
5. **Macros**: SCREAMING_SNAKE_CASE (e.g., `FCY`, `CLOCK_FREQ`)
6. **Hardware Defines**: Short uppercase names (e.g., `PB1`, `LED`)

### Header Guard Pattern
```c
#ifndef MODULE_NAME_H
#define MODULE_NAME_H
// ... content ...
#endif // MODULE_NAME_H
```

**Important**: Recent PR #1 fixed inconsistencies in header guard comments - ensure comments match the actual guard names.

### File Header Pattern
```c
/*
 * File:   filename.c
 * Author: name(s)
 *
 */
```

Some files include "(submit)" in the filename comment, indicating submission artifacts.

### Code Style
- **Indentation**: 4 spaces (observed pattern)
- **Braces**: K&R style (opening brace on same line)
- **Pointers**: Asterisk attached to type (e.g., `Timer* timer`)
- **Comments**: Sparse - code is self-documenting via good naming
- **Line Length**: No strict limit, pragmatic wrapping

---

## Key Technical Details

### Timer Tick Mechanism
The timer decrements every second when in `STATE_RUNNING`:
```c
void Timer_tick(Timer* timer) {
    if (seconds > 0) seconds--;
    else if (minutes > 0) {
        minutes--;
        seconds = 59;
    } else {
        state = STATE_FINISHED;
    }
}
```

### Long Press Detection
PB3 uses time-based press detection:
- Press duration < 3 seconds: Toggle start/pause
- Press duration >= 3 seconds: Reset timer
- Implementation tracks press start time and checks duration on release

### Display Protocol
All display functions clear the line first with `\r` and spaces, then write new content:
```c
XmitUART2('\r', 1);  // Carriage return
// ... write spaces ...
// ... write new content ...
```

### MCU Configuration Highlights
- **Oscillator**: Fast RC (FRC), no external oscillator
- **Watchdog**: Disabled (FWDTEN = OFF)
- **Brown-out**: Enabled at 1.8V
- **Deep Sleep Watchdog**: Enabled
- **Clock Switching**: Enabled, no fail-safe monitor
- **Debugging**: PGC2/PGD2 pins

---

## Development Workflows

### Building and Flashing
**Note**: This repository contains source code only. Build system (MPLAB X project files, Makefile, etc.) is not included.

**Expected build environment**:
- MPLAB X IDE or XC16 compiler
- Target device: PIC24F16KA101
- Programmer: PICkit 2/3/4 or ICD3

### Git Workflow
- **Default Branch**: Not specified (check with maintainers)
- **Recent Activity**: Header guard fixes (PR #1)
- **Commit Style**: Descriptive imperative mood ("Fix header guards", "Added relevant files")

### Testing
No automated tests present. Testing is manual on hardware:
1. Flash firmware to PIC24F
2. Connect UART terminal (baud rate set in UART2.c)
3. Verify:
   - PB1 increments minutes
   - PB2 increments seconds (1s/5s on long press)
   - PB3 starts/pauses/resets
   - LED blinks during countdown
   - Terminal displays correct states

---

## Guidelines for AI Assistants

### When Modifying Code

1. **Preserve Hardware Mapping**: Never change pin assignments without explicit direction
   - PB1 = RA2, PB2 = RA4, PB3 = RB4, LED = RB8

2. **Maintain State Machine**: Timer states are central to the architecture
   - Any new features should work within the existing state model
   - Consider adding new states to `TimerState` if needed

3. **Respect Module Boundaries**:
   - Timer logic stays in `TimerOperations`
   - Hardware I/O stays in `IOs`
   - Display logic stays in `TerminalDisplay`
   - Don't mix concerns

4. **Configuration Pragmas**: Changes to MCU configuration in main.c require deep knowledge
   - Consult PIC24F16KA101 datasheet before modifying
   - Clock changes affect all timing (UART baud, delays, etc.)

5. **Header Guards**: Ensure guard comment matches guard name (per PR #1 fix)

6. **Function Naming**: Follow the `Module_action()` pattern consistently

### Common Tasks

#### Adding a New Feature
1. Identify which module(s) it affects
2. Update header file with new function declarations
3. Implement in corresponding .c file
4. Update dependencies (includes) if needed
5. Test on hardware (no simulation environment documented)

#### Modifying Timer Behavior
- Core logic is in `TimerOperations.c` - start here
- UI changes require updates to `IOs.c` and `TerminalDisplay.c`
- Timing changes may affect `TimeInterval.c` delays

#### Debugging Display Issues
- Check `TerminalDisplay.c` for formatting
- Check `UART2.c` for baud rate and configuration
- Verify terminal settings match UART configuration

### Potential Issues to Watch

1. **Timing Dependencies**:
   - `IOControl_check()` assumes 10ms call interval for timer_ms tracking (see IOs.c:90)
   - But actual call interval is variable due to `Delay_ms()` calls
   - Consider this when modifying timing logic

2. **Interrupt Handler Not Used**:
   - `_CNInterrupt()` is defined but empty
   - System uses polling instead
   - Migration to interrupt-driven I/O would require significant refactoring

3. **Magic Numbers**:
   - 300ms debounce delay
   - 3000ms long press threshold
   - Consider defining as named constants

4. **Unused State Transitions**:
   - `STATE_SET_MINUTES` and `STATE_SET_SECONDS` defined but never used
   - May indicate planned features

### Questions to Ask Users

Before making significant changes, clarify:
- Is hardware modification allowed, or firmware-only?
- What is the target MPLAB X version?
- Are there timing constraints for the main loop?
- Should the system migrate to interrupt-driven I/O?
- What UART baud rate is configured? (check UART2.c implementation)

---

## Quick Reference

### File Responsibility Matrix

| Module | Purpose | Modifies Timer? | Hardware Access? | UART Output? |
|--------|---------|-----------------|------------------|--------------|
| main.c | Entry point | No | No | No |
| TimerOperations | Timer logic | Yes | No | No |
| IOs | Button/LED control | Yes (via Timer_*) | Yes | No |
| TerminalDisplay | Display abstraction | No | No | Yes (via UART2) |
| TimeInterval | Delays | No | Yes (timer regs) | No |
| UART2 | Serial driver | No | Yes (UART regs) | Yes |
| clkChange | Clock config | No | Yes (OSC regs) | No |

### Include Dependency Tree
```
main.c
├── IOs.h
│   └── TimerOperations.h
│       └── TimerState.h
├── TimeInterval.h
├── clkChange.h
├── UART2.h
└── TerminalDisplay.h
    └── TimerOperations.h
```

### State Transition Diagram
```
IDLE ──[Timer_start()]──> RUNNING
  ^                          │ │
  │                          │ └─[Timer_pause()]──> PAUSED
  │                          │                         │
  └──────[Timer_reset()]─────┴──[Timer_tick():done]> FINISHED
```

---

## Repository Health

**Last Updated**: 2025-11-15
**Recent Changes**:
- PR #1: Fixed header guard comments consistency
- Added missing files (IOs.c, TimeInterval, main.c)

**Code Quality Notes**:
- Clean modular architecture
- Consistent naming conventions
- Minimal comments (code is self-documenting)
- Some timing implementation issues (see Potential Issues)
- No unit tests (hardware-dependent embedded system)

**Missing**:
- Build system files (MPLAB X project)
- Hardware schematics
- UART baud rate documentation
- Detailed user manual

---

**End of CLAUDE.md**
