/*
 * demo.c
 *
 *  Created on: Feb 14, 2021
 *      Author: larry kiser
 *  Updated on: Sept 6, 2021
 *      Author: Mitesh Parikh
 */

#include <stdio.h>
#include <string.h>

// Custom Include files
#include "LED.h"
#include "UART.h"
#include "demo.h"
#include "stm32l4xx.h"

volatile uint8_t one_second_elapsed;


//static void demo_of_printf_scanf(){
//	char rxByte;
//	char *prompt = "Give Red LED control input (Y = On, N = off):\r\n" ;
//	char *LED_off = "LED is Off\r\n\r\n" ;
//	char *LED_on = "LED is On\r\n\r\n" ;
//
//	USART_Write( USART2, (uint8_t *)prompt, strlen( prompt ) ) ;
//	scanf ("%c", &rxByte);
//
//	if (rxByte == 'N' || rxByte == 'n'){
//		LED_Off();
//		USART_Write( USART2, (uint8_t *)LED_off, strlen( LED_off ) ) ;
//	}
//	else if (rxByte == 'Y' || rxByte == 'y'){
//		LED_On();
//		USART_Write( USART2, (uint8_t *)LED_on, strlen( LED_on ) ) ;
//	}
//}


// This function is to create a delay by consuming CPU cycle on counter
//static void delay_loop( int value )
//{
//	// spin loop consuming CPU to spend time.
//	for (int i = 0; i < value; i++)
//		;
//}

// This function is to Initialize SysTick registers
void init_systick()
{
	// Use the SysTick global structure pointer to do the following in this
	// exact order with separate lines for each step:
	//
	// Disable SysTick by clearing the CTRL (CSR) register.
	SysTick->CTRL = 0;
	// Set the LOAD (RVR) to 8 million to give us a 1 milliseconds timer.
	SysTick->LOAD = 80000;
	// Set the clock source bit in the CTRL (CSR) to the internal clock.
	// Set the enable bit in the CTRL (CSR) to start the timer.
	SysTick->CTRL =  (1 << 2) | (1 << 1) | (1 << 0);
}

//// This fuction is to create delay using SysTick timer counter
//static void delay_systick()
//{
//	// Using the SysTick global structure pointer do the following:
//	// Create a for loop that loops 10 times
//	for(int i = 0; i < 20; i++){
//		while(!(SysTick->CTRL & (1 << 16)))
//			;
//	}
//	// Inside that for loop check the COUNTFLAG bit in the CTRL (CSR)
//	// register in a loop. When that bit is set exit this inner loop
//	// to do another pass in the outer loop of 10.
//}

void SysTick_Handler()
{
	static uint32_t counter = 0;
	counter++;
	if(counter >= 1000){  // 1 second elapsed
		one_second_elapsed = 1;
		counter = 0;

		// Call LED toggle function every second
		LED_Flash_Handler();
	}

}

// This function is to print counter on UART port and toggle LED
static void demo_of_UART_print(int counter){
	int n ;
	uint8_t buffer[BUFFER_SIZE];

	n = sprintf((char *)buffer, "counter = %d\r\n", counter);
	USART_Write(USART2, buffer, n);

//    delay_loop( 8000000 ) ;	// comment this out when you are ready to test delay_systick
//    delay_systick() ;	// enable this when you are ready to test

	// Toggle LED
	LED_Toggle();
}

void run_demo(){

	int counter = 0;

	// Initialize Systick Register
	init_systick() ;

	// Run a loop to print counter value on UART port
	while (1)
	{
		//demo_of_printf_scanf();
		if(one_second_elapsed == 1){

			one_second_elapsed = 0;
			demo_of_UART_print(counter);
			counter++;
		}
	}
}

