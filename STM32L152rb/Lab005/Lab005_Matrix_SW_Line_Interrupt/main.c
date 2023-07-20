#include "stm32l1xx.h"

uint8_t numpad = 0; //declare variable numpad for checking which switch is pressed

int main(){
	/*Config Clock and Mode for GPIO*/
	RCC->AHBENR |= (1<<0) | (1<<1) | (1<<2);
	GPIOA->MODER |= (1<<2) | (1<<4) | (1<<6); 
	GPIOB->MODER |= (1<<28) | (1<<30); 
	GPIOC->MODER &= ~(0x0F);
	/*Set PA[3:1] to 1 to turn LED off*/
	GPIOA->ODR |= (7<<1);
	/*Config Interrupt*/
	RCC->APB2ENR |= (1<<0);

  SYSCFG->EXTICR[0] &= ~(15<<0);
	SYSCFG->EXTICR[0] |= (1<<1) | (1<<5) | (1<<9); //Attach Interrupt to PC[2:0]
  EXTI->IMR |= (7<<0); //Unmask PC[2:0]
  EXTI->FTSR |= (7<<0); //Detect Falling edge
	/*Enable NVIC for EXTI Line 0,1,2*/
  NVIC_EnableIRQ((IRQn_Type) 6); 
  NVIC_SetPriority((IRQn_Type) 6, 0);
	NVIC_EnableIRQ((IRQn_Type) 7);
  NVIC_SetPriority((IRQn_Type) 7, 0);
	NVIC_EnableIRQ((IRQn_Type) 8);
  NVIC_SetPriority((IRQn_Type) 8, 0);
	/*loop*/
	while(1){
		/*Activate Line 1 by setting PB14 to 0 and PB15 to 1*/
		GPIOB->ODR &= ~(1<<14);
		GPIOB->ODR |= (1<<15);
		/*Activate Line 2 by setting PB14 to 1 and PB15 to 0*/
		GPIOB->ODR &= ~(1<<15);
		GPIOB->ODR |= (1<<14);
	}
}
/*Handling interrupt when PC0 is pressed*/
void EXTI0_IRQHandler(void){
	if((GPIOB->ODR & (1<<14)) == 0){ //check which Line in activated
		numpad = 1;
		/*Set ODR[3:1] of PA to 110 to turn on LEDR*/
		GPIOA->ODR &= ~(7<<1);
		GPIOA->ODR |= (3<<1);
	}
	if((GPIOB->ODR & (1<<15)) == 0){
		numpad = 4;
		/*Set ODR[3:1] of PA to 001 to turn on LEDR & LEDY*/
		GPIOA->ODR &= ~(7<<1);
		GPIOA->ODR |= (1<<1);
	}
	EXTI->PR |= (1<<0); //Clear pending interrupt request
}
/*Handling interrupt when PC1 is pressed*/
void EXTI1_IRQHandler(void){
	if((GPIOB->ODR & (1<<14))== 0){
		numpad = 2;
		/*Set ODR[3:1] of PA to 101 to turn on LEDY*/
		GPIOA->ODR &= ~(7<<1);
		GPIOA->ODR |= (5<<1);
	}
	if((GPIOB->ODR & (1<<15)) == 0){
		numpad = 5;
		/*Set ODR[3:1] of PA to 100 to turn on LEDG & LEDY*/
		GPIOA->ODR &= ~(7<<1);
		GPIOA->ODR |= (4<<1);	
	}
	EXTI->PR |= (1<<0);//Clear pending interrupt request
}
/*Handling interrupt when PC2 is pressed*/
void EXTI2_IRQHandler(void){
	if((GPIOB->ODR & (1<<14))== 0){
		numpad = 3;
		/*Set ODR[3:1] of PA to 110 to turn on LEDG*/
		GPIOA->ODR &= ~(7<<1);
		GPIOA->ODR |= (6<<1);	
	}
	if((GPIOB->ODR & (1<<15)) == 0){
		numpad = 6;
		/*Set ODR[3:1] of PA to 000 to turn on LEDR & LEDY & LEDG*/
		GPIOA->ODR &= ~(7<<1);
	}
	EXTI->PR |= (1<<0);//Clear pending interrupt request
}