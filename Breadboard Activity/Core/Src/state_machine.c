// State Machine Example Code for SWEN 340 LED flashing project
// This has two state machines with one command string parser.
// It is an intentionally incomplete implementation.
// Larry Kiser, March 8, 2023


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <UART.h>
#include "StateMachine.h"
#include "LED.h"


// NOTE -- the index of these strings must match the values of
//			the command_events enum values.// using from demo.c
extern volatile int red_flashing ;
extern volatile int green_flashing ;
char *command_strings[] =
{
	"RON",
	"ROFF",
	"GON",
	"GOFF",
	"RFLASH",
	"GFLASH",
	"ALLOFF",
	"HELP",
	// add more entries for each command in exact order as the events for simplicity.,
	// terminate this array with a NULL pointer to make it easy to used this array.
	NULL
} ;

// check for match on name and return the corresponding command event
// or no_event if there is no match.
enum command_events check_name( char *possible_command )
{
	for ( enum command_events index = ron ; command_strings[ index ] ; index++ )
		if ( strcmp( possible_command, command_strings[ index ] ) == 0 )
			return index ;

	return no_event ;
}

enum command_events check_for_event()
{
	#define MAX_LENGTH (20)

	static char current_command[ MAX_LENGTH + 1 ] ;
	static int command_index = 0;

	// call non blocking read to build the current command buffer including processing deletes
	uint8_t received_char = USART_Read_NonBlocking(USART2);
	if(received_char != 0){
		// if an enter key
		if(received_char == '\r' || received_char == '\n'){
			current_command[command_index] = '\0'; // add terminating character
			command_index = 0;
			USART_Write(USART2, (uint8_t*)"\r\n", strlen("\r\n"));
			enum command_events result = check_name(current_command); // save the command
			memset(current_command, 0, MAX_LENGTH*sizeof(char)); // clear the buffer
			if(result == no_event){ // if no_event then invalid command since it failed the stringcmp
				char *invalid_msg = "Invalid Command\r\n";
				USART_Write(USART2, (uint8_t*)invalid_msg, strlen(invalid_msg));
			}
			return result; // return the enum command
		}else if (received_char == 0x7F) { // delete, since on mac it is delete.
            if (command_index > 0) {
                command_index--;
                current_command[command_index] = '\0';
                USART_Write(USART2, (uint8_t*)"\b \b", 3); // proper way to visually show the backspace
            }
        }else if (command_index < MAX_LENGTH){ // otherwise, echo the characters
			current_command[command_index++] = received_char;
			USART_Write(USART2, &received_char, 1);
		}
	}
	// On enter key add code to non-blocking collect a command and return corresponding event
	return no_event ;
}


// displays the commands
void display_help(){
	char *help_text = "***REMOTE LED CONTROL MENU***\r\n"
			"Available User Commands\r\n"
			"RON - Turn on RED LED\r\n"
			"ROFF - Turn off RED LED\r\n"
			"GON - Turn on GREEN LED\r\n"
			"GOFF - Turn off GREEN LEN\r\n"
			"RFLASH - Start flashing RED LED\r\n"
			"GFLASH - Start flashing GREEN LED\r\n"
			"ALLOFF - TURNOFF LEDS\r\n";
	USART_Write(USART2, (uint8_t*) help_text, strlen(help_text));
}

void red_led_state_machine( enum command_events new_event )
{
	static enum LED_states red_led_state = state_off ;

	switch ( red_led_state )
	{
		case state_off : // if red led is off, possible commands are to turn it on and to flash it
			if ( new_event == ron )
			{
				// action: turn on red LED
				red_led_state = state_on ;
				Red_LED_On();
				red_flashing = 0;
			}
			else if(new_event == rflash){
				red_led_state = state_flashing;
				red_flashing = 1;

			}
			break ;
		case state_on : // if led is on, can turn it off or flash it
			if ( new_event == roff || new_event == alloff)
			{
				// action: turn off red LED
				red_led_state = state_off ;
				Red_LED_Off();
				red_flashing = 0;
			}
			else if(new_event == rflash){

				red_led_state = state_flashing;
				red_flashing = 1;
			}
			break ;
		case state_flashing : // for flashing we can turn them off
			if(new_event == roff || new_event == alloff){
				red_led_state = state_off;
				Red_LED_Off();
				red_flashing = 0;
			}else if(new_event == ron){ // or turn them on and keep them on
				red_led_state = state_on;
				red_flashing = 0;
				Red_LED_On();
			}
			break ;
		case state_unknown :
			// action: turn off red LED
			red_led_state = state_off ;
			Red_LED_Off();
			red_flashing = 0;
			break ;
	}
}

void green_led_state_machine( enum command_events new_event )
{
	static enum LED_states green_led_state = state_off ;


	switch ( green_led_state )
	{
		case state_off :
			if ( new_event == gon )
			{
				// action: turn on green LED
				green_led_state = state_on ;
				Green_LED_On();
				green_flashing = 0;
			}
			if(new_event == gflash){
				green_led_state = state_flashing;
				Green_LED_Toggle();
				green_flashing = 1;
			}
			break ;
		case state_on :
			if ( new_event == goff || new_event == alloff)
			{
				// action: turn off green LED
				green_led_state = state_off ;
				Green_LED_Off();
				green_flashing = 0;
			}
			if(new_event == gflash){
				green_led_state = state_flashing;
				Green_LED_Toggle();
				green_flashing = 1;
			}

			break ;
		case state_flashing :
			if(new_event == goff || new_event == alloff){
				green_led_state = state_off;
				Green_LED_Off();
				green_flashing = 0;
			}else if(new_event == gon){
				green_led_state = state_on;
				green_flashing = 0;
				Green_LED_On();
			}
			break ;
		case state_unknown :
			// action: turn off green LED
			green_led_state = state_off ;
			Green_LED_Off();
			green_flashing = 0;
			break ;
	}
}


