#include <stm32l1xx.h>

int main(){
	RCC->AHBENR |= (1<<1);
	GPIOB->MODER |= (1<<12);
	GPIOB->ODR |= (1<<6);
	while(1);
}
