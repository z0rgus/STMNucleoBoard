#ifndef __NUCLEO476_LED_H
#define __NUCLEO476_LED_H

#include "stm32l476xx.h"

extern volatile int red_flashing;
extern volatile int green_flashing;

void GPIO_Output_Init( GPIO_TypeDef *GPIO, uint32_t pin );
void LED_Init(void);

void Red_LED_On(void);
void Red_LED_Off(void);
void Green_LED_On(void);
void Green_LED_Off(void);
void LED_Flash_Handler(void);
void Red_LED_Toggle(void);
void Green_LED_Toggle(void);


#endif /* __NUCLEO476_LED_H */
