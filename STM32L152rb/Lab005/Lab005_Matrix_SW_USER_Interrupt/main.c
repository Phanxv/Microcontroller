#include "stm32l1xx.h"

uint8_t numpad = 0; //declare variable numpad for checking which switch is pressed

int main(){
	/*Config Clock and Mode for GPIO*/
	RCC->AHBENR |= (1<<0) | (1<<1) | (1<<2);
	GPIOA->MODER |= (1<<2) | (1<<4) | (1<<6);	
	GPIOA->MODER &= ~(1<<0);
	GPIOB->MODER |= (1<<28) | (1<<30); 
	GPIOC->MODER &= ~(0x0F);
	/*Set PA[3:1] to 1 to turn LED off*/
	GPIOA->ODR |= (7<<1);
	/*Setup Interrupt USER PA0*/
	RCC->APB2ENR |= (1<<0);
	SYSCFG->EXTICR[0] &= ~(15<<0); //Attach Interrupt to PA0
	EXTI->IMR |= (1<<0); //Unmask PA0
	EXTI->RTSR |= (1<<0); //Detect Rising edge
	/*Enable NVIC for EXTI Line 0*/
	NVIC_EnableIRQ((IRQn_Type) 6); 
	NVIC_SetPriority((IRQn_Type) 6, 0);
	
	/*loop*/
	while(1){
		/*Activate Line 1 by setting PB14 to 0 and PB15 to 1*/
		GPIOB->ODR &= ~(1<<14);
		GPIOB->ODR |= (1<<15);
		/*Check whick switch is pressed, by checking IDR of PC[3:1]
		IDR[0] == 0 means switch 1 is pressed
		IDR[1] == 0 means switch 2 is pressed
		IDR[2] == 0 means switch 3 is pressed*/
		if((GPIOC->IDR & 0x01) == 0){
			numpad = 1;
			/*Set ODR[3:1] of PA to 110 to turn on LEDR*/
			GPIOA->ODR &= ~(7<<1);
			GPIOA->ODR |= (3<<1);
		}
		if((GPIOC->IDR & 0x02) == 0){
			numpad = 2;
			/*Set ODR[3:1] of PA to 101 to turn on LEDY*/
			GPIOA->ODR &= ~(7<<1);
			GPIOA->ODR |= (5<<1);
		}
		if(((GPIOC->IDR & 0x04)) == 0){
			numpad = 3;
			/*Set ODR[3:1] of PA to 110 to turn on LEDG*/
			GPIOA->ODR &= ~(7<<1);
			GPIOA->ODR |= (6<<1);
		}

		GPIOB->ODR &= ~(1<<15);
		GPIOB->ODR |= (1<<14);
		/*Check whick switch is pressed, by checking IDR of PC[3:1]
		IDR[0] == 0 means switch 4 is pressed
		IDR[1] == 0 means switch 5 is pressed
		IDR[2] == 0 means switch 6 is pressed*/
		if((GPIOC->IDR & 0x01) == 0){
			numpad = 4;
			/*Set ODR[3:1] of PA to 001 to turn on LEDR & LEDY*/
			GPIOA->ODR &= ~(7<<1);
			GPIOA->ODR |= (1<<1);
		}
		if(((GPIOC->IDR & 0x02)) == 0){
			numpad = 5;
			/*Set ODR[3:1] of PA to 100 to turn on LEDG & LEDY*/
			GPIOA->ODR &= ~(7<<1);
			GPIOA->ODR |= (4<<1);
		}
		if(((GPIOC->IDR & 0x04)) == 0){
			numpad = 6;
			/*Set ODR[3:1] of PA to 000 to turn on LEDR & LEDY & LEDG*/
			GPIOA->ODR &= ~(7<<1);
		}
	}
}
/*Interrupt handler*/
void EXTI0_IRQHandler(void){
		GPIOA->ODR |= (7<<1); //turn off all LED
		EXTI->PR |= (1<<0); //Clear pending request
}
