# Alarma de Seguridad README

This repository contains the source code for an Alarm System implemented on an 8051 microcontroller. The alarm system is designed to control access to a restricted area, using a keypad for input and a motor to open and close a door. The system has three states: "Apagado" (Off), "Encendido" (On), and "Alarma" (Alarm). The system requires a valid 4-digit code to be entered on the keypad to switch between states.

The result video is hosted in Youtube, https://youtu.be/bAArUunYhYE


![Maqueta](/img/alarma-maqueta.png "Maqueta alarma").

![Maqueta](/img/alarma-maqueta-2.png "Maqueta alarma").

## Hardware Requirements

To run this alarm system, you will need the following hardware components:

1. 8051 Microcontroller
2. Motor driver
3. Keypad (4x4 matrix)
4. LEDs (red, green, and yellow)
5. Sensors for door status (e.g., reed switches or proximity sensors)

![Motor](/img/motor.png "Motor").


## Connections

The keypad is connected to Port 1 of the 8051 microcontroller as follows:

   7  6  5  4    3  2  1  0
  NU-c1-f1-c0   f4-c2-f3-f2
        2         1        3


![Teclado](/img/teclado.png "Teclado").


## States and Operation

The alarm system operates in three different states:

1. **Apagado (Off)**: The alarm is disarmed, the green LED is on, and the door is open. The system is waiting for a valid 4-digit code to activate the alarm.

2. **Encendido (On)**: The alarm is armed, the red LED is on, and the door is closed. If a valid code is entered, the system transitions to the "Apagado" state. Otherwise, it will go to the "Alarma" state.

3. **Alarma (Alarm)**: The alarm has been triggered, and the yellow LED is on. The door remains closed, and the system waits for a valid code to deactivate the alarm and return to the "Apagado" state.

## Code Explanation

The source code consists of several functions to handle the alarm system's operation. Below is an overview of the main functions:

1. `teclado`: This function is responsible for reading input from the keypad. When a key is pressed, it waits until it is released and stores the pressed key's value.

2. `comparar`: This function compares the 4-digit code entered on the keypad with a predefined valid code. If the entered code matches the valid code, the global variable `OK` is set to '1', indicating a correct code. Otherwise, `OK` is set to '0'.

3. `inicioprograma`: This function serves as the main program loop and manages the alarm system's states. Depending on the current state, the appropriate LEDs are turned on, and the keypad is used to read the user's input. The `OK` variable is used to control state transitions based on the entered code.

4. `Timer0Init`: This function initializes Timer 0 for motor control.

## Usage

To use this alarm system, follow these steps:

1. Connect the hardware components as specified in the "Connections" section.

2. Flash the microcontroller with the provided source code.

3. Power on the system.

4. The system will be in the "Apagado" state by default. Enter the correct 4-digit code on the keypad to activate the alarm and transition to the "Encendido" state. If the code is incorrect, the system will transition to the "Alarma" state.

5. In the "Encendido" state, enter the correct 4-digit code to deactivate the alarm and return to the "Apagado" state.

6. In the "Alarma" state, the alarm is triggered, and the system waits for a valid code to deactivate the alarm.
