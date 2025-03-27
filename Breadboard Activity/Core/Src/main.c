// main.c with demo
// Updated and refactored Larry Kiser copyright 2021


// include project specific header files
#include "StateMachine.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "demo.h"


int main(void){

	// initialization code
	System_Clock_Init(); // set System Clock = 80 MHz
	LED_Init(); // initialize LED
	init_systick(); // this is for toggling
	UART2_Init();

	display_help(); // immediately show the list of commands
	while (1)
	    {
	        enum command_events new_event = check_for_event();
	        if (new_event != no_event)
	        {
	            if (new_event == help)
	            {
	                display_help();
	            }
	            else
	            {
	                red_led_state_machine(new_event);
	                green_led_state_machine(new_event);
	            }
	        }
	    }
	return 0;

	// application run function
}

