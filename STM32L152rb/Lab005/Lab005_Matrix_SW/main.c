#include "stm32l1xx.h"

uint8_t numpad = 0;

int main(){
	RCC->AHBENR |= (1<<1) | (1<<2);
	GPIOB->MODER |= (1<<28) | (1<<30);
	GPIOC->MODER &= ~(0x0F);
	
	while(1){
		GPIOB->ODR &= ~(1<<14);
		GPIOB->ODR |= (1<<15);
		if((GPIOC->IDR & 0x01) == 0)
			numpad = 1;
		if(((GPIOC->IDR & 0x02) >> 1) == 0)
			numpad = 2;
		
		GPIOB->ODR &= ~(1<<15);
		GPIOB->ODR |= (1<<14);
		if((GPIOC->IDR & 0x01) == 0)
			numpad = 3;
		if(((GPIOC->IDR & 0x02) >> 1) == 0)
			numpad = 4;
	}
}
