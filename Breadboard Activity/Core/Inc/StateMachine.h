#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <UART.h>
#include "StateMachine.h"
#include "LED.h"
// Enum for LED states
enum LED_states
{
    state_off,
    state_on,
    state_flashing,
    state_unknown
};
//
//// Enum for command events
enum command_events
{
    ron = 0,
    roff = 1,
    gon = 2,
    goff = 3,
    rflash = 4,
    gflash = 5,
    alloff = 6,
    help = 7,        // <--- Define HELP here
    no_event
};

// Function prototypes
enum command_events check_name(char *possible_command);
enum command_events check_for_event();
void display_help();
void red_led_state_machine(enum command_events new_event);
void green_led_state_machine(enum command_events new_event);

#endif // STATE_MACHINE_H
