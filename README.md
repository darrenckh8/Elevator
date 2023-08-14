# Elevator Control System

This repository contains code for a simple elevator control system using an Arduino and a LiquidCrystal display.

## Description

The code implements a basic elevator control system with the following features:

- Floor buttons: There are four floor buttons (8, 9, 10, 11) corresponding to floors 2, 3, 4, and Ground (G).
- Door buttons: There are two door buttons (12 and 13) for opening and closing the elevator doors.
- Internal open/close door buttons: Buttons (18 and 19) inside the elevator to manually open and close doors.
- Floor LEDs: Four LEDs (14, 15, 16, 17) indicate the current floor.
- LCD Display: A 16x2 LiquidCrystal display (7, 6, 5, 4, 3, 2) shows the elevator's status and floor information.

## Functionality

The code manages the elevator's states and operations:

1. **IDLE**: The initial state. The current floor is displayed on the LCD. If door open requests are detected, the system transitions to the OPENING_DOOR state.
2. **OPENING_DOOR**: The doors open, and LEDs and LCD display indicate this state. After a brief delay, the doors close, and the system transitions to the CLOSING_DOOR state.
3. **CLOSING_DOOR**: The elevator checks for floor selection requests. If a floor button is pressed, the system moves to the selected floor (MOVING_UP or MOVING_DOWN state). If the internal close door button is pressed, the system remains in this state. If door open requests or the internal open door button are pressed, the system transitions to the ABORTED state.
4. **MOVING_UP**: The elevator moves up one floor at a time. The LCD displays an arrow indicating upward movement and the target floor. After reaching the target floor, the system transitions to the ARRIVED state.
5. **MOVING_DOWN**: Similar to MOVING_UP, but the arrow points downward. After reaching the target floor, the system transitions to the ARRIVED state.
6. **ARRIVED**: The elevator displays the arrived floor on the LCD. The doors open, and after a delay, they close again. The target floor is reset, and the system transitions back to the IDLE state.
7. **ABORTED**: This state is entered if door open requests, door close requests, or internal open door button is pressed during any state. The doors open and then close after a delay, and the system transitions back to the IDLE state.

## Usage

1. Upload the code to an Arduino board.
2. Connect the hardware components as specified in the code (LCD, buttons, LEDs).
3. Run the elevator by pressing the buttons and observing the behavior on the LCD and LEDs.

## Limitations

1. This Code does not facilitate the functionality of queuing floors.
