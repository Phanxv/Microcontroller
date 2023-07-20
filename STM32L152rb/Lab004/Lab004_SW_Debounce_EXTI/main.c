#include "stm32l1xx.h"

int LED_status = 0;

int main(){
		//Setup GPIO
    RCC->AHBENR |= (6<<0);
    GPIOC->MODER &= ~(3<<6);
		GPIOB->MODER |= (1<<12);
		//Setup Interrupt
    RCC->APB2ENR |= (1<<0);
    SYSCFG->EXTICR[0] &= ~(15<<12);
		SYSCFG->EXTICR[0] |= (2<<12);
    EXTI->IMR |= (1<<3);
    EXTI->FTSR |= (1<<3);

    NVIC_EnableIRQ((IRQn_Type) 9);
	  NVIC_SetPriority((IRQn_Type) 9, 0);
	
	  while(1);
}

void EXTI3_IRQHandler(void){
		GPIOB->ODR |=  (1<<6); 
}